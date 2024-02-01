#include "response.hpp"

Response::Response()
{
}
Response::~Response() {}

int Response::_len_slach(string url)
{
    int count = 0;
    int i = 0;
    while (url[i])
    {
        if (url[i] == '/')
            count++;
        i++;
    }
    return count;
}

string Response::_deletePartOfUrl(string url, int i, int len)
{
    if ((len - i) == 1)
        return ("/");
    else if ((len - i) > 1)
    {
        int c = url.find("/", len - i);
        if (c != string::npos)
        {
            return url.substr(0, c);
        }
    }
    return 0;
}

Location *Response::_check_location(Config *conf, string path)
{
    // CHECK LOCATION IF EXIST
    map<string, Location *>             location;
    int                                 len;
    string                              url;

    location = conf->getLocation();
    len = 0;
    url = path;
    len = _len_slach(url);
    if (location[path])
        return location[path];
    else if (location[path + "/"])
        return location[path + "/"];
    else
    {
        for (int i = 0; i < len; i++)
        {
            url = _deletePartOfUrl(url, i, len);
            for (map<string, Location *>::iterator it = location.begin(); it != location.end(); it++)
            {
                cout << "url: (" << url << ")"
                     << "    it: (" << it->first << ")\n";
                if (url == it->first || url + "/" == it->first)
                {
                    cout << "\n " << it->first << " +1\n";
                    cout << "im not\n";
                    return it->second;
                }
            }
        }
    }
    return 0;
}

vector<string>  Response::_generate_autoindex(DIR *dir, int sd)
{
    dirent              *entry;
    vector<string>      vec;
    int                 i;

    i = 0;
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] != '.')
        {
            vec.push_back(entry->d_name);
            i++;
        }
    }
    return (vec);
}

string Response::_generate_index(Location *location)
{
    vector<string>  indexes = location->getIndexes();
    for (int i = 0; i < indexes.size() && !indexes.empty(); i++)
    {
        ifstream file( location->getRoot() + "/" + indexes[i]);
        if(file.good())
        {
            cout << indexes[i] << "9wd\n";
            return location->getRoot() + "/" + indexes[i];
        }
    }
    return "";
}

void Response::_response_part(Config *conf, int sd)
{
    map<int, Request *> request;
    Location *location;
    DIR *dir;
    string url;

    request = conf->_requests;
    location = _check_location(conf, request[sd]->getUrl());

    if (!location)
        _response_errors_(conf, sd, 404);
    else
    {
        // check if url found in root
        url = location->getRoot() + request[sd]->getUrl();
        ifstream file(url);
        dir = opendir(url.c_str());
        if (dir || file.good())
        {
            if (dir)
                if (location->getautoIndex() == "on")
                {
                    // IF WAS DIR AND (AUTOINDEX: ON)
                    _response_get_(conf, sd, _generate_autoindex(dir, sd), 200);
                }
                else
                {
                    // IF WAS DIR AND (AUTOINDEX: OFF)
                    cout << "dir off \n";
                    string str =  _generate_index(location); 
                    if(str.empty())
                        _response_errors_(conf, sd, 404);
                    _response_index_(conf, sd, str, 200);
                }
            else
                {
                    // IF WAS FILE 
                    stringstream ssa;
                    ssa << file.rdbuf();
                    string str = ssa.str();
                    vector<string>  vec;
                    cout << url << str <<  "\n";
                    string content = _generate_code(conf, sd, str, vec, _code_status(200), _content_type("text"));
                    _send_reponse(content, sd);
                }
        }
        else
            _response_errors_(conf, sd, 404);
    }
}
