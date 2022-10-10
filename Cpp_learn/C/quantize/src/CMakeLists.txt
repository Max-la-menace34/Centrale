# The header files are in this source directory
# they need to be included for compiling the library and GUI
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# In old cmake (v3.10) we need to include the library dirs
# globally
# In recent cmake, we can restrict that by calling
# target_link_libraries
link_libraries(${OPENCV_LIBRARIES} ${GTKMM_LIBRARIES})

###################################
# build up the library
###################################
add_library(cvquantize
	SHARED
	quantize-cv.cpp)
target_include_directories(cvquantize PUBLIC ${OPENCV_INCLUDE_DIRS})
target_link_libraries(cvquantize ${OPENCV_LIBRARIES})
# target_link_directories(cvquantize PUBLIC ${OPENCV_LIBRARY_DIRS})
install(TARGETS cvquantize
	DESTINATION lib)

#######################################
# Install the headers of the package
# All the headers will be installed 
# in CMAKE_PREFIX/include/quantize/
#######################################

file(
	GLOB 
	headerfiles	
	quantize*.hpp
	)

install(FILES ${headerfiles}
	DESTINATION include/${CMAKE_PROJECT_NAME})


###################################
# build up the GUI
###################################

message(STATUS "Looking for gtkmm")
pkg_check_modules(GTKMM gtkmm-3.0>=3.22)
if(GTKMM_FOUND)
	message(STATUS "gtkmm is found with version ${GTKMM_VERSION}. We are going to compile the GUI")

	file(
		GLOB 
		guifiles	
		gui-*.cpp
		)

	add_executable(gui ${guifiles})
	target_include_directories(gui PUBLIC ${GTKMM_INCLUDE_DIRS})	
	target_link_libraries(gui cvquantize ${GTKMM_LIBRARIES} pthread)
	# target_link_directories(gui PUBLIC ${GTKMM_LIBRARY_DIRS})
	target_compile_definitions(gui PUBLIC QUANTIZE_SHARE="${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}/")

	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/gui 
		DESTINATION bin
		RENAME ${CMAKE_PROJECT_NAME}-gui)
else()
	MESSAGE(WARNING "We did not find the required version of gtkmm. We are not going to compile the GUI")
endif()


