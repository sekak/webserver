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

#define BUFFER_SIZE 100


class Client
{
private:
    string              _request;

public:
    Client();
    ~Client();

    void                setRequest(string req);
    string              getRequest();
    bool                _isFinished;
    void                reuseBuffer();
};


#endif