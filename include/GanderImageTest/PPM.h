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
#ifndef __GANDERIMAGETEST_PPM_H__
#define __GANDERIMAGETEST_PPM_H__

#include "boost/noncopyable.hpp"

#include "Gander/Common.h"

namespace Gander
{

namespace ImageTest
{

/// A simple class for reading and writing PPM files.
/// This class is implemented as simply as possible. It can only represent images that have
/// interleaved RGB channels where each channel is an int8u in the range of 0-255.
/// Its only purpose is for the testing of low-level image operations and shouldn't be used
/// in any production code.
struct PPM : private boost::noncopyable
{
	PPM() : width(0), height(0), buf(NULL) {}
	PPM( int32u w, int32u h ) : width(w), height(h), buf( new int8u[ 3 * w * h ] ) {}
	~PPM();

	bool operator == ( const PPM &rhs ) const;
	void read( std::string file );
	bool write( std::string file ) const;
	
	int32u width;
	int32u height;

	/// A buffer of size: width * height * 3.
	int8u *buf;
};

}; // namespace ImageTest

}; // namespace Gander

#endif
