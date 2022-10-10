#include <iostream>
#include "quantize-cv.hpp"
#include "gui-application-window.hpp"

quantize::ApplicationWindow::ApplicationWindow(BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& refGlade):
	Gtk::ApplicationWindow(cobject),
	m_refGlade(refGlade),
	m_Dispatcher() {
		connect_signals();
	}

quantize::ApplicationWindow::~ApplicationWindow() {
	delete open_button;
	delete process_button;
	delete quit_button;
	delete about_button;
	delete input_image;
	delete output_image;
	delete spin_colors;
	delete about_window;
	delete file_chooser;
	delete file_open_button;
	delete file_cancel_button;

	if (worker)
	{
		if (worker->joinable())
			worker->join();
		delete worker;
	}
}

void quantize::ApplicationWindow::connect_signals() {
	// Open
	m_refGlade->get_widget("open_button", open_button);
	m_refGlade->get_widget("file_chooser", file_chooser);
	open_button->signal_clicked().connect(sigc::mem_fun(file_chooser,
				&Gtk::Widget::show));

	// Configure the file chooser
	file_chooser->set_transient_for(*this);

	//add filters, so that only certain file types can be selected:
	auto filter_image = Gtk::FileFilter::create();
	filter_image->set_name("Image files");
	filter_image->add_mime_type("image/jpeg");
	filter_image->add_mime_type("image/png");
	file_chooser->add_filter(filter_image);

	m_refGlade->get_widget("file_open_button", file_open_button);
	file_open_button->signal_clicked().connect(sigc::mem_fun(*this,
				&ApplicationWindow::on_file_open_clicked));
	m_refGlade->get_widget("file_cancel_button", file_cancel_button);
	file_cancel_button->signal_clicked().connect(sigc::mem_fun(file_chooser,
				&Gtk::Widget::hide));


	// Process
	m_refGlade->get_widget("spin_colors", spin_colors);
	spin_colors->set_sensitive(false);

	m_refGlade->get_widget("process_button", process_button);
	process_button->set_sensitive(false);
	process_button->signal_clicked().connect(sigc::mem_fun(*this,
				&ApplicationWindow::on_process_clicked));

	m_refGlade->get_widget("input_image", input_image);
	m_refGlade->get_widget("output_image", output_image);

	m_Dispatcher.connect(sigc::mem_fun(*this,
				&ApplicationWindow::on_process_done));

	// Quit
	m_refGlade->get_widget("quit_button", quit_button);
	quit_button->signal_clicked().connect(sigc::mem_fun(*this,
				&Gtk::ApplicationWindow::hide));

	// About
	m_refGlade->get_widget("info_button", about_button);
	m_refGlade->get_widget("about_window", about_window);
	about_window->set_transient_for(*this);
	about_button->signal_clicked().connect(sigc::mem_fun(*about_window,
				&::Gtk::Window::show));


	/* this->signal_configure_event().connect(sigc::mem_fun(*this, */
	/* 			&ApplicationWindow::on_configure), false); */
}

void quantize::ApplicationWindow::on_process_clicked() {
	std::size_t num_colors{std::size_t(spin_colors->get_value())};

	std::cout << "Processing the image" << std::endl;
	this->set_sensitive(false);

	worker = new std::thread([this, num_colors]{
			quantize::cv::compressor compressor(num_colors);	
			{
			std::lock_guard<std::mutex> lock(mutex_img);			
			out_img = compressor.quantize(in_img);
			}
			m_Dispatcher.emit();
			});
}

void quantize::ApplicationWindow::on_process_done() {

	if (worker)
	{
		// Work is done.
		if (worker->joinable())
			worker->join();
		delete worker;
		worker = nullptr;


		::cv::Mat tmp;
		{
			std::lock_guard<std::mutex> lock(mutex_img);			
			::cv::cvtColor(out_img, tmp, ::cv::COLOR_BGR2RGB);
		}
		int out_width = output_image->get_allocated_width();
		int out_height = output_image->get_allocated_height();

		float scale_w = float(out_width)/tmp.size().width;
		float scale_h = float(out_height)/tmp.size().height;
		float scale = std::min(scale_w, scale_h);

		::cv::resize(tmp, out_img_RGB, 
				{int(scale * tmp.size().width),
				int(scale * tmp.size().height)});
		auto size = out_img_RGB.size();
		auto out_pixbuf = Gdk::Pixbuf::create_from_data(
				out_img_RGB.data,
				Gdk::COLORSPACE_RGB,
				out_img_RGB.channels() == 4,
				8, size.width, size.height,
				(int) out_img_RGB.step
				);

		output_image->set(out_pixbuf);
	}
	this->set_sensitive(true);
}

