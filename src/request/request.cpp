#include "./request.hpp"
#include <cstring>
#include <unistd.h>


Request::Request(Config *conf,int fd)
{
    parser_request(conf, fd);
}

Request::~Request(){}

// setters
void Request::setMethod(string method)
{
    _method  = method;
}

void Request::setUrl(string url)
{
    _url = url;
}
void Request::setVersion(string version)
{   
        _version = version;
}
void Request::setHeaders(string key, string value)
{   
        _headers[key] = value;
}

void Request::setBody(string body)
{
    _body.append(body);
}

void Request::setFilename(string filename)
{
    _filename = filename;
}


//getters
string  Request::getMethod()
{
    return _method; 
}

string  Request::getUrl()
{
    return _url;
}

string  Request::getVersion()
{
    return _version;
}

map<string, string> Request::getHeaders()
{
    return _headers;
}

string Request::getBody()
{
    return _body;
}

string Request::getFilename()
{
    return _filename;
}



void     Request::getHeadersAndLineHeader(string req)
{
    stringstream            ss(req);
    string                  line, url, version, method;

    getline(ss, line);
    stringstream ssi(line);
    ssi >> method >> url >> version;
    setMethod(method);
    setUrl(url);
    setVersion(version);
    while (std::getline(ss, line) && line != "\r") 
    {
        string key,value;
        stringstream ssa(line);
        getline(ssa, key,':');
        getline(ssa, value);
        value.erase(0,value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \n"));
        setHeaders(key, value);
    }   
}

void Request::parser_post(Client *cli)
{
    string                      req;
    string                      file_name;
    string                      body;
    int                         i;
    
    req = cli->_FullRequest;
    if(cli->content_length > 0)
    {
        i = 0;
        int start_dispo = req.find("filename=");
        start_dispo +=10;
        if(start_dispo != string::npos)
        {
            while(req[start_dispo + i] != '\r')
            {
                if(req[start_dispo + i] != '\"')
                    file_name += req[start_dispo + i];
                i++;
            }
            start_dispo  += i;
            i = 0;
            req = req.substr(start_dispo);
            int start_body = req.find("\r\n\r\n");
            start_body +=4;
            int end_body = req.find("-----");
            if(start_body != string::npos && end_body != string::npos)
            {
                while(start_body < (end_body - 2))
                {
                    body += req[start_body];
                    start_body++;
                }
            }
        }
        setBody(body);
        setFilename(file_name);
    }
    req = cli->_FullRequest;
    getHeadersAndLineHeader(req);

}

void    Request::parser_get(Client *cli)
{
    getHeadersAndLineHeader(cli->_FullRequest);
}


void        Request::parser_request(Config *conf,int fd)
{
    Client *cli;

    cli = conf->_clients[fd];
    if(cli->_method  == "POST")
        parser_post(cli);
    else if(cli->_method == "GET")
        parser_get(cli);
    // showAllData();
}   


// SHOW DATA
void Request::showAllData()
{
    cout << "\n\n*** // * * *  SHOW ALL DATA ** * * / /\n";
    cout << getMethod() << endl;
    cout << getUrl() << endl;
    cout << getVersion() << endl;
    map<string, string> head = getHeaders();
    for (map<string, string>::iterator it = head.begin();
    it != head.end(); it++)
    {
        cout << it->first << " = " << it->second << endl;
    }
    // cout << "body = > \n";
    // cout << getBody();
    cout << "*** // * * *  FINISHED ALL DATA ** * * / /\n\n";

}
