/*
* HTTPClient.cpp - Archivo principal de source C++ del cliente de HTTP
* Grupo 2: Causse, Peralta, Venier
*/

#include "HTTPClient.h"

#define PATH_MAX_LEN 150	//Maximum length (in chars) of the file path (including the null terminator)

int getFilePathCallback (char* key, char* value, void* userData);

int client(int argc, char* argv[]) {
	//GET FILE PATH FROM COMMAND LINE
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

int getFilePathCallback(char* key, char* value, void* userData) {
	if (key != NULL)
		return 0;
	else {
		strcpy((char*)userData, value);
	}
	return 1;
}

int main (int argc, char ** argv){
    client(argc, argv);
    return 0;
}


