#ifndef CONFIG_HPP
#define CONFIG_HPP

using namespace std;

#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <cstddef> 
#include <iterator>
#include <sstream>
#include "../location/location.hpp"


class Location; // class location each location in config

class Config
{
private:
    string                    _ip;
    string                    _port;
    map<string, string>       _errors;
    map<string,Location *>    _locations; // <<=== location ===>>
    string                    _server_name;
    int                       _count_braces;
    //setters
    void                      setIp(string Ip);  
    void                      setPort(string port);
    void                      setServName(string name);
    void                      setErrors(string path, string status_code);
    void                      setLocation(string path, Location *location);
    
    

public:
    Config();
    ~Config();
    //getters
    string                     getIp();
    string                     getPort();
    map<string, string>        getErrors();
    map<string,Location *>     getLocation();
    string                     getServName();

    void                       parse_config();
    string                     check_is_file();
    void                       check_content_config();
    void                       Conf_server(string file_config);
    void                       check_location(string file_config);
};
 

std::vector<std::string> splitString(const std::string& input, const std::string& delimiter);

#endif