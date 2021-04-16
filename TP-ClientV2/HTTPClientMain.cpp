/*
* HTTPClientMain.cpp - Archivo principal de source C++ del cliente de HTTP
* Grupo 2: Causse, Peralta, Venier
*/

//INCLUDES
#include <cstdlib>
#include "cmdparser.h"
#include "HTTPClient.h"

#pragma warning(disable : 4996)		// Al compilador no le gusta strcpy

//CONSTANTS
#define PATH_MAX_LEN 150	//Maximum length (in chars) of the file path (including the null terminator)

//ERROR CODES
#define MALLOC_ERROR -1
#define PARSER_EXPECTED_ERROR -2
#define PARSER_UNEXPECTED_ERROR -3

//FUNCTION PROTOTYPES
int client(int argc, char* argv[]);
int getFilePathCallback (char* key, char* value, void* userData);

int main (int argc, char ** argv){
    client(argc, argv); //Run client
    return 0;
}

//MAIN HTTP CLIENT FUNCTION
int client(int argc, char* argv[]) {
    //GET FILE PATH FROM COMMAND LINE USING PARSER
    char* URL = (char*)malloc(PATH_MAX_LEN * sizeof(char));
    if (URL == nullptr)
        return MALLOC_ERROR;
    pCallback pathCallback = &getFilePathCallback;
    short parserReturn = parseCmdLine(argc, argv, pathCallback, (void*) URL, 1, 0);
    switch (parserReturn) {
        case -1: case -2:
            std::cout << "Error parsing file path. It must not begin with \"-\"." << std::endl;
            return PARSER_EXPECTED_ERROR;
        case -4: case -5: case -6: case -7:
            std::cout << "Error parsing file path. Input must be only 1 parameter." << std::endl;
            return PARSER_EXPECTED_ERROR;
        case -8:
            std::cout << "Error parsing file path. No input received." << std::endl;
            return PARSER_EXPECTED_ERROR;
        case -3: default:
            std::cout << "Error parsing file path. Unexpected parser error." << std::endl;
            return PARSER_UNEXPECTED_ERROR;
        case 1:
            std::cout << "Parsing successful." << std::endl;
            break;
    }
    std::string filePathString(URL);   //Convert file path from C string to std string
    std::cout << "Received: " << filePathString << std::endl;
    HTTPClient client(filePathString);
    client.connect();
    free(URL);
    return EXIT_SUCCESS;
}

//CALLBACK FOR PARSER
int getFilePathCallback(char* key, char* value, void* userData) {
    if (key != nullptr)
        return 0;
    else {
        strcpy((char*)userData, value);
    }
    return 1;
}