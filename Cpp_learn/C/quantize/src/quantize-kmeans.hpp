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
#include <iostream>
#include <vector>
#include <variant>
#include <random>

#include "quantize-exceptions.hpp"
#include "quantize-vector.hpp"

namespace quantize::algorithm {

  template<typename TVector> class kmeans;
  template<typename TVector>
  std::ostream& operator<<(std::ostream& os,
			   const kmeans<TVector>& algo); 

  /** \ingroup generic
   * The stop condition of a minimal value for the distortion
   */
  struct MinVariationDistortions {
    /**
     * The minimal variation before stopping
     */
    double min_variation;
  };

  /** \ingroup generic
   * The stop condition on a maximum number of steps
   */
  struct MaxSteps {
    /**
     * The maximum number of steps before stopping
     */
    int steps;
  };

  using StopCondition = std::variant<MinVariationDistortions, MaxSteps>;

  /** \ingroup generic
   * A generic Kmeans
   *
   * \param TVector a vector type supporting various operations
   *                \sa quantize::vector
   */
  template<typename TVector>
  class kmeans {

    template<typename T>
    friend std::ostream& operator<<(std::ostream& os,
				    const kmeans<T>& algo); 

  private:
    /** 
     * Learning rate
     */
    double epsilon;
    /** 
     * The collection of codebook vectors
     */
    std::vector<TVector> prototypes;
    /** 
     * An index used for initialization
     */
    std::size_t init_idx;

  public:

    /**
     * Default construction is forbidden
     */
    kmeans() = delete;

    /**
     * \brief Constructor
     * \param k The number of codebook vectors to use
     * \param epsilon The learning rate
     */
    kmeans(std::size_t k, double epsilon): 
      epsilon(epsilon), prototypes(k), init_idx(0) {
    }

    /**
     * \brief Process one sample
     * Process the provided sample by updating the 
     * appropriate codebook vector
     *
     * \param xi The input sample
     */
    void operator()(const TVector& xi) {
      // The first prototypes.size() inputs
      // are used to initialize the prototypes
      if(init_idx < size()) {
	prototypes[init_idx++] = xi;
	return;
      }

      std::vector<typename TVector::value_type> dists(prototypes.size());
      std::transform(prototypes.begin(),
		     prototypes.end(),
		     dists.begin(),
		     [&xi](TVector& p) {
		       return (p-xi).norm();
		     });
      auto it_dbmu = std::min_element(dists.begin(),
				      dists.end());
      auto diff = std::distance(dists.begin(), it_dbmu);
      auto itbmu = prototypes.begin();
      std::advance(itbmu, diff);
      (*itbmu) += epsilon * (xi - (*itbmu));
    }

    /**
     * \brief Process a collection of samples
     * \param rd A random device used to random initialize
     * \param begin An iterator to the beginning of the sample collection to process
     * \param end An iterator to the end of the sample collection to process
     * \param condition The stopping criteria
     */
    template<typename ITERATOR, typename RANDOM_DEVICE>
    void operator()(
		    RANDOM_DEVICE& rd,
		    ITERATOR begin,
		    ITERATOR end,
		    const MaxSteps& condition) {

      reset();

      // Initialization
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(-1.0, 1.0);

      TVector mean{0.0, 0.0, 0.0};
      {
	auto it = begin;
	while(it != end)
	  mean += *(it++);
	mean /= double(std::distance(begin, end));
      }

      for(unsigned int i = 0; i < prototypes.size(); ++i)
	(*this)(mean+TVector{dis(gen), dis(gen), dis(gen)});

      // Processing
      for(unsigned int i = 0 ; i < condition.steps; ++i){
	std::shuffle(begin, end, gen);
	auto it = begin;
	while(it != end)
	  (*this)(*it++);
      }
    }

