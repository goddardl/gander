//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
//  Copyright (c) 2004-2012, Industrial Light & Magic, a division of Lucas
//  Digital Ltd. LLC
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
#ifndef __GANDER_BOX__
#define __GANDER_BOX__

#include <limits>

#include "Gander/Common.h"

#include "Gander/Math.h"
#include "Eigen/Dense"

namespace Gander
{

template <class T>	
class Box
{
	public :

		///	Constructors - an "empty" box is created by default.
		Box(); 
		Box( const T &point );
		Box( const T &minT, const T &maxT );

		///  The equality operators.
		bool equalTo( const Box<T> &src ) const;
		bool operator == ( const Box<T> &src ) const;
		bool operator != ( const Box<T> &src ) const;

		///	Box manipulation.
		void makeEmpty();
		void extendBy( const T &point );
		void extendBy( const Box<T> &box );
		void makeInfinite();    

		///	Query functions - these compute results each time.
		T size() const;
		T center() const;
		bool intersects( const T &point ) const;
		bool intersects( const Box<T> &box ) const;

		/// Stream operator.	
		friend std::ostream &operator << ( std::ostream &out, const Gander::Box<T> &box )
		{
			out << box.min(0) << ", " << box.min(1) << ", " << box.max(0) << ", " << box.max(1);
			return out;
		}

		///	Classification.
		bool isEmpty() const;
		bool hasVolume() const;
		bool isInfinite() const;

		/// The data members.
		T min;
		T max;

};

typedef Box< Eigen::Vector2d > Box2d;
typedef Box< Eigen::Vector2f > Box2f;
typedef Box< Eigen::Vector2i > Box2i;

}// namespace Gander

#include "Gander/Box.inl"

#endif

