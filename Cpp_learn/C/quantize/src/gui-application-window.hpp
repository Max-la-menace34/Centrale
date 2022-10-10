#pragma once

/*
 *   Copyright (C) 2020,  CentraleSupelec
 *
 *   Author : Jérémy Fix 
 *
 *   Contributor :
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public
 *   License (GPL) as published by the Free Software Foundation; either
 *   version 3 of the License, or any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *   General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Contact : jeremy.fix@centralesupelec.fr
 *
 */

#include <gtkmm.h>

#include <opencv/cv.hpp>
// #include <opencv2/opencv.hpp>

#include <thread>

namespace quantize {

	/** @ingroup cvgui
	 * The main application window 
	 *
	 */
	class ApplicationWindow : public Gtk::ApplicationWindow {

		private:

			// Main window widgets
			Gtk::ToolButton* open_button;
			Gtk::ToolButton* process_button; 
			Gtk::ToolButton* quit_button; 
			Gtk::ToolButton* about_button; 
			Gtk::Image* input_image;
			Gtk::Image* output_image;

			Gtk::SpinButton* spin_colors;

			Gtk::AboutDialog* about_window;

			// File chooser
			Gtk::FileChooserDialog* file_chooser;
			Gtk::Button* file_open_button;
			Gtk::Button* file_cancel_button;

			std::string input_filename;

			// The object handling communication with the worker thread
			Glib::Dispatcher m_Dispatcher;
			std::thread* worker;
			std::mutex mutex_img;

			::cv::Mat in_img, in_img_RGB;
			::cv::Mat out_img, out_img_RGB;

			/**
			 * Getting the widgets and connectings the signals
			 */
			void connect_signals();

		public:   
			/**
			 * Constructor. Gets called when invoking get_widget_derived
			 */
			ApplicationWindow(BaseObjectType* cobject,
					const Glib::RefPtr<Gtk::Builder>& refGlade);

			virtual ~ApplicationWindow();

			/**
			 * Slot called when open button is clicked. For opening an image
			 */
			void on_open_clicked(void);
			/**
			 * Slot called when process button is clicked. Triggers a worker
			 * thread
			 */
			void on_process_clicked(void);
			/**
			 * Slot called when the open button of the file_chooser is clicked 
			 */	
			void on_file_open_clicked(void);

			/* bool on_configure(GdkEventConfigure*); */

			/**
			 * Slot called by the dispatcher, itself called when the 
			 * worker has finished its job
			 */
			void on_process_done(void);

		protected:
			/**
			 * The object for accessing the gtk glade widgets
			 */
			Glib::RefPtr<Gtk::Builder> m_refGlade;

	};
}
