#include "client.hpp"

Client::Client() : _isFinished(0), _lengthPost(-1)
{
    _FullRequest = "";
    content_length = 0;
    reset_values = 0;
}

Client::~Client() {}

void Client::setRequest(int fd, Config *conf)
{
    int i = 0;
    char buf[BUFFER_SIZE];
    ofstream file("file"); // remove
    int byte = read(fd, buf, BUFFER_SIZE - 1);
    if (byte == 0 || byte < 0)
    {
        conf->_clients[fd]->reset_values = 1;
    }
    int content = 0;
    conf->_clients[fd]->_FullRequest.append(buf, byte);
    file << conf->_clients[fd]->_FullRequest; // remove
    int start_length = conf->_clients[fd]->_FullRequest.find("Content-Length: ");
    if (start_length != string::npos && content == 0)
    {
        while (conf->_clients[fd]->_FullRequest[start_length + 16 + i] != '\r')
        {
            content *= 10;
            content += (conf->_clients[fd]->_FullRequest[start_length + 16 + i] - '0');
            // cout << "===> " << content << endl; // remove
            i++;
        }
        conf->_clients[fd]->content_length = content;
        // content = 0;
    }
    if (conf->_clients[fd]->_FullRequest.find("\r\n\r\n-----") != string::npos && conf->_clients[fd]->_FullRequest.find("POST ") != string::npos)
    {
        int start = conf->_clients[fd]->_FullRequest.find("\r\n\r\n-----");
        conf->_clients[fd]->content_length = conf->_clients[fd]->_FullRequest.substr(start + 4).size();
        if (content == conf->_clients[fd]->content_length)
        {
            conf->_clients[fd]->_isFinished = 1;
            conf->_clients[fd]->_method = "POST";
            conf->_requestOfClient[fd] = new Request(conf, fd);
        }
        cout << "post " << conf->_clients[fd]->content_length << endl;
    }
    else if (conf->_clients[fd]->_FullRequest.find("\r\n\r\n") != string::npos && conf->_clients[fd]->_FullRequest.find("GET ") != string::npos)
    {
        conf->_clients[fd]->_isFinished = 1;
        conf->_clients[fd]->_method = "GET";
        conf->_requestOfClient[fd] = new Request(conf, fd);
    }
    else if (conf->_clients[fd]->_FullRequest.find("\r\n\r\n") != string::npos)
    {
        int start = conf->_clients[fd]->_FullRequest.find("\r\n\r\n");
        int size = conf->_clients[fd]->_FullRequest.substr(start + 4).size();
        if (size == conf->_clients[fd]->content_length)
        {
            cout << size << " " << conf->_clients[fd]->content_length << " other\n";
            conf->_clients[fd]->_isFinished = 1;
            conf->_clients[fd]->_method = "GET";
            conf->_requestOfClient[fd] = new Request(conf, fd);
        }
    }
}

string Client::getRequest()
{
    return (_FullRequest);
}

void Client::reuseBuffer()
{

    _FullRequest.clear();
}