void quantize::ApplicationWindow::on_file_open_clicked() {
	input_filename = file_chooser->get_filename();

	{
		std::lock_guard<std::mutex> lock(mutex_img);			
		in_img = ::cv::imread(input_filename); 
	}
	if(! in_img.empty())
	{
		::cv::Mat tmp;
		::cv::cvtColor(in_img, tmp, ::cv::COLOR_BGR2RGB);

		int in_width = input_image->get_allocated_width();
		int in_height = input_image->get_allocated_height();

		float scale_w = float(in_width)/tmp.size().width;
		float scale_h = float(in_height)/tmp.size().height;
		float scale = std::min(scale_w, scale_h);

		::cv::resize(tmp, in_img_RGB, 
				{int(scale * tmp.size().width),
				int(scale * tmp.size().height)});
		auto size = in_img_RGB.size();
		auto in_pixbuf = Gdk::Pixbuf::create_from_data(
				in_img_RGB.data,
				Gdk::COLORSPACE_RGB,
				in_img_RGB.channels() == 4,
				8, size.width, size.height,
				(int) in_img_RGB.step
				);

		input_image->set(in_pixbuf);
		process_button->set_sensitive(true);
		spin_colors->set_sensitive(true);
	}
	else 
		std::cout << "Could not read input image file: " << input_filename << std::endl;

	file_chooser->hide();
}

/* bool quantize::ApplicationWindow::on_configure(GdkEventConfigure* configure_event) { */
/* 	if(! in_img.empty()) */
/* 	{ */
/* 		::cv::Mat tmp; */
/* 		::cv::cvtColor(in_img, tmp, ::cv::COLOR_BGR2RGB); */

/* 		int in_width = input_image->get_allocated_width(); */
/* 		int in_height = input_image->get_allocated_height(); */

/* 		float scale_w = float(in_width)/tmp.size().width; */
/* 		float scale_h = float(in_height)/tmp.size().height; */
/* 		float scale = std::min(scale_w, scale_h); */

/* 		::cv::resize(tmp, in_img_RGB, */ 
/* 				{int(scale * tmp.size().width), */
/* 				 int(scale * tmp.size().height)}); */
/* 		auto size = in_img_RGB.size(); */
/* 		auto in_pixbuf = Gdk::Pixbuf::create_from_data( */
/* 				in_img_RGB.data, */
/* 				Gdk::COLORSPACE_RGB, */
/* 				in_img_RGB.channels() == 4, */
/* 				8, size.width, size.height, */
/* 				(int) in_img_RGB.step */
/* 				); */

/* 		input_image->set(in_pixbuf); */
/* 		process_button->set_sensitive(true); */
/* 	} */
/* 	if(! out_img.empty()) */
/* 	{ */
/* 		::cv::Mat tmp; */
/* 		::cv::cvtColor(out_img, tmp, ::cv::COLOR_BGR2RGB); */

/* 		int out_width = output_image->get_allocated_width(); */
/* 		int out_height = output_image->get_allocated_height(); */

/* 		float scale_w = float(out_width)/tmp.size().width; */
/* 		float scale_h = float(out_height)/tmp.size().height; */
/* 		float scale = std::min(scale_w, scale_h); */

/* 		::cv::resize(tmp, out_img_RGB, */ 
/* 				{int(scale * tmp.size().width), */
/* 				 int(scale * tmp.size().height)}); */
/* 		auto size = out_img_RGB.size(); */
/* 		auto out_pixbuf = Gdk::Pixbuf::create_from_data( */
/* 				out_img_RGB.data, */
/* 				Gdk::COLORSPACE_RGB, */
/* 				out_img_RGB.channels() == 4, */
/* 				8, size.width, size.height, */
/* 				(int) out_img_RGB.step */
/* 				); */

/* 		output_image->set(out_pixbuf); */
/* 		process_button->set_sensitive(true); */
/* 	} */
/* 	return false; */
/* } */
