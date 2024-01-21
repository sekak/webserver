#include "server.hpp"


Server::Server(Config &conf)
{

}

Server::~Server(){}


void Server::initiate_server(Config &conf)
{

    int socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_server < 0)
        cerr << "failed to create socket server\n";
    
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;
    server_add.sin_port = htonl(2222);
    server_add.sin_addr.s_addr = INADDR_ANY;
    bind(socket_server, (struct sockaddr*) server_add, sizeof(server_add));

    if (getsockname(socket_server, (struct sockaddr*)&server_add, &addr_len) == -1) {
        perror("Getsockname failed");
        exit(EXIT_FAILURE);
    }

    // Convert the IP address to a human-readable string
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sockaddr.sin_addr), ip_address, INET_ADDRSTRLEN);

    // Display the bound IP address and port
    printf("Server bound to IP: %s, Port: %d\n", ip_address, ntohs(sockaddr.sin_port));

}