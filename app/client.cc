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

	if (auto res = connect(fd, (struct sockaddr*)&addr, sizeof(addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "connect");
	}

	if (auto res = write(fd, "hello", 5); res < 0) {
		throw std::system_error(errno, std::generic_category(), "write");
	}

	close(fd);

	return 0;
}
