#ifndef _HTTP_S_H_
#define _HTTP_S_H_

#include <boost/asio.hpp>
#include <fstream>

class HTTPServer {

public:

	HTTPServer(boost::asio::io_context& context, unsigned short port);

	bool start();

	void shutdown();

private:

	void connectionReceivedCb(const boost::system::error_code& error);

	bool startWaitingConnection();

	void messageReceivedCb(const boost::system::error_code& error, std::size_t size);

	void sendResponse();

	void messageSentCb(const boost::system::error_code& error, std::size_t size);

	std::string getHTTPResponse(std::string data);

	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;

	boost::asio::streambuf buff;

	std::string data;
	std::string response;

	char incomingByte;
};

#endif	// _HTTP_S_H_