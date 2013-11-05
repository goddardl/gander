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
#include "GanderImage/Channel.h"

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

/// A simple struct for holding the raw data of a channel.
struct ChannelDescriptor
{
	/// Pointer to the channels data cast to an int8u*.
	int8u *data;
	/// The size of the step between two consecutive pixels in bytes.
	size_t step;
	/// The stride required to step from the end of a row to the
	/// beginning of the next in bytes.
	size_t stride;
};

/// A simple struct for describing an image buffer of a single channel.
struct Buffer : public ChannelDescriptor
{
	/// The width and height of the image. This should correspond with the
	/// step and stride of the ChannelDescriptor.
	int32u width, height;
};

class Image
{
	public :

	/// Constructs a new image of a specified width and height.
	inline Image( int32u width, int32 height ) : m_width( width ), m_height( height ) {}

	/// Adds a new channel to the image by inserting the data pointer in the same location
	/// as the Channel is found in the ChannelSet.
	void addChannel( const Buffer &buffer, Channel channel )
	{
		BOOST_ASSERT( !m_channels.contains( channel ) );
		BOOST_ASSERT( buffer.width == m_width );
		BOOST_ASSERT( buffer.height == m_height );

		m_channels += channel;
		int index = m_channels.index( channel );
		m_channelPtrs.insert( m_channelPtrs.begin()+index, buffer );
	}

	//! @name Accessors
	/// Various accessors for setting and querying the contents of the Image.
	//////////////////////////////////////////////////////////////
	//@{
	/// Returns a pointer to the channel at a specified index within the list of data pointers.
	inline int8u* channelPtr( int index ) const { BOOST_ASSERT( index < m_channelPtrs.size() ); return m_channelPtrs[index].data; };
	/// Returns a pointer to the specified channel within the list of data pointers.
	inline int8u* channelPtr( Channel channel ) const { return m_channelPtrs[ m_channels.index( channel ) ].data; };
	//@}
	
	private :
	
	int32u m_width;
	int32u m_height;
	ChannelSet m_channels;
	std::vector<ChannelDescriptor> m_channelPtrs;
};

template< class T >
class iterator : public ChannelDescriptor
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
};

template< class T >
class Tile
{
	public :

		typedef T ImageType;
		typedef typename T::PixelType PixelType;

		Tile( ImageType &image, int32 x, int32 y, int32 r, int32 t ) :
			m_image( image )
		{
		}

		template< class S = PixelType >
		const PixelIterator<S>& operator [] ( ChannelSet channel ) const
		{
			// Return a PixelIterator of the particular Channel type.
			// Set the step of the PixelIterator to match the size in bytes of the channel + the step to the next pixel in the row.
		}
		
		template< class S = PixelType >
		const PixelIterator<S>& begin() const
		{
			// Return an iterator to the beginning of the row.
		}

		template< class S = PixelType >
		const PixelIterator<S>& end() const
		{
			// Return an iterator to the end of the row (that is not a valid address)..
		}

	private :
		
		ImageType &m_image;

};

}; // namespace Image

}; // namespace Gander

#endif

