
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../config/config.hpp"

class Config;

class Response
{
private:
  
public:
    Response();
    ~Response();
    
    //utils
    string                  _deletePartOfUrl(string url, int i, int len);
    int                     _len_slach(string url);
    string                  _code_status(int code);
    string                  _content_type(string type_con);
    vector<string>          _generate_autoindex(DIR *dir, int sd);
    string                  _generate_code(Config *conf, int sd, string content, vector<string> vec, string status, string type);

    //PART GET
    void                    _response_part(Config *conf, int sd);
    void                    _response_errors_(Config *conf, int sd, int code);
    void                    _response_index_(Config *conf, int sd, string str, int code);
    void                    _send_reponse(string content, int sd);
    void                    _response_get_(Config *conf, int sd, vector<string> vec, int code);
    string                  _generate_index(Location *location);
    Location*               _check_location(Config *conf, string path);



};
#endif