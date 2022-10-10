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

#include <exception>
#include <stdexcept>
#include <sstream>

namespace quantize::exception {

	/**
	 * A general purpose exception
	 */
	class Any : public std::exception {
		private:
			std::string message;
		public:
			/**
			 * Constructor
			 */
			Any(const std::string& kind, const std::string& msg) {
				message = std::string("quantize exception: ") + 
					kind + ": " + msg;
			}

			/**
			 * Destructor
			 */
			virtual ~Any(void) throw() {}

			/**
			 * Returns the message of the exception 
			 */
			virtual const char* what() const noexcept override {
				return message.c_str();
			}
	};

	/**
	 * An exception when an index is out of a [min, max] range
	 */
	class OutOfRange : public Any {
		friend auto out_of_range(std::size_t, std::size_t, std::size_t);
		private:
		/**
		 * Constructor, private to construct through the factory only
		 */
		OutOfRange(const std::string& msg): Any("OutOfRange", msg) {}	
	};

	/**
	 * Factory of OutOfRange 
	 *
	 * @param val The queried index
	 * @param min The lower bound of the index
	 * @param max The upper bound of the index
	 * @returns an OutOfRange exception
	 */
	auto out_of_range(std::size_t val,
			std::size_t min,
			std::size_t max) {
		std::ostringstream ostr;
		ostr << "Index " << val 
			<< " is out of the range ["
			<< min << "; " << max << "]";
		return OutOfRange(ostr.str());
	}

}
