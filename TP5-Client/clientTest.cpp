#include <curl/curl.h>

#include <iostream>

using namespace std;

int main() {

	CURLcode error = curl_global_init(CURL_GLOBAL_ALL);


	if (error == CURLE_OK) {

		CURL* curl = curl_easy_init();

		if (curl) {
			CURLcode res;

			// Para probar sin server:
			curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

			// CON SERVER:
			//curl_easy_setopt(curl, CURLOPT_URL, "127.0.0.1/hola.txt");
			//curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
			//curl_easy_setopt(curl, CURLOPT_PORT, 80);
			//curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);

		}

	}

	curl_global_cleanup();
}