#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

//INCLUDES
#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>

//CLASSES
class HTTPClient {
public:
    HTTPClient(std::string& requestURL_);    //Constructor
    int connect();                          //Method used to connect to server

private:
    std::string requestURL;                 //URL path to request
};

#endif /*HTTPCLIENT_H*/