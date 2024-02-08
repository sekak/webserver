
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
    string                  _content; // content for file
    vector<string >         _content_autoIndex; // auto index
    int                     _length; // length of content
    string                  _file_index; // for dir off and choose index 
    string                  _type_of_content; // for dir off and choose index 
    int                     _code_status;
    //utils
    string                  _deletePartOfUrl(string url, int i, int len);
    int                     _len_slach(string url);
    string                  _content_type(string type_con);
    void                    generate_html_autoindex(Config *conf, int fd);

    //PART GET
    void                    _response_part(Config *conf, int fd);
    void                    _response_errors_(Config *conf, int fd, int code);
    void                    _response_index_(Config *conf, int fd, string str, int code);
    void                    _send(Config *conf, int fd);
    void                    _response_get_(Config *conf, int fd, vector<string> vec, int code);
    void                    _generate_index(Location *location);
    Location*               _check_location(Config *conf, string path);

    // VERION 2
    void        _generate_content(Location *location,Config* conf,int fd,DIR *dir,ifstream *file,int type);
void content_autoIndex(Config* conf, int fd,DIR *dir);
void  content_file(ifstream *file);
string    generate_header(Config *config, int fd);
void    check_extentsion(string name);


};
#endif