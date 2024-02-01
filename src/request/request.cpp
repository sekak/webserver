#include "./request.hpp"
#include <cstring>
#include <unistd.h>


Request::Request(string req)
{
    // cout << req;
    parser_request(req);
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

void        Request::parser_request(string req)
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
    while(getline(ss, line))
    {
        setBody(line + "\n");
        // cout << line<<endl;
    }
    
    // cout << getMethod() << endl;
    // cout << getUrl() << endl;
    // cout << getVersion() << endl;
    // map<string, string> head = getHeaders();
    // for (map<string, string>::iterator it = head.begin();
    // it != head.end(); it++)
    // {
    //     cout << it->first << " = " << it->second << endl;
    // }
    // cout << "body = > \n";
    // cout << getBody();             
}   


void Request::showAllData()
{
    cout << getMethod() << endl;
    cout << getUrl() << endl;
    cout << getVersion() << endl;
    map<string, string> head = getHeaders();
    for (map<string, string>::iterator it = head.begin();
    it != head.end(); it++)
    {
        cout << it->first << " = " << it->second << endl;
    }
    cout << "body = > \n";
    cout << getBody();
}
