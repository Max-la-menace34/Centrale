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

#include <iostream>
#include <fstream>
#include "quantize-cv.hpp"

int main(int argc, char* argv[]) {
	
	{
		// Our compressing algorithm
		constexpr std::size_t num_colors = 10;
		quantize::cv::compressor compressor(num_colors);	
		
		// Our test image
		cv::Mat img(cv::Size(150, 100), CV_8UC3);

		img = cv::Scalar(255, 255, 255);
		cv::Mat(img, cv::Rect(10, 10, 40, 40)) = cv::Scalar(0, 255, 0);
		cv::Mat(img, cv::Rect(100, 10, 40, 40)) = cv::Scalar(0, 255, 0);
		cv::Mat(img, cv::Rect(70, 30, 10, 30)) = cv::Scalar(255, 255, 0);
		cv::Mat(img, cv::Rect(20, 80, 110, 15)) = cv::Scalar(0, 0, 255);

		// Process the image
		auto outimg = compressor.quantize(img);

		// Write the results
		cv::imwrite("001_in_image.png", img);	
		cv::imwrite("001_out_image.png", outimg);	
		std::cout << "001 done" << std::endl;
	}

	{
		// Our compressing algorithm
		constexpr std::size_t num_colors = 4;
		quantize::cv::compressor compressor(num_colors);	
		
		// Our test image
		cv::Mat img(cv::Size(255, 200), CV_8UC3);
		img = cv::Scalar{255, 255, 255};

		for(unsigned int j = 0 ; j < img.size().width; ++j)
			cv::Mat(img, cv::Rect(j, 0, 1, img.size().height)) = cv::Scalar(j, 255, 255);

		// Process the image
		auto outimg = compressor.quantize(img);

		// Write the results
		cv::imwrite("002_in_image.png", img);	
		cv::imwrite("002_out_image.png", outimg);	
		std::cout << "002 done" << std::endl;
	}


	{
		// Our compressing algorithm
		constexpr std::size_t num_colors = 10;
		quantize::cv::compressor compressor(num_colors);	
		
		// Our test image
		cv::Mat img(cv::Size(150, 100), CV_8UC3);
		img = cv::Scalar{255, 255, 255};
	
		// Add a red gradient
		img = cv::imread(QUANTIZE_SHARE "/clint.jpeg"); 
		if(img.empty())
		{
			std::cout << "Could not read input image file: " << QUANTIZE_SHARE "/clint.jpeg" << std::endl;
			std::exit(EXIT_FAILURE);

		}
		cv::imwrite("003_in_image.png", img);	

		// Process the image
		auto outimg = compressor.quantize(img);

		// Write the results
		cv::imwrite("003_out_image.png", outimg);	

		std::cout << "003 done" << std::endl;
	}
}
