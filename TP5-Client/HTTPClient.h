#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

//INCLUDES
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include "cmdparser.h"

//CLASSES
class HTTPClient {
public:
    HTTPClient(std::string requestURL_);    //Constructor
    int connect();                          //Method used to connect to server

private:
    std::string requestURL;                 //URL path to request
    std::ifstream file;                     //File to save server response
};

//ERROR CODES
#define MALLOC_ERROR -1
#define PARSER_EXPECTED_ERROR -2
#define PARSER_UNEXPECTED_ERROR -3

#endif /*HTTPCLIENT_H*/