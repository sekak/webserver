#include "./server/server.hpp"

#define MAX_CLIENT 10
#define MAX_SERVER 3

int main()
{
   Config conf;
   Server server(&conf);
}

// int main()
// {
//     int serverSocket , clientSocket;
//     struct sockaddr_in serverSocketser;
//     struct sockaddr_in sockClient;
//     fd_set fds, writefds, readfds;
//     int clientSockets[MAX_CLIENT] = {0};

//     socklen_t addrSize = sizeof(serverSocketser);
//     // CREATE SOCKET SERVER
//     if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
//     {
//         perror("socket");
//         exit(EXIT_FAILURE);
//     }
//     if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
//     {
//         perror("socket");
//         exit(EXIT_FAILURE);
//     }

//     int reuse = 1;
//     if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
//     {
//         perror("setsockopt");
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     } if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
//     {
//         perror("setsockopt");
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }

//     // SETUP SERVER ADDRESS
//     serverSocketser.sin_family = AF_INET;
//     serverSocketser.sin_addr.s_addr = INADDR_ANY;
//     serverSocketser.sin_port = htons(8080);

//     sockClient.sin_family = AF_INET;
//     sockClient.sin_addr.s_addr = INADDR_ANY;
//     sockClient.sin_port = htons(5000);
//     // RESUSE ADD

//     if (bind(serverSocket, (struct sockaddr *)&serverSocketser, sizeof(serverSocketser)) == -1)
//     {
//         perror("bind5");
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }
//     if (bind(clientSocket, (struct sockaddr *)&sockClient, sizeof(sockClient)) == -1)
//     {
//         perror("bind");
//         close(clientSocket);
//         exit(EXIT_FAILURE);
//     }

//     // LISTEN FOR INCOMING CONNECTION
//     char ip[50];
//     inet_ntop(AF_INET, &serverSocketser, ip, addrSize);
//     cout << "IP address: " << ip << "\n";
//     cout << "PORT: " << ntohs(serverSocketser.sin_port) << "\n";
//     cout << "Server listening on port " << ntohs(serverSocketser.sin_port) << " ....\n";
//     cout << "im here";
//     cout << serverSocket;
//     if (listen(serverSocket, 5) < 0)
//     {
//         perror("listen");
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }
//     char ip1[50];
//     inet_ntop(AF_INET, &sockClient, ip1, addrSize);
//     cout << "IP1 address: " << ip << "\n";
//     cout << "PORT: " << ntohs(sockClient.sin_port) << "\n";
//     cout << "Server listening on port " << ntohs(sockClient.sin_port) << " ....\n";
//     cout << "im here";
//     if (listen(clientSocket, 5) < 0)
//     {
//         perror("listen");
//         close(serverSocket);
//         exit(EXIT_FAILURE);
//     }
//     cout << "liset";

//     // int newSocket = accept(serverSocket, (struct sockaddr *)&serverSocketser, &addrSize);
//     // if (newSocket == -1)
//     // {
//     //     perror("accept");
//     // }
//     // else
//     // {
//     //     cout << "New connection socket: " << newSocket << "\n";
//     //     for (int i = 0; i < MAX_CLIENT; i++)
//     //     {
//     //         int client = clientSockets[i];
//     //         if (client == 0)
//     //         {
//     //             clientSockets[i] = newSocket;
//     //             break;
//     //         }
//     //     }
//     // }
//     // GET ADDRESS,PORT THAT ASSIGN FOR SOCKET

//     while (true)
//     {
//         FD_ZERO(&fds);
//         FD_ZERO(&writefds);

//         FD_SET(serverSocket, &fds);

//         int maxSocket = serverSocket;
//         for (int i = 0; i < MAX_CLIENT; i++)
//         {
//             int client = clientSockets[i];
//             if (client > 0)
//             {
//                 FD_SET(client, &fds);
//                 if (client > serverSocket)
//                     maxSocket = client;
//             }
//         }
//         struct timeval timeout;
//         timeout.tv_sec = 1; // 5 seconds timeout
//         timeout.tv_usec = 0;
//         int result = select(maxSocket + 1, &fds, &writefds, NULL, &timeout);
//         if (result == -1)
//         {
//             perror("select");
//         }

