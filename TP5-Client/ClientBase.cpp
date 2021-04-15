#include "ClientBase.h"

#include <curl/curl.h>
#include <iostream>

#pragma warning(disable : 4996)

using namespace std;


HTTPClient::HTTPClient(string requestURL_) : requestURL(requestURL_) {
}

int HTTPClient::connect() {

	string filename = requestURL.substr(requestURL.rfind("/")+1);	 // Nombre del arhivo a partir de la ultima barra

	if (filename.empty() || filename == requestURL) {
		cout << "ERROR: filename not specified" << endl;
		return 1;
	}

	cout << "Looking for file: " << filename << endl;

	FILE* file = fopen(filename.c_str(), "w");

	if (file) {

		CURLcode error = curl_global_init(CURL_GLOBAL_ALL);

		if (error == CURLE_OK) {

			CURL* curl = curl_easy_init();

			if (curl) {


				curl_easy_setopt(curl, CURLOPT_URL, requestURL.c_str());
				curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
				curl_easy_setopt(curl, CURLOPT_PORT, 80);
				//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
				curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);	// Que falle en caso de 404 o similar
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);	// Por default, guarda la data en un archivo
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);	// le paso el puntero al archivo

				CURLcode code = curl_easy_perform(curl);

				if (code == CURLE_OK) {
					cout << "Operation succeded" << endl;
				}
				else if (code == CURLE_HTTP_RETURNED_ERROR) {
					cout << "ERROR: the requested file was not found in the server" << endl;
				}
				else {
					cout << "ERROR: an unexpected error ocurred" << endl;
				}

			}

			curl_easy_cleanup(curl);
		}

		fclose(file);

		curl_global_cleanup();

	}
	else {
		cout << "ERROR: cannot open the filename specified" << endl;
	}

	return 0;

}