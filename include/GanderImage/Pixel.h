#ifndef _GANDER_PIXEL_H_
#define _GANDER_PIXEL_H_

#include <vector>

#include "Gander/Common.h"

namespace Gander
{

namespace Image
{

Make this tile class provide a way of clipping an image to a box. It should look up the stride etc. of a particular channel from the image object T.
This means that you should be able to nest these classes so that you can apply multiple tiles to crop an image several times. A Row tile would be a special case of this
and would basically be a tile with a height of 1. By using steps and strides, we should be able to interleave RGB channels with single float channels if we want. A mapping of the channel's
data layout needs to be kept along with a pointer to the first element. Internally we hold an array of bytes rather than an array of the channel types as it means that if we really wanted, we could
mix the types of the channels.
template< class T >
class Tile
{
	public :
		typedef T ImageType;
		typedef typename T::PixelType PixelType;

		Tile( ImageType &image, /*The tile area*/ ) :
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

}; // namespace Image

}; // namespace Gander

#endif

