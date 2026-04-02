# SC4051 PROJECT AY2025/26 SEM 2 - ZEUS DISTRIBUTED BANKING SYSTEM

# Zeus Banking - Java Client

The client-side application for the Zeus Banking System. This is a Java-based implementation designed to interact with the C++ backend via custom network protocols.

### Architecture
* **`handle/`**: Contains the logic for processing user requests and managing the UI flow.
* **`net/`**: Low-level networking logic (UDP Client, marshalling/unmarshalling socket management, input/output streams).
* **`protocol/`**: Define the schema and protocol of the request, response, and callback messages

### Key Components
* **`ZeusBankingClient.java`**: The main entry point for the client application.
* **`Constants.java`**: Global configurations such as Server IP, Port numbers, and timeout settings.
* **`protocol/StatusCode.java`**: The configuration of the response status code
* **`Utils.java`**: Helper methods for string parsing, validation, and logging.

### Getting Started
1.  Ensure you have **JDK 11+** installed.
2.  Compile the source:
    ```bash
    javac -d bin client/*.java client/**/*.java
    ```
3.  Run the client:
    ```bash
    java -cp bin client.ZeusBankingClient
    ```

---
# Zeus Banking - C++ Server

The core engine of the Zeus Banking System. A high-performance, multi-threaded C++ server responsible for account management, transaction consistency, and monitoring.

### Architecture
* **`net/`**: Socket handling and connection listening.
* **`protocol/`**: C++ implementation of the shared communication protocol
* **`bin/`**: Compiled binaries and executables.
  
### Core  Modules
| Module | Description |
| :--- | :--- |
| **AccountManager** | Manages the lifecycle of bank accounts and persistent storage. |
| **Handler** | Dispatches incoming unmarshalled network requests to the appropriate business logic. |
| **MonitorManager** | Manages the monitor client stores and update active client on bank update |
| **Account** | Data model representing individual user accounts. |

    ```
### Notes
Further details on the setup can be seen in [CLIENT_README.md](CLIENT_README.md) and [SERVER_README.md](SERVER_README.md)
