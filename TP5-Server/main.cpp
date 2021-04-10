#include <iostream>

#include "HTTPServer.h"

using namespace std;

int main() {

	boost::asio::io_context context;

	HTTPServer server(context, 80);

	server.start();

	context.run();

}