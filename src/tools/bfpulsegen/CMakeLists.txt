add_executable(bfpulsegen bfpulsegen.cpp)

target_link_libraries(bfpulsegen bigfoot ${BIGFOOT_TOOLLIBRARIES})

	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
set_target_properties(bfpulsegen PROPERTIES OUTPUT_NAME bfpulsegen-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	set_target_properties(bfpulsegen PROPERTIES OUTPUT_NAME bfpulsegen)
ENDIF()


install(TARGETS bfpulsegen DESTINATION bin)
