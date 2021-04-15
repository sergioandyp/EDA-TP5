/*
* HTTPClient.cpp - Archivo principal de source C++ del cliente de HTTP
* Grupo 2: Causse, Peralta, Venier
*/

#include "HTTPClient.h"

#pragma warning(disable : 4996)

#define PATH_MAX_LEN 150	//Maximum length (in chars) of the file path (including the null terminator)

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
	char* filePath = (char*)malloc(PATH_MAX_LEN * sizeof(char));
	if (filePath == NULL)
		return MALLOC_ERROR;
	pCallback pathCallback = &getFilePathCallback;
	short parserReturn = parseCmdLine(argc, argv, pathCallback, (void*) filePath, 1, 0);
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

	free(filePath);
	return EXIT_SUCCESS;
}

//CALLBACK FOR PARSER
int getFilePathCallback(char* key, char* value, void* userData) {
	if (key != NULL)
		return 0;
	else {
		strcpy((char*)userData, value);
	}
	return 1;
}

//HTTP CLIENT CLASS METHOD DEFINITIONS

//CLASS CONSTRUCTOR
HTTPClient::HTTPClient(std::string requestURL_) : requestURL(requestURL_) {
}

//CONNECTION METHOD FOR THE HTTP CLIENT
int HTTPClient::connect() {
    std::string filename = requestURL.substr(requestURL.rfind("/")+1);	 // Nombre del arhivo a partir de la ultima barra
    if (filename.empty() || filename == requestURL) {
        std::cout << "ERROR: filename not specified" << std::endl;
        return 1;
    }
    std::cout << "Looking for file: " << filename << std::endl;
    FILE* file = fopen(filename.c_str(), "w");
    if (file) {
        CURLcode error = curl_global_init(CURL_GLOBAL_ALL);
        if (error == CURLE_OK) {
            CURL* curl = curl_easy_init();
            if (curl) {
                //CURL SETUP
                curl_easy_setopt(curl, CURLOPT_URL, requestURL.c_str());
                curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
                curl_easy_setopt(curl, CURLOPT_PORT, 80);
                //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);	// Que falle en caso de 404 o similar
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);	// Por default, guarda la data en un archivo
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);	// le paso el puntero al archivo
                //CURL PERFORM CONNECTION
                CURLcode code = curl_easy_perform(curl);
                if (code == CURLE_OK) {
                    std::cout << "Operation succeded" << std::endl;
                }
                else if (code == CURLE_HTTP_RETURNED_ERROR) {
                    std::cout << "ERROR: the requested file was not found in the server" << std::endl;
                }
                else {
                    std::cout << "ERROR: an unexpected error ocurred" << std::endl;
                }
            }
            //CLEANUP CURL MEMORY AFTER USE
            curl_easy_cleanup(curl);
        }
        //GENERAL CLEANUP
        fclose(file);
        curl_global_cleanup();
    }
    else {
        std::cout << "ERROR: cannot open the filename specified" << std::endl;
    }
    return EXIT_SUCCESS;
}