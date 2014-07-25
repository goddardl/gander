//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2011-2013, Image Engine Design Inc. All rights reserved.
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

#ifndef __GANDERMURMURHASH_INL__
#define __GANDERMURMURHASH_INL__

namespace Gander
{

inline MurmurHash::MurmurHash()
	:	m_h1( 0 ), m_h2( 0 )
{
}

inline MurmurHash::MurmurHash( const MurmurHash &other )
	:	m_h1( other.m_h1 ), m_h2( other.m_h2 )
{
}

inline MurmurHash &MurmurHash::append( char data )
{
	append( &data, sizeof( char ), sizeof( char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( unsigned char data )
{
	append( &data, sizeof( unsigned char ), sizeof( unsigned char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( short data )
{
	append( &data, sizeof( short ), sizeof( short ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( unsigned short data )
{
	append( &data, sizeof( unsigned short ), sizeof( unsigned short ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( int data )
{
	append( &data, sizeof( int ), sizeof( int ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( unsigned int data )
{
	append( &data, sizeof( unsigned int ), sizeof( unsigned int ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( int64_t data )
{
	append( &data, sizeof( int64_t ), sizeof( int64_t ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( uint64_t data )
{
	append( &data, sizeof( uint64_t ), sizeof( uint64_t ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( float data )
{
	append( &data, sizeof( float ), sizeof( float ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( double data )
{
	append( &data, sizeof( double ), sizeof( double ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const char *data )
{
	append( data, strlen( data ), sizeof( char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const std::string &data )
{
	append( data.c_str(), data.size(), sizeof( char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const MurmurHash &data )
{
	append( &data.m_h1, 2 );
	return *this;
}

inline MurmurHash &MurmurHash::append( const char *data, size_t numElements )
{
	append( data, numElements * sizeof( char ), sizeof( char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const unsigned char *data, size_t numElements )
{
	append( data, numElements * sizeof( unsigned char ), sizeof( unsigned char ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const short *data, size_t numElements )
{
	append( data, numElements * sizeof( short ), sizeof( short ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const unsigned short *data, size_t numElements )
{
	append( data, numElements * sizeof( unsigned short ), sizeof( unsigned short ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const int *data, size_t numElements )
{
	append( data, numElements * sizeof( int ), sizeof( int ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const unsigned int *data, size_t numElements )
{
	append( data, numElements * sizeof( unsigned int ), sizeof( unsigned int ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const int64_t *data, size_t numElements )
{
	append( data, numElements * sizeof( int64_t ), sizeof( int64_t ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const uint64_t *data, size_t numElements )
{
	append( data, numElements * sizeof( uint64_t ), sizeof( uint64_t ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const float *data, size_t numElements )
{
	append( data, numElements * sizeof( float ), sizeof( float ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const double *data, size_t numElements )
{
	append( data, numElements * sizeof( double ), sizeof( double ) );
	return *this;
}

inline MurmurHash &MurmurHash::append( const std::string *data, size_t numElements )
{
	for( size_t i=0; i<numElements; i++ )
	{
		append( *data++ );
	}
	return *this;
}

inline const MurmurHash &MurmurHash::operator = ( const MurmurHash &other )
{
	m_h1 = other.m_h1;
	m_h2 = other.m_h2;
	return *this;
}

inline bool MurmurHash::operator == ( const MurmurHash &other ) const
{
	return m_h1 == other.m_h1 && m_h2 == other.m_h2;
}

inline bool MurmurHash::operator != ( const MurmurHash &other ) const
{
	return m_h1 != other.m_h1 || m_h2 != other.m_h2;
}

inline bool MurmurHash::operator < ( const MurmurHash &other ) const
{
	return m_h1 < other.m_h1 || ( m_h1 == other.m_h1 && m_h2 < other.m_h2 );
}

inline MurmurHash::operator std::string () const
{
	return toString();
}

/// Implementation of tbb_hasher for MurmurHash, allowing MurmurHash to be used
/// as a key in tbb::concurrent_hash_map.
inline size_t tbb_hasher( const MurmurHash &h )
{
	return h.m_h1 ^ h.m_h2;
}

}; // namespace Gander

#endif // __GANDERMURMURHASH_INL__
