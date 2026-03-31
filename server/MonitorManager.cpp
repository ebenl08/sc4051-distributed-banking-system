#include "MonitorManager.h"

#include <algorithm>
#include <cstring>

MonitorManager::MonitorManager() {}

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

int MonitorManager::buildCallbackPacket(char* buffer, const std::string& message) {
    int counter = 0;

    // reqID = 0 for server callback
    Marshalling::marshal(buffer + counter, (int32_t)0);
    counter += 4;

    // serviceID = 0xFFFF means monitor callback
    Marshalling::marshal(buffer + counter, (int32_t)0xFFFF);
    counter += 4;

    // statusCode = SUCCESS
    Marshalling::marshal(buffer + counter, (int32_t)0);
    counter += 4;

    // payload = string message
    Marshalling::marshal(buffer + counter, message.c_str());
    counter += 4 + message.length();

    return counter;
}

void MonitorManager::notifyAll(Socket& sock, const std::string& message) {
    removeExpiredClients();

    char packet[BUFFER_SIZE];
    int length = buildCallbackPacket(packet, message);

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