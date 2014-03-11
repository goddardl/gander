//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2014, Luke Goddard. All rights reserved.
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
#ifndef __GANDERIMAGE_ROW__
#define __GANDERIMAGE_ROW__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Interfaces.h"

#include "GanderImage/StaticAssert.h"
#include "GanderImage/Pixel.h"

namespace Gander
{

namespace Image
{

template< class Layout >
class Row
{
	public :
		
		typedef Layout LayoutType;
		typedef Row< LayoutType > Type;
		typedef typename Gander::Image::template Pixel< LayoutType > Pixel;
		typedef typename Gander::Image::template PixelIterator< LayoutType > PixelIterator;

		typedef PixelIterator iterator;
		typedef const Gander::Image::PixelIterator< LayoutType > const_iterator;

		inline Row( unsigned int width ) :
			m_width( width )
		{
		}
		
		inline void setStart( const PixelIterator &it )
		{
			m_start = it;
		}

		inline const PixelIterator &getStart() const
		{
			return m_start;
		}

		inline unsigned int width() const
		{
			return m_width;
		}

		inline ChannelSet channels() const
		{
			return m_start.channels();
		}

		inline unsigned int numberOfChannels() const
		{
			return m_start.numberOfChannels();
		}

		const_iterator &begin() const
		{
			return m_start;
		}

		const_iterator &end() const
		{
			return iterator( m_start ).increment( m_width );
		}

		template< class T >
		inline bool operator == ( const T &rhs ) const
		{
			return m_start == rhs.m_start && m_width == rhs.m_width;
		}

		template< class T >
		inline bool operator != ( const T &rhs ) const
		{
			return !( m_start == rhs.m_start && m_width == rhs.m_width );
		}

	protected :
		
		inline Row( const PixelIterator &start, unsigned int width ) :
			m_start( start ),
			m_width( width )
		{}

		PixelIterator m_start;
		unsigned int m_width;

};

}; // namespace Image

}; // namespace Gander

#endif
