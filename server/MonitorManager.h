#ifndef MONITORMANAGER_H
#define MONITORMANAGER_H

#include <vector>
#include <ctime>
#include <string>
#include <netinet/in.h>
#include <cstdint>

#include "server/Constant.h"

#include "server/net/Socket.h"
#include "server/net/Marshalling.h"
#include "server/protocol/Packet.h"

class MonitorManager {
public:

    struct MonitorClient {
        sockaddr_in addr;
        std::time_t expiryTime;
    };

    MonitorManager(Socket socket);


    void registerClient(const sockaddr_in& clientAddr, int32_t intervalSeconds);
    void removeExpiredClients();
    void printExpiredClientsDebug(const std::vector<MonitorClient>& expiredClients, const Request& request);

    void notifyAll(const std::string& message);

    std::vector<MonitorClient>& getClients();
    const std::vector<MonitorClient>& getClients() const;

private:
    std::vector<MonitorClient> clients;
    Socket sock;

    bool isSameClient(const sockaddr_in& a, const sockaddr_in& b) const;
    int32_t buildCallbackPacket(char* buffer, const std::string& message);
};

#endif