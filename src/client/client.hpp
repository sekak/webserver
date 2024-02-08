#ifndef CLIENT_HPP
#define CLIENT_HPP

using namespace std;
#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <cstddef> 
#include <cstring>
#include <iterator>
#include <sstream>
#include "../config/config.hpp"
#include "../request/request.hpp"

// #define BUFFER_SIZE 5000

class Config;
class Request;

class Client
{
private:

public:
    Client();
    ~Client();
    string              _FullRequest;

    bool                _isFinished;
    int                 _lengthPost;
    string              getRequest();
    void                setRequest(int fd, Config *conf);
    void                reuseBuffer();
    int                 content_length;
    int                 reset_values;
    string              _method;
    string              _body;


    //getter

};


#endif