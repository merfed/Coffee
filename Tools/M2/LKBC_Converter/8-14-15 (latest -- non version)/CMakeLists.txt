cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/modules")

project(LKBC_Converter)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#config header
get_directory_property(COMPILE_DEFINITIONS COMPILE_DEFINITIONS)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.h")

#Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	#adapted from https://www.tty1.net/blog/2014/cmake-doxygen_en.html
	set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
	set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
	configure_file("${doxyfile_in}" "${doxyfile}")
	add_custom_target(doc
		COMMAND "${DOXYGEN_EXECUTABLE}" "${doxyfile}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM)
endif(DOXYGEN_FOUND)

#unit tests
include(tests/tests.cmake)

#files
add_executable(lkbc_converter src/lkbc_converter.c src/reading.c src/fallback.c src/conversion.c src/writing.c src/printer.c src/common.c src/fcaseopen.c)
