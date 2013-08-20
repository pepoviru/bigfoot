add_executable(bftozeros bftozeros.cpp)

target_link_libraries(bftozeros bigfoot ${BIGFOOT_TOOLLIBRARIES})

	IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
set_target_properties(bftozeros PROPERTIES OUTPUT_NAME bftozeros-dbg)
	ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	set_target_properties(bftozeros PROPERTIES OUTPUT_NAME bftozeros)
ENDIF()


install(TARGETS bftozeros DESTINATION bin)
