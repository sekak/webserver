#include "../config/config.hpp"

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void Response::_send_reponse(string content, int sd)
{
    send(sd, content.c_str(), content.size(), 0);
}

string generate_header(string status, string type)
{
    string header;
    header = "HTTP/1.1 " + status + "\r\nContent-Type: " + type + "\r\nServer: WebServ/1.1\r\nConnection: Keep-Alive\r\n";
    return header;
}

string Response::_generate_code(Config *conf, int sd, string content, vector<string> vec, string status, string type)
{
    string                  slach;
    stringstream            ss;
    int                     i;

    i = 0;
    // GENERATE MAINLY HEADER 
    ss << generate_header(status, type);
    if (!vec.empty())
    {
        string tmp;
        while (vec.size() > i)
        {
            if(conf->_requests[sd]->getUrl().size() > 1)
                    slach = "/";
            else
                slach = "";
            tmp += "<a href=\"" + conf->_requests[sd]->getUrl() + slach  + vec[i] + "\">" + vec[i] + "</a>\n";
            i++;
        }
        ss << "Content-Length: " << tmp.size() << "\r\n\r\n";
        ss << tmp;
    }
    else
    {
        ss << "Content-Length: " << content.size() 
        << "\r\n\r\n" << content;
    }
    return (ss.str());
}

string Response::_code_status(int code)
{
    if (code == 200)
        return ("200 OK");
    if (code == 500)
        return ("500 Internal Server Error");
    return "404 Not Found";
}

string Response::_content_type(string type_con)
{
    if (type_con == "videos")
        return ("videos/*");
    else if (type_con == "image")
        return ("image/*");
    return ("text/html");
}


// GENERATE AND RETURN REPONSE FOR CLIENT
void Response::_response_get_(Config *conf, int sd, vector<string> vec, int code)
{
    string content = _generate_code(conf, sd, "", vec, _code_status(code), _content_type("text"));
    _send_reponse(content, sd);
}

// GENERATE AND RETURN REPONSE FOR CLIENT (CASE INDEX)
void Response::_response_index_(Config *conf, int sd, string str, int code)
{
    vector<string> vec;
    ifstream file(str);
    if (file.good())
    {
        stringstream ss;
        ss << file.rdbuf();
        string str1 = ss.str();
        string content = _generate_code(conf, sd, str1, vec, _code_status(code), _content_type("text"));
        _send_reponse(content, sd);
    }
    
}

// GENERATE AND RETURN REPONSE FOR CLIENT (CASE ERRORS)
void Response::_response_errors_(Config *conf, int sd, int code)
{
    map<string, string> errors;
    stringstream sa;
    vector<string> vec;

    errors = conf->getErrors();
    ifstream errFile(errors[to_string(code)]);

    if (errFile.good())
    {
        sa << errFile.rdbuf();
        string content = sa.str();
        string res = _generate_code(conf, sd, content, vec, _code_status(code), _content_type("text"));
        _send_reponse(res, sd);
    }
    else
    {
        ifstream err_File("errors/" + to_string(code) + ".html");
        sa << err_File.rdbuf();
        string content = sa.str();
        string res = _generate_code(conf, sd, content, vec, _code_status(code), _content_type("text"));
        _send_reponse(res, sd);
    }
}
