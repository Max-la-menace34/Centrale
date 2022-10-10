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

#include "quantize-vector.hpp"

BOOST_AUTO_TEST_CASE(TestVectorConstruct)
{
	quantize::vector<2> p{1., 2.};
	BOOST_CHECK_EQUAL(1., p[0]);
	BOOST_CHECK_EQUAL(2., p[1]);
}

BOOST_AUTO_TEST_CASE(TestVectorAt)
{
	quantize::vector<2> p{1., 2.};
	BOOST_CHECK_THROW(p.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestVectorMult)
{
	double f = 2.5;
	quantize::vector<2> p{1., 2.};
	auto p1 = p * f;
	BOOST_CHECK_EQUAL(f * p[0], p1[0]);
	BOOST_CHECK_EQUAL(f * p[1], p1[1]);
}

