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
#ifndef __GANDERIMAGE_LAYOUT__
#define __GANDERIMAGE_LAYOUT__

#include <type_traits>
#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Assert.h"
#include "Gander/Common.h"
#include "Gander/Tuple.h"

#include "Gander/StaticAssert.h"
#include "GanderImage/StaticAssert.h"
#include "GanderImage/Channel.h"
#include "GanderImage/ChannelBrothers.h"

namespace Gander
{

namespace Image
{

namespace Detail
{

template< class Layout >
struct ChannelContainer
{
	public :
		
		typedef typename Layout::StorageType StorageType;
		typedef Layout LayoutType;

		ChannelContainer()
		{
			GANDER_IMAGE_STATIC_ASSERT( !Layout::IsDynamic, CLASS_CONTAINS_A_DYNAMIC_LAYOUT_PLEASE_USE_THE_CONSTRUCTOR_THAT_INITIALIZES_IT );
		}

		ChannelContainer( const Layout &layout ) :
			m_layout( layout )
		{
		}

		inline const Layout &layout() const
		{
			return m_layout;
		}
	
		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline StorageType &channelAtIndex()
		{
			return m_data[ m_layout.template maskedChannelIndex< Index, Mask, DisableStaticAsserts >() ];
		}

		template< ChannelDefault C, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline StorageType &channel()
		{
			if( m_layout.template containsChannel< C >() )
			{
				return m_data[ m_layout.template indexOfChannel< C, Mask, DisableStaticAsserts >() ];
			}
			else
			{
				GANDER_ASSERT( DisableStaticAsserts, "Channel does not exist in the ChannelContainer." );

				// The compiler never reaches here but we need to return a valid value to prevent any warnings.
				static StorageType unusedValue = StorageType(0);
				return unusedValue;
			}
		}

	private :
		
		typedef Gander::template Tuple< typename Layout::StorageType, Layout::NumberOfChannels, Layout::IsDynamic > ChannelContainerType;

		ChannelContainerType m_data;
	
		Layout m_layout;

};

template< class CompoundLayout >
struct CompoundChannelContainerBase
{
	public :
		
		template< ChannelDefault C >
		struct ChannelTraits : public CompoundLayout::template ChannelTraits< C >
		{
			ChannelTraits( CompoundLayout layout, Channel channel ) :
				CompoundLayout::template ChannelTraits< C >( layout, channel )
			{
			}
		};
		
		template< EnumType LayoutIndex >
		struct LayoutTraits : public CompoundLayout::template LayoutTraits< LayoutIndex >
		{};

};

template< class CompoundLayout, EnumType N >
struct CompoundChannelContainerRecurse;

template< class CompoundLayout >
struct CompoundChannelContainerRecurse< CompoundLayout, 0 > : public CompoundChannelContainerBase< CompoundLayout >
{
	public :

		typedef CompoundChannelContainerBase< CompoundLayout > BaseType;
	
	protected :
		
		template< class ReturnType, EnumType Index >
		inline ReturnType &container()
		{
			GANDER_ASSERT( Index >= 0 || Index < CompoundLayout::NumberOfLayouts, "Index is out of bounds." );
			static ReturnType r; // We never get here but we still need to return something to appease the compiler.
			return r;
		};
};

template< class CompoundLayout, EnumType N >
struct CompoundChannelContainerRecurse : public CompoundChannelContainerRecurse< CompoundLayout, N - 1 >
{
	public :

		enum { LayoutIndex = CompoundLayout::NumberOfLayouts - N };
		typedef CompoundChannelContainerRecurse< CompoundLayout, N - 1  > BaseType;
		typedef typename CompoundLayout::template LayoutTraits< LayoutIndex >::LayoutType LayoutType;
		typedef typename LayoutType::StorageType StorageType;
		typedef ChannelContainer< LayoutType > ContainerType;

	protected :
		
