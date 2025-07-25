// C++ headers
#include <iostream>
#include <system_error>

// C headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

// local
#include "lib.hh"

int main(int argc, char **argv) {

	auto sockfd = socket(AF_INET, SOCK_STREAM, 0); // tcp
	if (sockfd < 0) {
		throw std::system_error(errno, std::generic_category(), "socket");
	}

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	if (auto res = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); res == 1) {
		// success
	} else if (res == 0) {
		throw std::runtime_error("inet_pton: invalid format");
	} else { // res < 0
		throw std::system_error(errno, std::generic_category(), "inet_pton");
	}

	if (auto res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "connect");
	}

	if (auto res = write(sockfd, "hello", 5); res < 0) {
		throw std::system_error(errno, std::generic_category(), "write");
	}

	close(sockfd);

	return 0;
}
