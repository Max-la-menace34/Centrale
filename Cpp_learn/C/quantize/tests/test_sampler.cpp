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

#define BOOST_TEST_MODULE quantize::vectors

#include <boost/test/unit_test.hpp>

#include "quantize-sampler.hpp"

BOOST_AUTO_TEST_CASE(TestSamples)
{
	constexpr std::size_t dim {1000};
	double lbound {-5};
	double ubound { 5};

	std::array<double, dim> ubounds;
	std::fill(ubounds.begin(), ubounds.end(), ubound);

	std::array<double, dim> lbounds {lbound};
	std::fill(lbounds.begin(), lbounds.end(), lbound);

	std::random_device rd;
	quantize::sampler::uniform<dim> sampler(rd, lbound, ubound);

	auto sample = sampler();

	BOOST_TEST(sample <= ubounds, boost::test_tools::per_element());
	BOOST_TEST(sample >= lbounds, boost::test_tools::per_element());
}


