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
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (auto res = bind(fd, (sockaddr*)&addr, sizeof(addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "bind");
	}

	if (auto res = listen(fd, 1); res < 0) {
		throw std::system_error(errno, std::generic_category(), "listen");
	}

	std::cout << "listening on 127.0.0.1:12345" << std::endl;
	char buf[1024];

	while (true) {


		sockaddr_in client{};
		socklen_t len = sizeof(client);
		auto clientfd = accept(fd, (sockaddr*)&client, &len);
		if (clientfd < 0) {
			throw std::system_error(errno, std::generic_category(), "accept");
		}

		ssize_t n = read(clientfd, buf, sizeof(buf));
		std::cout << std::string(buf, n) << std::endl;

		close(clientfd);

	}


	close(fd);

	return 0;
}
