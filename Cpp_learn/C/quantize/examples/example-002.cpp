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
#include "quantize-sampler.hpp"
#include "quantize-kmeans.hpp"

constexpr int dim = 2;
using vec2d_type = quantize::vector<dim>;
using algo_type = quantize::algorithm::kmeans<vec2d_type>;

using sampler_type = quantize::sampler::uniform<dim>; 

int main(int argc, char* argv[]) {
	std::random_device rd; 
	std::mt19937 gen(rd());

	std::size_t K{16};
	double epsilon{1e-2};
	algo_type kmeans{K, epsilon};	

	sampler_type sampler(gen, 0, 1);

	// Let's learn from samples
	for(unsigned int i = 0 ; i< 100000; ++i) {
		std::cout << "\r Step " << i << std::flush;
		kmeans(sampler());
	}
	std::cout << std::endl;

	// Display the result
	std::cout << kmeans << std::endl;

	// Save the prototypes for latter display in gnuplot
	std::ofstream outfile("points.data");
	for(unsigned int i = 0 ; i< K; ++i) {
		const auto& pi = kmeans[i];
		outfile << pi[0] << " " << pi[1] << std::endl;
	}
	outfile.close();
	std::cout << "Prototypes saved in points.data" << std::endl;
	std::cout << "In gnuplot : " << std::endl;
	std::cout << "   plot  \"points.data\" using 1:2 pt 7 ps 3 " << std::endl;
}
