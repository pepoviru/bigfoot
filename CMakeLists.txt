project(BIGFOOT)
cmake_minimum_required(VERSION 2.8)

########################################################################################
# Configure options

EXEC_PROGRAM( git ARGS describe --abbrev=0 --tags OUTPUT_VARIABLE VER_STR)
string(REPLACE "." ";" VER_LIST ${VER_STR})
LIST(GET VER_LIST 0 VER_MAJOR)
LIST(GET VER_LIST 1 VER_MINOR)
LIST(GET VER_LIST 2 VER_PATCH)
LIST(LENGTH VER_LIST len)

if ( len GREATER 3)
	LIST(GET VER_LIST 3 VER_BUILD)
	SET(VER_PATCH ${VER_PATCH}.${VER_BUILD})
else()
	SET(VER_BUILD "")
endif()

option(WITH_STATIC "With static" OFF)
if(NOT WITH_STATIC)
	if(WIN32)
		set(WITH_STATIC 1)
	else()
		set(WITH_STATIC 0)
	endif()
endif()

option(WITH_SHARED "With shared" OFF)
if(NOT WITH_SHARED)
	if(UNIX)
		set(WITH_SHARED 1)
	else()
		set(WITH_SHARED 0)
	endif()
endif()

if(NOT WITH_SHARED AND NOT WITH_STATIC)
	message(FATAL_ERROR "Cannot compile without static and shared!")
endif()

#option(WITH_TEST "With testing" OFF)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

########################################################################################
# Detect necessary libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build_aux/cmake/Modules/")

set(BIGFOOT_LIBRARIES "" CACHE INTERNAL "BIGFOOT LIBRARIES")
set(BIGFOOT_TOOLLIBRARIES "" CACHE INTERNAL "BIGFOOT LIBRARIES")

# Boost
if( "$ENV{BOOST_ROOT}" STREQUAL "")
	message(STATUS "environment variable BOOST_ROOT not defined searching in default paths")
else()
	message(STATUS "BOOST_ROOT set to: $ENV{BOOST_ROOT}, so skipping system path")
	set(Boost_NO_SYSTEM_PATHS TRUE)
endif()

include(FindBoost)
if(WITH_TOOLS)
	find_package(Boost COMPONENTS mpi)
	
	include(FindMPI)
	if(MPI_CXX_FOUND)
		message(STATUS "MPI_INCLUDE_DIR = ${MPI_CXX_INCLUDE_PATH}")
		message(STATUS "MPI_LIB_DIR = ${MPI_CXX_LIBRARIES}")

		include_directories(include ${MPI_CXX_INCLUDE_PATH})
		set(BIGFOOT_TOOLLIBRARIES "${MPI_CXX_LIBRARIES}" CACHE INTERNAL "BIGFOOT LIBRARIES")
		set(WITH_MPI ON)
	else()
		if(WITH_MPI)
			message(FATAL_ERROR "MPI NOT FOUND")
		endif()
	endif()
endif()

find_package(Boost COMPONENTS date_time filesystem system regex program_options iostreams serialization)

if(Boost_FOUND)
	message(STATUS "Boost_MAJOR_VERSION = ${Boost_MAJOR_VERSION}")
	message(STATUS "Boost_MINOR_VERSION = ${Boost_MINOR_VERSION}")
	message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
	message(STATUS "Boost_LIB_DIR = ${Boost_LIBRARY_DIRS}")

	# FIXME Check version has fs3
	include_directories(include ${Boost_INCLUDE_DIR})
	set(BIGFOOT_LIBRARIES "${BIGFOOT_LIBRARIES};${Boost_LIBRARIES}" CACHE INTERNAL "BIGFOOT LIBRARIES")
else()
	message(FATAL_ERROR "Boost not found")
endif()

# Armadillo (lapack/blas)
include(FindArmadillo)
if(ARMADILLO_FOUND)
	message(STATUS "Armadillo_MAJOR_VERSION = ${ARMADILLO_VERSION_MAJOR}")
	message(STATUS "Armadillo_MINOR_VERSION = ${ARMADILLO_VERSION_MINOR}")

	include_directories(include ${ARMADILLO_INCLUDE_DIRS})
	if(UNIX)
		set(BIGFOOT_LIBRARIES "${BIGFOOT_LIBRARIES};${ARMADILLO_LIBRARY}" CACHE INTERNAL "BIGFOOT LIBRARIES")
	endif()
else()
	message(FATAL_ERROR "Armadillo not found")
endif()

if( "$ENV{BLAS_LAPACK}" STREQUAL "")
	include(FindBLAS)
	include(FindLAPACK)
	if(BLAS_FOUND AND LAPACK_FOUND)
		message(STATUS "Blas libraries = ${BLAS_LIBRARIES}")
		message(STATUS "Lapack libraries = ${LAPACK_LIBRARIES}")

		set(BIGFOOT_LIBRARIES "${BIGFOOT_LIBRARIES};${LAPACK_LIBRARIES}" CACHE INTERNAL "BIGFOOT LIBRARIES")
	else()
		message(FATAL_ERROR "Lapack/Blas not found")
	endif()
else()
	message(STATUS "Skipping autodetect for lapack/atlas and use the following libraries: ${$ENV{LAPACK_ROOT}}")
	set(BIGFOOT_LIBRARIES "${BIGFOOT_LIBRARIES};$ENV{BLAS_LAPACK}" CACHE INTERNAL "BIGFOOT LIBRARIES")
endif()


set(BIGFOOT_TOOLLIBRARIES "${BIGFOOT_LIBRARIES};${BIGFOOT_TOOLLIBRARIES}" CACHE INTERNAL "BIGFOOT LIBRARIES")

########################################################################################
# Setup packaging / output
IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	SET(PACKAGE_NAME "bigfoot-dbg")
	SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${CMAKE_CXX_DEBUG_FLAGS} -DTIXML_USE_TICPP=YES")
ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	SET(PACKAGE_NAME "bigfoot")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_TICPP=YES")
ENDIF()

set(CPACK_PACKAGE_DESCRIPTION "Big Binary Files Access Library")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Big Bimary Files Access Library written in C++")
set(CPACK_PACKAGE_NAME ${PACKAGE_NAME})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_PACKAGE_CONTACT "Jose Vicente")
set(CPACK_PACKAGE_VENDOR "Jose Vicente")
set(CPACK_PACKAGE_VERSION_MAJOR ${VER_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VER_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VER_PATCH})

########################################################################################
# Generate config.hpp
SET( BIGFOOT_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(BIGFOOT_VERSION_MAJOR ${VER_MAJOR})
set(BIGFOOT_VERSION_MINOR ${VER_MINOR})
set(BIGFOOT_VERSION_PATCH ${VER_PATCH})

INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)

TEST_BIG_ENDIAN( BIG_ENDIAN )

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/src/bigfoot/config.hpp.cmake ${CMAKE_SOURCE_DIR}/src/bigfoot/config.hpp )

if(WIN32)
	set(CPACK_GENERATOR "TGZ")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
else()
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}")
	set(CPACK_GENERATOR "DEB;TGZ")
endif()

include(CPack)

message(STATUS "**************************SUMMARY*************************")
message(STATUS "Version: ${BIGFOOT_VERSION}")
message(STATUS "Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Tools: ${WITH_TOOLS}")
message(STATUS "MPI: ${WITH_MPI}")
message(STATUS "Static: ${WITH_STATIC}")
message(STATUS "Shared: ${WITH_SHARED}")
message(STATUS "**********************************************************")

add_subdirectory(src)

