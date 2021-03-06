//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013-2014, Luke Goddard. All rights reserved.
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
		
template< class T, ChannelBrothers B >
inline void BrothersLayout< T, B >::increment( ChannelPointerContainerType &container, int v )
{
	typename ChannelPointerContainerType::iterator it( container.begin() );
	for( ; it != container.end(); ++it )
	{
		*it += BrotherTraits<B>::NumberOfBrothers;
	}
}

template< class T, ChannelBrothers B >
inline void BrothersLayout< T, B >::decrement( ChannelPointerContainerType &container, int v )
{
	typename ChannelPointerContainerType::iterator it( container.begin() );
	for( ; it != container.end(); ++it )
	{
		*it -= BrotherTraits<B>::NumberOfBrothers;
	}
}

template< class T, ChannelBrothers B >
inline ChannelSet BrothersLayout< T, B >::_requiredChannels() const
{
	return ChannelSet( ChannelDefault( BrotherTraits<B>::FirstBrotherInBrothers ) );
}

template< class T, ChannelBrothers B >
template< ChannelDefault C >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ReferenceType BrothersLayout< T, B >::_channel( ChannelPointerContainerType &container )
{
	return *( container[0] + BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value );
}

template< class T, ChannelBrothers B >
template< ChannelDefault C >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ConstReferenceType BrothersLayout< T, B >::_channel( const ChannelPointerContainerType &container ) const
{
	return *( container[0] + BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value );
}

template< class T, ChannelBrothers B >
	template< ChannelDefault C >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ReferenceType BrothersLayout< T, B >::_channel( ChannelContainerType &container )
{
	return container[ BrotherTraits<B>::template BrotherIndexToChannelIndex< BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value >::Value ];
}

template< class T, ChannelBrothers B >
template< ChannelDefault C >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ConstReferenceType BrothersLayout< T, B >::_channel( const ChannelContainerType &container ) const
{
	return container[ BrotherTraits<B>::template BrotherIndexToChannelIndex< BrotherTraits<B>::template IndexOfChannelInBrothers< C >::Value >::Value ];
}

template< class T, ChannelBrothers B >
template< EnumType Index >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ReferenceType BrothersLayout< T, B >::_channelAtIndex( ChannelPointerContainerType &container )
{
	return *( container[0] + BrotherTraits<B>::template ChannelIndexToBrotherIndex< Index >::Value );
}

template< class T, ChannelBrothers B >
template< EnumType Index >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ConstReferenceType BrothersLayout< T, B >::_channelAtIndex( const ChannelPointerContainerType &container ) const
{
	return *( container[0] + BrotherTraits<B>::template ChannelIndexToBrotherIndex< Index >::Value );
}


template< class T, ChannelBrothers B >
template< EnumType Index >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ReferenceType BrothersLayout< T, B >::_channelAtIndex( ChannelContainerType &container )
{
	return container[ Index ];
}

template< class T, ChannelBrothers B >
template< EnumType Index >
inline typename StaticLayoutBase< BrothersLayout< T, B >, T >::ConstReferenceType BrothersLayout< T, B >::_channelAtIndex( const ChannelContainerType &container ) const
{
	return container[ Index ];
}

template< class T, ChannelBrothers B >
inline void BrothersLayout< T, B >::_setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer )
{
	container[0] = pointer;
}

template< class T, ChannelBrothers B >
template< EnumType Index, Gander::Image::ChannelMask Mask, bool DisableStaticAsserts >
inline int BrothersLayout< T, B >::_maskedChannelIndex() const
{
	return BaseType::template MaskedChannelIndex< Index, Mask, DisableStaticAsserts >::Value;
}

}; // namespace Image

}; // namespace Gander

