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

	template<typename myDataType> arma::mat bufferedfile<myDataType>::operator()(arma::span rows,arma::span cols){
	//FIXME: Implement this method properly
		//compute page containing requested span
		//and populate the matrix
		arma::mat m;
		return m;
	}
}
