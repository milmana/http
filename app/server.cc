// C++ includes
#include <iostream>
#include <sstream>
#include <system_error>
#include <map>
#include <vector>

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


class HttpResponse {
public:
	HttpResponse(const char*, ssize_t n) { }
	~HttpResponse() { }
};

class HttpRequest {

	std::map<std::string, std::string> kv_;

public:

	HttpRequest() : kv_{} { }
	HttpRequest(std::string& request) {

		std::istringstream is (request);

		kv_.clear();

		kv_["method"];
		is >> kv_["method"];

		kv_["path"];
		is >> kv_["path"];

		kv_["version"];
		is >> kv_["version"];
	}

	~HttpRequest() { 
		std::cout << "dtor: " << this << std::endl;
	}

	// copy
	HttpRequest(const HttpRequest& other) : kv_{other.kv_} { }
	HttpRequest& operator=(const HttpRequest& other) {
		kv_ = other.kv_;
		return *this;
	}

	// move
	HttpRequest(HttpRequest&& other) : kv_{other.kv_} {
		other.kv_.clear();
	}
	HttpRequest& operator=(HttpRequest&& other) {
		kv_ = std::move(other.kv_);
		return *this;
	}

};

class HttpWorker {

	int sock_;
	sockaddr_in addr_;
	std::vector<char> buf_;

public:

	inline static bool g_stop;

	HttpWorker() : sock_{}, addr_{}, buf_{} {
		std::cout << this << " ctor (default)" << std::endl;
	}
	HttpWorker(const char* ip, int port, int bufsize) : HttpWorker() {

		buf_.resize(bufsize);

		addr_.sin_family = AF_INET;
		addr_.sin_port = htons(port);
		addr_.sin_addr.s_addr = inet_addr(ip);

		if (auto res = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); res == -1) {
			throw std::system_error(errno, std::generic_category(), "socket");
		} else {
			sock_ = res;
		}

		int yes = 1;
		if (auto res = setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); res == -1) {
			throw std::system_error(errno, std::generic_category(), "setsockopt");
		}

		if (auto res = bind(sock_, (sockaddr*)&addr_, sizeof(addr_)); res == -1) {
			throw std::system_error(errno, std::generic_category(), "bind");
		}

		if (auto res = listen(sock_, 1); res == -1) {
			throw std::system_error(errno, std::generic_category(), "listen");
		}

		std::cout << this << " ctor" << std::endl;
	}

	~HttpWorker() {
		close(sock_);
		std::cout << this << " dtor" << std::endl;
	}


	void operator()() {

		while (!g_stop) {

			sockaddr_in addr{};
			socklen_t len = sizeof(addr);

			int sock;
			if (auto res = accept(sock, (sockaddr*)&addr, &len); res == -1) {
				throw std::system_error(errno, std::generic_category(), "accept");
			} else {
				sock = res;
			}


			buf_.clear();
			ssize_t n;
			if (auto res = read(sock, &buf_[0], buf_.size()-1); res == -1) {
				throw std::system_error(errno, std::generic_category(), "read");
			} else {
				n = res;
			}

			std::string http;

			if (auto res = write(sock, http.c_str(), http.size()); res == -1) {
				throw std::system_error(errno, std::generic_category(), "write");
			}

			close(sock);

		}
	}

};

int main(int argc, char **argv) {

	constexpr const char *ip = "127.0.0.1";
	constexpr const int port = 12345;
	constexpr const int bufsize = (1 << 12);

	HttpWorker::g_stop = false;

	HttpWorker worker {ip, port, bufsize};
	worker();


//	const char* ip = argv[1];
//	const short int port = std::atoi(argv[2]);
//	const size_t bufsize = std::atoi(argv[3]);
//
//	auto fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (fd < 0) {
//		throw std::system_error(errno, std::generic_category(), "socket");
//	}
//
//	int yes = 1;
//	if (auto res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); res < 0) {
//		throw std::system_error(errno, std::generic_category(), "setsockopt");
//	}
//
//	sockaddr_in addr{};
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(port);
//	addr.sin_addr.s_addr = inet_addr(ip);
//
//	if (auto res = bind(fd, (sockaddr*)&addr, sizeof(addr)); res < 0) {
//		throw std::system_error(errno, std::generic_category(), "bind");
//	}
//
//	if (auto res = listen(fd, 1); res < 0) {
//		throw std::system_error(errno, std::generic_category(), "listen");
//	}
//
//	std::cout << "listening on 127.0.0.1:12345" << std::endl;
//
//	while (true) {
//
//		sockaddr_in client{};
//		socklen_t len = sizeof(client);
//		auto clientfd = accept(fd, (sockaddr*)&client, &len);
//		if (clientfd < 0) {
//			throw std::system_error(errno, std::generic_category(), "accept");
//		}
//
//		char buf[bufsize];
//		ssize_t n = read(clientfd, buf, bufsize-1);
//
//		HttpRequest obj {};
//		std::string resp = obj.test_response();
//		write(clientfd, resp.c_str(), resp.size());
//		close(clientfd);
//
//	}
//
//
//	close(fd);

	return 0;
}
