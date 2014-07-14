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
inline LayoutBase< Derived, DataType >::LayoutBase()
{
}

template< class Derived, class DataType >
inline unsigned int LayoutBase< Derived, DataType >::numberOfChannelPointers()
{
	return Derived::NumberOfChannelPointers;
}

template< class Derived, class DataType >
inline ChannelSet LayoutBase< Derived, DataType >::channels()
{
	return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelMask ) );
}

template< class Derived, class DataType >
inline unsigned int LayoutBase< Derived, DataType >::numberOfChannels()
{
	return Derived::NumberOfChannels;
}
		
template< class Derived, class DataType >
inline bool LayoutBase< Derived, DataType >::contains( ChannelSet channels )
{
	return Derived::channels().contains( channels );
}
		
template< class Derived, class DataType >
inline ChannelSet LayoutBase< Derived, DataType >::requiredChannels()
{
	return ChannelSet( Gander::Image::ChannelMask( Derived::ChannelPointerMask ) );
}
		
template< class Derived, class DataType >
template< unsigned Index, bool DisableStaticAsserts, class ReturnType >
inline ReturnType &LayoutBase< Derived, DataType >::child()
{
	GANDER_IMAGE_STATIC_ASSERT( !Derived::IsCompound, DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS )
	return *( static_cast< Derived * >( this ) );
}
		
template< class Derived, class DataType >
template< class L >
inline bool LayoutBase< Derived, DataType >::equalTo( const L &rhs ) const
{
	return (
		rhs.channels() == Derived::channels() &&
		std::is_same< typename L::StorageType, typename Derived::StorageType >::value
   );
}

template< class Derived, class DataType >
template< class ContainerType >
inline void LayoutBase< Derived, DataType >::setChannelPointer( ContainerType &container, Channel channel, void * pointer )
{
	PointerType ptr = static_cast< PointerType >( pointer );
	this->setChannelPointer( container, channel, ptr );
}

template< class Derived, class DataType >
template< class ContainerType >
inline void LayoutBase< Derived, DataType >::setChannelPointer( ContainerType &container, Channel channel, PointerType pointer )
{
	GANDER_ASSERT( Derived::requiredChannels().contains( channel ) && pointer != NULL,
		"Channel pointers can only be set for channels that are in the ChannelSet returned by requiredChannels()"
	);
	static_cast< Derived * >( this )->_setChannelPointer( container, channel, pointer );
}

template< class Derived, class DataType >
template< class ContainerType, ChannelDefault C, bool DisableStaticAsserts >
inline typename LayoutBase< Derived, DataType >::ReferenceType LayoutBase< Derived, DataType >::channel( ContainerType &container )
{
	return static_cast< Derived * >( this )->template _channel< C >( container );
}

template< class Derived, class DataType >
template< class ContainerType, ChannelDefault C, bool DisableStaticAsserts >
inline typename LayoutBase< Derived, DataType >::ConstReferenceType LayoutBase< Derived, DataType >::channel( const ContainerType &container ) const
{
	return static_cast< const Derived * >( this )->template _channel< C >( container );
}
		
template< class Derived, class DataType >
template< class ContainerType, EnumType Index, EnumType Mask, bool DisableStaticAsserts >
inline typename LayoutBase< Derived, DataType >::ReferenceType LayoutBase< Derived, DataType >::channelAtIndex( ContainerType &container )
{
	return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< Index, Mask, DisableStaticAsserts >::Value >( container );
}

template< class Derived, class DataType >
template< class ContainerType, EnumType Index, EnumType Mask, bool DisableStaticAsserts >
inline typename LayoutBase< Derived, DataType >::ConstReferenceType LayoutBase< Derived, DataType >::channelAtIndex( const ContainerType &container ) const
{
	return static_cast< const Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< Index, Mask, DisableStaticAsserts >::Value >( container );
}
		
template< class Derived, class DataType >
template< class ContainerType, class Type >
inline void LayoutBase< Derived, DataType >::setChannel( ContainerType &container, Channel c, const Type &value )
{
	GANDER_ASSERT( static_cast< Derived * >( this )->channels().contains( c ), "Channel is not represented by this layout." );
	switch( c )
	{
		case( 1 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 1 ) >( container ) = value; break;
		case( 2 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 2 ) >( container ) = value; break;
		case( 3 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 3 ) >( container ) = value; break;
		case( 4 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 4 ) >( container ) = value; break;
		case( 5 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 5 ) >( container ) = value; break;
		case( 6 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 6 ) >( container ) = value; break;
		case( 7 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 7 ) >( container ) = value; break;
		case( 8 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 8 ) >( container ) = value; break;
		case( 9 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 9 ) >( container ) = value; break;
		case( 10 ) : static_cast< Derived * >( this )->template _channel< ChannelDefault( 10 ) >( container ) = value; break;
		default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
	}
}

