#include "websocket_handler.h"
#include "file_writer.h"
#include "reconnect_manager.h"
#include <libwebsockets.h>
#include <jansson.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Callback function for WebSocket events.
 *
 * This function handles WebSocket lifecycle events and data processing. It handles
 * connection establishment, receiving messages, sending subscription messages,
 * and managing disconnections or errors. The function is invoked by the
 * libwebsockets library based on the events specified in the `lws_callback_reasons`.
 *
 * - **LWS_CALLBACK_CLIENT_ESTABLISHED**: Confirms connection and initiates a subscription.
 * - **LWS_CALLBACK_CLIENT_RECEIVE**: Parses incoming JSON data and processes "trade" messages.
 * - **LWS_CALLBACK_CLIENT_WRITEABLE**: Sends a subscription message.
 * - **LWS_CALLBACK_CLIENT_CLOSED** and **LWS_CALLBACK_CLIENT_CONNECTION_ERROR**:
 *   Sets the reconnection flag.
 *
 * @param wsi The WebSocket instance.
 * @param reason The reason for the callback, corresponding to various WebSocket events.
 * @param user User-defined data (not used in this implementation).
 * @param in Input data from the WebSocket server.
 * @param len Length of the input data.
 * @return Always returns 0.
 */
static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("WebSocket connection established\n");

            // Request permission to send a message
            lws_callback_on_writable(wsi);

            should_reconnect = 0;  // Reset reconnection flag
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE: {
            const char *data = (const char *)in; // Incoming data
            json_error_t error;

            // Parse the incoming JSON message
            json_t *root = json_loads(data, 0, &error);
            if (!root) {
                fprintf(stderr, "JSON parsing error: %s\n", error.text);
                break;
            }

            // Extract the "type" field from the JSON object
            json_t *type = json_object_get(root, "type");
            if (!json_is_string(type)) {
                printf("Message does not contain a valid 'type' field.\n");
                json_decref(root); // Free the JSON object
                break;
            }

            // Handle trade messages only
            const char *type_str = json_string_value(type);
            if (!strcmp(type_str, "trade") == 0) {
                // Handle ping messages
                if (strcmp(type_str, "ping") == 0) {
                    printf("Ping message received, ignoring.\n");
                }
                else {
                    printf("Unhandled message type: %s\n", type_str);
                }
                json_decref(root); // Free the JSON object
                break;
            }

            // Extract the "data" array from the JSON object
            json_t *data_array = json_object_get(root, "data");
            if (!json_is_array(data_array)) {
                printf("Invalid or missing 'data' array.\n");
                json_decref(root); // Free the JSON object
                break;
            }

            size_t array_size = json_array_size(data_array);
            if (array_size == 0) {
                printf("Data array is empty.\n");
                json_decref(root); // Free the JSON object
                break;
            }

            // Extract the last trade in the array
            json_t *last_trade = json_array_get(data_array, array_size - 1);
            json_t *price = json_object_get(last_trade, "p");
            json_t *symbol = json_object_get(last_trade, "s");

            if (!(price) || !json_is_string(symbol)) {
                printf("Invalid data in last trade object.\n");
                json_decref(root); // Free the JSON object
                break;
            }

            const char *symbol_str = json_string_value(symbol);
            double price_val = json_number_value(price);

            // Allocate memory for trade data to pass to the file-writing thread
            trade_data_t *stock_data = malloc(sizeof(trade_data_t));
            if (!stock_data) {
                fprintf(stderr, "Memory allocation failed\n");
                json_decref(root); // Free the JSON object
                break;
            }
            
            // Copy the symbol and price into the allocated structure
            strncpy_s(stock_data->symbol, sizeof(stock_data->symbol), symbol_str, sizeof(stock_data->symbol) - 1);
            stock_data->symbol[sizeof(stock_data->symbol) - 1] = '\0'; // Ensure null termination
            stock_data->price = price_val;

            // Launch a new thread to write the trade data to a file
            pthread_t thread;
            if (pthread_create(&thread, NULL, write_to_file, stock_data) != 0) {
                perror("Failed to create thread");
                free(stock_data); // Free the memory if thread creation fails
            } else {
                // Detach the thread to allow it to run independently
                pthread_detach(thread);
            }

            // Free the parsed JSON object
            json_decref(root);

            break;
        }

        case LWS_CALLBACK_CLIENT_WRITEABLE: {
            // Prepare the subscription message
            char msg[BUFFER_SIZE];
            snprintf(msg, sizeof(msg), "{\"type\":\"subscribe\",\"symbol\":\"%s\"}", SYMBOL);

            unsigned char buf[LWS_PRE + BUFFER_SIZE];
            unsigned char *p = &buf[LWS_PRE]; // Data must be offset by LWS_PRE
            size_t msg_len = strlen(msg);

            // Copy the message into the buffer
            memcpy(p, msg, msg_len);

            // Send the message to the server
            lws_write(wsi, p, msg_len, LWS_WRITE_TEXT);
            break;
        }

        case LWS_CALLBACK_CLIENT_CLOSED:
            printf("WebSocket connection closed\n");
            should_reconnect = 1;  // Set reconnection flag
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("Connection error: %s\n", in ? (char *)in : "unknown reason");
            should_reconnect = 1;  // Set reconnection flag
            break;
        
        default:
            break;
    }

    return 0;
}


