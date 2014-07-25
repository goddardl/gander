//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007-2011, Image Engine Design Inc. All rights reserved.
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
//     * Neither the name of Image Engine Design nor the names of any
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

#ifndef __GANDER_REFCOUNTED__
#define __GANDER_REFCOUNTED__

#include <cassert>

#include "boost/noncopyable.hpp"

#include "tbb/atomic.h"

#include "Gander/IntrusivePtr.h"

namespace Gander
{

#define GANDER_DECLAREPTR( TYPENAME ) \
typedef Gander::IntrusivePtr< TYPENAME > TYPENAME ## Ptr; \
typedef Gander::IntrusivePtr< const TYPENAME > Const ## TYPENAME ## Ptr; \

#define GANDER_DECLAREMEMBERPTR( TYPENAME ) \
		typedef Gander::IntrusivePtr< TYPENAME > Ptr; \
		typedef Gander::IntrusivePtr< const TYPENAME > ConstPtr;

/// This macro can be used when declaring member pointers in template classes with 2 template parameters,
/// where the comma in the template arguments would otherwise confuse the pre-processor
#define GANDER_DECLAREMEMBERPTR2( PART1, PART2 ) \
		typedef Gander::IntrusivePtr< PART1, PART2 > Ptr; \
		typedef Gander::IntrusivePtr< const PART1, PART2 > ConstPtr;

/// This macro can be used when declaring member pointers in template classes with 3 template parameters,
/// where the commas in the template arguments would otherwise confuse the pre-processor
#define GANDER_DECLAREMEMBERPTR3( PART1, PART2, PART3 ) \
		typedef Gander::IntrusivePtr< PART1, PART2, PART3 > Ptr; \
		typedef Gander::IntrusivePtr< const PART1, PART2, PART3 > ConstPtr;

#define GANDER_FORWARDDECLARE( TYPENAME )									\
	class TYPENAME;															\
	GANDER_DECLAREPTR( TYPENAME )											\

/// A simple class to count references.
/// \todo Disallow construction on the heap by having a private destructor - do we
/// need to do this for all derived classes as well?
/// \ingroup utilityGroup
class RefCounted : private boost::noncopyable
{
	public:

		GANDER_DECLAREMEMBERPTR( RefCounted );

		typedef size_t RefCount;

		RefCounted();

		/// Add a reference to the current object
		inline void addRef() const { m_numRefs++; };

		/// Remove a reference from the current object
		inline void removeRef() const
		{
			assert( m_numRefs > 0 );
			if( --m_numRefs==0 )
			{
				delete this;
			}
		};

		/// Returns the current reference count.
		inline RefCount refCount() const { return m_numRefs; };

	protected:

		virtual ~RefCounted();

	private :

		mutable tbb::atomic<RefCount> m_numRefs;

};

GANDER_DECLAREPTR( RefCounted )

/// Functions required to allow use of RefCounted with IntrusivePtr
inline void intrusive_ptr_add_ref( const Gander::RefCounted *r )
{
	r->addRef();
}

inline void intrusive_ptr_release( const Gander::RefCounted *r )
{
	r->removeRef();
}

} // namespace Gander


#endif // __GANDER_REFCOUNTED__