template< class Derived, class DataType >
template< class ContainerType, class Type >
inline Type LayoutBase< Derived, DataType >::getChannel( const ContainerType &container, Channel c ) const
{
	GANDER_ASSERT( static_cast< const Derived * >( this )->channels().contains( c ), "Channel is not represented by this layout." );
	
	Type v = static_cast<Type>(0);
	switch( c )
	{
		case( 1 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 1 ) >( container ); break;
		case( 2 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 2 ) >( container ); break;
		case( 3 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 3 ) >( container ); break;
		case( 4 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 4 ) >( container ); break;
		case( 5 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 5 ) >( container ); break;
		case( 6 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 6 ) >( container ); break;
		case( 7 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 7 ) >( container ); break;
		case( 8 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 8 ) >( container ); break;
		case( 9 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 9 ) >( container ); break;
		case( 10 ) : v = static_cast< const Derived * >( this )->template _channel< ChannelDefault( 10 ) >( container ); break;
		default : GANDER_ASSERT( 0, "Channel does not exist in the LayoutContainer." ); break;
	}
	return v;
}

template< class Derived, class DataType >
template< class ContainerType, EnumType Mask >
inline typename LayoutBase< Derived, DataType >::ReferenceType LayoutBase< Derived, DataType >::channelAtIndex( ContainerType &container, unsigned int index )
{
	GANDER_ASSERT( index < static_cast< Derived * >( this )->channels().size(), "Channel is not represented by this layout." );
	switch( index )
	{
		case( 0 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 0, Mask, true >::Value >( container );
		case( 1 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 1, Mask, true >::Value >( container );
		case( 2 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 2, Mask, true >::Value >( container );
		case( 3 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 3, Mask, true >::Value >( container );
		case( 4 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 4, Mask, true >::Value >( container );
		case( 5 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 5, Mask, true >::Value >( container );
		case( 6 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 6, Mask, true >::Value >( container );
		case( 7 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 7, Mask, true >::Value >( container );
		case( 8 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 8, Mask, true >::Value >( container );
		case( 9 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 9, Mask, true >::Value >( container );
		case( 10 ) : return static_cast< Derived * >( this )->template _channelAtIndex< Derived::template MaskedChannelIndex< 10, Mask, true >::Value >( container );
		default : GANDER_ASSERT( 0, "Index is out of bounds." ); break;
	};
}
		
template< class Derived, class DataType >
template< EnumType MaskedIndex, EnumType Mask, bool DisableStaticAsserts >
struct LayoutBase< Derived, DataType >::MaskedChannelIndex
{
	private :

		enum
		{
			/// Although this looks like voodoo, the idea is quite simple. What we want to do is find out what channel lies at an index (MaskedIndex) into a set of channels,
			/// represented by the intersection of "Mask" and "BaseType::ChannelMask".
			/// To do that, we first create an array of 4 4-bit numbers and flatten them into an EnumArray.
			/// This results in the enum 0x3210 which can be thought of as an array of 4 4-bit values, shifted into an enum. This would look like:
			/// Index:     4     3     2     1     0
			/// Value:{ 1111, 0011, 0010, 0001, 0000 } = 0xF3210
			/// We then use the mask to remove all elements from this array that aren't masked. This gives us a mapping of the remaining masked channel indices to the
			/// actual index of the channel within this layout's set of (up to) 4 channels.
			/// Finally, we use this lookup to select the element that is at "MaskedIndex".
			/// As an additional note, we fill the remaining bits of the enum with the value "15" so that we can test at compile time whether a bad index has been given.
			RemainingIndices = 
				Gander::template EnumArrayHelper< 0xFFFFFFFFFFFF3210, 4 >:: // The enum array.
				template RemoveElements< ( ( ( Mask >> ( Gander::template EnumHelper< Derived::ChannelMask >::FirstSetBit ) ) & 0xF ) | 0xFFFFFFF0 ) >::Value,
			Index = Gander::template EnumArrayHelper< RemainingIndices, 4 >::template Element< MaskedIndex >::Value
		};

		GANDER_IMAGE_STATIC_ASSERT( Index != 0xF || DisableStaticAsserts, CHANNEL_DOES_NOT_EXIST_IN_THE_LAYOUT );
		GANDER_IMAGE_STATIC_ASSERT( Derived::NumberOfChannels <= 4, THIS_FUNCTION_DOES_NOT_SUPPORT_MORE_THAN_FOUR_CHANNELS__PLEASE_EXTEND_IT_OR_OVERLOAD_IT_TO_ADD_SUPPORT_FOR_MORE );

	public :

		enum
		{
			Value = Index
		};
};
		
}; // namespace Image

}; // namespace Gander

