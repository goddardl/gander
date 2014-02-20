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

template< class Derived, class DataType >
template< Gander::Image::ChannelMask Mask >
inline unsigned int DynamicLayoutBase< Derived, DataType >::_maskedChannelIndex( unsigned int index ) const
{
	GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
	return 0;
}
	
template< class Derived, class DataType >
template< class ContainerType >
inline void DynamicLayoutBase< Derived, DataType >::setChannelPointer( ContainerType &container, Channel channel, PointerType pointer )
{
	return static_cast< Derived * >( this )->_setChannelPointer( container, channel, pointer );
}
		
template< class Derived, class DataType >
template< class ContainerType >
void DynamicLayoutBase< Derived, DataType >::_addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b )
{
	GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
}
		
template< class Derived, class DataType >
void DynamicLayoutBase< Derived, DataType >::_addChannels( ChannelSet c, ChannelBrothers b )
{
	GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
}

template< class Derived, class DataType >
template< Gander::Image::ChannelMask Mask >
inline unsigned int DynamicLayoutBase< Derived, DataType >::maskedChannelIndex( unsigned int index ) const
{
	return static_cast< Derived const * >( this )->template _maskedChannelIndex< Mask >( index );
}

template< class Derived, class DataType >
template< class ContainerType, EnumType Mask >
inline typename DynamicLayoutBase< Derived, DataType >::ReferenceType DynamicLayoutBase< Derived, DataType >::channelAtIndex( ContainerType &container, unsigned int index )
{
	GANDER_ASSERT(
			( container.size() == static_cast< Derived * >( this )->channels().size() ),
			"Container has a different number of elements to the Layout's number of channels."
			);

	GANDER_ASSERT( index < static_cast< Derived * >( this )->channels().size(), "Channel is not represented by this layout." );
	return static_cast< Derived * >( this )->_channelAtIndex( container, maskedChannelIndex< ChannelMask( Mask ) >( index ) );
}

template< class Derived, class DataType >
template< class ContainerType >
inline typename DynamicLayoutBase< Derived, DataType >::ReferenceType DynamicLayoutBase< Derived, DataType >::channel( ContainerType &container, Channel channel )
{
	GANDER_ASSERT(
			( container.size() == static_cast< Derived * >( this )->channels().size() ),
			"Container has a different number of elements to the Layout's number of channels."
			);

	GANDER_ASSERT( static_cast< Derived * >( this )->channels().contains( channel ), "Channel is not represented by this layout." );
	return static_cast< Derived * >( this )->_channel( container, channel );
}


template< class Derived, class DataType >
template< class ContainerType >
void DynamicLayoutBase< Derived, DataType >::addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b )
{
	return static_cast< Derived * >( this )->_addChannels( container, c, b );
}

template< class Derived, class DataType >
void DynamicLayoutBase< Derived, DataType >::addChannels( ChannelSet c, ChannelBrothers b )
{
	return static_cast< Derived * >( this )->_addChannels( c, b );
}

}; // namespace Image

}; // namespace Gander

