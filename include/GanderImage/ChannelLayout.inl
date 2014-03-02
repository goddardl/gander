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

template< class T, ChannelDefault S >
template< ChannelDefault C >
inline typename StaticLayoutBase< ChannelLayout< T, S >, T >::ReferenceType ChannelLayout< T, S >::_channel( ChannelContainerType &container )
{
	return container[0];
}

template< class T, ChannelDefault S >
template< ChannelDefault C >
inline typename StaticLayoutBase< ChannelLayout< T, S >, T >::ReferenceType ChannelLayout< T, S >::_channel( ChannelPointerContainerType &container )
{
	return *container[0];
}

template< class T, ChannelDefault S >
template< EnumType Index >
inline typename StaticLayoutBase< ChannelLayout< T, S >, T >::ReferenceType ChannelLayout< T, S >::_channelAtIndex( ChannelContainerType &container )
{
	return container[0];
}

template< class T, ChannelDefault S >
template< EnumType Index >
inline typename StaticLayoutBase< ChannelLayout< T, S >, T >::ReferenceType ChannelLayout< T, S >::_channelAtIndex( ChannelPointerContainerType &container )
{
	return *container[0];
}

template< class T, ChannelDefault S >
inline void ChannelLayout< T, S >::_setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer )
{
	container[0] = pointer;
}

/// Returns a ChannelSet of the channels that pointers are required for in order
/// to access all of the channels in this layout.
template< class T, ChannelDefault S >
inline ChannelSet ChannelLayout< T, S >::_requiredChannels() const
{
	return ChannelSet( S );
}

/// Returns the index of a channel in the layout when masked.
template< class T, ChannelDefault S >
template< EnumType Index, Gander::Image::ChannelMask Mask, bool DisableStaticAsserts >
inline int ChannelLayout< T, S >::_maskedChannelIndex() const
{
	GANDER_IMAGE_STATIC_ASSERT( ( Mask & ChannelMask ) != 0 || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
	return 0;
}

/// Returns the index of a channel within the layout.	
template< class T, ChannelDefault S >
template< EnumType Channel, EnumType Mask, bool DisableStaticAsserts >
inline unsigned int ChannelLayout< T, S >::_indexOfChannel() const
{
	GANDER_IMAGE_STATIC_ASSERT( ( Mask & ChannelMask ) != 0 || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
	return 0;
}

}; // namespace Image

}; // namespace Gander
