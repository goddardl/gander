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
#ifndef __GANDERIMAGE_PIXEL_H__
#define __GANDERIMAGE_PIXEL_H__

#include <vector>

#include "Gander/Common.h"

namespace Gander
{

namespace Image
{

/*
Make this tile class provide a way of clipping an image to a box. It should look up the stride etc. of a particular channel from the image object T.
This means that you should be able to nest these classes so that you can apply multiple tiles to crop an image several times. A Row tile would be a special case of this
and would basically be a tile with a height of 1. By using steps and strides, we should be able to interleave RGB channels with single float channels if we want. A mapping of the channel's
data layout needs to be kept along with a pointer to the first element. Internally we hold an array of bytes rather than an array of the channel types as it means that if we really wanted, we could
mix the types of the channels.
*/
/*
template< class T >
class Tile
{
	public :
		typedef T ImageType;
		typedef typename T::PixelType PixelType;

		Tile( ImageType &image,  ) :
			m_image( image )
		{
		}

		template< class S = PixelType >
		const PixelIterator<S>& operator [] ( int Channel ) const
		{
			Return a PixelIterator of the particular Channel type. Set the step of the PixelIterator to match the size in bytes of the channel + the step to the next pixel in the row.
		}
		
		template< class S = PixelType >
		const PixelIterator<S>& begin() const
		{
			Return an iterator to the beginning of the row.
		}

		template< class S = PixelType >
		const PixelIterator<S>& end() const
		{
			Return an iterator to the end of the row (that is not a valid address)..
		}

	private :
		
		ImageType &m_image;

};

template< class T >
class PixelIterator
{
	public :
		
		inline void operator ++ () { m_ptr += m_step; }
		inline void operator -- () { m_ptr -= m_step; }

		/// Overloaded * operator for pixel access.
		inline T &operator * () const
		{
			return *static_cast<T*>( m_ptr );
		}

	private :
		
		friend class Tile;

		PixelIterator( int8u *buffer, int step, int stride ) :
			m_ptr( buffer ), m_step( step )
		{
		}

		int8u *m_ptr;
		int m_step;
};

};
*/
}; // namespace Image

}; // namespace Gander

#endif

