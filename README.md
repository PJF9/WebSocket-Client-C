# WebSocket Client with Reconnection and Data Logging

This project is a C-based WebSocket client that connects to a WebSocket API, subscribes to a data stream, logs received trade data to a file, and supports automatic reconnection. The project is designed to be modular, scalable, and secure, with support for dynamic configurations via environment variables.

---

## WebSockets Integration

WebSockets are a protocol that enables a persistent, low-latency connection between the client and server. Unlike traditional HTTP requests, WebSocket connections allow both the client and the server to send messages independently of each other. This makes WebSockets ideal for real-time, bidirectional communication.

This project integrates [Finnhub API](https://finnhub.io/docs/api) using WebSockets to fetch real-time market data (stock prices. WebSockets allow continuous streaming of updates, which is ideal for applications requiring real-time data, such as stock tickers, financial dashboards, or trading platforms.

---

## Features
* `WebSocket Connection`: Establishes a WebSocket connection to a specified API endpoint.
* `Dynamic Subscription`: Subscribes to a trade data stream for a configurable symbol.
* `Data Logging`: Logs received trade data (symbol and price) to a file.
* `Automatic Reconnection`: Handles reconnection attempts in the background if the connection is lost.
* `Thread-Safe`: Uses threads to log data asynchronously, ensuring responsiveness.
* `Environment Variable Support`: Retrieves the API token at runtime from an environment variable for secure configuration.

---

## Project Structure
```bash
├── build/                     # Build files
├── scripts/                   # Build and run automation scripts
├── include/
│   ├── file_writer.h          # Interface for the file writing module
│   ├── reconnect_manager.h    # Interface for reconnection management
│   ├── websocket_handler.h    # Interface for WebSocket initialization and handling
├── src/
│   ├── file_writer.c          # Implementation of the file writing module
│   ├── reconnect_manager.c    # Implementation of reconnection management
│   ├── websocket_handler.c    # Implementation of WebSocket handling
├── main.c                     # Entry point of the application
```

---

## Prerequisites

All the following configurations are for Windows machines.

1. `C Compiler`: I used the Visual Studio C compiler, which can be downloaded [here](https://visualstudio.microsoft.com/downloads/). In the installation, make sure to include _Desktop Development with C++_, _Windows 11 SDK_, and _MSVC v143_.
2. `CMake`: Download it from [here](https://cmake.org/download/). Ensure that CMake and the compiler have the same architecture (e.g., x64).
3. `vcpkg Package Manager`: Clone the official vcpkg repository [here](https://github.com/microsoft/vcpkg.git). Navigate into the cloned repo and run the `bootstrap-vcpkg.bat` script. Then, add vcpkg to your PATH environemtn variable.
4. `Install libwebsockets`: Open your terminal and run:
```bash
vcpkg install libwebsockets
```
5. `Install jansson`: To install jansson, run:
```bash
vcpkg install jansson
```
6. `Install pthreads`: You can install pthreads with the following command:
```bash
vcpkg install pthreads
```
7. `Poweshell Core`: PowerShell Core is a more modern and powerful version of PowerShell compared to the traditional Windows PowerShell, which vcpkg needs. You can download it [here](https://github.com/PowerShell/PowerShell/releases). After downloading, follow the on-screen instructions to install PowerShell.
8. `Finnhub API`: Visit this [page](https://finnhub.io/) to obtain a free API key from Finnhub. Create an environment variable named FINNHUB_API and set it to your API key.
9. `CMakeLists.txt`: To configure the CMake file, set the `C_LIBRARIES_PATH` envoronment variable to the location where the vcpkg manager is installed.
10. `Symbol` :The project logs prices from **BINANCE:SOLUSDT** into a file named trade_prices.txt. To change the symbol, navigate to _include/websocket_handler.h_. To change the log file, navigate to _src/file_writer.c_.

And you're done!

---

## Setup Instructions

1.  `Build the Client`: Run the build script:
```bash
script/build.bat
```

2. `Run the Client`: Ensure the Python server is running. Execute the run script:
```bash
scripts/run.bat
```

To build and run both the client:
```bash
scripts/build_and_run.bat
```
