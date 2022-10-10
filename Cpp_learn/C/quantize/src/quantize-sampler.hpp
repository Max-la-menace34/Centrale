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

#include <random>
#include <functional>

#include "quantize-vector.hpp"

namespace quantize::sampler {

	/**
	 * A uniform sampler in dim space
	 */
	template<std::size_t dim>
		struct uniform {
			/**
			 * The type of a single coordinate
			 */
			using value_type = typename quantize::vector<dim>::value_type;

			/**
			 * The function generating a random value
			 */
			std::function<value_type(void)> fgen;

			/**
			 * The constructor
			 *
			 * @param gen A random device 
			 * @param min the lower bound for every dimension
			 * @param max the upper bound for every dimension
			 */
			template<typename RANDOM_DEVICE>
				uniform(RANDOM_DEVICE& gen,
						value_type min,
						value_type max) : fgen([&gen, dis=std::uniform_real_distribution<value_type>(min, max)](void) mutable -> value_type { return dis(gen); }){
				}

			/**
			 * The generator of samples
			 */
			quantize::vector<dim> operator()() {
				quantize::vector<dim> res;
				std::for_each(res.begin(),
						res.end(),
						[this](value_type& v) {
						v = this->fgen();
						});
				return res;
			}
		};
}
