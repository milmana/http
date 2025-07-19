#include "naive.hh"

namespace naive {

worker_t::worker_t() {
	std::cout << "created " << this << std::endl;
}

worker_t::~worker_t() { 
	std::cout << "destroyed " << this << std::endl;
}

}
