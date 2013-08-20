include_directories(${BIGFOOT_SOURCE_DIR}/src)
link_directories(${LINK_DIRECTORIES}/src)

add_subdirectory(bigfoot)

IF ( WITH_TOOLS )
	add_subdirectory(tools)
ENDIF()
