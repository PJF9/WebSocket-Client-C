#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#define BUFFER_SIZE 1024
#define SYMBOL "BINANCE:SOLUSDT"

#include <libwebsockets.h>


/**
 * @brief Initializes the WebSocket context configuration.
 *
 * This function prepares the `lws_context_creation_info` structure with the
 * necessary settings for establishing a WebSocket connection. It sets up
 * protocols, ports, and SSL options.
 *
 * @param info A pointer to a `struct lws_context_creation_info` structure
 *             that will be initialized with the required configuration.
 */
void initialize_websocket(struct lws_context_creation_info *info);


/**
 * @brief Establishes a WebSocket connection and enters the event loop.
 *
 * This function sets up the WebSocket client connection parameters and
 * manages the event loop using `lws_service`. It ensures continuous
 * handling of WebSocket events and manages reconnections when necessary.
 *
 * @param context A pointer to the `struct lws_context` representing the WebSocket context.
 */
void connect_and_loop(struct lws_context *context);


/**
 * @brief Array defining the supported WebSocket protocols.
 *
 * This structure maps protocol names to their corresponding callback functions
 * and defines buffer sizes. It is used to initialize the WebSocket context.
 */
extern struct lws_protocols protocols[];

#endif
