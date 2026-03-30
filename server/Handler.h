#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>

#include <string>

#include "server/Constant.h"
#include "server/Account.h"
#include "server/AccountManager.h"
#include "server/MonitorManager.h"

#include "server/net/Socket.h"
#include "server/net/Marshalling.h"
#include "server/net/UDPServer.h"

#include "server/protocol/Args.h"
#include "server/protocol/Packet.h"

#define argvec std::vector<Argument>

using namespace std;


class Handler{
  public:
    Handler();

    Response handle_service(Request request, const sockaddr_in& clientAddr);

    argvec open_account(const argvec& args, int32_t& statusCode);
    argvec close_account(const argvec& args, int32_t& statusCode);
    argvec deposit_withdraw(const argvec& args, int32_t& statusCode);
    argvec view_account(const argvec& args, int32_t& statusCode);
    argvec transfer(const argvec& args, int32_t& statusCode);
    argvec change_password(const argvec& args, int32_t& statusCode);
    argvec register_monitor_client(const argvec& args, const sockaddr_in& clientAddr, int32_t& statusCode);

  private:
    
    // Application Layer
    AccountManager accountManager;
    MonitorManager monitorManager;
    
    // Observer
    vector<sockaddr_in> active_clients;
    void notify_monitor_clients(const std::string& message);


};


#endif