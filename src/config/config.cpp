#include "./config.hpp"

Config::Config() : _count_braces(0) {parse_config();}
Config::~Config() {}

// setters
void Config::setServName(string name)
{
    _server_name = name;
}
void Config::setPort(string port)
{
    _port = port;
}
void Config::setIp(string Ip)
{
    _ip = Ip;
}
void Config::setErrors(string path, string status_code)
{
    _errors[status_code] = path;
}
void Config::setLocation(string path, Location *location)
{
    _locations[path] = location;
}

// getters
string Config::getServName()
{
    return (_server_name);
}
string Config::getPort()
{
    return (_port);
}
string Config::getIp()
{
    return (_ip);
}
map<string, string> Config::getErrors()
{
    return (_errors);
}
map<string, Location *> Config::getLocation()
{
    return (_locations);
}

void Error(string message)
{
    cout << "Error: " << message;
    exit(1);
}

string Config::check_is_file()
{
    DIR *dir;

    dir = opendir(".");
    if (dir == nullptr)
        cout << "null";
    while (dirent *read = readdir(dir))
    {
        std::string name = read->d_name;
        if (name.find(".config") != string::npos)
            return (name);
    }
    cout << "no exist file config\n";
    exit(1);
}

void    check_file_exist(string content)
{
    DIR *dir = opendir(content.c_str());
    if(!dir)
        Error("not found root or upload (config)\n");
}


void Config::check_location(string file_config)
{
    std::vector<std::string> vec;
    std::string str;
    int is_close;

    is_close = 1;
    ifstream stream(file_config);
    if (stream.is_open() == false)
        cout << "can't open file config\n";
    stringstream ss;
    ss << stream.rdbuf();
    string content;
    while (!ss.eof())
    {
        ss >> content;
        if (content == "location")
        {
            Location *locationPtr = new Location();
            ss >> content;
            if (content.find("/") != string::npos)
                locationPtr->setPath(content);
            ss >> content;
            if (content != "{")
                Error("check braces (config)\n");
            while (!ss.eof() && content != "}")
            {
                ss >> content;
                if (content == "autoindex")
                {
                    ss >> content;
                    content.pop_back();
                    // cout << "=>" << content << "<";
                    if (content != "on" && content != "off")
                        Error("check autoindex");
                    locationPtr->setautoIndex(content);
                }
                else if (content == "allow_methods")
                {
                    ss >> content;
                    while (content.find(";") == string::npos)
                    {
                        if (content != "GET" && content != "POST" && content != "DELETE")
                            Error("check autoindex (config)\n");
                        locationPtr->setMethods(content);
                        ss >> content;
                    }
                    content.pop_back();
                    locationPtr->setMethods(content);
                    if (content != "GET" && content != "POST" && content != "DELETE")
                        Error("check methods (config)\n");
                }
                else if (content == "root" || content == "root;")
                {
                    if(content == "root;")
                        Error("check root (config)\n");
                    ss >> content;
                    if(content == ";" || content.find(";") == string::npos)
                        Error("check root (config)\n");
                    content.pop_back();
                    locationPtr->setRoot(content);
                    if (locationPtr->getRoot().empty())
                        Error("check root (config)\n");
                    check_file_exist(content);
                    
                }
                else if (content == "upload_dir" || content == "upload_dir;")
                {
                    if(content == "upload_dir;")
                        Error("check upload_dir (config)\n");
                    ss >> content;
                    if(content == ";" || content.find(";") == string::npos)
                        Error("check upload_dir (config)\n");
                    content.pop_back();
                    locationPtr->setUploadDir(content);
                    if (locationPtr->getUploadDir().empty())
                        Error("check upload_dir (config)\n");
                    check_file_exist(content);
                }
                else if (content == "index" || content == "index;")
                {
                    if(content == "index;")
                        Error("check index (config)\n");
                    ss >> content;
                    if(content == ";")
                        Error("check index (config)\n");
                    while(1)
                    {
                        if(content.find(";") != string::npos)
                        {
                            content.pop_back();
                            locationPtr->setIndexes(content);
                            break;
                        }
                        locationPtr->setIndexes(content);
                        ss >> content;
                    }

                }
                else if (content == "cgi")
                {
                    while (content.find(";") == string::npos)
                    {
                        ss >> content;
                        if(content.find(";") == string::npos)
                            locationPtr->setCgi(content);
                    }
                    if(content.find(";") != string::npos)
                        content.pop_back();
                    locationPtr->setCgi(content);
                    if (locationPtr->getCgi().size() != 2)
                        Error("check cgi (config)\n");
                }
                else if (content == "}")
                    is_close = 0;
                setLocation(locationPtr->getPath(), locationPtr);
            }
            if (is_close)
                Error("check braces location (config)\n");
            // vector<string> i = locationPtr->getCgi();
            // cout << i[0] << "  " << i[1] << "\n";
        }
    }
}

