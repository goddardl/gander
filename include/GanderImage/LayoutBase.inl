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

namespace Gander
{

namespace Image
{

template< class Derived >
inline LayoutBase< Derived >::LayoutBase()
{
	GANDER_IMAGE_STATIC_ASSERT(
	( 
		( Derived::IsDynamic && ( Derived::NumberOfChannels == 0 ) && ( Derived::NumberOfChannelPointers == 0 ) && ( EnumType( Derived::ChannelMask ) == EnumType( Mask_None ) ) )
			|| ( !Derived::IsDynamic ) || ( Derived::IsCompound )
		),
		THE_DERIVED_LAYOUT_IS_DYNAMIC_AND_THEREFORE_MUST_NOT_DECLARE_ENUM_VALUES_THAT_ARE_TO_BE_USED_BY_STATIC_LAYOUTS_ONLY
	);

	GANDER_IMAGE_STATIC_ASSERT(
	(
			( ( !Derived::IsDynamic ) && ( Derived::NumberOfChannels != 0 ) && ( Derived::NumberOfChannelPointers != 0 ) && ( EnumType( Derived::ChannelMask ) != EnumType( Mask_None ) ) )
			|| Derived::IsDynamic
		),
		THE_DERIVED_LAYOUT_HASNT_DECLARED_ALL_REQUIRED_ENUM_VALUES
	);
}

template< class Derived >
inline unsigned int LayoutBase< Derived >::numberOfChannels() const
{
	GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
	return Derived::NumberOfChannels;
}

template< class Derived >
inline unsigned int LayoutBase< Derived >::numberOfChannelPointers() const
{
	GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
	return Derived::NumberOfChannelPointers;
}

template< class Derived >
inline ChannelSet LayoutBase< Derived >::channels() const
{
	GANDER_IMAGE_STATIC_ASSERT( ( !Derived::IsDynamic ), DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
	return ChannelSet( static_cast<Gander::Image::ChannelMask>( Derived::ChannelMask ) );
}
		
template< class Derived >
inline bool LayoutBase< Derived >::contains( ChannelSet channels ) const
{
	return static_cast< Derived const * >( this )->channels().contains( channels );
}

template< class Derived >
template< EnumType C >
inline bool LayoutBase< Derived >::containsChannel( Gander::Image::Channel c ) const
{
	if( C == Chan_None || Derived::IsDynamic )
	{
		return static_cast< Derived const * >( this )->contains( ChannelSet( c ) );
	}
	else
	{
		return ( ChannelToMask< C >::Value & Derived::ChannelMask ) != 0;
	}
}
		
template< class Derived >
inline bool LayoutBase< Derived >::isCompound() const
{
	return Derived::IsCompound;
}

template< class Derived >
inline bool LayoutBase< Derived >::isDynamic() const
{
	return Derived::IsDynamic;
}

template< class Derived >
inline ChannelSet LayoutBase< Derived >::requiredChannels() const
{
	return static_cast< Derived const * >( this )->_requiredChannels();
}
		
template< class Derived >
template< unsigned Index, bool DisableStaticAsserts, class ReturnType >
inline ReturnType &LayoutBase< Derived >::child()
{
	GANDER_IMAGE_STATIC_ASSERT( !Derived::IsCompound, DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS )
	return *( static_cast< Derived * >( this ) );
}
		
template< class Derived >
inline ChannelSet LayoutBase< Derived >::_requiredChannels() const
{
	GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
	return 0; // We never get here.
}

template< class Derived >
template< class L >
inline bool LayoutBase< Derived >::equalTo( const L &rhs ) const
{
	return (
		rhs.channels() == static_cast< Derived const * >( this )->channels() &&
		std::is_same< typename L::StorageType, typename Derived::StorageType >::value
   );
}
		
}; // namespace Image

}; // namespace Gander