		template< class ReturnType, EnumType Index >
		inline ReturnType &container()
		{
			if( Index == LayoutIndex )
			{
				return ( ReturnType & ) m_container;
			}
			else
			{
				return BaseType::template container< ReturnType, Index >();
			}
		};

	private :

		ContainerType m_container;

};

template< class CompoundLayout >
class CompoundChannelContainer : public Gander::Image::Detail::CompoundChannelContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts >
{
	public :
		
		template< EnumType Index >
		inline ChannelContainer< typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType > &container()
		{
			return BaseType::template container< ChannelContainer< typename CompoundLayout::template LayoutTraits< Index, true >::LayoutType >, Index >();
		};

		template< EnumType Index, ChannelMask Mask = Mask_All, class ChannelType = typename CompoundLayout::template ChannelTraitsAtIndex< Index, Mask >::StorageType >
		inline ChannelType &channelAtIndex()
		{
			enum
			{
				ContainerIndex = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, true >::LayoutIndex,
				ChannelIndexInLayout = CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, true >::ChannelIndexInLayout,
			};
			
			GANDER_ASSERT( ( std::is_same< ChannelType, typename CompoundLayout::template ChannelTraitsAtIndex< Index, Mask, true >::StorageType >::value ), "Incorrect return type specified." );
		
			return ( ChannelType & ) container< ContainerIndex >().template channelAtIndex< ChannelIndexInLayout, Mask, true >();
		}
		
		template< ChannelDefault C, ChannelMask Mask = Mask_All, class ChannelType = typename CompoundLayout::template ChannelTraits< C, true >::StorageType >
		inline ChannelType &channel()
		{
			enum
			{
				ContainerIndex = CompoundLayout::template ChannelTraits< C, true >::LayoutIndex,
			};
			
			GANDER_ASSERT( ( std::is_same< ChannelType, typename CompoundLayout::template ChannelTraits< C, true >::StorageType >::value ), "Incorrect return type specified." );
			
			return ( ChannelType & ) container< ContainerIndex >().template channel< C, Mask, true >();
		}
		
		template< class ChannelType, ChannelMask Mask = Mask_All >
		inline ChannelType &channel( Channel c )
		{
			switch( c )
			{
				case( 0 ) : return channel< ChannelDefault( 0 ), Mask, ChannelType >(); break;
				case( 1 ) : return channel< ChannelDefault( 1 ), Mask, ChannelType >(); break;
				case( 2 ) : return channel< ChannelDefault( 2 ), Mask, ChannelType >(); break;
				case( 3 ) : return channel< ChannelDefault( 3 ), Mask, ChannelType >(); break;
				case( 4 ) : return channel< ChannelDefault( 4 ), Mask, ChannelType >(); break;
				case( 5 ) : return channel< ChannelDefault( 5 ), Mask, ChannelType >(); break;
				case( 6 ) : return channel< ChannelDefault( 6 ), Mask, ChannelType >(); break;
				case( 7 ) : return channel< ChannelDefault( 7 ), Mask, ChannelType >(); break;
				case( 8 ) : return channel< ChannelDefault( 8 ), Mask, ChannelType >(); break;
				case( 9 ) : return channel< ChannelDefault( 9 ), Mask, ChannelType >(); break;
				case( 10 ) : return channel< ChannelDefault( 10 ), Mask, ChannelType >(); break;
				case( 11 ) : return channel< ChannelDefault( 11 ), Mask, ChannelType >(); break;
				case( 12 ) : return channel< ChannelDefault( 12 ), Mask, ChannelType >(); break;
				case( 13 ) : return channel< ChannelDefault( 13 ), Mask, ChannelType >(); break;
				case( 14 ) : return channel< ChannelDefault( 14 ), Mask, ChannelType >(); break;
				case( 15 ) : return channel< ChannelDefault( 15 ), Mask, ChannelType >(); break;
				default : GANDER_ASSERT( 0, "Channel does not exist in the ChannelContainer." ); break;
			}
		}
		
