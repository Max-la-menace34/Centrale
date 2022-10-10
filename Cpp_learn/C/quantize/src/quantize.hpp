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

#include "quantize-exceptions.hpp"
#include "quantize-vector.hpp"
#include "quantize-kmeans.hpp"
#include "quantize-sampler.hpp"

/**
 * \example example-001.cpp
 * \example example-002.cpp
 * \example cv-example-003.cpp
 */

/**
 * \mainpage
 *
 * \section Overview
 *
 * This is a project with pedagogical purpose. The only aim of this
 * project is to demonstrate how to make use of <a href="https://cmake.org">CMake</a>
 * 
 * \section More in depth
 * But, actually, why not making an interesting app, even for pedagocial purpose ?
 * So this app is providing the Kmeans algorithm in a generic templated 
 * way and applies it to quantize open cv images. 
 *
 * The algorithm described in a few words : given a vector space \f$\mathcal{X}\f$, the KMeans algorithms starts with some randomly initialized prototypes (also called codebook vectors) \f$\xi_i \in \mathcal{X}\f$ and then processes, in an online fashion some input samples \f$x \in \mathcal{X}\f$. After each \f$x\f$, the closest codebook vector, with respect to the Euclidean norm in \f$\mathcal{X}\f$ is updated :
 * \f{eqnarray*}{
 *   i^\star &=& \mbox{argmin}_i \|\xi_i - x\| \\
 *   \xi_{i^\star} &=& \xi_{i^\star} + \epsilon (x - \xi_{i^\star} ) 
 * \f}
 *
 * We also provide an instanciation of the generic algorithm on open cv images.
 * 
 * Finally, you can experiment with that algorithm using the dedicated GUI illustrated below : 
 *
 * \image html gui.png "My GUI for demonstrating Kmeans" width=25%
 * 
 */

/**
 *  @defgroup generic The generic KMeans implementation
 *  This group holds all the methods/classes used to implement the 
 *  generic Kmeans algorithm.
 */

/**
 *  @defgroup cvgui The CV KMeans instantiation with the GUI
 *  This group holds the instantiation of the KMenas templates on 
 *  opencv images and the dedicated compressor algorithm.
 *  It holds the GUI as well.
 */