    /**
     * \brief Process a collection of samples
     * \param rd A random device used to random initialize
     * \param begin An iterator to the beginning of the sample collection to process
     * \param end An iterator to the end of the sample collection to process
     * \returns A pair made of the number of processing steps and distortion obtained
     * \param condition The stopping criteria
     */
    template<typename ITERATOR, typename RANDOM_DEVICE>
    std::pair<unsigned int, double> operator()(
					       RANDOM_DEVICE& rd,
					       ITERATOR begin,
					       ITERATOR end,
					       const MinVariationDistortions& condition) {
      // Initialization
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(-1.0, 1.0);
      
      std::shuffle(begin, end, gen);

      reset();


      TVector mean{0.0, 0.0, 0.0};
      {
	auto it = begin;
	while(it != end)
	  mean += *(it++);
	mean /= double(std::distance(begin, end));
      }

      for(unsigned int i = 0; i < prototypes.size(); ++i)
	(*this)(mean+TVector{dis(gen), dis(gen), dis(gen)});

      // Processing
      double disto = distortion(begin, end);
      unsigned int i = 1;
      while(true) {	
	std::shuffle(begin, end, gen);
	auto it = begin;
	while(it != end)
	  (*this)(*it++);
	double ndisto = distortion(begin, end);
	if(std::fabs(ndisto - disto) < condition.min_variation) 
	  break;
	disto = ndisto;
	++i;
      }	
      return std::make_pair(i, disto);
    }

    /**
     * \brief Returns the number of codebook vectors
     */
    std::size_t size() const {
      return prototypes.size();
    }

    /**
     * \brief Reset the prototypes
     * The next K received prototypes will init them
     */
    void reset() {
      init_idx = 0;
    }

    /**
     * \brief Returns the i-th codebook vector
     * \param idx The index of the codebook to get
     *  \exception quantize::exception::out_of_range idx is out of range.
     * \return The codebook vector ref
     */
    const TVector& operator[](std::size_t idx) const {
      if(idx >= size())
	throw quantize::exception::out_of_range(idx, 0, size()-1);	
      return prototypes[idx];
    }

    /**
     * \brief Finds the closest codebook vector
     * \param xi The input to which to compare
     * \return The codebook vector closest to xi
     */
    const TVector& get_closest(const TVector& xi) {
      std::vector<typename TVector::value_type> dists(prototypes.size());
      std::transform(prototypes.begin(),
		     prototypes.end(),
		     dists.begin(),
		     [&xi](TVector& p) {
		       return (p-xi).norm();
		     });
      auto it_dbmu = std::min_element(dists.begin(),
				      dists.end());
      auto diff = std::distance(dists.begin(), it_dbmu);
      auto itbmu = prototypes.begin();
      std::advance(itbmu, diff);
      return *itbmu;
    }

    /**
     * \brief Distortion
     * \param begin An iterator to the beginning of the sample collection to process
     * \param end An iterator to the end of the sample collection to process
     * \return The distortion for the provided samples
     */
    template<typename ITERATOR, typename = typename std::enable_if<std::is_same<typename ITERATOR::value_type, TVector>::value, ITERATOR > >
    double distortion(ITERATOR begin, ITERATOR end) const {
      double disto = 0.0;
      auto it = begin;
      std::vector<typename TVector::value_type> dists(prototypes.size());
      while(it != end) {
	std::transform(prototypes.cbegin(),
		       prototypes.cend(),
		       dists.begin(),
		       [it](const TVector& p) {
			 return (p-(*it)).norm();
		       });
	auto it_dbmu = std::min_element(dists.begin(),
					dists.end());
	auto diff = std::distance(dists.begin(), it_dbmu);
	auto itbmu = prototypes.cbegin();
	std::advance(itbmu, diff);
	disto += ((*itbmu) - (*it)).norm();	
	++it;
      }
      return disto / double(std::distance(begin, end));	
    }
  };

  /** \ingroup generic
   * \brief Serialize the algorithms for display
   */
  template<typename TVector>
  std::ostream& operator<<(std::ostream& os,
			   const kmeans<TVector>& algo) {
    os << "Kmeans "; 
    std::for_each(algo.prototypes.cbegin(),
		  algo.prototypes.cend(),
		  [&os](auto& p) {
		    os << p << ","; 
		  });
    return os;
  }
}
