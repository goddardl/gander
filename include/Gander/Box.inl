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

namespace Gander
{

template <class T>
inline Box<T>::Box()
{
    makeEmpty();
}

template <class T>
inline Box<T>::Box( const T &point )
{
    min = point;
    max = point;
}

template <class T>
inline Box<T>::Box( const T &minT, const T &maxT )
{
    min = minT;
    max = maxT;
}

template <class T>
inline bool Box<T>::equalTo ( const Box<T> &src ) const
{
    return ( min == src.min && max == src.max );
}

template <class T>
inline bool Box<T>::operator == ( const Box<T> &src ) const
{
    return ( min == src.min && max == src.max );
}

template <class T>
inline bool Box<T>::operator != ( const Box<T> &src ) const
{
    return ( min != src.min || max != src.max );
}



template <class T>
inline void Box<T>::makeEmpty()
{
    min = T::Constant( std::numeric_limits< typename T::Scalar >::max() );
    max = T::Constant( std::numeric_limits< typename T::Scalar >::min() );
}

template <class T>
inline void Box<T>::makeInfinite()
{
    min = T::Constant( std::numeric_limits< typename T::Scalar >::min() );
    max = T::Constant( std::numeric_limits< typename T::Scalar >::max() );
}


template <class T>
inline void Box<T>::extendBy(const T &point)
{
    for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( point[i] < min[i] )
		{
			min[i] = point[i];
		}

		if( point[i] > max[i] )
		{
			max[i] = point[i];
		}
	}
}

template <class T>
inline void Box<T>::extendBy( const Box<T> &box )
{
    for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( box.min[i] < min[i] )
		{
			min[i] = box.min[i];
		}

		if( box.max[i] > max[i] )
		{
			max[i] = box.max[i];
		}
	}
}

template <class T>
inline bool Box<T>::intersects( const T &point ) const
{
    for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( point(i) < min( i ) || point(i) > max( i ) )
		{
			return false;
		}
	}

	return true;
}

template <class T>
inline bool Box<T>::intersects( const Box<T> &box ) const
{
	for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( box.max(i) < min( i ) || box.min(i) > max( i ) )
		{
			return false;
		}
	}

	return true;
}


template <class T> 
inline T Box<T>::size() const 
{ 
	if( isEmpty() )
	{
		return T::Constant( 0 );
	}

	return max - min;
}


template <class T> 
inline T Box<T>::center() const 
{ 
	return ( max + min ) / 2;
}


template <class T>
inline bool Box<T>::isEmpty() const
{
	for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( max[i] < min[i] )
		{
			return true;
		}
	}

	return false;
}

template <class T>
inline bool Box<T>::isInfinite() const
{
	for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( min[i] != std::numeric_limits< typename T::Scalar >::min() || max[i] != std::numeric_limits< typename T::Scalar >::max() )
		{
			return false;
		}
	}

	return true;
}


template <class T>
inline bool Box<T>::hasVolume() const
{
	for( unsigned int i = 0; i < min.size(); ++i )
	{
		if( max[i] <= min[i] )
		{
			return false;
		}
	}

	return true;
}

}; // namespace Gander

