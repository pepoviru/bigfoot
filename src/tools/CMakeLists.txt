link_directories(${LINK_DIRECTORIES} ${BIGFOOT_BINARY_DIR}/src/bigfoot)

#add_subdirectory(whatevertool)

add_executable(bfprint bfprint.cpp)

target_link_libraries(bfprint bigfoot ${BIGFOOT_TOOLLIBRARIES})

IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	set_target_properties(bfprint PROPERTIES OUTPUT_NAME bfprint-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		set_target_properties(bfprint PROPERTIES OUTPUT_NAME bfprint)
		ENDIF()


		install(TARGETS bfprint DESTINATION bin)
