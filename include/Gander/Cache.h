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
#ifndef __GANDERIMAGE_CACHE__
#define __GANDERIMAGE_CACHE__

#include <map>
#include <string>

#include "boost/format.hpp"

#include "Gander/Exception.h"
#include "Gander/MurmurHash.h"

namespace Gander
{

// The cache manages accesses to shared memory. Data can be requested using the read() and write() methods.
// Theses methods return a reference counted accessor that grants access to the data. A write accessor
// cannot be granted for any data entry that is already being read. The allocate() method creates entries in the
// cache and deallocate() removes them. The cache should copy data between devices as required. For example, if
// data is requested (using request()) or read from a device that is not the cpu, the data has to be copied.
// Memory limits should be able to be set for each device and an LRU system used when copying data to and from them.
// The hashes supplied with the methods are appended to the size of the data that is required.
//
//  Todo:
//  Start by creating a MemoryDevice struct that registers itself to the cache and is able to allocate, dellocate and copy data.
//  The cache can then keep a registry of these so that it can copy data between cpus or gpus.

struct MemoryInterface
{
	public :

		enum
		{
			CpuMemoryInterfaceTypeId = 0
		};

		inline MemoryInterface( int instanceId ) : m_deviceTypeId( 0 ), m_instanceId( instanceId ) {} 

		//! @name Memory Interface Registry
		/// A set of methods to query the available MemoryInterfaces and create them.
		//////////////////////////////////////////////////////////////
		//@{
		/// Instantiates a new MemoryInterface.
		/// @param deviceTypeId The ID number of a registered device memory type.
		/// @param deviceInstanceId The instance of the device of the specified type.
		static MemoryInterface *create( int deviceTypeId, int deviceInstanceId = 0 );
		//@}

		/// Returns the instance ID number of this memory interface.
		inline int instanceId() const
		{
			return m_instanceId;
		}
		
		/// Returns the name of the memory interface device.
		inline const std::string &name() const
		{
			return name( m_deviceTypeId );
		}
	
		/// Returns the interface type ID number.
		inline int deviceTypeId() const
		{
			return m_deviceTypeId;
		}
	
		// Returns a list of the hashes that are given for each memory interface.
		static std::vector<int> &memoryInterfaceTypeIds()
		{
			static std::vector<int> g_memoryInterfaceTypeIds;
			return g_memoryInterfaceTypeIds;
		}
		
		/// A registry of the names for each memory type. 
		static const std::string &name( int deviceTypeId )
		{
			std::map<int, std::string>::const_iterator it = names().find( deviceTypeId );
			if( it == names().end() )
			{
				static std::string noName( "None" );
				return noName;
			}

			return it->second;
		}

		
//		virtual unsigned char *allocate( size_t );
//		virtual void deallocate( unsigned char * );
	
	protected :
	
		typedef MemoryInterface * (*CreatorFn)( int );
	
		/// Registration mechanism for MemoryInterface classes.
		/// We keep a vector of the types (returned by memoryInterfaceTypeIds) so that we can maintain an order. 
		static std::vector< CreatorFn > &creators()
		{
			static std::vector< CreatorFn > g_creators;
			return g_creators;
		}
		
		/// A registry of the names for each memory type. 
		static std::map< int, std::string > &names()
		{
			static std::map< int, std::string > g_names;
			return g_names;
		}

		template<class T>
		struct MemoryInterfaceRegistration
		{
			public:
			
			/// Registers the Device Memory Interface.
			MemoryInterfaceRegistration( int deviceTypeId, const std::string &name )
			{
				const std::vector<int> &ids = MemoryInterface::memoryInterfaceTypeIds();
				std::vector<int>::const_iterator it = std::find( ids.begin(), ids.end(), deviceTypeId );

				// Throw an exception if the type has already been registered.
				if( it != ids.end() )
				{
					throw Gander::Exception( ( boost::format( "Cannot register memory interface type %d as it already exists as \"%s\"." ) % deviceTypeId, MemoryInterface::name( *it ) ) );
				}

				MemoryInterface::memoryInterfaceTypeIds().push_back( deviceTypeId );
				MemoryInterface::creators().push_back( creator );
				MemoryInterface::names()[deviceTypeId] = name;
			}
			
			/// Returns a new instance of the MemoryInterface class.
			static MemoryInterface *creator( int instanceTypeId )
			{
				return new T( instanceTypeId );
			}
		};

	private :

		int m_deviceTypeId;
		int m_instanceId;
};

struct CpuMemoryInterface : public MemoryInterface
{
	public :
	
		CpuMemoryInterface( int instanceId ) :
			MemoryInterface( instanceId )
		{
		}

		virtual unsigned char *allocate( size_t size )
		{
			return new unsigned char [size];
		}
		
		virtual void deallocate( unsigned char *ptr )
		{
			delete [] ptr;
		}

	private :

		/// Register this memory interface so that it can be created using the MemoryInterface::create method.
		static MemoryInterfaceRegistration<CpuMemoryInterface> m_registration;
};

class Cache
{

	enum
	{
		DEVICE_CPU = 0,
		DEVICE_GPU0 = 1,
		DEVICE_GPU1 = 2,
		DEVICE_GPU2 = 3,
		DEVICE_GPU3 = 4,
	};
	
	void allocate( size_t sizeInBytes, MurmurHash hash = MurmurHash(), int deviceID = DEVICE_CPU )
	{

	}

	/*
	void deallocate( MurmurHash hash = MurmurHash() );

	WriteAccessor write( MurmurHash hash, int deviceID = DEVICE_CPU );
	ReadAccessor read( MurmurHash hash, int deviceID = DEVICE_CPU );
	void request( MurmurHash hash, int deviceID = DEVICE_CPU );
	*/

}; // namespace Cache

}; // namespace Gander

#endif