//         if (FD_ISSET(serverSocket, &fds))
//         {
//             int newSocket = accept(serverSocket, (struct sockaddr *)&serverSocketser, &addrSize);
//             if (newSocket == -1)
//             {
//                 perror("accept");
//             }
//             else
//             {
//                 cout << "New connection socket: " << newSocket << "\n";
//                 for (int i = 0; i < MAX_CLIENT; i++)
//                 {
//                     int client = clientSockets[i];
//                     if (client == 0)
//                     {
//                         clientSockets[i] = newSocket;
//                         break;
//                     }
//                 }
//             }
//         }

//         for (int i = 0; i < MAX_CLIENT; i++)
//         {
//             int client = clientSockets[i];
//             if (FD_ISSET(client, &fds))
//             {
//                 char buf[5000];
//                 int result = read(client, buf, 5000);
//                 if (result == 0)
//                 {
//                     cout << "Client that socket: " << client << " closed\n";
//                     close(client);
//                     clientSockets[i] = 0;
//                 }
//                 else if (result > 0)
//                 {
//                     cout << "Recieved request from: " << client << "\n";
//                     cout << "       || \n";
//                     cout << "       || \n";
//                     cout << "       \\/ \n";
//                     cout << buf << "\n";
//                     cout << "       /\\ \n";
//                     cout << "       || \n";
//                     cout << "       || \n";
//                 }
//             }
//         }
//     }
// }

//     while (true) {
//         FD_ZERO(&readfds);
//         FD_SET(serverSocket, &readfds);

//         int maxSocket = serverSocket;
//         for (int i = 0; i < MAX_CLIENTS; ++i) {
//             int clientSocket = clientSockets[i];
//             if (clientSocket > 0) {
//                 FD_SET(clientSocket, &readfds);
//                 if (clientSocket > maxSocket) {
//                     maxSocket = clientSocket;
//                 }
//             }
//         }

//         // Use select to monitor sockets for reading
//         fd_set write;
//         if (select(maxSocket + 1, &readfds, &write, nullptr, nullptr) == -1) {
//             perror("Select failed");
//             exit(EXIT_FAILURE);
//         }

//         // Check if there is a new incoming connection
//         if (FD_ISSET(serverSocket, &readfds)) {
//             if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) == -1) {
//                 perror("Accept failed");
//                 exit(EXIT_FAILURE);
//             }

//             std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " on port " << ntohs(clientAddr.sin_port) << std::endl;

//             // Add the new socket to the array of client sockets
//             for (int i = 0; i < MAX_CLIENTS; ++i) {
//                 if (clientSockets[i] == 0) {
//                     clientSockets[i] = newSocket;
//                     break;
//                 }
//             }
//         }

//         // Check data from clients
//         for (int i = 0; i < MAX_CLIENTS; ++i) {
//             int clientSocket = clientSockets[i];
//             if (FD_ISSET(clientSocket, &readfds)) {
//                 char buffer[BUFFER_SIZE] = {0};
//                 ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
//                 if (bytesRead <= 0) {
//                     // Connection closed or error
//                     std::cout << "Connection from socket " << clientSocket << " closed" << std::endl;
//                     close(clientSocket);
//                     clientSockets[i] = 0;
//                 } else {
//                     std::cout << "f";
//                     // Process the received data and send a response
//                     std::cout << "Received from socket " << clientSocket << ": \n" << buffer << std::endl;
//                     std::cout << "send => \n";
//                     std::string str = buffer;
//                     if(str.find("/favicon.ico") != std::string::npos)
//                         send(clientSocket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: Keep-Alive\r\nContent-Length: 76\r\n\r\n<html><head><title>check</title></head></html>", 135, 0);
//                     else
//                     send(clientSocket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: Keep-Alive\r\nContent-Length: 14\r\n\r\n<h1>hello</h1>", 103, 0);
//                 // <html>
//                 }
//             }
//         }

//            for (int i = 0; i < MAX_CLIENTS; ++i) {
//             int clientSocket = clientSockets[i];
//             if (FD_ISSET(clientSocket, &write)) {
//                     send(clientSocket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: Keep-Alive\r\nContent-Length: 14\r\n\r\n<h1>hello</h1>", 103, 0);
//             }
//         }
//     }

//     return 0;
// }
