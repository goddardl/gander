//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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

#include "Gander/DecomposeRQ3x3.h"
#include "Gander/AreClose.h"

#include "boost/assert.hpp"

#include <iostream>

namespace Gander
{

void givensDecomposeRQ3x3( const Eigen::Matrix3d &A, Eigen::Matrix3d &R, Eigen::Matrix3d &Q, Eigen::Matrix3d &Qx, Eigen::Matrix3d &Qy, Eigen::Matrix3d &Qz )
{
	double z, c, s;
	Eigen::Matrix3d M;
	M = A;

	// Find Givens rotation Qx for x axis (left multiplication).
	//
	//      ( 1  0  0 )
	// Qx = ( 0  c  s ), c = m33/sqrt(m32^2 + m33^2), s = m32/sqrt(m32^2 + m33^2)
	//      ( 0 -s  c )
	//
	s = M(2,1);
	c = M(2,2);
	z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
	c *= z;
	s *= z;
	Qx << 1, 0, 0, 0, c, s, 0, -s, c;
	
	R = M * Qx;        
	BOOST_ASSERT( fabs( R(2,1) ) < std::numeric_limits<double>::epsilon() );
	R(2,1) = 0.;

	// Find Givens rotation for y axis.
	//
	//      ( c  0 -s )
	// Qy = ( 0  1  0 ), c = m33/sqrt(m31^2 + m33^2), s = -m31/sqrt(m31^2 + m33^2)
	//      ( s  0  c )
	//
	s = -R(2,0);
	c = R(2,2);
	z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
	c *= z;
	s *= z;

	Qy << c, 0, -s, 0, 1, 0, s, 0, c;
	M = R * Qy;

	BOOST_ASSERT( fabs( M(2,0) ) < std::numeric_limits<float>::epsilon() );
	M(2,0) = 0.;

	// Find Givens rotation for z axis.
	//
	//      ( c  s  0 )
	// Qz = (-s  c  0 ), c = m22/sqrt(m21^2 + m22^2), s = m21/sqrt(m21^2 + m22^2)
	//      ( 0  0  1 )
	//
	s = M(1,0);
	c = M(1,1);
	z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
	c *= z;
	s *= z;
	
	Qz << c, s, 0, -s, c, 0, 0, 0, 1;
	R = M * Qz;

	BOOST_ASSERT( fabs( R(1,0) ) < std::numeric_limits<float>::epsilon() );
	R(1,0) = 0.;

	// Solve the decomposition ambiguity.
	// Diagonal entries of R, except the last one, shall be positive.
	// Further rotate R by 180 degree if necessary.
	if( R(0,0) < 0 )
	{
		if( R(1,1) < 0 )
		{
			// Rotate around z by 180 degrees.
			// I.E. A rotation matrix of:
			// [-1, 0, 0 ],
			// [ 0,-1, 0 ],
			// [ 0, 0, 1 ]
			R(0,0) *= -1;
			R(0,1) *= -1;
			R(1,1) *= -1;

			Qz(0,0) *= -1;
			Qz(0,1) *= -1;
			Qz(1,0) *= -1;
			Qz(1,1) *= -1;
		}
		else
		{
			// Rotate around Y by 180 degrees.
			// I.E. A rotation matrix of:
			// [-1, 0, 0 ],
			// [ 0, 1, 0 ],
			// [ 0, 0,-1 ]
			R(0,0) *= -1;
			R(0,2) *= -1;
			R(1,2) *= -1;
			R(2,2) *= -1;

			Qz.transposeInPlace();

			Qy(0,0) *= -1;
			Qy(0,2) *= -1;
			Qy(2,0) *= -1;
			Qy(2,2) *= -1;
		}
	}
	else if( R(1,1) < 0 )
	{
		// Rotate around x by 180 degrees.
		// I.E. A rotation matrix of
		// [ 1, 0, 0 ],
		// [ 0,-1, 0 ],
		// [ 0, 0,-1 ]
		R(0,1) *= -1;
		R(0,2) *= -1;
		R(1,1) *= -1;
		R(1,2) *= -1;
		R(2,2) *= -1;

		Qz.transposeInPlace();
		Qy.transposeInPlace();
		Qx(1,1) *= -1;
		Qx(1,2) *= -1;
		Qx(2,1) *= -1;
		Qx(2,2) *= -1;
	}

	// Calulate the orthogonal matrix.
	Q = ( Qx * Qy * Qz ).transpose();
}

}; // namespace Gander