void Config::Conf_server(string file_config)
{
    std::string str;
    std::vector<std::string> vec;
    std::vector<std::string> tmp;

    ifstream stream(file_config);
    if (stream.is_open() == false)
        cout << "can't open file config\n";
    while (getline(stream, str))
    {
        vec = splitString(str, " ");
        if (str.find("server_name") != string::npos && vec.size() == 2)
        {
            vec[1].pop_back();
            setServName(vec[1]);
        }
        else if (str.find("listen") != string::npos && vec.size() == 2)
        {
            tmp = splitString(vec[1], ":");
            if (tmp.size() == 2)
            {
                tmp[1].pop_back();
                setIp(tmp[0]);
                setPort(tmp[1]);
            }
        }
        else if (str.find("error_page") != string::npos)
        {
            if (vec.size() == 3 && !vec[0].empty() && !vec[1].empty() && !vec[2].empty())
            {
                vec[2].pop_back();
                setErrors(vec[2], vec[1]);
            }
            else
                Error("check error_page (config)");
        }
        if (str.find("}") != string::npos || str.find("{") != string::npos)
            _count_braces++;
    }
    if (_count_braces % 2 != 0)
        Error("check braces (config)\n");
    check_location(file_config);
}

void Config::check_content_config()
{
    if (getServName().empty() || getErrors().empty() || getPort().empty() || getIp().empty())
        Error("server_name, ip, port, error_page\n");
}

void                       Config::check_tab_location(string file_config)
{

}


void Config::parse_config()
{
    int i;
    string str;
    string file_config;

    i = 0;
    file_config = check_is_file();
    ifstream stream(file_config);
    if (stream.is_open() == false)
        cout << "can't open file config\n";
    while (getline(stream, str))
    {
        if (str.find("server") != string::npos && i == 0 && str.find_first_not_of(" /t") == 0)
            i++;
        else if (i == 0 && str.find_first_not_of(" /t"))
            Error("Tab or space is redundant\n");
        if (str.find("{") != string::npos && i == 1 && str.find_first_not_of(" /t") == 0)
            i++;
        else if (i == 1 && str.find_first_not_of(" /t"))
            Error("Tab or space is redundant\n");
        else if (i == 2 && !stream.eof())
            if (!str.empty() && str.at(0) != '\t')
                Error("check scops\n");
        if (stream.eof())
        {
            if (str.find("}") != string::npos)
                if (str.find(" ") != string::npos || str.find("\t") != string::npos)
                    Error("Tab or space is redundant\n");
        }
        if(str.find("{") == string::npos &&  str.find("server") == string::npos 
            &&  str.find("}") == string::npos  &&  !str.empty() 
            &&  str.find(";") == string::npos &&  str.find_first_not_of(" \t") != string::npos
        )
            Error("forget \";\" in (config)\n");
    }
    Conf_server(file_config);
    check_tab_location(file_config);
    check_content_config();
}
