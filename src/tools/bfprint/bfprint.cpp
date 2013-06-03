#include <bigfoot/bigfoot.hpp>
#include <iostream>
#include <armadillo>
#include <time.h>

int main(int argc, char **argv) {
using namespace bigfoot;

	if (argc <6){
		std::cout << "Usage: bfprint file nrows ncols numofmappedelements offset" <<std::endl;
		return -1;
	}

	std::size_t nrows = atoi(argv[2]);
	std::size_t ncols = atoi(argv[3]);
	std::size_t nummappedels = atoi(argv[4]);
	std::size_t offset = atoi(argv[5]);
	bigfoot::bufferedfile<double> bf(argv[1], nrows, ncols, nummappedels, offset);

	std::cout << "First 5 samples of each row." << endl;
	double val;
	for(std::size_t i=0;i<nrows;++i){
		for(std::size_t j=0;j<5;++j){
			val = bf(i,j);
			std::cout << val << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Last cached page #: " << bf.page() <<std::endl;
/*	int page = bf.page();
	for(std::size_t j=0;j<ncols;++j){
		for(std::size_t i=0;i<nrows;++i){
			val = bf(i,j);
			//std::cout << val << "\t";
			if (page!=bf.page()){
				page = bf.page();
				std::cout << "=============" << endl;
				std::cout << "Pagebreak to page # " << page << endl;
				std::cout << "(" << i << "," << j << ")" << endl;
				std::cout << "=============" << endl;
			}
			//std::cout << "(" << i << "," << j << ")";
		}
		//std::cout << std::endl;
	}
*/	
	std::cout << "Last 5 samples of each row. " << std::endl;
	for(std::size_t i=0;i<nrows;++i){
		for(std::size_t j=ncols-5;j<ncols;++j){
			val = bf(i,j);
			std::cout << val << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Last cached page #: " << bf.page() <<std::endl;
}
