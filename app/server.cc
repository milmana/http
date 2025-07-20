// C++ includes
#include <iostream>
#include <system_error>

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

int main(int argc, char **argv) {

	auto fd = socket(AF_INET, SOCK_STREAM, 0); // tcp
	if (fd < 0) {
		throw std::system_error(errno, std::generic_category(), "socket");
	}

	int yes = 1;
	if (auto res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "setsockopt");
	}

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (auto res = bind(fd, (sockaddr*)&addr, sizeof(addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "bind");
	}

	if (auto res = listen(fd, 1); res < 0) {
		throw std::system_error(errno, std::generic_category(), "listen");
	}

	sockaddr_in client_addr{};
	socklen_t client_len = sizeof(client_addr);
	auto client_fd = accept(fd, (sockaddr*)&client_addr, &client_len);
	if (client_fd < 0) {
		throw std::system_error(errno, std::generic_category(), "accept");
	}

	std::cout << "client_addr connected..." << std::endl;

	char buf[1024];
	ssize_t n = read(client_fd, buf, sizeof(buf));
	std::cout << "received: " << std::string(buf, n) << std::endl;


	close(fd);
	close(client_fd);

	return 0;
}
