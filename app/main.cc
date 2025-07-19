#include <iostream>
#include <memory>

#include "lib.hh"

int main(int argc, char **argv) {

	std::cout << "hello" << std::endl;

	auto ptr = std::make_unique<lib::worker_t>();

	return 0;
}
