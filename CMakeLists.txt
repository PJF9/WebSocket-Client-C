cmake_minimum_required(VERSION 3.10)

# Project name
project(WebSocketsC C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set executable name
set(EXECUTABLE_NAME main)

# Specify the path to libwebsockets and OpenSSL
set(LIBWEBSOCKETS_ROOT "${C_LIBRARIES_PATH}/vcpkg/packages/libwebsockets_x64-windows")
set(OPENSSL_ROOT "${C_LIBRARIES_PATH}/vcpkg/packages/openssl_x64-windows")
set(JANSSON_ROOT "${C_LIBRARIES_PATH}/vcpkg/packages/jansson_x64-windows")
set(PTHREADS_ROOT "${C_LIBRARIES_PATH}/vcpkg/packages/pthreads_x64-windows")

# Specify the directories containing the include files
include_directories(include)

# Define the source files
set(SOURCES
    src/file_writer.c
    src/reconnect_manager.c
    src/websocket_handler.c
    main.c
)

# Include the libwebsockets and opensll headers
include_directories("${LIBWEBSOCKETS_ROOT}/include")
include_directories("${OPENSSL_ROOT}/include")
include_directories("${JANSSON_ROOT}/include")
include_directories("${PTHREADS_ROOT}/include")

# Link the libwebsockets and openssl library
link_directories("${LIBWEBSOCKETS_ROOT}/lib")
link_directories("${OPENSSL_ROOT}/lib")
include_directories("${JANSSON_ROOT}/lib")
include_directories("${PTHREADS_ROOT}/lib")

# Add the executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Link libwebsockets and OpenSSL to the project
target_link_libraries(${EXECUTABLE_NAME} PRIVATE websockets)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE libssl)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE libcrypto)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE jansson)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES WIN32_EXECUTABLE OFF)
