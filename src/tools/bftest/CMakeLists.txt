add_executable(bftest bftest.cpp)

target_link_libraries(bftest bigfoot ${BIGFOOT_TOOLLIBRARIES})

	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
set_target_properties(bftest PROPERTIES OUTPUT_NAME bftest-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	set_target_properties(bftest PROPERTIES OUTPUT_NAME bftest)
ENDIF()


install(TARGETS bftest DESTINATION bin)
