#include "server/MonitorManager.h"
#include "server/net/PacketBuilder.h"
#include "server/protocol/Packet.h"


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

    MonitorClient newClient;
    newClient.addr = clientAddr;
    newClient.expiryTime = expiry;
    clients.push_back(newClient);
}

void MonitorManager::removeExpiredClients() {
    std::time_t now = std::time(nullptr);

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
    int32_t length = PacketBuilder::buildCallbackPacket(buffer, request);
    return length;
}

void MonitorManager::notifyAll(const std::string& message) {
    removeExpiredClients();

    char packet[BUFFER_SIZE];
    int32_t length = buildCallbackPacket(packet, message);

    for (const MonitorClient& client : clients) {
        sock.send(packet, length, client.addr);
    }
}

std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() {
    return clients;
}

const std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() const {
    return clients;
}