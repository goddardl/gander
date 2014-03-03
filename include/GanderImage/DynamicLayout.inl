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

template< class T >
inline ChannelSet DynamicLayout<T>::channels() const
{
	return m_channels;
}

template< class T >
inline unsigned int DynamicLayout<T>::numberOfChannels() const
{
	return m_channels.size();
}

template< class T >
inline unsigned int DynamicLayout<T>::numberOfChannelPointers() const
{
	return m_channels.size();
}

template< class T >
inline void DynamicLayout<T>::_setChannelPointer( ChannelPointerContainerType &container, Channel channel, PointerType pointer )
{
	GANDER_ASSERT(
			( container.size() == _requiredChannels().size() ),
			"Container has a different number of elements to the Layout's number of channels."
			);
	container[ channels().index( channel ) ] = pointer;
}

template< class T >
template< class ContainerType >
void DynamicLayout<T>::_addChannels( ContainerType &container, ChannelSet c, ChannelBrothers b )
{
	GANDER_ASSERT(
			( container.size() == m_channels.size() ),
			"Container has a different number of elements to the Layout's number of channels."
			);

	GANDER_ASSERT(
			( b == Brothers_None || BrotherTraits<>::channels( b ).contains( c ) ),
			( boost::format( "DynamicLayout: Channels \"%s\" do not belong to the specified brothers." ) % c ).str()
			);

	GANDER_ASSERT(
			( !m_allBrothers.contains( c ) ),
			( boost::format( "DynamicLayout: Channels \"%s\" cannot be added as another set of ChannelBrothers represents it." ) % c ).str()
			);

	// Get a set of the current channels and the unique new ones.
	ChannelSet currentChannels( channels() );
	ChannelSet newChannels( c - currentChannels );

	_addChannels( c, b );

	// Loop over the new channels and insert a new data element for each one
	// at the same index that it is stored in the channel set.
	ChannelSet::const_iterator it( newChannels.begin() );
	ChannelSet::const_iterator end( newChannels.end() );
	for( ; it != end; ++it )
	{
		currentChannels += *it;
		int index = currentChannels.index( *it );
		container.insert( container.begin() + index, typename ContainerType::StorageType( 0 ) );
	}
}

template< class T >
void DynamicLayout<T>::_addChannels( ChannelSet c, ChannelBrothers b )
{
	GANDER_ASSERT(
			( b == Brothers_None || BrotherTraits<>::channels( b ).contains( c ) ),
			( boost::format( "DynamicLayout: Channels \"%s\" do not belong to the specified brothers." ) % c ).str()
			);

	GANDER_ASSERT(
			( !m_allBrothers.contains( c ) ),
			( boost::format( "DynamicLayout: Channels \"%s\" cannot be added as another set of ChannelBrothers represents it." ) % c ).str()
			);

	int8u step = 1;
	if( b != Brothers_None )
	{
		m_allBrothers += BrotherTraits<>::brotherMask( b );
		step = BrotherTraits<>::numberOfBrothers( b );
	}
	else
	{
		m_allBrothers += c;
	}

	ChannelSet newChannels( c - m_channels );
	ChannelSet existingChannels( c - newChannels );

	GANDER_ASSERT(
			( existingChannels.size() == 0 ),
			( boost::format( "DynamicLayout: DynamicLayout: Cannot add channels \"%s\" as they already exist." ) % existingChannels ).str()
			);

	ChannelSet::const_iterator it( newChannels.begin() );
	ChannelSet::const_iterator end( newChannels.end() );
	for( ; it != end; ++it )
	{
		m_channels += *it;
		int index = m_channels.index( *it );
		m_steps.insert( m_steps.begin() + index, step );
	}
}

template< class T >
inline ChannelSet DynamicLayout<T>::_requiredChannels() const
{
	return m_channels;
}

template< class T >
template< Gander::Image::ChannelMask Mask >
inline unsigned int DynamicLayout<T>::_maskedChannelIndex( unsigned int index ) const
{
	ChannelSet i = m_channels.intersection( Mask );
	GANDER_ASSERT( index < i.size(), "Index is out of bounds when accessing a channel in a masked set." );
	return m_channels.index( i[ index ] );
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ConstReferenceType DynamicLayout<T>::_channel( const ChannelContainerType &container, Channel c ) const
{
	return container[ m_channels.index( c ) ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ConstReferenceType DynamicLayout<T>::_channel( const ChannelPointerContainerType &container, Channel c ) const
{
	return *container[ m_channels.index( c ) ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ConstReferenceType DynamicLayout<T>::_channelAtIndex( const ChannelContainerType &container, unsigned int index ) const
{
	return container[ index ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ConstReferenceType DynamicLayout<T>::_channelAtIndex( const ChannelPointerContainerType &container, unsigned int index ) const
{
	return *container[ index ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ReferenceType DynamicLayout<T>::_channel( ChannelContainerType &container, Channel c )
{
	return container[ m_channels.index( c ) ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ReferenceType DynamicLayout<T>::_channel( ChannelPointerContainerType &container, Channel c )
{
	return *container[ m_channels.index( c ) ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ReferenceType DynamicLayout<T>::_channelAtIndex( ChannelContainerType &container, unsigned int index )
{
	return container[ index ];
}

template< class T >
inline typename DynamicLayoutBase< DynamicLayout< T >, T >::ReferenceType DynamicLayout<T>::_channelAtIndex( ChannelPointerContainerType &container, unsigned int index )
{
	return *container[ index ];
}

}; // namespace Image

}; // namespace Gander

