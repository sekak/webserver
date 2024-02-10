#include "../config/config.hpp"
#include "../response/response.hpp"

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>




string generate_status(int code)
{
    if(code == 404)
        return "404 Not Found";
    else if(code == 500)
        return "500 Interval Server Error";
    else
        return "200 OK";
}

string Response::generate_header(Config *config, int fd)
{
    stringstream ss;
    int size = _content.size();
    string  code = generate_status(_code_status);
    ss <<  "HTTP/1.1 " << code  << "\r\nContent-Length: " << size << "\r\nServer: webserver/0.1\r\nContent-Type: "<< _type_of_content <<"\r\nConnection: Keep-Alive\r\n\r\n"; 
    return ss.str();
}

string generate_header(string status, string type)
{
    string header;
    header = "HTTP/1.1 " + status + "\r\nContent-Type: " + type + "\r\nServer: WebServ/1.1\r\nConnection: Keep-Alive\r\n";
    return header;
}

// ==>> SEND
void Response::_send(Config *conf, int fd)
{
    string header =  generate_header(conf, fd);
    header += _content;
    int size = header.size();
    send(fd, header.c_str(), size, 0);
}

void Response::generate_html_autoindex(Config *conf, int fd)
{
    string                  slach;
    stringstream            ss;
    int                     i;
    string                  tmp;

    i = 0;
    ifstream file("auto.html");
    if(file.good())
    {
        ss << file.rdbuf();
        tmp = ss.str();
    }
    while (_content_autoIndex.size() > i)
    {
        if(conf->_requestOfClient[fd]->getUrl().size() > 1)
                slach = "/";
        else
            slach = "";
        tmp += "<div class=\"item\"><span class=\"name\">" + _content_autoIndex[i] + "</span><a class=\"url\" href=\"" + conf->_requestOfClient[fd]->getUrl() + slach  + _content_autoIndex[i] + "\"> Visit </a> </div>\n";
        i++;
    }
    tmp += " </div></div></body></html>";
    _content = tmp;
}


// GENERATE AND RETURN REPONSE FOR CLIENT (CASE ERRORS)
void Response::_response_errors_(Config *conf, int sd)
{
    map<string, string> errors;
    stringstream sa;
    vector<string> vec;
    errors = conf->getErrors();
    ifstream errFile(errors[to_string(_code_status)]);
    ifstream err_File("errors/" + to_string(_code_status) + ".html");

    if (errFile.good())
    {
        sa << errFile.rdbuf();
        _content = sa.str();
    }
    else if(err_File.good())
    {
        sa << err_File.rdbuf();
        _content = sa.str();
    }
    else
        _content = "<h1>Not Found fuck</h1>";
}


void  Response::content_file(ifstream *file)
{
    string          res;
    stringstream    ss;
    if(_favicon == "favicon")
    {
        ifstream file1("favicon.ico");
        ss <<  file1.rdbuf();
        res = ss.str();
        _content = res;
        _favicon = "";
        return;
    }
    ss << file->rdbuf();
    res = ss.str();
    _content = res;
}


void Response::content_autoIndex(Config* conf, int fd,DIR *dir)
{
    dirent              *entry;

    while((entry = readdir(dir)) != NULL)
    {
        if(entry->d_name[0] != '.')
            _content_autoIndex.push_back(entry->d_name);
    }
    if(!_content_autoIndex.empty())
        generate_html_autoindex(conf, fd);
}

void Response::_generate_index(Location *location)
{
    vector<string> indexes = location->getIndexes();
    for (int i = 0; i < indexes.size() && !indexes.empty(); i++)
    {
        ifstream file(location->getRoot() + "/" + indexes[i]);
        if (file.good())
            _file_index = location->getRoot() + "/" + indexes[i];
    }
    if(_file_index.empty())
        _code_status = ERROR_404;
    else 
    {
        ifstream file(_file_index);
        content_file(&file);
    }
}

void Response::_check_cgi(Location *location, string url)
{
    vector<string> vec = location->getCgi();
    if(!vec.empty())
    {
        if(url.find(".php") != string::npos && vec[0] == ".php")
            _support_cgi = 1;
        else if(url.find(".py") != string::npos && vec[0] == ".py")
            _support_cgi = 1;
    }
}

void        Response::_generate_content(Location *location, Config* conf,int fd,DIR *dir,ifstream *file,int type)
{
    string                  url;
    map<int, Request *>     request;

    request = conf->_requestOfClient;
    url = location->getRoot() + request[fd]->getUrl();
    // request[fd]->showAllData();
    if(type == FILE)
    {
        //check extension after
            _check_cgi(location, url);
        if((url.find(".php") != string::npos || url.find(".py") != string::npos) && _support_cgi)
        {
            Cgi cgi;
            _content = (cgi.execute_cgi(location, conf, fd, url));
            if(_content == "error")
            {
                _code_status = 500; 
                _response_errors_(conf, fd);
            }
        }
        else
            content_file(file);
    }
    else if(type == FOLDER_ON)
    {
        // cout << "generate_content = folder_on\n";
        content_autoIndex(conf, fd, dir);
    }
    else if(type == FOLDER_OFF)
    {
        // cout << "generate_content = folder_off\n";
        _generate_index(location);
    }
    else 
    {
        _code_status = type; 
        _response_errors_(conf, fd);
    }
}
 