		template< class ChannelType, ChannelMask Mask = Mask_All >
		inline ChannelType &channelAtIndex( unsigned int index )
		{
			switch( index )
			{
				case( 0 ) : return channelAtIndex< 0, Mask, ChannelType >(); break;
				case( 1 ) : return channelAtIndex< 1, Mask, ChannelType >(); break;
				case( 2 ) : return channelAtIndex< 2, Mask, ChannelType >(); break;
				case( 3 ) : return channelAtIndex< 3, Mask, ChannelType >(); break;
				case( 4 ) : return channelAtIndex< 4, Mask, ChannelType >(); break;
				case( 5 ) : return channelAtIndex< 5, Mask, ChannelType >(); break;
				case( 6 ) : return channelAtIndex< 6, Mask, ChannelType >(); break;
				case( 7 ) : return channelAtIndex< 7, Mask, ChannelType >(); break;
				case( 8 ) : return channelAtIndex< 8, Mask, ChannelType >(); break;
				case( 9 ) : return channelAtIndex< 9, Mask, ChannelType >(); break;
				case( 10 ) : return channelAtIndex< 10, Mask, ChannelType >(); break;
				case( 11 ) : return channelAtIndex< 11, Mask, ChannelType >(); break;
				case( 12 ) : return channelAtIndex< 12, Mask, ChannelType >(); break;
				case( 13 ) : return channelAtIndex< 13, Mask, ChannelType >(); break;
				case( 14 ) : return channelAtIndex< 14, Mask, ChannelType >(); break;
				case( 15 ) : return channelAtIndex< 15, Mask, ChannelType >(); break;
				default : GANDER_ASSERT( 0, "Index is out of bounds." ); break;
			};
		}
	private :

		typedef typename Gander::Image::Detail::CompoundChannelContainerRecurse< CompoundLayout, CompoundLayout::NumberOfLayouts > BaseType;

};

template< class LayoutType >
struct ChannelTraitsInterface
{
	public :
	
		ChannelTraitsInterface( const LayoutType &l, Channel channel = Chan_None ) :
			m_step( l.template step( channel ) )
		{
		}

		int8u step() const
		{
			return m_step;
		}

	private :

		int8u m_step;
};

}; // namespace Detail

/// The base class for defining how a set of channels are grouped together and of what type they are.
/// The Layout class defines an interface for the derived classes using static polymorphism.
/// The Derived classes need to implement several Enum values, a type declaration for the StorageType of the channels,
/// a several accessor methods.
/// In addition to these, the equalTo method can be optionally overridden.
template< class Derived >
struct Layout
{

	public :

		enum
		{
			NumberOfLayouts = 1,
			IsDynamic = false,
		};
		
		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int maskedChannelIndex() const
		{
			return static_cast< Derived const * >( this )->template _maskedChannelIndex< Index, Mask, DisableStaticAsserts >();
		}

		inline ChannelSet channels() const
		{
			return static_cast< Derived const * >( this )->_channels();
		}

		inline unsigned int numberOfChannels() const
		{
			return static_cast< Derived const * >( this )->_numberOfChannels();
		}

		inline ChannelSet requiredChannels() const
		{
			return static_cast< Derived const * >( this )->_requiredChannels();
		}
		
		inline void addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			static_cast< Derived * >( this )->_addChannels( c, b );
		}
		
		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u step( Channel channel = Chan_None ) const
		{
			GANDER_ASSERT( ( C == Chan_None || channel == Chan_None || channel == C ), "Specify either a compile-time argument or a runtime parameter but not both." );
			return static_cast< Derived const * >( this )->_step<C>( channel );
		}
		
		/// Returns whether the layout represents the given channel.
		inline bool contains( ChannelSet channels ) const
		{
			return static_cast< Derived const * >( this )->_contains( channels );
		}
				
