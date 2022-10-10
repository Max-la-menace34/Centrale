# We check Doxygen is available, otherwise we cannot
# build the doc
find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
	message("Doxygen not found, I will not generate/install the documentation")
else()
	# We fill in the CMake variables in the template file
	# Doxyfile.in	
	configure_file(Doxyfile.in Doxyfile)

	set(DOXYGEN_INPUT ${CMAKE_BINARY_DIR}/doc/Doxyfile)

	# We specify the command used by doxygen to build the doc
	add_custom_target(doc ALL
		COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} > /dev/null
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		)

	# And finally install the documation in the standard path
	# CMAKE_PREFIX/share/doc/gtkmmExample/html/
	install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html 
		    DESTINATION share/doc/${CMAKE_PROJECT_NAME})

	# We do not need to install the images, Doxygen is taking care of them

endif()
