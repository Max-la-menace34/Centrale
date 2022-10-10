# Look for the examples with a Regular Expression (regexp)
# For the demonstration purpose, we look for every source that matches
# example*.cpp 
# As this directory contains only examples, we could have simplified
# the regexp down to *.cpp
file(
	GLOB 
	usage_examples
	example*.cpp
)

# For every example
foreach(f ${usage_examples})
  # We build the name of the target by removing the extension ".cpp" 
  get_filename_component(exampleName ${f} NAME_WE) 

  # Actually define the target
  add_executable (${exampleName} ${f}) 

  # Define the include path to the headers or our package
  target_include_directories(${exampleName} PUBLIC ${CMAKE_SOURCE_DIR}/src)
 
  # And we install it in the "${CMAKE_PREFIX}/bin" directory
  # We add the prefix of the project name to avoid collision
  # of programs from other packages
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exampleName}
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-${exampleName})

endforeach(f)


# Look for the examples requiring to be linked with the library 
# provided by the package
file(
	GLOB 
	cv_usage_examples
	cv-example*.cpp
)

# For every example
foreach(f ${cv_usage_examples})
  # We build the name of the target by removing the extension ".cpp" 
  get_filename_component(exampleName ${f} NAME_WE) 

  # Actually define the target
  add_executable (${exampleName} ${f}) 

  # Define the include path to the headers or our package
  target_include_directories(${exampleName} PUBLIC ${CMAKE_SOURCE_DIR}/src)

  # Link with our library
  target_link_libraries(${exampleName} cvquantize)

  target_compile_definitions(${exampleName} PUBLIC QUANTIZE_SHARE="${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}")

  # And we install it in the "${CMAKE_PREFIX}/bin" directory
  # We add the prefix of the project name to avoid collision
  # of programs from other packages
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${exampleName}
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-${exampleName})

endforeach(f)
