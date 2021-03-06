#include "HTTPServer.h"

#include <boost/bind/bind.hpp>

#include <iostream>

#pragma warning(disable : 4996)		// Al compilador no le gusta gmtime

using boost::asio::ip::tcp;
using namespace std;

HTTPServer::HTTPServer(boost::asio::io_context& context, unsigned short port) :
				acceptor(context, tcp::endpoint(tcp::v4(), port)),
				socket(context)				// Inicializa socket sin abrirlo
{
}

bool HTTPServer::start() {

	if (socket.is_open()) {
		cout << "Socket is already open, cannot accept more connections" << endl;
		return false;
	}
	
	cout << "Server started" << endl;

	return startWaitingConnection();
}

void HTTPServer::shutdown() {
	if (socket.is_open()) {
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();
		cout << "Connection closed" << endl;
	}
}

bool HTTPServer::startWaitingConnection() {

	if (socket.is_open()) {
		cout << "Socket is already open, cannot accept more connections" << endl;
		return false;
	}

	cout << "Waiting for connection..." << endl;

	acceptor.async_accept(socket,
		boost::bind(&HTTPServer::connectionReceivedCb, this, boost::asio::placeholders::error));

	return true;
}

void HTTPServer::connectionReceivedCb(const boost::system::error_code& error) {

	if (!error) {
		cout << "Connection received" << endl;

		boost::asio::async_read_until(socket, buff, "\r\n\r\n",			// Leemos hasta el terminador HTTP
			boost::bind(&HTTPServer::messageReceivedCb, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	}
	else {
		cout << "Error creating connection: " << error.message() << endl;

		shutdown();					// Server restart
		startWaitingConnection();

	}

}

void HTTPServer::messageReceivedCb(const boost::system::error_code& error, std::size_t size) {

	if (!error) {

		istream is(&buff);			// MAGIA! Guarda tod la data recibida del buffer
		getline(is, data, {});		// como string
	
		cout << size << " bytes received" << endl;
		cout << "Received: " << endl << data << endl << endl;

		response = getHTTPResponse(data);
		
		sendResponse();

	}
	else {
		cout << "Error receiving message: " << error.message() << endl;
		
		shutdown();					// Server restart
		startWaitingConnection();

	}

}

void HTTPServer::sendResponse() {

	cout << "Sending response" << endl;

	cout << "Response:" << endl << response << endl << endl;

	boost::asio::async_write(socket, boost::asio::buffer(response),
		boost::bind(&HTTPServer::messageSentCb, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

}

void HTTPServer::messageSentCb(const boost::system::error_code& error, std::size_t size) {

	if (!error) {
		cout << "Response sent" << endl;
		cout << size << " bytes were sent" << endl;
	}
	else {
		cout << "Response failed: " << error.message() << endl;
	}

	shutdown();		// Close actual connection

	startWaitingConnection();	// Wait for the next connection
}



string HTTPServer::getHTTPResponse(std::string data) {

	string method = data.substr(0, data.find(" "));
	string path = data.substr(data.find(" /")+2);
	path = path.substr(0, path.find(" "));
	
	string host = data.substr(data.find("Host: ")+6);
	host.replace(host.find("\r"), 1, " ");
	host = host.substr(0, host.find(" "));

	string file_content;

	ifstream file;

	file.open(path, ios::out | ios::binary);

	time_t now = time(0);
	tm* nowTm = gmtime(&now);
	char timeNow[256];
	strftime(timeNow, 256, "%a, %d %b %Y %T GMT", nowTm);    //Ej: Tue, 04 Sep 2018 18:21:00 GMT

	time_t exp = now + 30;
	tm* expTm = gmtime(&exp);
	char timeExp[256];
	strftime(timeExp, 256, "%a, %d %b %Y %T GMT", expTm);
	
	string response;

	if (method != "GET" || host != "127.0.0.1" || file.fail()) {

		response = string(	"HTTP/1.1 404 Not Found \r\n") +
							"Date: " + timeNow + "\r\n" +
							"Cache-Control: public, max-age=30 \r\n" +
							"Expires: " + timeExp + "\r\n" +
							"Content-Length: 0 \r\n" +
							"Content-Type: text/html; charset=iso-8859-1 \r\n\r\n";;
	}
	else {

		getline(file, file_content, {});	// Guarda todo el contnido del archivo como string

		response = string(	"HTTP/1.1 200 OK \r\n") +
							"Date: " + timeNow + "\r\n" +
							"Location: 127.0.0.1/" + path + " \r\n" +
							"Cache-Control: max-age=30 \r\n" +
							"Expires: " + timeExp + "\r\n" +
							"Content-Length: " + to_string(file_content.length()) + " \r\n" +
							"Content-Type: text/html; charset=iso-8859-1 \r\n\r\n" +
							file_content;
	}

	file.close();
	return response;
}