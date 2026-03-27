#ifndef MONITORMANAGER_H
#define MONITORMANAGER_H

#include <vector>
#include <ctime>
#include <cstdint>
#include <netinet/in.h>

class MonitorManager {
public:
    struct MonitorClient {
        sockaddr_in addr;
        std::time_t expiryTime;
    };

private:
    std::vector<MonitorClient> clients;

public:
    MonitorManager();

    void registerClient(const sockaddr_in& clientAddr, int32_t intervalSeconds);
    void removeExpiredClients();

    std::vector<MonitorClient>& getClients();
    const std::vector<MonitorClient>& getClients() const;

    bool isSameClient(const sockaddr_in& a, const sockaddr_in& b) const;
};

#endif