#include "quantize-cv.hpp"
#include <algorithm>
#include <iterator>
#include <random>

quantize::cv::compressor::compressor(std::size_t num_colors): algo(num_colors, 1e-2) {} 

::cv::Mat quantize::cv::compressor::quantize(const ::cv::Mat& input) {
	algo.reset();

	// Build a collection of vectors from the pixels
	std::vector<vector_type> values;
	uchar* inptr = input.data;
	values.reserve(input.total());
	for(unsigned int i = 0 ; i < input.total(); ++i) 
		values.push_back(
				{
				static_cast<typename vector_type::value_type>(*(inptr++)),
				static_cast<typename vector_type::value_type>(*(inptr++)),
				static_cast<typename vector_type::value_type>(*(inptr++))
				}
				);

	// Let the algorithm process that collection
	std::random_device rd;

	/* auto distortion = algo(rd, values.begin(), values.end(), {.steps =100}); */
	/* std::cout << "Distortion : " << distortion << std::endl; */

	auto [num_steps, distortion] = algo(rd, values.begin(),
		   	values.end(),
		   	{.min_variation =0.1});
	std::cout << "Num steps : " << num_steps
			  << " , with distortion = " << distortion << std::endl;

	// Compute and display the end distortion
	auto disto = algo.distortion(values.begin(), values.end());

	// And then produce the quantized image
	::cv::Mat output(input.size(), CV_8UC3);
	inptr = input.data;
	uchar* outptr = output.data;
	for(unsigned int i = 0 ; i < input.total(); ++i) {
		auto bmu = algo.get_closest({
				static_cast<typename vector_type::value_type>(*(inptr++)),
				static_cast<typename vector_type::value_type>(*(inptr++)),
				static_cast<typename vector_type::value_type>(*(inptr++))});
		*(outptr++) = bmu[0];
		*(outptr++) = bmu[1];
		*(outptr++) = bmu[2];
	}
		
	return output;
}
