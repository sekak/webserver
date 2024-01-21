#include "location.hpp"

Location::Location()
{
}

Location::~Location()
{
}

// setters
void Location::setautoIndex(string is_index)
{
    _autoIndex = is_index;
}

void Location::setPath(string path)
{
    _path = path;
}

void Location::setMethods(string method)
{
    _methods.push_back(method);
}

void Location::setRoot(string root)
{
    _root = root;
}

void Location::setUploadDir(string path)
{
    _upload_dir = path;
}

void Location::setIndexes(string content)
{
    _indexes.push_back(content);
}

void Location::setCgi(string cgi)
{
    _cgi.push_back(cgi);
}

// getters

string Location::getPath()
{
    return (_path);
}

string Location::getautoIndex()
{
    return(_autoIndex);
}
string Location::getUploadDir()
{
    return(_upload_dir);
}
string Location::getRoot()
{
    return(_root);
}

vector<string> Location::getMethods()
{    
    return(_methods);
}
vector<string> Location::getIndexes()
{
    return(_indexes);
}
vector<string> Location::getCgi()
{
    return(_cgi);
}