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
#ifndef __GANDER_TUPLE__
#define __GANDER_TUPLE__

#include <iostream>
#include <stdexcept>

#include "boost/format.hpp"

#include "Gander/Common.h"
#include "Gander/Assert.h"
#include "Gander/Interfaces.h"
#include "Gander/StaticAssert.h"

namespace Gander
{

/// A simple wrapper class for defining either a static or dynamic array of data elements.
/// The Tuple class wraps up either a C-Style array or a std::vector to provide a uniform interface to both.
/// The class takes the data type as the first template argument, the number of elements for the second
/// and a flag that indicates whether the tuple is dynamic or not for the third.
template< class T, unsigned NumberOfElements = 1, bool IS_DYNAMIC = true >
struct Tuple;

/// The static Tuple.
template< class T, unsigned NumberOfElements >
struct Tuple< T, NumberOfElements, false >
{
	public :

		typedef T StorageType;
		typedef StorageType &ReferenceType;
		typedef const StorageType &ConstReferenceType;
		typedef StorageType *PointerType;
	
		/// iterator Type Declarations 	
		typedef const StorageType * const_iterator;
		typedef StorageType * iterator;
		
		/// The constructor only checks that the runtime size of the array matches that of the compile time.
		Tuple( unsigned int numberOfElements = NumberOfElements )
		{
			GANDER_ASSERT(
				numberOfElements == NumberOfElements,
				"A static tuple cannot be resized at runtime. Please use a dynamic tuple."
			);
		}
	
		inline ReferenceType operator[] ( unsigned int i ) { return m_data[i]; };
		inline ConstReferenceType operator[] ( unsigned int i ) const { return m_data[i]; };

		inline const_iterator begin() const
		{
			return &m_data[0];
		};
		
		inline iterator begin()
		{
			return &m_data[0];
		};

		inline const_iterator end() const
		{
			return m_data+NumberOfElements;
		};
		
		inline void resize( unsigned int size )
		{
			GANDER_STATIC_ASSERT_ERROR( THIS_METHOD_CANNOT_BE_CALLED_ON_A_STATIC_TUPLE );
		}
		
		inline void clear()
		{
			memset( &m_data[0], 0, sizeof( StorageType ) * NumberOfElements );
		}

		inline unsigned int size() const { return NumberOfElements; };

		inline void push_back( const StorageType &val )
		{
			GANDER_STATIC_ASSERT_ERROR( THIS_METHOD_CANNOT_BE_CALLED_ON_A_STATIC_TUPLE );
		}
		
		inline iterator insert( iterator it, const StorageType &val )
		{
			GANDER_STATIC_ASSERT_ERROR( THIS_METHOD_CANNOT_BE_CALLED_ON_A_STATIC_TUPLE );
		}

		inline iterator erase( iterator it )
		{
			GANDER_STATIC_ASSERT_ERROR( THIS_METHOD_CANNOT_BE_CALLED_ON_A_STATIC_TUPLE );
		}

	private :

		StorageType m_data[ NumberOfElements ];
};

/// The dynamic Tuple.
template< class T, unsigned NumberOfElements >
struct Tuple< T, NumberOfElements, true >
{
	public :
	
		typedef T StorageType;
		typedef StorageType &ReferenceType;
		typedef const StorageType &ConstReferenceType;
		typedef StorageType *PointerType;

		/// iterator Type Declarations 	
		typedef typename std::vector< StorageType >::const_iterator const_iterator;
		typedef typename std::vector< StorageType >::iterator iterator;
	
		/// Default constructor	
		Tuple() :
			m_data( NumberOfElements )
		{}
		
		Tuple( unsigned int numberOfElements ) :
			m_data( numberOfElements )
		{}

		inline ReferenceType operator[] ( unsigned int i ) { return m_data[i]; };
		inline ConstReferenceType operator[] ( unsigned int i ) const { return m_data[i]; };
		
		inline unsigned int size() const
		{
			return m_data.size();
		};
		
		inline void resize( unsigned int size )
		{
			m_data.resize( size );
		}
		
		inline void clear()
		{
			m_data.resize( size(), 0 );
		}

		inline const_iterator begin() const
		{
			return m_data.begin();
		};
		
		inline iterator begin()
		{
			return m_data.begin();
		};

		inline const_iterator end() const
		{
			return m_data.end();
		};

		inline void push_back( const StorageType &val )
		{
			return m_data.push_back( val );
		}
		
		inline iterator insert( iterator it, const StorageType &val )
		{
			return m_data.insert( it, val );
		}

		inline iterator erase( iterator it )
		{
			return m_data.erase( it );
		}

	private :

		std::vector< StorageType > m_data;
};

}; // namespace Gander

#endif
