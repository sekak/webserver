#include "client.hpp"

Client::Client(): _isFinished(0) {}

Client::~Client() {}

void Client::setRequest(string req)
{
    if(req.find("\r\n\r\n") != string::npos)
        _isFinished  = 1;
    cout << "\nfinished " << _isFinished << std::endl;
    _request.append(req);
}

string Client::getRequest()
{
    return(_request);
}


void          Client::reuseBuffer()
{
    // if(!_request.empty())
        _request.clear();
}
