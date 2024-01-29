#include "./config.hpp"

int Config::_setClientReq(int sd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int res = read(sd, buffer, BUFFER_SIZE - 1);
    if (res == 0 || res == -1)
    {
        cout << "Clients [" << sd << "] disconnected\n";
        return 1;
    }

    if (_clients[sd])
    {
        if (_clients[sd]->_isFinished)
        {
            _clients[sd]->reuseBuffer();
            _clients[sd]->_isFinished = 0;
            _clients[sd]->_lengthPost = -1;
        }
        _clients[sd]->setRequest(buffer);
        std::cout << "found";
    }
    else
    {
        _clients[sd] = new Client;
        _clients[sd]->setRequest(buffer);
        std::cout << "not found\n";
    }
    if (_clients[sd]->_isFinished)
        cout << _clients[sd]->getRequest() << _clients[sd]->getRequest().size() << "\n\n";
    return 0;
}

map<int, Client *> Config::_getClientsReq()
{
    return (_clients);
}