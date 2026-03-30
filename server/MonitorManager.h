#ifndef MONITORMANAGER_H
#define MONITORMANAGER_H

#include <vector>
#include <ctime>
#include <string>
#include <netinet/in.h>
#include <cstdint>

#include "Socket.h"
#include "Constant.h"
#include "Marshalling.h"

class MonitorManager {
public:
    struct MonitorClient {
        sockaddr_in addr;
        std::time_t expiryTime;
    };

    MonitorManager();

    void registerClient(const sockaddr_in& clientAddr, int32_t intervalSeconds);
    void removeExpiredClients();

    void notifyAll(Socket& sock, const std::string& message);

    std::vector<MonitorClient>& getClients();
    const std::vector<MonitorClient>& getClients() const;

private:
    std::vector<MonitorClient> clients;

    bool isSameClient(const sockaddr_in& a, const sockaddr_in& b) const;
    int buildCallbackPacket(char* buffer, const std::string& message);
};

#endif