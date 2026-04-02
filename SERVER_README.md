# ZeusBanking Distributed Server

This server is part of the ZeusBanking distributed banking system. It handles account management, transactions, and communication with clients over UDP.

## Prerequisites
- C++ compiler (e.g., g++, clang++)
- Make (optional, if Makefile is provided)
- All dependencies required by your code (e.g., pthreads, standard C++ libraries)

## Build Instructions

### Bash Script

Simply run the following files to automatically setup and run the server under default configuration:

```
Server Hostname		: localhost
Server Port		: 8080
Max Retry		: 3
Error Rate Simulation	: 30%
Invocation Semantic	: At Most Once
```

To adjust its parameter please take a look at the [Configuration](#configuration) section of this file.

```sh
source script/setup-server.sh
source script/run-server.sh
```

### Manual Compilation

Compile all `.cpp` files:

```sh
cd server
mkdir -p bin

g++ *.cpp net/*.cpp -o bin/app
```

## Running the Server

1. From the `server/` directory, run:
	```sh
	./bin/app
	```
	or, if you are in the project root:
	```sh
	./server/bin/app
	```

2. The server will start and listen for client requests.

## Custom Configuration

Run this after compiling (After running the `source script/setup-server.sh`)

```
Usage: ./server/bin/app [OPTIONS]

Options:
  -h, --host <name>      Server hostname/IP Address (default: localhost)
  -p, --port <num>       Port number (1-65535)
  -s, --semantic <type>  Invocation Semantic: (default: '1')
                         '0' or 'at-least-once'
                         '1' or 'at-most-once'
  -r, --max-retry <num>  Max number of retries
  -e, --error <num>      Simulated error/packet loss rate (0 - 100)
  --help                 Display this help message
```

Example: `./server/bin/app --host localhost --port 8080 -s 1 -r 3 -e 50`

## Troubleshooting
- Ensure all dependencies are installed.
- If you encounter permission errors, run: 
  
  ```sh
  chmod +x ./server/bin/app
  ```
- Check for compilation errors and resolve missing includes or libraries.

## Stopping the Server
- Press `Ctrl+C` in the terminal where the server is running.

## Notes
- The server must be running before starting any clients.
- For testing, see the `server/test/` directory.

---
For more details, refer to the code comments and documentation in the `server/` directory.


