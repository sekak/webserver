

#ifndef LOCATION_HPP
#define LOCATION_HPP

using namespace std;

#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <cstddef> 
#include <iterator>

class Location
{
private:
    string                  _autoIndex;
    string                  _upload_dir;
    string                  _root;
    string                  _path;
    string                  _name;
    vector<string>          _methods;
    vector<string>          _indexes;
    vector<string>          _cgi;

public:
    Location();
    ~Location();
    
    //setters
    void                    setPath(string path);
    void                    setRoot(string root);
    void                    setCgi(string content);
    void                    setMethods(string method);
    void                    setUploadDir(string path);
    void                    setIndexes(string content);
    void                    setautoIndex(string is_index);

    //getters
    vector<string>          getCgi();
    string                  getRoot();
    string                  getPath();
    vector<string>          getMethods();
    vector<string>          getIndexes();
    string                  getUploadDir();
    string                  getautoIndex();
    
};
 
 
 

#endif