/**
 * @brief Array defining the protocols supported by the WebSocket client.
 *
 * This structure maps the protocol name to the corresponding callback function.
 * The callback function (`callback_websocket`) processes events for the protocol
 * "stock_streaming".
 */
struct lws_protocols protocols[] = {
    {
        "stock_streaming",  // Name of the protocol.
        callback_websocket, // The function that handles all WebSocket-related events.
        0,                  // No per-session user data required for this protocol.
        BUFFER_SIZE,        // The size of the buffer used for receiving messages.
    },
    { NULL, NULL, 0, 0 }    // A terminating entry, required by libwebsockets.
};


/**
 * @brief Initializes the WebSocket context creation information.
 *
 * This function configures the `lws_context_creation_info` structure, which is
 * used to create the WebSocket context. It sets the port, protocol, and SSL options.
 *
 * @param info Pointer to the `lws_context_creation_info` structure to be initialized.
 */
void initialize_websocket(struct lws_context_creation_info *info) {
    // Clear the memory of the `info` structure to ensure no garbage values are present.
    memset(info, 0, sizeof(*info));

    info->port = CONTEXT_PORT_NO_LISTEN; // Set the port to indicate this is a client-only WebSocket connection
    info->protocols = protocols;
    info->options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT; // Enable SSL initialization globally
}


/**
 * @brief Connects to the WebSocket server and enters the event loop.
 *
 * This function handles the connection to the WebSocket server using the
 * `lws_client_connect_info` structure. It manages reconnections using
 * the `handle_reconnection` function and processes WebSocket events
 * through the `lws_service` loop.
 *
 * @param context Pointer to the `lws_context` structure, representing the WebSocket context.
 */
void connect_and_loop(struct lws_context *context) {
    // Initialize the client connection info structure with zeroed memory.
    struct lws_client_connect_info ccinfo = { 0 };

    // Retrieve the API token from the environment variable
    const char *api_token = getenv("FINEHUB_API");
    if (!api_token) {
        fprintf(stderr, "Error: API_TOKEN environment variable not set.\n");
        return;
    }

    // Dynamically construct the WebSocket path using the token
    char path[256];
    snprintf(path, sizeof(path), "/?token=%s", api_token);

    // Configure the connection parameters.
    ccinfo.context = context;
    ccinfo.address = "ws.finnhub.io";
    ccinfo.port = 443;
    ccinfo.path = path;
    ccinfo.host = "ws.finnhub.io";
    ccinfo.origin = "https://finnhub.io";
    ccinfo.protocol = NULL; // Use default protocol.
    ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED; // Enable SSL with self-signed certs.

    while (1) {
        // Check if a reconnection is required and attempt to reconnect if necessary.
        handle_reconnection(&ccinfo);

        // Enter the WebSocket service loop to process events.
        lws_service(context, 1000);
    }
}
