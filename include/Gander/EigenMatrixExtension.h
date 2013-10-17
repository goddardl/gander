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

//////////////////////////////////////////////////////////////////////////
/// Matrix and Vector Extensions
/// This file gets inserted into the Eigen::Matrix class which provides
/// us with a simple way of extending it.
//////////////////////////////////////////////////////////////////////////

/// If we are building against Imath then add casting operators between the common Eigen and Imath types.
#ifdef USE_IMATH_EXTENSIONS

/// Constructors for Matrices and Vectors which are initialized by Imath primitives.
//\todo: Although we don't use it, check that these constructors and the cast operators work with matrices that are Row major and remove the static asserts.
EIGEN_STRONG_INLINE Matrix( const Imath::Vec2<RealScalar> &v )
	: Base( 2, RowsAtCompileTime == 1 ? 1 : 2, ColsAtCompileTime == 1 ? 1 : 2 )
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 2 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	Base::_check_template_params();
	if( SizeAtCompileTime == Dynamic )
	{
		Base::resize(2, 1);
	}
	RealScalar* to( m_storage.data() );
	const RealScalar* from( &v[0] );
	*to++ = *from++;
	*to++ = *from++;
}

EIGEN_STRONG_INLINE Matrix( const Imath::Vec3<RealScalar> &v )
	: Base( 3, RowsAtCompileTime == 1 ? 1 : 3, ColsAtCompileTime == 1 ? 1 : 3 )
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 3 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	Base::_check_template_params();
	if( SizeAtCompileTime == Dynamic )
	{
		Base::resize(3, 1);
	}
	Base::_check_template_params();
	Base::resize(3, 1);
	RealScalar* to( m_storage.data() );
	const RealScalar* from( &v[0] );
	*to++ = *from++;
	*to++ = *from++;
	*to++ = *from++;
}

EIGEN_STRONG_INLINE Matrix( const Imath::Vec4<RealScalar> &v )
	: Base( 4, RowsAtCompileTime == 1 ? 1 : 4, ColsAtCompileTime == 1 ? 1 : 4 )
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 4 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	Base::_check_template_params();
	if( SizeAtCompileTime == Dynamic )
	{
		Base::resize(4, 1);
	}
	Base::_check_template_params();
	Base::resize(4, 1);
	RealScalar* to( m_storage.data() );
	const RealScalar* from( &v[0] );
	*to++ = *from++;
	*to++ = *from++;
	*to++ = *from++;
	*to++ = *from++;
}

EIGEN_STRONG_INLINE Matrix( const Imath::Matrix33<RealScalar> &m )
	: Base( 9, RowsAtCompileTime, ColsAtCompileTime )
{
	EIGEN_STATIC_ASSERT( (Flags&RowMajorBit)==0, THIS_METHOD_IS_ONLY_FOR_COLUMN_MAJOR_MATRICES );
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 9 || SizeAtCompileTime == Dynamic, YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES )
	Base::_check_template_params();
	if( SizeAtCompileTime == Dynamic )
	{
		Base::resize(3, 3);
	}
	RealScalar* to( m_storage.data() );
	for( int j = 0; j < 3; ++j )
	{
		for( int i = 0; i < 3; ++i )
		{
			*to++ = m[i][j];
		}
	}
}

EIGEN_STRONG_INLINE Matrix( const Imath::Matrix44<RealScalar> &m )
	: Base( 16, RowsAtCompileTime, ColsAtCompileTime )
{
	EIGEN_STATIC_ASSERT( (Flags&RowMajorBit)==0, THIS_METHOD_IS_ONLY_FOR_COLUMN_MAJOR_MATRICES );
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 16 || SizeAtCompileTime == Dynamic, YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES )
	Base::_check_template_params();
	if( SizeAtCompileTime == Dynamic )
	{
		Base::resize(4, 4);
	}
	RealScalar* to( m_storage.data() );
	for( int j = 0; j < 4; ++j )
	{
		for( int i = 0; i < 4; ++i )
		{
			*to++ = m[i][j];
		}
	}
}

inline operator Imath::Vec2<RealScalar>() const
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 2 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	const RealScalar* d( m_storage.data() );
	return Imath::Vec3<RealScalar>( d[0], d[1] );
}

inline operator Imath::Vec3<RealScalar>() const
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 3 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	const RealScalar* d( m_storage.data() );
	return Imath::Vec3<RealScalar>( d[0], d[1], d[2] );
}

inline operator Imath::Vec4<RealScalar>() const
{
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 4 || SizeAtCompileTime == Dynamic, YOU_MIXED_VECTORS_OF_DIFFERENT_SIZES )
	const RealScalar* d( m_storage.data() );
	return Imath::Vec3<RealScalar>( d[0], d[1], d[2], d[3] );
}

inline operator Imath::Matrix33<RealScalar>() const
{
	EIGEN_STATIC_ASSERT( (Flags&RowMajorBit)==0, THIS_METHOD_IS_ONLY_FOR_COLUMN_MAJOR_MATRICES );
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 9 || SizeAtCompileTime == Dynamic, YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES )
	const RealScalar* d( m_storage.data() );
	return Imath::Matrix33<RealScalar>( d[0], d[3], d[6], d[1], d[4], d[7], d[2], d[5], d[8] );
}

inline operator Imath::Matrix44<RealScalar>() const
{	
	EIGEN_STATIC_ASSERT( (Flags&RowMajorBit)==0, THIS_METHOD_IS_ONLY_FOR_COLUMN_MAJOR_MATRICES );
    EIGEN_STATIC_ASSERT( SizeAtCompileTime == 16 || SizeAtCompileTime == Dynamic, YOU_MIXED_MATRICES_OF_DIFFERENT_SIZES )
	const RealScalar* d( m_storage.data() );
	return Imath::Matrix44<RealScalar>( d[0], d[4], d[8], d[12], d[1], d[5], d[9], d[13], d[2], d[6], d[10], d[14], d[3], d[7], d[11], d[15] );
}

#endif // USE_IMATH_EXTENSIONS 

