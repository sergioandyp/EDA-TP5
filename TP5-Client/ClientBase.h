#include <string>
#include <fstream>

class HTTPClient {
public:
	HTTPClient(std::string requestURL_);
	int connect();

private:
	std::string requestURL;
	std::ifstream file;
};