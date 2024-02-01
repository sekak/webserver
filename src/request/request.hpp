
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../config/config.hpp"
// #include <cstring>
// #include <cstdlib>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>

// class Config;

class Request
{
private:
    string                          _url;
    string                          _method;
    string                          _version;
    map<string,string>              _headers;
    string                          _body;
    void                            setMethod(string method);
    void                            setUrl(string url);
    void                            setVersion(string version);
    void                            setHeaders(string key, string value);
    void                            setBody(string body);

public:
    Request(string req);
    ~Request();
    
    void                            parser_request(string req);
    string                          getMethod();
    string                          getUrl();
    string                          getVersion();
    map<string, string>             getHeaders();
    string                          getBody();
    void                            showAllData();

};

#endif