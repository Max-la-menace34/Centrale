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

#include <opencv/cv.hpp>
//#include <opencv2/opencv.hpp>

#include "quantize-kmeans.hpp"
#include "quantize-vector.hpp"

namespace quantize::cv {

	/** @ingroup cvgui
	 * A compression algorithm for opencv images
	 * uses Kmeans for clustering the colorspace 
	 *
	 */
	class compressor {
		private:
			using vector_type = quantize::vector<3>;
			using algo_type = quantize::algorithm::kmeans<vector_type>;

			/**
			 * The algorithm for processing the image
			 */
			algo_type algo;
		public:

			/**
			 * The constructor
			 */
			compressor(std::size_t num_colors);

			/**
			 * The algorithm
			 */
			::cv::Mat quantize(const ::cv::Mat& input);
	};
}
