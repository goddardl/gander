//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2011-2013, Image Engine Design Inc. All rights reserved.
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
//     * Neither the name of Image Engine Design nor the names of any
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

#ifndef __GANDER_MURMURHASH_H__
#define __GANDER_MURMURHASH_H__

#include <stdint.h>
#include <iostream>

#include "tbb/concurrent_hash_map.h"

namespace Gander
{

/// A nice class for hashing arbitrary chunks of data, based on
/// code available at http://code.google.com/p/smhasher.
///
/// From that page :
///
/// "All MurmurHash versions are public domain software, and the
/// author disclaims all copyright to their code."
///
/// \todo Deal with endian-ness.
class MurmurHash
{

	public :

		MurmurHash();
		MurmurHash( const MurmurHash &other );
		
		inline MurmurHash &append( char data );
		inline MurmurHash &append( unsigned char data );
		inline MurmurHash &append( short data );
		inline MurmurHash &append( unsigned short data );
		inline MurmurHash &append( int data );
		inline MurmurHash &append( unsigned int data );
		inline MurmurHash &append( int64_t data );
		inline MurmurHash &append( uint64_t data );
		inline MurmurHash &append( float data );
		inline MurmurHash &append( double data );
		inline MurmurHash &append( const char *data );
		inline MurmurHash &append( const std::string &data );
		inline MurmurHash &append( const MurmurHash &data );
		
		inline MurmurHash &append( const char *data, size_t numElements );
		inline MurmurHash &append( const unsigned char *data, size_t numElements );
		inline MurmurHash &append( const short *data, size_t numElements );
		inline MurmurHash &append( const unsigned short *data, size_t numElements );
		inline MurmurHash &append( const int *data, size_t numElements );
		inline MurmurHash &append( const unsigned int *data, size_t numElements );
		inline MurmurHash &append( const int64_t *data, size_t numElements );
		inline MurmurHash &append( const uint64_t *data, size_t numElements );
		inline MurmurHash &append( const float *data, size_t numElements );
		inline MurmurHash &append( const double *data, size_t numElements );
		inline MurmurHash &append( const std::string *data, size_t numElements );
		
		inline const MurmurHash &operator = ( const MurmurHash &other );
		
		inline bool operator == ( const MurmurHash &other ) const;
		inline bool operator != ( const MurmurHash &other ) const;

		inline bool operator < ( const MurmurHash &other ) const;
		
		std::string toString() const;
		inline operator std::string () const;

		std::ostream &operator << ( std::ostream &o );

	private :
	
		void append( const void *data, size_t bytes, int elementSize );
	
		uint64_t m_h1;
		uint64_t m_h2;
		
		friend size_t tbb_hasher( const MurmurHash &h );

};

} // namespace Gander

#include "Gander/MurmurHash.inl"

#endif // GANDER_MURMURHASH_H
