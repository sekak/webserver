
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../config/config.hpp"
#include "../client/client.hpp"
// #include <cstring>
// #include <cstdlib>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>

class Config;
class Client;

class Request
{
private:
    string                          _url;
    string                          _method;
    string                          _version;
    map<string,string>              _headers;
    string                          _body;
    string                          _filename;
    void                            setMethod(string method);
    void                            setUrl(string url);
    void                            setVersion(string version);
    void                            setHeaders(string key, string value);
    void                            setBody(string body);
    void                            setFilename(string filename);

public:
    Request(Config *conf,int fd);
    ~Request();
    
    void                            parser_request(Config *conf,int fd);
    string                          getMethod();
    string                          getUrl();
    string                          getVersion();
    map<string, string>             getHeaders();
    string                          getBody();
    string                          getFilename();
    void                            showAllData();

    void     getHeadersAndLineHeader(string req);
    void     parser_post(Client *cli);
    void     parser_get(Client *cli);
};

#endif