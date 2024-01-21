#include <iostream>
#include <string>
#include "./config/config.hpp"
#include "./location/location.hpp"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;

// void print_config()
// {
//     Config conf;
//     conf.parse_config();
//     cout <<  conf.getServName() << "\n";
//     cout << conf.getPort() << "\n";
//     cout << conf.getIp() << "\n";
//     try{

//     map<string, string> error = conf.getErrors();
//     cout << error["403"] << "\n";
//     cout << error["404"] << "\n";
//     cout << error["405"] << "\n";
//     cout << error["413"] << "\n";
//     cout << error["500"] << "\n";
//     // cout <<  << "\n";
//     }
//     catch(exception &error)
//     {
//         cout << error.what();
//     }

//     map<string,Location *>  loca = conf.getLocation();
//     cout << loca["/potato/"]->getautoIndex();
//     cout << loca["/"]->getautoIndex();

// }
int main()
{
    int socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server < 0)
    {
        cerr << "failed to create socket server\n";
        return 1; // Return an error code
    }

    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(5000);
    server_add.sin_addr.s_addr = inet_addr("127.0.0.2");
    int reuse = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        cerr << "failed to set socket options\n";
        close(socket_server);
        return 1; // Return an error code
    }
    // Check for bind error
    if (bind(socket_server, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        cerr << "failed to bind socket server\n";
        close(socket_server);
        return 1; // Return an error code
    }

    // show ip and port that assigned to the socket
    char ip_add[INET_ADDRSTRLEN]; // INET_ADDRSTRLEN is the maximum length of an IPv4 address
    inet_ntop(AF_INET, &server_add.sin_addr, ip_add, sizeof(ip_add));
    int port_add = ntohs(server_add.sin_port);
    cout << "IP Address: " << ip_add << "\n";
    cout << "Port: " << port_add << "\n";

    listen(socket_server, 10);

    while(1)
    {

    sockaddr_in client_add;
    socklen_t add_len = sizeof(client_add);
    int client_socket = accept(socket_server, (struct sockaddr *)&client_add, &add_len);
    if (client_socket < 0)
    {
        cerr << "failed to accept connection\n";
        close(socket_server);
        return 1; // Return an error code
    }

    // Continue with your code...
    char buffer[1000];
    int flags = fcntl(client_socket, F_GETFL, 0);
if (flags == -1) {
    cerr << "Error getting socket flags\n";
    close(client_socket);
    return 1;
}

if (fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
    cerr << "Error setting socket to non-blocking mode\n";
    close(client_socket);
    return 1;
}

// Read with non-blocking socket
ssize_t bytesRead = read(client_socket, buffer, sizeof(buffer));

if (bytesRead > 0) {
    // Process the received data
    cout << "Received data: " << buffer << endl;
} else if (bytesRead == 0) {
    // Connection closed by the client
    cout << "Connection closed by the client" << endl;
    close(client_socket);
} else if (errno == EAGAIN || errno == EWOULDBLOCK) {
    // No data available for now
} else {
    cerr << "Error reading from client" << endl;
    close(client_socket);
}
    cout << buffer << "\n";
    
    send(client_socket,"fuck u",6,0);
    }

    close(socket_server);

    return 0; // Return 0 on success
}

// int main() {
//     // Assuming you have a sockaddr_in structure that represents the server address
//     struct sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Replace with your actual IP address
//     serverAddr.sin_port = htons(5000); // Replace with your actual port number

//     // Convert binary IP address to human-readable string
//     char ipString[INET_ADDRSTRLEN];
//     inet_ntop(AF_INET, &(serverAddr.sin_addr), ipString, INET_ADDRSTRLEN);

//     // Convert network-byte order port to host-byte order
//     int port = ntohs(serverAddr.sin_port);

//     // Print the information
//     std::cout << "Server IP: " << ipString << std::endl;
//     std::cout << "Server Port: " << port << std::endl;

//     return 0;
// }
