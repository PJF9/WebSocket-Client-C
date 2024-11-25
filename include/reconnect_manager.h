#ifndef RECONNECT_MANAGER_H
#define RECONNECT_MANAGER_H

#define RECONNECT_DELAY 5  // Seconds to wait before reconnecting

#include <libwebsockets.h>


/**
 * @brief Global flag to indicate whether a reconnection is required.
 *
 * This variable is used by the WebSocket client logic to determine
 * if the application should attempt to reconnect after a disconnection
 * or connection error.
 */
extern int should_reconnect;


/**
 * @brief Handles WebSocket reconnection logic.
 *
 * This function attempts to reconnect to the WebSocket server using the
 * provided connection parameters. If a reconnection attempt fails, the
 * function waits for `RECONNECT_DELAY` seconds before retrying.
 *
 * @param ccinfo A pointer to a `struct lws_client_connect_info` structure containing
 *               the connection parameters required to establish the WebSocket connection.
 */
void handle_reconnection(struct lws_client_connect_info *ccinfo);

#endif
