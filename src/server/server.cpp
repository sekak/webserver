#include "server.hpp"

#define MAX_CLIENT 10
#define MAX_SERVER 1

Server::Server(Config *conf)
{
    initiate_server(conf);
}

Server::~Server(){}


void Server::initiate_server(Config *conf)
{
    std::map<string, Location*>     location = conf->getLocation();
    struct sockaddr_in              sockAddr[MAX_SERVER], clientAddr;
    socklen_t                       addrSize, lenCli;
    fd_set                          readfds, writefds;
    int                             serverSockets[MAX_SERVER];
    int                             clientSockets[MAX_CLIENT] = {0};
    int                             newClient;
    int                             reuse;
    int                             maxSocket;
    int                             monitor;
    int                             newSocket;
    // int t = 0;
    
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

    lenCli = sizeof(clientAddr);
    while (true)
    {

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        maxSocket = serverSockets[0];
        for (int i = 0; i < MAX_SERVER; i++)
        {
            FD_SET(serverSockets[i], &readfds);
            if (serverSockets[i] > maxSocket)
                maxSocket = serverSockets[i];
        }

        for (int i = 0; i < MAX_CLIENT; i++)
        {
            int client = clientSockets[i];
            if (client > 0)
            {
                FD_SET(client, &readfds);
                FD_SET(client, &writefds);
                if (client > maxSocket)
                    maxSocket = client;
            }
        }

        // cout << "\n\n*********select1*********\n\n";
        monitor = select(maxSocket + 1, &readfds, &writefds, NULL, NULL);
        cout << "*********select2*/*******";

        if (monitor < 0)
        {
            perror("Select");
            for (int j = 0; j < MAX_SERVER; j++)
                close(serverSockets[j]);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < MAX_SERVER; i++)
        {
            if (FD_ISSET(serverSockets[i], &readfds))
            {
                newSocket = accept(serverSockets[i], (struct sockaddr *)&clientAddr, &lenCli);
                if (newSocket < 0)
                {
                    perror("Select");
                    for (int j = 0; j < MAX_SERVER; i++)
                        close(serverSockets[j]);
                    exit(EXIT_FAILURE);
                }
                for (int j = 0; j < MAX_CLIENT; j++)
                {
                    if (clientSockets[j] == 0)
                    {
                        cout << newSocket <<"*********accept*********";
                        clientSockets[j] = newSocket;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_CLIENT; i++)
        {
            map<int,Client *>  cli  = conf->_getClientsReq();
            if (FD_ISSET(clientSockets[i], &readfds))
            {
                    cout << cli.size() << "\n";
                    if(conf->_setClientReq(clientSockets[i]))
                    {
                        cli.erase(clientSockets[i]);
                        close(clientSockets[i]);
                        clientSockets[i] = 0;
                        break;
                    }
            }
            else if (FD_ISSET(clientSockets[i], &writefds) && !cli.empty() && cli[clientSockets[i]]->_isFinished)
            {
                cout << "\n*********check*********" <<  "\n";
                send(clientSockets[i], "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: Keep-Alive\r\nContent-Length: 14\r\n\r\n<h1>hello</h1>", 103, 0);
            }
        }
        
    }
}