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

	if (argc < 4) {
		std::cerr << "USAGE: test_client \"ip\" port \"msg\"" << std::endl;
		return -1;
	}

	const char* ip_str = argv[1];
	short int   port   = std::atoi(argv[2]);
	const char* msg    = argv[3];

	auto fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		throw std::system_error(errno, std::generic_category(), "socket");
	}

	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (auto res = inet_pton(AF_INET, ip_str, &server_addr.sin_addr); res < 0) {
		throw std::system_error(errno, std::generic_category(), "inet_pton");
	}

	if (auto res = connect(fd, (sockaddr*)&server_addr, sizeof(server_addr)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "connect");
	}

	// const char* msg = "you don't have the cards";
	if (auto res = write(fd, msg, strlen(msg)); res < 0) {
		throw std::system_error(errno, std::generic_category(), "socket");
	}

	std::cout << "message sent..." << std::endl;

	close(fd);

	return 0;
}
