#ifndef HANDLER_H
#define HANDLER_H

#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>

#include <string>

#include "Constant.h"
#include "Account.h"
#include "AccountManager.h"
#include "Socket.h"

using namespace std;

class Handler{
  public:
    Handler();
    void setCurrentClientAddr(const sockaddr_in& clientaddr);
    void setSocket(Socket* sock);

    static int32_t service1(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service2(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service3(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service4(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service5(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service6(const std::vector<Argument>& args, char* respond, int& length);
    static int32_t service7(const std::vector<Argument>& args, char* respond, int& length);

  private:
    AccountManager accountManager;
    MonitorManager monitorManager;

    sockaddr_in currentClientAddr;
    bool hasCurrentClientAddr = false;

    Socket* serverSocket = nullptr;

    void notifyMonitorClients(const std::string& message);
};


#endif