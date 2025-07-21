// C++ includes
#include <iostream>
#include <sstream>
#include <system_error>
#include <unordered_map>

// C includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

// local includes

// classes
#if 0
		std::string http = "HTTP/1.1 200 OK\r\n"
					"Content-Length: 5\r\n"
					"Content-Type: text/plain\r\n"
					"Connection: close\r\n"
					"\r\n"
					"hello";
#endif

class HttpRequest {
public:

	enum class HttpMethod {GET, POST, PUT, PATCH, DELETE, HEAD, OPTIONS, TRACE, CONNECT} method;
	std::string target;
	enum class HttpVersion {HTTP_11} version;
	std::unordered_map<std::string, std::string> headers;
	std::string body;

	HttpRequest() : method(HttpMethod::GET), target("/"), version(HttpVersion::HTTP_11), headers({}), body("") {
		std::cout << "HttpRequest " << this << std::endl;
	}

	HttpRequest(std::string incoming) {
		std::cout << "HttpRequest " << this << std::endl;

		std::istringstream is (incoming);

		// preamble: method, target, version
		std::string m, t, v;
		if (!(is >> m >> t >> v)) {
			throw std::runtime_error("preamble");
		}
		if (m == "GET") {
			method = HttpMethod::GET;
		} else {
			throw std::runtime_error("http method not supported");
		}
		// target: TODO
		if (v == "HTTP/1.1") {
			version = HttpVersion::HTTP_11;
		} else {
			throw std::runtime_error("http version not supported");
		}
	}

	~HttpRequest() {
		std::cout << "HttpRequest " << this << " destroyed" << std::endl;
	}

	static std::string test_response() {
		return "HTTP/1.1 200 OK\r\n"
			"Content-Length: 5\r\n"
			"Content-Type: text/plain\r\n"
			"Connection: close\r\n"
			"\r\n"
			"hello";
	}

};


int main(int argc, char **argv) {

	if (argc < 4) {
		std::cerr << "usage: server \"ip\" port bufsize" << std::endl;
		return -1;
	}

	const char* ip = argv[1];
	const short int port = std::atoi(argv[2]);
	const size_t bufsize = std::atoi(argv[3]);

	auto fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		throw std::system_error(errno, std::generic_category(), "socket");
	}

	int yes = 1;
	if (auto res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "setsockopt");
	}

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	if (auto res = bind(fd, (sockaddr*)&addr, sizeof(addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "bind");
	}

	if (auto res = listen(fd, 1); res < 0) {
		throw std::system_error(errno, std::generic_category(), "listen");
	}

	std::cout << "listening on 127.0.0.1:12345" << std::endl;

	while (true) {

		sockaddr_in client{};
		socklen_t len = sizeof(client);
		auto clientfd = accept(fd, (sockaddr*)&client, &len);
		if (clientfd < 0) {
			throw std::system_error(errno, std::generic_category(), "accept");
		}

		char buf[bufsize];
		ssize_t n = read(clientfd, buf, bufsize-1);

		HttpRequest obj {};
		std::string resp = obj.test_response();
		write(clientfd, resp.c_str(), resp.size());
		close(clientfd);

	}


	close(fd);

	return 0;
}
