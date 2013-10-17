//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013, Luke Goddard. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Luke Goddard nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////
#ifndef __GANDER_MATH_H__
#define __GANDER_MATH_H__

///\todo: Have the SConstruct add this #define if gander is installed with OpenEXR support.
// #define USE_IMATH_EXTENSIONS

/// Include the Imath headers that we need for the Eigen::MatrixBase extensions.
#ifdef USE_IMATH_EXTENSIONS
#include "ImathMatrix.h"
#include "ImathVec.h"
#endif

/// This variable needs to be defined before any Eigen header is included.
/// It extends the Eigen::MatrixBase class to add support for any other math
/// libraries that we may be using too. For example, it will add casting
/// operators to and from the common Imath types if USE_IMATH_EXTENSIONS is
/// defines.
#define EIGEN_MATRIXBASE_PLUGIN "EigenMatrixBaseExtension.h"
#define EIGEN_MATRIX_PLUGIN "EigenMatrixExtension.h"

/// Now that the base classes have been extended, add the eigen includes.
#include "Eigen/Eigen"
#include "Eigen/Dense"
#include "Eigen/Core"

#endif
