#include "server.hpp"
#include <fcntl.h>
#include <poll.h>

#define MAX_CLIENT 10
#define MAX_SERVER 1

Server::Server(Config *conf)
{
    initiate_server(conf);
}

Server::~Server() {}

void Server::initiate_server(Config *conf)
{
    std::map<string, Location *> location = conf->getLocation();
    struct sockaddr_in              sockAddr[MAX_SERVER], clientAddr;
    socklen_t                       addrSize, lenCli;
    int                             serverSockets[MAX_SERVER];
    int                             reuse;


    addrSize = sizeof(sockAddr);
    // CREATE SOCKET SERVER
    for (int i = 0; i < MAX_SERVER; i++)
    {
        serverSockets[i] = socket(AF_INET, SOCK_STREAM, 0); // 0 default to os take protocol
        if (serverSockets[i] < 0)
        {
            perror("Socket");
            exit(EXIT_FAILURE);
        }
        // FORCE TO REUSABLE ADDR
        reuse = 1;
        if (setsockopt(serverSockets[i], SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        // ASSIGN ADDR AND PORT TO SOCKET SERVER
        sockAddr[i].sin_port = htons(stoi(conf->getPort()));
        sockAddr[i].sin_addr.s_addr = inet_addr(conf->getIp().c_str());
        sockAddr[i].sin_family = AF_INET;
        if ((bind(serverSockets[i], (struct sockaddr *)&sockAddr[i], addrSize) < 0)) // cast sockaddr to sockaddr_in that mean and it's
        {                                                                            // a way to reuse memory for different types.
            perror("Bind");
            for (int j = 0; j < MAX_SERVER; j++)
                close(serverSockets[j]);
            exit(EXIT_FAILURE);
        }
        char ip[10];
        inet_ntop(AF_INET, &sockAddr[i].sin_addr.s_addr, ip, 10);
        cout << "Server listening on port: " << ntohs(sockAddr[i].sin_port) << " [" << ip << "]\n";
        // LISTEN TO INCOMING CONNECTION
        if (listen(serverSockets[i], MAX_CLIENT) < 0)
        {
            perror("Listen");
            for (int j = 0; j < MAX_SERVER; i++)
                close(serverSockets[j]);
            exit(EXIT_FAILURE);
        }
    }

    string str;
    lenCli = sizeof(clientAddr);
    vector<pollfd> fds(1);
    fds[0].events = POLLIN;
    fds[0].fd = serverSockets[0];

    while (true)
    {
        int activity = poll(fds.data(), fds.size(), -1);
        if (activity < 0)
        {
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            int newSocket = accept(serverSockets[0], (struct sockaddr *)&clientAddr, &lenCli);
            if (newSocket < 0)
            {
                perror("accept");
                break;
            }
            conf->_clients[newSocket] = new Client;
            fds.push_back({newSocket, POLLIN | POLLOUT});
            cout << "Add  New Socket: [" << newSocket << "]" << endl;
        }
        // string res;
        for (size_t i = 1; i < fds.size(); i++)
        {
            int fd = fds[i].fd;
            if (fds[i].revents & POLLIN)
            {
                conf->_clients[fd]->setRequest(fd, conf);
                if (conf->_clients[fd]->reset_values)
                {
                    close(fd);
                    fds.erase(fds.begin() + i);
                    conf->_clients[fd]->_FullRequest.clear();
                    cout << fd << " Connection closed \n";
                    --i;
                }
                
            }
            else if (conf->_clients[fd] && conf->_clients[fd]->_isFinished == 1)
            {
                Response   response;
                cout << "POLLOUT\n";
                response._response_part(conf, fd);
                response._send(conf,fd);
                conf->_clients[fd]->_FullRequest.clear(); // clear request to get new
                conf->_clients[fd]->_isFinished = 0;
            }
                
        }
    }
}