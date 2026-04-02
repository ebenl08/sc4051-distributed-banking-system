#include "MonitorManager.h"
#include "server/net/PacketBuilder.h"
#include "server/protocol/Packet.h"
#include "server/Helper.h"


#include <algorithm>
#include <cstring>

MonitorManager::MonitorManager(Socket socket) : sock(socket) {}

bool MonitorManager::isSameClient(const sockaddr_in& a, const sockaddr_in& b) const {
    return a.sin_addr.s_addr == b.sin_addr.s_addr &&
           a.sin_port == b.sin_port;
}

void MonitorManager::registerClient(const sockaddr_in& clientAddr, int32_t intervalSeconds) {
    removeExpiredClients();

    std::time_t expiry = std::time(nullptr) + intervalSeconds;

    for (MonitorClient& client : clients) {
        if (isSameClient(client.addr, clientAddr)) {
            client.expiryTime = expiry;
            return;
        }
    }

    pinfo("REGISTERING MONITOR CLIENT");
    MonitorClient newClient;
    newClient.addr = clientAddr;
    newClient.expiryTime = expiry;
    clients.push_back(newClient);
}

void MonitorManager::removeExpiredClients() {
    std::time_t now = std::time(nullptr);

    // Collect expired clients
    std::vector<MonitorClient> expiredClients;
    for (const auto& client : clients) {
        if (client.expiryTime <= now) {
            expiredClients.push_back(client);
        }
    }

    // notify clients about their expired sessions
    if (!expiredClients.empty()) {
        pwarn ("MONITOR: Removing %zu expired monitor clients", expiredClients.size());
        Request request;
        request.reqID = 0;
        request.serviceID = SERVICE_MONITOR_END;
        request.args.push_back(Argument("A monitor client has expired and been removed."));


        char packet[BUFFER_SIZE];
        int32_t length = PacketBuilder::buildCallbackPacket(packet, request);
        pdebug("MONITOR CALLBACK END PACKET\n%s", hex_to_string(packet, length).c_str());
        for (const MonitorClient& client : clients) {
            pdebug("NOTIFYING CLIENT %s:%d OF EXPIRY", inet_ntoa(client.addr.sin_addr), ntohs(client.addr.sin_port));
            sock.send(packet, length, client.addr);
        }
        printExpiredClientsDebug(expiredClients, request);
        expiredClients.clear();
    }

    // Remove expired clients
    clients.erase(
        std::remove_if(
            clients.begin(),
            clients.end(),
            [now](const MonitorClient& client) {
                return client.expiryTime <= now;
            }
        ),
        clients.end()
    );
}

int32_t MonitorManager::buildCallbackPacket(char* buffer, const std::string& message) {
    Request request;
    request.reqID = 0;
    request.serviceID = SERVICE_MONITOR_LIVE; // Special service ID for monitor callbacks
    request.args.push_back(Argument(message.c_str()));

    pinfo("MONITOR CALLBACK MESSAGE: %s\n%s", message.c_str(), 
                hex_to_string(buffer, PacketBuilder::buildCallbackPacket(buffer, request)).c_str());
    int32_t length = PacketBuilder::buildCallbackPacket(buffer, request);
    return length;
}

void MonitorManager::notifyAll(const std::string& message) {
    removeExpiredClients();

    char packet[BUFFER_SIZE];
    
    string completeMessage = "[" + get_timestamp() + "] " + message;
    int32_t length = buildCallbackPacket(packet, completeMessage);

    if(!clients.empty()){
        pinfo("NOTIFYING MONITOR CLIENTS");
    }
    for (const MonitorClient& client : clients) {
        sock.send(packet, length, client.addr);
    }
}

void MonitorManager::printExpiredClientsDebug(const std::vector<MonitorClient>& expiredClients, const Request& request) {
    std::stringstream ss;
    ss << "MONITOR CALLBACK: The following monitor clients have expired and been removed:\n";
    for (const auto& client : expiredClients) {
        ss << "  - " << inet_ntoa(client.addr.sin_addr) << ":" << ntohs(client.addr.sin_port) << " (expiry: " << client.expiryTime << ")\n";
    }
    pdebug("%s", ss.str().c_str());
}

std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() {
    return clients;
}

const std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() const {
    return clients;
}