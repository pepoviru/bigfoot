#include <bigfoot/bigfoot.hpp>
#include <iostream>
#include <armadillo>
#include <time.h>

int main(int argc, char **argv) {
using namespace bigfoot;

	if (argc <10){
		std::cout << "Usage: bfpulsegen file nrows ncols numofmappedelements offset fromsample tosample value period" <<std::endl;
		return -1;
	}

	std::size_t nrows = atoi(argv[2]);
	std::size_t ncols = atoi(argv[3]);
	std::size_t nummappedels = atoi(argv[4]);
	std::size_t offset = atoi(argv[5]);
	std::size_t fromsample = atoi(argv[6]);
	std::size_t tosample = atoi(argv[7]);
	double val = atof(argv[8]);
	unsigned int period = atoi(argv[9]);
	bigfoot::bufferedfile bf(argv[1], nrows, ncols, nummappedels, offset);

	//std::cout << "file,nrows,ncols,numelems,offset,from,to,start,stop,exect" << std::endl;
	std::cout << argv[1] << "," << nrows << "," << ncols << "," << nummappedels << "," << offset << "," << fromsample << "," << tosample;
	
	for(std::size_t j=fromsample;j<tosample;++j){
		for(std::size_t i=0;i<nrows;++i){
			try {
				if (j % period != 0){
					bf(i,j,0);
				} else {
					bf(i,j,val);
				}
			} catch(...) {
				std::cerr << "Error updating element (" << i << ", " << j << ")" << std::endl;
				std::flush(std::cerr);
				return -1;
			}
		}
	}
}
