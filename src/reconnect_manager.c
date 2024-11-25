#include "reconnect_manager.h"
#include <pthread.h>
#include <stdio.h>
#include <windows.h> // For Sleep on Windows

int should_reconnect = 1;


/**
 * @brief Handles the reconnection logic for the WebSocket client.
 *
 * This function checks the global `should_reconnect` flag and attempts to reconnect
 * to the WebSocket server if the flag is set. The reconnection attempt is made using
 * the provided `lws_client_connect_info` structure, which contains the necessary
 * connection parameters. If the reconnection attempt fails, the function waits
 * for a predefined delay (`RECONNECT_DELAY`) before retrying.
 *
 * Upon successful reconnection, the `should_reconnect` flag is reset to prevent
 * further reconnection attempts.
 *
 * @param ccinfo A pointer to a `lws_client_connect_info` structure containing
 *               the connection parameters required to initiate a WebSocket connection.
 * @param context A pointer to the `lws_context` that represents the WebSocket
 *                context in which the connection is managed.
 *
 * @return void This function does not return any value.
 *
 * @note The function assumes that `should_reconnect` is managed elsewhere in the
 *       program (e.g., set to `1` when a connection is lost). It also assumes
 *       that `RECONNECT_DELAY` is defined appropriately as the delay in seconds
 *       between reconnection attempts.
 *
 * @warning If the `lws_client_connect_via_info` function fails persistently,
 *          the program will remain in an infinite loop, repeatedly attempting
 *          to reconnect and delaying by `RECONNECT_DELAY` seconds. Ensure this
 *          behavior is acceptable for your use case.
 */ 
void handle_reconnection(struct lws_client_connect_info *ccinfo) {
    // Check if a reconnection is required.
    if (should_reconnect) {
        printf("Attempting to reconnect...\n");

        // Attempt to establish a new WebSocket connection using the provided connection info.
        struct lws *wsi = lws_client_connect_via_info(ccinfo);

        // Check if the connection attempt was successful.
        if (!wsi) {
            fprintf(stderr, "WebSocket reconnection failed, retrying...\n");
            Sleep(RECONNECT_DELAY * 1000); // Reconnect delay
        } else {
            printf("Reconnection successful.\n");
            should_reconnect = 0;
        }
    }
}
