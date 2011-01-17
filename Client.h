#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifndef client_class
#define client_class

class client 
{
  public:
    client (std::string, int, std::string);

    std::string ip;
    std::string task;
    std::string number;
    int port;

    std::string register_in_server(void);
    std::string accept_data();
    std::string send_data(std::string);
};

#endif
