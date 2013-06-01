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
	class bufferedfile{
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
			double* _iocache;
			/*! Memory-mapped file */
			boost::iostreams::mapped_file _file;
			/*! remaps the cache to the matrix page with \a pageidx number */
			void populatecache(std::size_t pageidx);

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
			bufferedfile(std::string filename, std::size_t nrows, std::size_t ncols, std::size_t nmappedelements=0, std::size_t offset=0);
			~bufferedfile();
			/*! get current loaded page index */
			std::size_t page() const; 
			/*! set \a value of the element at (\a row,\a col) */
			void operator()(const std::size_t row, const std::size_t col, double value);
			/*! get \a value of element at (\a row,\a col) */
			double operator()(const std::size_t row, const std::size_t col);
			/*! get submatrix of \a rows \a cols */
			arma::mat operator()(arma::span rows,arma::span cols);
	};
}

/*! @} End of Doxygen Groups*/
#endif
