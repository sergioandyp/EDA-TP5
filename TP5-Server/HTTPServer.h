#ifndef _HTTP_S_H_
#define _HTTP_S_H_

#include <boost/asio.hpp>

class HTTPServer {

public:

	HTTPServer(boost::asio::io_context& context, unsigned short port);

	bool start();

	void shutdown();

private:

	void connectionReceivedCb(const boost::system::error_code& error);

	void messageReceivedCb(const boost::system::error_code& error);

	bool startWaitingConnection();

	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;

	boost::asio::streambuf buff;

	std::string data;

	char incomingByte;
};

#endif	// _HTTP_S_H_