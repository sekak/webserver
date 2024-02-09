#include "response.hpp"
#include "../config/config.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>

Response::Response()
{
    _code_status = 0;
    _support_cgi = 0;
    _length = 0;
    _type_of_content = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8";
}
Response::~Response() {}

int Response::_len_slach(string url)
{
    int count = 0;
    int i = 0;
    while (url[i])
    {
        if (url[i] == '/')
            count++;
        i++;
    }
    return count;
}

string Response::_deletePartOfUrl(string url, int i, int len)
{
    if ((len - i) == 1)
        return ("/");
    else if ((len - i) > 1)
    {
        int c = url.find("/", len - i);
        if (c != string::npos)
        {
            return url.substr(0, c);
        }
    }
    return 0;
}

Location *Response::_check_location(Config *conf, string path)
{
    // CHECK LOCATION IF EXIST
    map<string, Location *>         location;
    int                             len;
    string                          url;

    location = conf->getLocation();
    len = 0;
    url = path;
    len = _len_slach(url);
    if (location[path])
        return location[path];
    else if (location[path + "/"])
        return location[path + "/"];
    location = conf->getLocation();
    for (int i = 0; i < len; i++)
    {
        url = _deletePartOfUrl(url, i, len);
        for (map<string, Location *>::iterator it = location.begin(); it != location.end(); it++)
        {
            if (url == it->first || url + "/" == it->first)
            {
                // cout << "url: (" << url << ")"
                //  << "    it: (" << it->first << ")\n";
                return it->second;
            }
        }
    }
    return 0;
}


//check extension for video/mp4
void    Response::check_extentsion(string name)
{
    if(name.find(".mp4") != string::npos)   
        _type_of_content = "video/mp4";
    
}

void Response::_response_part(Config *conf, int fd)
{
    // CHECK REQUEST FIRSTLY
    map<int, Request *>     request;
    Location                *location;
    string                  url;
    DIR                     *dir;
    

    request = conf->_requestOfClient;
    if (request[fd])
    {
        // CHECK LOCATION, PATH OF REQUEST IF IS SAME THING IN LOCATION
        location = _check_location(conf, request[fd]->getUrl());
        if (!location)
        {
            send(fd, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nConnection: Keep-Alive\r\nContent-Length: 18\r\n\r\n<h1>Not location</h1>", 117, 0);
            return;
        }
        // check if url found in root
        url = location->getRoot() + request[fd]->getUrl();
        check_extentsion(request[fd]->getUrl());
        dir = opendir(url.c_str());
        ifstream file(url);
        if(request[fd]->getUrl() == "/favicon.ico")
        {
            _favicon = "favicon";
            _generate_content(location, conf, fd, dir, &file, FILE);

        }
        else if (dir || file.good())
        {
            if (dir)
                if (location->getautoIndex() == "on")
                {
                    // IF WAS DIR AND (AUTOINDEX: ON)
                    _generate_content(location, conf, fd, dir, &file, FOLDER_ON);
                }
                else
                {
                    // IF WAS DIR AND (AUTOINDEX: OFF)
                    _generate_content(location, conf, fd, dir, &file, FOLDER_OFF);
                }
            else
            {
                // IF WAS FILE
                _generate_content(location, conf, fd, dir, &file, FILE);
            }
            if(request[fd]->getMethod() == "POST" && !request[fd]->getBody().empty())
            {
                // UPLOAD
                ofstream file(location->getUploadDir() + "/" + request[fd]->getFilename());
                file << request[fd]->getBody();
            }
        }
        else
        {
            _generate_content(location, conf, fd, dir, &file, ERROR_404);
            return;
        }
    }
}