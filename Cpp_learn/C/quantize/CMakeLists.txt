cmake_minimum_required(VERSION 3.10)

# Project definition, this sets the variable 
# ${CMAKE_PROJECT_NAME} and ${PROJECT_VERSION}
project(quantize
		VERSION 1.51)

# Sets up the compile flags  
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

message(STATUS "Some information about the available build types:")
foreach(buildtype "DEBUG" "RELEASE" "RELWITHDEBINFO" "MINSIZEREL")
	message("   Using ${buildtype} uses the following cflags : ${CMAKE_C_FLAGS_${buildtype}}")
endforeach(buildtype)

# Search for dependencies
find_package(PkgConfig)


# For demonstration, we request opencv version higher than 3.0
pkg_check_modules(OPENCV REQUIRED opencv>=3.0)
#pkg_check_modules(OPENCV REQUIRED opencv4)


###################################
# Set up specific C++ flags
##################################
# Cmake 3.10 supports the standard up to c++17
# for C++20, you need at least cmake 3.12.4
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##################################
# Project hierarchy
##################################
add_subdirectory(src)
add_subdirectory(share)
add_subdirectory(doc)
add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)

##################################
# Pkg-config generation
##################################

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pc
	"
Name: ${CMAKE_PROJECT_NAME}
Description: A generic C++ implementation of K-means with examples using OpenCV and gtkmm3
Version: ${PROJECT_VERSION}
Requires: opencv >= 3.0
Libs: -L${CMAKE_INSTALL_PREFIX}/lib -lcvquantize
Cflags: -I${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME} -std=c++17
	"
	)


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pc
	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

###################################
# dist target
###################################
SET(DIST_DIR "${CMAKE_PROJECT_NAME}")
ADD_CUSTOM_TARGET(dist 
	COMMAND rm -rf ${DIST_DIR}
	COMMAND mkdir  ${DIST_DIR}
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/* ${DIST_DIR} || true 
	COMMAND rm -rf ${DIST_DIR}/build
	COMMAND mkdir ${DIST_DIR}/build
	COMMAND tar --exclude="*~" --exclude="._*" -zcvf ${DIST_DIR}-${PROJECT_VERSION}.tar.gz ${DIST_DIR}
	COMMAND rm -rf  ${DIST_DIR}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

###################################
# dist target
###################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ application for demonstrating Kmeans clustering")
SET(CPACK_PACKAGE_VENDOR "Jeremy Fix")
SET(CPACK_PACKAGE_CONTACT "{Jeremy_DOT_fix_AT_centralesupelec.fr")
SET(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
SET(CPACK_PACKAGE_LICENSE "GPL")
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
SET(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
SET(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/README.md)

ADD_CUSTOM_TARGET(packages
		  COMMAND rm -rf build
		  COMMAND mkdir build
		  COMMAND cd build && cmake ${CMAKE_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX=/usr && make package -j
		  COMMAND cp build/*.deb . || true
		  COMMAND rm -rf build
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

###############################################
# For DEBs
###############################################
find_program( DPKGDEB
	 NAMES dpkg-deb
	 PATHS "/usr/bin"
)
IF(NOT DPKGDEB STREQUAL  "DPKGDEB-NOTFOUND")
  MESSAGE("Set up for building DEB")

  SET(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME}-dev)
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-3.0-dev (>=  3.22), libopencv-core-dev (>= 3.0)")

  SET(CPACK_PACKAGE_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_PROCESSOR})

  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
  SET(CPACK_GENERATOR "DEB")
ENDIF(NOT DPKGDEB STREQUAL  "DPKGDEB-NOTFOUND")

###############################################
INCLUDE(CPack)


##################################
# CMake Debug
##################################
# For debugging, display the variables defined by cmake
# By default, do not display them
if(NOT DISPLAY_CMAKE_VAR)
	set(DISPLAY_CMAKE_VAR FALSE)
endif()
if(DISPLAY_CMAKE_VAR)
	get_cmake_property(_variableNames VARIABLES)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
endif()
# We need to remove the variable from the cache. Otherwise, if you do not
# specify it, the value in the cache will be used
unset(DISPLAY_CMAKE_VAR CACHE)
