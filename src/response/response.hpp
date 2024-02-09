
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../config/config.hpp"
#include "../request/request.hpp"
#include "../cgi/cgi.hpp"

#define FOLDER_ON 21
#define FOLDER_OFF 20
#define FILE 1
#define ERROR_404 404

class Config;
class Request;
class Cgi;

class Response
{
private:
  
public:
    Response();
    ~Response();
    int                     _length; // length of content
    int                     _code_status;
    string                  _content; // content for file
    string                  _file_index; // for dir off and choose index 
    string                  _type_of_content; // for dir off and choose index 
    string                  _favicon;
    int                  _support_cgi;
    vector<string >         _content_autoIndex; // auto index

    //utils
    int                     _len_slach(string url);
    void                    generate_html_autoindex(Config *conf, int fd);
    string                  _content_type(string type_con);
    string                  _deletePartOfUrl(string url, int i, int len);

    //PART GET
    void                    _response_part(Config *conf, int fd);
    void                    _response_errors_(Config *conf, int fd);
    void                    _response_index_(Config *conf, int fd, string str, int code);
    void                    _send(Config *conf, int fd);
    void                    _response_get_(Config *conf, int fd, vector<string> vec, int code);
    void                    _generate_index(Location *location);
    Location*               _check_location(Config *conf, string path);

    // VERION 2
    void                    _generate_content(Location *location,Config* conf,int fd,DIR *dir,ifstream *file,int type);
    void                    content_autoIndex(Config* conf, int fd,DIR *dir);
    void                    content_file(ifstream *file);
    void                    check_extentsion(string name);
    string                  generate_header(Config *config, int fd);

    void                    _check_cgi(Location *location, string url);


};
#endif