#include "HTTPServer.h"

#include <boost/bind/bind.hpp>

#include <iostream>

#pragma warning(disable : 4996)

using boost::asio::ip::tcp;
using namespace std;

HTTPServer::HTTPServer(boost::asio::io_context& context, unsigned short port) :
				acceptor(context, tcp::endpoint(tcp::v4(), port)),
				socket(context),				// Inicializa socket sin abrirlo
				incomingByte() 
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
	}
}

bool HTTPServer::startWaitingConnection() {

	if (socket.is_open()) {
		cout << "Socket is already open, cannot accept more connections" << endl;
		return false;
	}

	acceptor.async_accept(socket,
		boost::bind(&HTTPServer::connectionReceivedCb, this, boost::asio::placeholders::error));

	return true;
}

void HTTPServer::connectionReceivedCb(const boost::system::error_code& error) {

	if (!error) {
		cout << "Connection received" << endl;

		//socket.async_read_some(boost::asio::buffer(&incomingByte, 1),		// Empezamos a leer
		//	boost::bind(&HTTPServer::messageReceivedCb, this, boost::asio::placeholders::error));

		boost::asio::async_read_until(socket, buff, "\r\n\r\n",			// Leemos hasta el terminador HTTP
			boost::bind(&HTTPServer::messageReceivedCb, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	}

}

void HTTPServer::messageReceivedCb(const boost::system::error_code& error, std::size_t size) {

	if (!error) {

		istream is(&buff);			// MAGIA! Guarda tod la data recibida del buffer
		getline(is, data, {});		// como string
	
		cout << size << "bytes received" << endl;
		cout << "Received: " << data << endl;
		
		sendResponse();

	}
	else {
		cout << "Error receiving message: " << error.message() << endl;
	}

}

void HTTPServer::sendResponse() {

	cout << "Sending response" << endl;

	time_t now = time(0);
	time_t exp = now + 99999.0;

	response = "Gracias por conectartre, vuelve pronto!\r\n\r\n";
	response = string(	"HTTP/1.1 200 OK \r\n") + 
						"Date: " + ctime(&now) + "\r" +
						"Location: 127.0.0.1 \r\n" +
						"Cache-Control: public, max-age=30 \r\n" + 
						"Expires: " + ctime(&exp) + "\r" +
						"Content-Length: " + to_string(response.length()) + " \r\n" +
						"Content-Type: text/html; charset=iso-8859-1 \r\n" + 
						response + " \r\n\r\n";

	cout << "Response:" << endl << response << endl;

	boost::asio::async_write(socket, boost::asio::buffer(response),
		boost::bind(&HTTPServer::messageSentCb, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

}

void HTTPServer::messageSentCb(const boost::system::error_code& error, std::size_t size) {

	if (!error) {
		cout << "Response sent" << endl;
		cout << size << "bytes were sent" << endl;
	}
	else {
		cout << "Response failed: " << error.message() << endl;
	}

	shutdown();		// Close actual connection

	startWaitingConnection();	// Wait for the next connection
}
