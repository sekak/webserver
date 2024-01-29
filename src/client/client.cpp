#include "client.hpp"


Client::Client(): _isFinished(0) , _lengthPost(-1) {}

Client::~Client() {}

void Client::setRequest(string req)
{
    
    cout << "\nfinished " << _isFinished << std::endl;
    _request.append(req);

    if(req.find("\r\n\r\n") != string::npos && _request.find("GET") != string::npos)
        _isFinished  = 1;
    else if(_request.find("POST") != string::npos)
    {
            stringstream ss(_request);
            string str;
            while(!ss.eof())
            {
                ss >> str;
                if(str == "Content-Length:")
                {
                    ss >> str;
                    _lengthPost = stoi(str);
                    break;
                }
            }
    }
    if(_lengthPost != -1 && _request.find("\r\n\r\n") !=string::npos)
    {
        int i = _request.find("\r\n\r\n");
        std::string str =  _request.substr(i + 4);
        if(_lengthPost == str.size())
        {
            cout << _lengthPost <<"    "<< str.size();
            _isFinished = 1;
        }
    }
}

string Client::getRequest()
{
    return(_request);
}


void          Client::reuseBuffer()
{
        _request.clear();
}
