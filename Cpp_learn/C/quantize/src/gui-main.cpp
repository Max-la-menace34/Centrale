#include <iostream>
#include "gui-application-window.hpp"


int main(int argc, char* argv[]) {

	auto app = Gtk::Application::create(argc, argv, "fr.centralesupelec.quantize");

	std::string ui_file{QUANTIZE_SHARE "/ui/windows.glade"};
	/* std::cout << "Opening : " << ui_file << std::endl; */
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file(ui_file);
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

	quantize::ApplicationWindow* appWin = nullptr;
	refBuilder->get_widget_derived("mainWindow", appWin);

	app->run(*appWin);

	delete appWin;
}
