/*
* HTTPClient.cpp - Archivo principal de clase del cliente de HTTP
* Grupo 2: Causse, Peralta, Venier
*/

#include "HTTPClient.h"

//CLASS CONSTRUCTOR
HTTPClient::HTTPClient(std::string& requestURL_) : requestURL(requestURL_) {
}

//CONNECTION METHOD FOR THE HTTP CLIENT
int HTTPClient::connect() {
    char filename[] = "response.html";
    FILE* file = fopen(filename, "w");
    if (file == nullptr){
        std::cout << "Error creating file." << std::endl;
        return EXIT_FAILURE;
    }
    else {
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
    return EXIT_SUCCESS;
}