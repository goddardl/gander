
#ifndef __LG_IMAGE_BUFFER_H__
#define __LG_IMAGE_BUFFER_H__

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cassert>

#include "boost/shared_ptr.hpp"

#include "OpenEXR/ImfArray.h"

#include "LGImage/pixel.h"
#include "LGCore/box.h"
#include "LGCore/bitAccess.h"

namespace LG
{

namespace Image
{

/// Buffer
/// A class to hold a 2D array of data.
template< typename PIXELTYPE >
class Buffer
{
	public:

		typedef PIXELTYPE PixelType;
		typedef typename PixelType::ChannelType ChannelType;

		Buffer<PixelType>()
			: m_data( new Imf::Array2D<PixelType> )
		{
		}

		Buffer<PixelType>( int w, int h )
			: m_dataWindow( w, h ),
			m_data( new Imf::Array2D<PixelType> )
		{
			reset( w, h );
		}
			
		Buffer<PixelType>( const Buffer<PixelType> &rhs ) :
			m_dataWindow( rhs.m_dataWindow ),
			m_data( rhs.m_data )
		{
		}
		
		/// Accessors
		///@{
		inline int w() const { return m_dataWindow.w(); }
		inline int h() const { return m_dataWindow.h(); }
		inline PixelType *operator [] ( int y ) { return (*m_data)[y]; }
		inline const PixelType *operator [] ( int y ) const { return (*m_data)[y]; }
		inline const LG::Core::Box2i &dataWindow() const { return m_dataWindow; }
		static inline unsigned int nChannels() { return PixelType::nChannels(); };

		///@}
		/// Utility Methods
		///@{
		void clear( PixelType v )
		{
			for ( int y = 0; y < h(); ++y )
			{
				for ( int x = 0; x < w(); ++x )
				{
					(*m_data)[y][x] = v;
				}
			}
		}

		void clear( int c, ChannelType v )
		{	
			for ( int y = 0; y < h(); ++y )
			{
				for ( int x = 0; x < w(); ++x )
				{
					(*m_data)[y][x][c] = v;
				}
			}
		}
		
		void copy( const Buffer<PixelType> &rhs )
		{
			m_dataWindow = rhs.m_dataWindow;
			m_data->resizeErase( m_dataWindow.h(), m_dataWindow.w() );
			memcpy( &(*m_data)[0][0], &(*rhs.m_data)[0][0], m_dataWindow.w()*m_dataWindow.h()*sizeof( PixelType ) );
		}

		void reset( int w, int h )
		{
			assert( w > 0 );
			assert( h > 0 );
			m_data->resizeErase( h, w );
			setDataWindow( w, h );
		}

		void reset( int w, int h, PixelType v )
		{
			m_data->resizeErase( h, w );
			clear(v);
		}
		///@}

		void swap( Buffer<PixelType> &rhs )
		{
			LG::Core::Box2i box = m_dataWindow;
			m_dataWindow = rhs.m_dataWindow;
			rhs.m_dataWindow = box;
			m_data.swap( rhs.m_data );
		}

	private:
		
		inline void setDataWindow( int w, int h )
		{
			m_dataWindow = LG::Core::Box2i( w, h );
		};

		LG::Core::Box2i m_dataWindow;
		boost::shared_ptr< Imf::Array2D<PixelType> > m_data;
};

typedef Buffer<LG::Image::Pixel3f> Buffer3f;
typedef Buffer<LG::Image::Pixel2f> Buffer2f;
typedef Buffer<LG::Image::Pixel1f> Buffer1f;

template< typename T, typename S >
inline T clamp( const T &in, S l, S h )
{
	return ( in > h ) ? h : ( in < l ) ? l : in;
}


/*
void StereoDisparity::calculateIntensity( const Imf::Array2D<Pixel> &pixels, int.w, int.h, Imf::Array2D<float> &out )
{
    out.resizeErase(.h,.w );

    for ( int y = 0; y <.h; ++y )
    {
	for ( int x = 0; x <.w; ++x )
	{
	    out[y][x] = at<Pixel>( pixels, x, y,.w,.h ).intensity();
	}
    }
}

template< typename T >
void normalize( Imf::Array2D<T> &out )
{
    float max = -10e-6;
    float min = 10e-6;
    for ( int y = 0; y <.h; ++y )
    {
	for ( int x = 0; x <.w; ++x )
		{
			if (out[y][x] < min) min = out[y][x];
			if (out[y][x] > max) max = out[y][x];
		}
	}
	
	float d = max - min;
	for ( int y = 0; y <.h; ++y )
	{
		for ( int x = 0; x <.w; ++x )
		{
			out[y][x] = (out[y][x]-min)/d;
		}
	}
}
*/
} // namespace Image

} // namespace LG

#endif

