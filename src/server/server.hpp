
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../config/config.hpp"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Config;

class Server
{
private:
public:
    ~Server();
    Server(Config *conf);
    void initiate_server(Config *conf);
};

#endif