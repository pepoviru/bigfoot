#include <bigfoot/bigfoot.hpp>
#include <iostream>
#include <armadillo>
#include <time.h>

int main(int argc, char **argv) {
using namespace bigfoot;

	if (argc <7){
		std::cout << "Usage: bftozeros file nrows ncols numofmappedelements offset fromsample tosample" <<std::endl;
		return -1;
	}

	std::size_t nrows = atoi(argv[2]);
	std::size_t ncols = atoi(argv[3]);
	std::size_t nummappedels = atoi(argv[4]);
	std::size_t offset = atoi(argv[5]);
	std::size_t fromsample = atoi(argv[6]);
	std::size_t tosample = atoi(argv[7]);
	bigfoot::bufferedfile bf(argv[1], nrows, ncols, nummappedels, offset);

	//std::cout << "file,nrows,ncols,numelems,offset,from,to,start,stop,exect" << std::endl;
	std::cout << argv[1] << "," << nrows << "," << ncols << "," << nummappedels << "," << offset << "," << fromsample << "," << tosample;
	//clock_t t1,t2;
	//t1=clock();
 	double val;
	for(std::size_t j=fromsample;j<tosample;++j){
		for(std::size_t i=0;i<nrows;++i){
			//val = bf(i,j);
			//++val;
			val = 0;
			bf(i,j,val);
		}
	}
	//t2=clock();
	//float diff ((float(t2) - (float)t1));
	//std::cout << t1 << "," << t2 << "," << diff << 
	//stdf::cout << std::endl;

/*
	std::cout << "First 5 samples of each row." << std::endl;
	for(std::size_t i=0;i<nrows;++i){
		for(std::size_t j=0;j<5;++j){
			double val = bf(i,j);
			std::cout << val << "\t";
			++val;
			//val = 0;
			bf(i,j,val);
		}
		std::cout << std::endl;
	}
	std::cout << "Last 5 samples of each row." << std::endl;
	for(std::size_t i=0;i<nrows;++i){
		for(std::size_t j=ncols-5;j<ncols;++j){
			double val = bf(i,j);
			std::cout << val << "\t";
			++val;
			//val = 0;
			bf(i,j,val);
		}
		std::cout << std::endl;
	}
	*/
}
