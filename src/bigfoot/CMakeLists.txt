SET(BIGFOOT_SRCS
	bigfoot.cpp
)

IF( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	# Base
	install(DIRECTORY . DESTINATION include/bigfoot/ FILES_MATCHING PATTERN "*.hpp")
ENDIF()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if(WITH_STATIC)
	add_library(bigfootStatic STATIC ${BIGFOOT_SRCS})
	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		set_target_properties(bigfootStatic PROPERTIES OUTPUT_NAME bigfoot-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		set_target_properties(bigfootStatic PROPERTIES OUTPUT_NAME bigfoot)
	ENDIF()
	
	install(TARGETS bigfootStatic ARCHIVE DESTINATION lib)
endif()

if(WITH_SHARED)
	add_library(bigfoot SHARED ${BIGFOOT_SRCS})
	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		set_target_properties(bigfoot PROPERTIES OUTPUT_NAME bigfoot-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		set_target_properties(bigfoot PROPERTIES OUTPUT_NAME bigfoot)
	ENDIF()

	target_link_libraries(bigfoot ${BIGFOOT_LIBRARIES})

	if(WIN32)
		install(TARGETS bigfoot RUNTIME DESTINATION lib)
	else()
		install(TARGETS bigfoot LIBRARY DESTINATION lib)
	endif()
endif()