		template< EnumType Channel, EnumType Mask = Mask_All, bool DisableStaticAsserts = false >
		inline unsigned int indexOfChannel() const
		{
			return static_cast< Derived const * >( this )->template _indexOfChannel< Channel, Mask, DisableStaticAsserts >();
		}

		template< EnumType C = Chan_None >
		inline bool containsChannel( Gander::Image::Channel c = Chan_None ) const
		{
			return static_cast< Derived const * >( this )->template _containsChannel< C >( c );
		}

		template< EnumType Index >
		struct LayoutTraits
		{
			GANDER_IMAGE_STATIC_ASSERT( Index == 0, THE_REQUESTED_LAYOUT_AT_THE_GIVEN_INDEX_DOES_NOT_EXIST );
			typedef Derived LayoutType;
			typedef typename LayoutType::StorageType StorageType;
		
			enum
			{
				LayoutIndex = Index,
			};
		};

		template< class L > inline bool operator == ( L const &rhs ) const { return static_cast< Derived const * >( this )->equalTo( rhs ); }
		template< class L > inline bool operator != ( L const &rhs ) const { return !static_cast< Derived const * >( this )->equalTo( rhs ); }

	private :	

		/// Deried classes need to override this function to return the channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline ChannelSet _channels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return ChannelSet(); // We never get here.
		}

		/// Deried classes need to override this function to return the number of channels that this layout represents.
		/// If they do not, a static assertion prevents the code from compiling.
		inline unsigned int _numberOfChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}
		
		/// Returns a ChannelSet of the channels that pointers are required for in order
		/// to access all of the channels in this layout.
		inline ChannelSet _requiredChannels() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}

		/// Returns the step value for a given channel.
		template< ChannelDefault C = Chan_None >
		inline int8u _step( Channel channel = Chan_None ) const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}

		/// Returns whether the layout represents the given set of channels.
		inline bool _contains( ChannelSet channels ) const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0; // We never get here.
		}
		
		/// Returns whether the layout contains the given channel. This method can be passed either a template argument or a function argument.
		/// If the template argument is different to Chan_None then the template argument should be used. If it is equal to Chan_None, the 
		/// function argument should be used.
		template< EnumType C = Chan_None >
		inline bool _containsChannel( Gander::Image::Channel c = Chan_None ) const
		{
			if( C == Chan_None )
			{
				return static_cast< Derived const * >( this )->_contains( ChannelSet( c ) );
			}
			else
			{
				return ( ChannelToMask< C >::Value & Derived::ChannelMask ) != 0;
			}
		}

		/// Adds the channel to the Layout and logs all pertenant information.
		inline void _addChannels( ChannelSet c, ChannelBrothers b = Brothers_None )
		{
			GANDER_IMAGE_STATIC_ASSERT( Derived::IsDynamic, THE_LAYOUT_MUST_BE_DYNAMIC_IN_ORDER_TO_ADD_CHANNELS_TO_IT )
		}

		/// A function for comapring two Layouts. The overloaded equality operators call this method on the Derived class,
		/// allowing the Derived class to overload this function.
		template< class L >
		inline bool equalTo( const L &rhs ) const
		{
			return (
				rhs.channels() == channels() &&
				std::is_same< typename L::StorageType, typename Derived::StorageType >::value
		   );
		}
		
		/// Returns the index of a channel in the layout when masked.
		template< EnumType Index, ChannelMask Mask = Mask_All, bool DisableStaticAsserts = false >
		inline int _maskedChannelIndex() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0;
		}
	
		/// Returns the index of a channel within the layout.	
		template< EnumType Channel, EnumType Mask = Mask_All, bool >
		inline unsigned int _indexOfChannel() const
		{
			GANDER_STATIC_ASSERT_ERROR( DERIVED_CLASS_HAS_NOT_IMPLEMENTED_ALL_PURE_STATIC_METHODS_REQUIRED_BY_THE_BASE_CLASS );
			return 0;
		}
};

}; // namespace Image

}; // namespace Gander

#endif
