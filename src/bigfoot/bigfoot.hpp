/*!
 * \file bigfoot.hpp 
 * \brief Memory-mapped page based to access big binary files
 * \author Jose Vicente
 * \date 05/31/2013
 *
 * This class provides read/write access to big binary files
 * using a page level cache which access the file using pages
 * based on boost file memory-mapping
 */

#ifndef BIGFOOT_BIGFOOT_JVR_2013_05_31
#define BIGFOOT_BIGFOOT_JVR_2013_05_31 1

#include <boost/iostreams/device/mapped_file.hpp>
#include <armadillo>

/*!
 * \addtogroup bigfoot
 * @{
 */

//! Read/write access to big binary files using pages based on boost file memory-mapping
namespace bigfoot {

	using namespace boost;
	using namespace arma;

	/*!
	 * \class bufferedfile
	 *
	 * \brief Provides read/write access to big binary files
	 *
	 * This class provides read and write access to big binary files
	 * containing a header followed by a 2D matrix.
	 * It uses an internal cache that maps in memory a subset of the 
	 * big file. It manages the mapped area depending on the index
	 * of the last accessed element
	 *
	 * \author Jose Vicente
	 * \date 05/31/2013
	 */
	template <typename myDataType> class bufferedfile{
		private:
			/*! Name of the file containing the matrix*/
			std::string _filename;
			/*! Matrix number of rows */
			std::size_t _nrows;
			/*! Matrix number of columns */
			std::size_t _ncols;
			/*! Minimum number of elements to be mapped in memory */
			std::size_t _reqnmappedelements;
			/*! Actual number of elements mapped so that the pagesize is multiple of mapped_file::alignment() */
			std::size_t _nmappedelements;
			/*! Number of the currently mapped page */
			std::size_t _cachedpagenumber;
			/*! currently mapped page's first element address in the whole data */
			std::size_t _pageinitaddress;
			/*! currently mapped page's last element address in the whole data */
			std::size_t _pageendaddress;
			/*! Element size in BYTES */
			std::size_t _elementsize;
			/*! Offset in BYTES where the matrix begins */
			std::size_t _offset;
			/*! Cache page size in BYTES */
			std::size_t _pagesizeinbytes;
			/*! Pointer to the page in cache */
			myDataType* _iocache;
			/*! Memory-mapped file */
			boost::iostreams::mapped_file _file;
			/*! remaps the cache to the matrix page with \a pageidx number */
			void populatecache(std::size_t pageidx){
				if (_file.is_open()){
					_file.close();
				}

				int end = pageidx * _pagesizeinbytes + (_offset + _pagesizeinbytes);
				int realend = (_nrows * _ncols * _elementsize) + _offset;
				int read = _offset + _pagesizeinbytes;
				if (end > realend) {
					read = 0;
				}

				_file.open(_filename, ios::out | ios::binary, read, pageidx*_pagesizeinbytes) ;

				if (!_file.is_open()){
					throw std::logic_error("Input file could not be mapped.");
				}

				_cachedpagenumber = pageidx;
				_iocache = (myDataType *)(_file.data() + _offset);
				_pageinitaddress = pageidx * _nmappedelements;
				_pageendaddress = _pageinitaddress + _nmappedelements - 1;
			};

		public:
			//Constructors
			/*! Opens and map into memory the first page of the matrix in the \a filename file.
			 * \param filename Path to the file being mapped
			 * \param nrows Number of rows in the matrix
			 * \param ncols Number of columns in the matrix
			 * \param nmappedelements Number of elements to be mapped into memory
			 * \param offset Offset in bytes from the begining of the file where the matrix begins (to bypass file's header)
			 *
			 */
			bufferedfile(std::string filename, std::size_t nrows, std::size_t ncols, std::size_t nmappedelements=0, std::size_t offset=0){
				_nrows = nrows;
				_ncols = ncols;
				_elementsize = sizeof(myDataType);

				if (nmappedelements==0){
					_reqnmappedelements = _nrows*_ncols;
				} else {
					_reqnmappedelements = nmappedelements;
				}

				_offset = offset;

				//compute the minimum _pagesizeinbytes able to contain the _reqpagesize and is in compliance with alignment requirements
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
			};
				
			~bufferedfile(){
				if (_file.is_open()){
					_file.close();
				}
			};


			/*! get current loaded page index */
			std::size_t page() const{
				return _cachedpagenumber;
			};


			/*! set \a value of the element at (\a row,\a col) */
			void operator()(const std::size_t row, const std::size_t col, myDataType value){
				std::size_t elementaddress = row + col*_nrows;
				if ((elementaddress > _pageendaddress) || (elementaddress < _pageinitaddress)){
					populatecache(elementaddress/_nmappedelements);
				}
				size_t elementpageaddress = elementaddress % _nmappedelements;
				try 
				{
					(*(_iocache + elementpageaddress))=value;
				}catch(...){
					std::cerr << "Error updating element (" << row << ", " << col << ") at position " << elementaddress << " in page # " << _cachedpagenumber << " with value " << value << endl;
					throw ("bigfoot update element error.");
				}
			};

			/*! get \a value of element at (\a row,\a col) */
			myDataType operator()(const std::size_t row, const std::size_t col){
				std::size_t elementaddress = row + col*_nrows;
				if ((elementaddress > _pageendaddress) || (elementaddress < _pageinitaddress)){
					populatecache(elementaddress/_nmappedelements);
				}
				size_t elementpageaddress = elementaddress % _nmappedelements;
				return (*(_iocache + elementpageaddress));
			};


			/*! get submatrix of \a rows \a cols */
			arma::mat operator()(arma::span rows,arma::span cols);
	};
}

/*! @} End of Doxygen Groups*/
#endif
