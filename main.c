#include "websocket_handler.h"


int main(void) {
    // Structure to hold the configuration information for the WebSocket context.
    struct lws_context_creation_info info;

    // Pointer to the WebSocket context, which manages all WebSocket operations.
    struct lws_context *context;

    // Initialize the WebSocket context creation structure with default settings.
    initialize_websocket(&info);

    // Create the WebSocket context using the initialized configuration.
    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create WebSocket context\n");
        return -1;
    }

    // Start the WebSocket connection and enter the main event loop.
    connect_and_loop(context);

    // Clean up and destroy the WebSocket context.
    lws_context_destroy(context);

    return 0;
}
