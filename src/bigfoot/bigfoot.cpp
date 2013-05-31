/*!
 * \file bigfoot.cpp 
 * \brief Memory-mapped page based to access big binary files
 * \author Jose Vicente
 * \date 05/31/2013
 *
 * This class provides read/write access to big binary files
 * using a page level cache which access the file using pages
 * based on boost file memory-mapping
 */

#include "bigfoot.hpp"
#include <iostream>
#include <armadillo>
#include <time.h>

namespace bigfoot {

	using namespace boost;
	using namespace arma;

	//Constructors

	//read/write constructor
	bufferedfile::bufferedfile(std::string filename, std::size_t nrows, std::size_t ncols, std::size_t nmappedelements, std::size_t offset){
		//nrows : number of rows of elements
		//ncols : number of cols of elements
		//pagesize: number of elements to be mapped
		//offset: offset to data location in bytes (to skip file's header if necessary)
		_nrows = nrows;
		_ncols = ncols;
		_elementsize = sizeof(double);

		if (nmappedelements==0){
			_reqnmappedelements = _nrows*_ncols;
		} else {
			_reqnmappedelements = nmappedelements;
		}

		_offset = offset;
		
		//compute the minimum _pagesizeinbytes able to contain the _reqpagesie
		//FIXME: review pagewidth for offsets computations (offset and element size)
		if ((_reqnmappedelements * _elementsize) % boost::iostreams::mapped_file::alignment() == 0){
			_pagesizeinbytes = _reqnmappedelements * _elementsize;
			_nmappedelements = _reqnmappedelements;
		} else {
			int prevmultiple = (_reqnmappedelements * _elementsize) / boost::iostreams::mapped_file::alignment();
			_pagesizeinbytes = (prevmultiple + 1) * boost::iostreams::mapped_file::alignment();  
			_nmappedelements = (_pagesizeinbytes) / _elementsize;
		}
		_filename = filename;
		populatecache(0);
	}

	void bufferedfile::populatecache(std::size_t pageidx){
		if (_file.is_open()){
			_file.close();
		}
		//request one extra 4*aligment page to avoid data elements missalignment caused by the initial offset size
		std::size_t pagetrail = 4 * boost::iostreams::mapped_file::alignment();
			//std::cout << "requested page #: " << pageidx << std::endl;
		_file.open(_filename, ios::out | ios::binary, (_pagesizeinbytes + pagetrail), pageidx*_pagesizeinbytes) ;

		if (!_file.is_open()){
			throw("Input file could not be mapped.");
		}
		_cachedpagenumber = pageidx;
		if(pageidx>0){
			_iocache = (double *)_file.data();
			_pageinitaddress = pageidx * _nmappedelements;
			_pageendaddress = _pageinitaddress + _nmappedelements - 1;

		} else {
			//FIXME:
			_iocache = (double *)_file.data() + _offset;
			_pageinitaddress = 0;
			_pageendaddress = _nmappedelements - 1;
		}
	}

	std::size_t bufferedfile::page() const{
		return _cachedpagenumber;
	}

	//getter andf setters assuming rowmajor storage
	void bufferedfile::operator()(const std::size_t row, const std::size_t col, double value){
		std::size_t elementaddress = row + col*_nrows;
		//std::size_t elementaddress = col + row*(_ncols-1);
		if ((elementaddress > _pageendaddress) || (elementaddress < _pageinitaddress)){
			populatecache(elementaddress/_nmappedelements);
		}
		size_t elementpageaddress = elementaddress % _nmappedelements;
		(*(_iocache + elementpageaddress))=value;
	}

	double bufferedfile::operator()(const std::size_t row, const std::size_t col) {
		std::size_t elementaddress = row + col*_nrows;
		//std::size_t elementaddress = col + row*(_ncols-1);
		if ((elementaddress > _pageendaddress) || (elementaddress < _pageinitaddress)){
			populatecache(elementaddress/_nmappedelements);
		}
		size_t elementpageaddress = elementaddress % _nmappedelements;
		return (*(_iocache + elementpageaddress));
	}
	
	arma::mat bufferedfile::operator()(arma::span rows,arma::span cols){
	//FIXME: Implement this method properly
		//compute page containing requested span
		//and populate the matrix
		arma::mat m;
		return m;
	}
}
/*
int main(int argc, char **argv) {

	if (argc <7){
		std::cout << "Usage: bigfoot file nrows ncols numofmappedelements offset fromsample tosample" <<std::endl;
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
			val = bf(i,j);
			++val;
			//val = 0;
			bf(i,j,val);
		}
	}
	//t2=clock();
	//float diff ((float(t2) - (float)t1));
	//std::cout << t1 << "," << t2 << "," << diff << 
	//stdf::cout << std::endl;

*
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
	*
}
*/
