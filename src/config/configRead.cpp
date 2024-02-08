#include "./config.hpp"



int Config::_setClientReq(int sd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int byte_read = read(sd, buffer, BUFFER_SIZE - 1);
    if (byte_read == 0 || byte_read == -1)
    {
        cout << "Clients [" << sd << "] disconnected\n";
        return 1;
    }
    if(_clients[sd])
    {
        cout << "read data\n";
        _clients[sd]->_FullRequest.append(buffer, byte_read);
    }
    else
    {
        _clients[sd] = new Client;
        _clients[sd]->_FullRequest.append(buffer, byte_read);
    }
    // if(_clients[sd]->_FullRequest.)
    // {

    // }
    // // i = byte_read;
    // cout << _s << endl;

    // if(_clients[sd])
    // {

    // }else
    // {
    //     _clients[sd] = new Client;
    //     _clients[sd]->setRequest(buffer)
    // }
    // if (_clients[sd])
    // {
    //     if (_clients[sd]->_isFinished)
    //     {
    //         _clients[sd]->reuseBuffer();
    //         _clients[sd]->_isFinished = 0;
    //         _clients[sd]->_lengthPost = -1;
    //     }
    //     _clients[sd]->setRequest(buffer, byte_read);
    //     // std::cout << "found";
    // }
    // else
    // {
    //     _clients[sd] = new Client;
    //     _clients[sd]->setRequest(buffer, byte_read);
    //     // std::cout << "not found\n";
    // }
    // if (_clients[sd]->_isFinished)
    //     cout << _clients[sd]->getRequest() << _clients[sd]->getRequest().size() << "\n\n";
    return 2;
}

// map<int, Client *> Config::_getClientsReq()
// {
//     // return (_clients);
// }