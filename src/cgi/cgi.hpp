#ifndef CGI_HPP
#define CGI_HPP

#include "../config/config.hpp"
#include "../request/request.hpp"
#include <sys/types.h>
#include <sys/wait.h>

class Config;
class Location;

class Cgi
{
private:

public:
    Cgi();
    ~Cgi();
    string execute_cgi(Location *location,Config* conf,int fd, string url);


};


#endif