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
#include "quantize-vector.hpp"

using vector2d = quantize::vector<2>;

int main(int argc, char* argv[]) {

	vector2d p0 {1., 1.};
	vector2d p1 {2., 2.};

	std::cout << "The middle of " << p0 
		<< " and " << p1 << " is at "
		<< (p0 + p1)/2 << std::endl;

	std::cout << "The middle of " << p0 
		<< " and " << p1 << " is at "
		<< 0.5 * (p0 + p1) << std::endl;
	p1[1];
}
