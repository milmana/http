#include <iostream>
#include <system_error>

#include <cstring> // memset

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo
#include <arpa/inet.h> // INET6_ADDRSTRLEN

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " hostname" << std::endl;
		exit(1);
	}

	std::cout << "argv[0] = " << argv[0] << std::endl;
	std::cout << "argv[1] = " << argv[1] << std::endl;

	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP

	struct addrinfo *res = nullptr;
	if (auto status = getaddrinfo(argv[1], nullptr, &hints, &res); status != 0) {
		throw std::system_error(errno, std::system_category(), gai_strerror(status));
	}
	for (auto p = res; p != nullptr; ++p) {
		if (p->ai_family == AF_INET) { // IPv4
			auto ipv4 = (struct sockaddr_in *)p->ai_addr;
			auto addr = &(ipv4->sin_addr);
		} else { // IPv6
			auto ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			auto addr = &(ipv6->sin6_addr);
		}
	}
	freeaddrinfo(res);

	// char ipstr[INET6_ADDRSTRLEN];

}
