#include "HTTPServer.h"

#include <boost/bind/bind.hpp>

#include <iostream>

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
		return true;
	}
	
	cout << "Server started" << endl;

	return startWaitingConnection();
}

bool HTTPServer::startWaitingConnection() {

	if (socket.is_open()) {
		cout << "Socket is already open, cannot accept more connections" << endl;
		return true;
	}

	acceptor.async_accept(socket,
		boost::bind(&HTTPServer::connectionReceivedCb, this, boost::asio::placeholders::error));

	return false;
}

void HTTPServer::connectionReceivedCb(const boost::system::error_code& error) {

	if (!error) {
		cout << "Connection received" << endl;

		//socket.async_read_some(boost::asio::buffer(&incomingByte, 1),		// Empezamos a leer
		//	boost::bind(&HTTPServer::messageReceivedCb, this, boost::asio::placeholders::error));

		boost::asio::async_read_until(socket, buff, "\r\n\r\n",
			boost::bind(&HTTPServer::messageReceivedCb, this, boost::asio::placeholders::error));

		startWaitingConnection();	// Never stop waiting for connections

	}

}

void HTTPServer::messageReceivedCb(const boost::system::error_code& error) {

	if (!error) {

		istream is(&buff);
		getline(is, data, {}); 
	
		cout << "Message received" << endl;
		cout << "Received: " << data << endl;

		//socket.async_read_some(boost::asio::buffer(&incomingByte, 1),		// Leemos el proximo byte
		//	boost::bind(&HTTPServer::messageReceivedCb, this, boost::asio::placeholders::error));

	}
	else if (error != boost::asio::error::eof) {
		cout << "ERROR: " << error << endl;
	}
	else {
		cout << "message successfully reeceived" << endl;
	}
}