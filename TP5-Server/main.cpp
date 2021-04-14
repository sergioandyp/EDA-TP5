#include <iostream>

#include "HTTPServer.h"

using namespace std;

int main() {

	boost::asio::io_context context;

	HTTPServer server(context, 80);

	if (!server.start()) {
		cout << "Error, el server no se pudo iniciar" << endl;
		return 0;
	}

	context.run();

}