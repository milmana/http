#include <iostream>
#include <memory>

#include "naive.hh"

int main(int argc, char **argv) {

	std::cout << "hello" << std::endl;

	auto ptr = std::make_unique<naive::worker_t>();

	return 0;
}
