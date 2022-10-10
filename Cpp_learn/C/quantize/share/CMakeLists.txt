install(FILES clint.jpeg 
	DESTINATION share/${CMAKE_PROJECT_NAME})

configure_file(ui/windows.glade ${CMAKE_CURRENT_BINARY_DIR}/ui/windows.glade)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ui/windows.glade
	DESTINATION share/${CMAKE_PROJECT_NAME}/ui)
