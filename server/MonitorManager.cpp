#include "MonitorManager.h"

#include <algorithm>

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

std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() {
    return clients;
}

const std::vector<MonitorManager::MonitorClient>& MonitorManager::getClients() const {
    return clients;
}