#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <arpa/inet.h>

#include<getopt.h>

#include "server/net/UDPServer.h"
#include "server/net/Marshalling.h"
#include "server/net/Socket.h"
#include "server/Handler.h"
#include "server/net/PacketBuilder.h"

using namespace std;

struct Config {
    int port;
    const char* hostname;
    int semanticInvoc;
    int maxRetry;
    int errRate;

    std::string toString() const {
        std::stringstream ss;
        ss << "\n--------------------------------------------" << endl;
        ss << "Server Configuration:" << endl;
        ss << "---------------------------------------------" << endl;
        ss << "  Hostname       : " << hostname << endl;
        ss << "  Port           : " << port << endl;
        ss << "  Max Retry      : " << maxRetry << endl;
        ss << "  Simulated Error Rate: " << errRate << "%" << endl;
        ss << "  Invocation Semantic: " << (semanticInvoc == AT_MOST_ONE_SEM_INVO ? 
                                                "At-Most-Once" : "At-Least-Once") 
                                        << endl;
        return ss.str();
    }
};

void print_usage(char* prog_name) {
    printf("Usage: %s [OPTIONS]\n\n", prog_name);
    printf("Options:\n");
    printf("  -h, --host <name>      Server hostname (default: localhost)\n");
    printf("  -p, --port <num>       Port number (1-65535)\n");
    printf("  -t, --type <type>      Invocation Semantic: (default: '1')\n");
    printf("                         '0' or 'at-least-once'\n");
    printf("                         '1' or 'at-most-once'\n");
    printf("  -r, --max-retry <num>  Max number of retries\n");
    printf("  -e, --error <num>      Simulated error/packet loss rate (0 - 100)\n");
    printf("  --help                 Display this help message\n");
}

int parseArguments(int argc, char* argv[], Config& cfg) {

    cfg.port = DEFAULT_PORT;
    cfg.hostname = DEFAULT_HOST;
    cfg.semanticInvoc = AT_MOST_ONE_SEM_INVO;
    cfg.maxRetry = DEFAULT_MAX_RETRY;
    cfg.errRate = DEFAULT_FAILURE_RATE;

    int opt;

    static struct option long_options[] = {
        {"host",  required_argument, 0, 'h'},
        {"port",  required_argument, 0, 'p'},
        {"semantic",  required_argument, 0, 's'},
        {"max-retry", required_argument, 0, 'r'},
        {"error", required_argument, 0, 'e'},
        {"help", no_argument, 0, 'H'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "h:p:s:r:e:H", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'h': cfg.hostname = optarg; break;
            case 'p': cfg.port = std::stoi(optarg); break;
            case 's': 
                // Parse string to int code
                if (std::string(optarg) == "at-most-once") cfg.semanticInvoc = AT_MOST_ONE_SEM_INVO;
                else if (std::string(optarg) == "at-least-once") cfg.semanticInvoc = AT_LEAST_ONE_SEM_INVO;
                else cfg.semanticInvoc = std::stoi(optarg); 
                break;
            case 'r': cfg.maxRetry = std::stoi(optarg); break;
            case 'e': cfg.errRate = std::stoi(optarg); break;
            case 'H': print_usage(argv[0]); return 0;
            default: print_usage(argv[0]); return 1;
        }
    }

    return 0;
}



int main(int argc, char* argv[]){
    Config cfg;
    if (parseArguments(argc, argv, cfg) != 0) return 1;

    cout << cfg.toString() << endl;

    cout << "Starting server on " << cfg.hostname << ":" << cfg.port << "..." << endl;
    cout << "Initializing socket..." << endl;
    Socket sock(cfg.port, cfg.hostname);
    cout << "Initializing service handler and loading server configuration..." << endl;
    Handler handler(sock);
    serverConfig config = {
        .semanticInvoc = cfg.semanticInvoc,
        .maxRetry = cfg.maxRetry,
        .errRate = cfg.errRate,
        .socket = sock,
        .handler = handler
    };
    UDPServer server(config);

    cout << "Server is running and ready to accept requests." << endl;

    server.startServer();

    cout << "Server has stopped." << endl;
    
    
    
    // vector<CachedReply> cache;

    // do {
    //     char buffer[BUFFER_SIZE];

    //     sockaddr_in clientaddr;
    //     sock.receive(buffer, clientaddr);

    //     printf("Message received\n");

    //     int32_t reqID = Marshalling::unmarshalInt32(buffer);
    //     int32_t serviceID = Marshalling::unmarshalInt32(buffer + 4);

    //     printf("[Client %s port %d] Packet received\n",
    //            inet_ntoa(clientaddr.sin_addr),
    //            ntohs(clientaddr.sin_port));

    //     if(serviceID == 1 || serviceID == 2 || serviceID == 3 || serviceID == 4){
    //         int idx = findCachedReply(cache, reqID, clientaddr);

    //         if(idx != -1){
    //             printf("Duplicate request detected. Resending cached response.\n");
    //             sock.send(cache[idx].response, cache[idx].length, clientaddr);
    //             continue;
    //         }
    //     }

    //     char respond[BUFFER_SIZE];
    //     int length = 0;

    //     int parseCounter = 0;
    //     Request request = PacketBuilder::parseRequestPacket(buffer, parseCounter);
    //     Response response = handler.handle_service(request, clientaddr);
    //     int32_t statusCode = response.statusCode;

    //     if(serviceID == 1 || serviceID == 2 || serviceID == 3 || serviceID == 4){
    //         CachedReply entry;
    //         entry.reqID = reqID;
    //         entry.clientIP = inet_ntoa(clientaddr.sin_addr);
    //         entry.clientPort = ntohs(clientaddr.sin_port);
    //         memcpy(entry.response, respond, length);
    //         entry.length = length;
    //         cache.push_back(entry);
    //     }

    //     char sendBuffer[BUFFER_SIZE];
    //     int counter = 0;

    //     // reqID
    //     Marshalling::marshal(sendBuffer + counter, reqID);
    //     counter += 4;

    //     // serviceID
    //     Marshalling::marshal(sendBuffer + counter, serviceID);
    //     counter += 4;

    //     // statusCode
    //     Marshalling::marshal(sendBuffer + counter, statusCode);
    //     counter += 4;

    //     // payload
    //     memcpy(sendBuffer + counter, respond, length);
    //     counter += length;

    //     sock.send(sendBuffer, counter, clientaddr);

    //     printf("Response message sent.\n");

    // } while (true);
}