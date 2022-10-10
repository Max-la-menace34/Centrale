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

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <numeric>
#include <tuple>

namespace quantize {

	/** @ingroup generic
	 * A vector in an N dimensional space
	 *
	 * @param dim The dimension of the space in which the vectors lie
	 */
	template<std::size_t dim>
		struct vector: public std::array<double, dim> {

			/**
			 *  The value hosted by the array
			 */
			using value_type = typename std::array<double, dim>::value_type;

			/**
			 *  Importing the base class constructors
			 */
			using std::array<double, dim>::array;

			/**
			 * Allow for aggregate initialization 
			 */
			template<typename ...E>
				vector(E&&...e) : std::array<double, dim>{{std::forward<E>(e)...}} {}

			/**
			 * Divides all the components by the given value 
			 */
			void operator/=(const value_type& val) {
				for(auto& v: (*this))
					v /= val;
			}

			/**
			 * The sum of two vectors
			 *
			 * @param other A vector to add 
			 */
			vector<dim> operator+(const vector<dim>& other) const {
				vector<dim> res;
				std::transform(other.cbegin(),
					   	other.cend(),
						this->cbegin(),
						res.begin(),
						std::plus<value_type>());
				return res;
			}

			/** 
			 * In place addition
			 */
			void operator+=(const vector<dim>& other) {
				std::transform(other.cbegin(),
						       other.cend(),
							   this->cbegin(),
							   this->begin(),
							   std::plus<value_type>());
			}

			/**
			 * The difference of two vectors
			 *
			 * @param other A vector to substract 
			 */
			vector<dim> operator-(const vector<dim>& other) const {
				vector<dim> res;
				std::transform(this->cbegin(),
					   	this->cend(),
						other.cbegin(),
						res.begin(),
						std::minus<value_type>());
				return res;
			}

			/**
			 * Left Multiplication by a scalar
			 *
			 * @param f The multiplier 
			 */
			vector<dim> operator*(value_type f) const {
				vector<dim> res;
				std::transform(this->cbegin(), 
						this->cend(),
						res.begin(),
						[&f](typename vector<dim>::value_type v) { return f*v ;});
				return res;
			}

			/**
			 * Division by a scalar
			 *
			 * @param f The divider 
			 */
			vector<dim> operator/(value_type f) {
				vector<dim> res;
				std::transform(this->cbegin(), 
						this->cend(),
						res.begin(),
						[&f](typename vector<dim>::value_type v) { return v/f ;});
				return res;
			}


			/**
			 * Computes and returns the norm2 of the vector
			 */
			value_type norm(void) const {
				return std::sqrt(std::inner_product(this->cbegin(),
										  this->cend(), 
										  this->cbegin(), value_type(0.0)));
			}
		};

	/**
	 * Right Multiplication by a scalar
	 *
	 * @param f The multiplier 
	 */
	template<std::size_t dim>
		vector<dim> operator*(double f, const vector<dim>& p) {
			return p * f;
		}

	/**
	 * Serialization operator
	 *
	 * @param os The stream in which to serialize 
	 * @param p The vector to serialize
	 */
	template<std::size_t dim>
		std::ostream& operator<<(std::ostream& os, const vector<dim>& p) {
			std::size_t idx=0;
			os << '(' ;
			for(const auto& v: p) {
				++idx;
				os << v << (idx < dim ? ',' : char());
			}
			os << ')';	
			return os;
		}
}
