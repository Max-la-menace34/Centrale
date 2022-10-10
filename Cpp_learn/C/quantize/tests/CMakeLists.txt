
# Check for the required dependencies
find_package (Boost COMPONENTS unit_test_framework REQUIRED)

# List every unitary test file
file(
	GLOB 
	testfiles
	test*.cpp
)

# For every example
foreach(f ${testfiles})
  # We build the name of the target by removing the extension ".cpp" 
  get_filename_component(testname ${f} NAME_WE) 

  # Actually define the target
  add_executable(${testname} ${f}) 

  # Add the include directories for the headers of the boost unit test framework
  target_include_directories (${testname} 
	  PUBLIC ${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

  # Add the flag required to link against the Unit Test Framework
  target_compile_definitions(${testname} PUBLIC BOOST_TEST_DYN_LINK)

  target_link_libraries(${testname} 
	  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	  )

  # And then register the example within the test suite
  add_test(NAME ${testname}
	       COMMAND ${testname})
endforeach(f)

