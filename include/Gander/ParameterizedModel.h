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
#ifndef __GANDER_PARMETERIZEDMODEL_H__
#define __GANDER_PARMETERIZEDMODEL_H__

#include <vector>
#include <string>

#include "Gander/Math.h"
#include "Gander/Assert.h"
#include "Gander/StaticAssert.h"
#include "Gander/PointArray.h"

#include "Eigen/Dense"

namespace Gander
{

/// ParameterizedModel
template< class Real >
class ParameterizedModel
{
	public :
		
		typedef ParameterizedModel< Real > Type;
		GANDER_DECLARE_EIGEN_TYPES( Real )

		inline ParameterizedModel()
		{
		};

		template< class EigenType = MatrixXType >
		void addParameter( const std::string &name, const EigenType &defaultValue )
		{
			GANDER_STATIC_ASSERT( ( std::is_same< RealType, typename EigenType::Scalar >::value ), CANNOT_ASSIGN_TO_CLASSES_THAT_HAVE_A_DIFFERENT_STORAGE_TYPE );
			GANDER_ASSERT( std::find( m_parameterNames.begin(), m_parameterNames.end(), name ) == m_parameterNames.end(), "Parameter already exists." );
			
			Parameter parameter;
			parameter.rows = defaultValue.rows();
			parameter.cols = defaultValue.cols();
			parameter.firstElementIndex = m_serializedParameters.size();
			m_parameters.push_back( parameter );
			
			m_parameterNames.push_back( name );
			m_serializedParameters.conservativeResize( m_serializedParameters.size() + defaultValue.size() );
			m_serializedParameters.segment( parameter.firstElementIndex, defaultValue.size() ) = VectorXType::Map( defaultValue.data(), defaultValue.size() );
		}

		template< class EigenType = MatrixXType >
		void getParameter( const std::string &name, EigenType &parameter ) const
		{
			GANDER_STATIC_ASSERT( ( std::is_same< RealType, typename EigenType::Scalar >::value ), CANNOT_ASSIGN_TO_CLASSES_THAT_HAVE_A_DIFFERENT_STORAGE_TYPE );
			std::vector< std::string >::const_iterator it( std::find( m_parameterNames.begin(), m_parameterNames.end(), name ) );
			GANDER_ASSERT( it != m_parameterNames.end(), "Parameter does not exist." );
		
			getParameter< EigenType >( it - m_parameterNames.begin(), parameter );
		}

		template< class EigenType = MatrixXType >
		void getParameter( unsigned int index, EigenType &parameter ) const
		{
			GANDER_STATIC_ASSERT( ( std::is_same< RealType, typename EigenType::Scalar >::value ), CANNOT_ASSIGN_TO_CLASSES_THAT_HAVE_A_DIFFERENT_STORAGE_TYPE );
			GANDER_ASSERT( index < m_parameters.size(), "Index is out of bounds." );
			GANDER_ASSERT( parameter.rows() == m_parameters[index].rows && parameter.cols() == m_parameters[index].cols, "The parameter to get is a different size to the data to place it in." );

			const Parameter &p = m_parameters[index];
			parameter = EigenType::Map( &m_serializedParameters.data()[ p.firstElementIndex ], parameter.cols(), parameter.rows() );
		}
		
		template< class EigenType = MatrixXType >
		void setParameter( const std::string &name, const EigenType &parameter )
		{
			GANDER_STATIC_ASSERT( ( std::is_same< RealType, typename EigenType::Scalar >::value ), CANNOT_ASSIGN_TO_CLASSES_THAT_HAVE_A_DIFFERENT_STORAGE_TYPE );
			std::vector< std::string >::const_iterator it( std::find( m_parameterNames.begin(), m_parameterNames.end(), name ) );
			GANDER_ASSERT( it != m_parameterNames.end(), "Parameter does not exist." );
		
			setParameter< EigenType >( it - m_parameterNames.begin(), parameter );
		}

		template< class EigenType = MatrixXType >
		void setParameter( unsigned int index, const EigenType &parameter )
		{
			GANDER_STATIC_ASSERT( ( std::is_same< RealType, typename EigenType::Scalar >::value ), CANNOT_ASSIGN_TO_CLASSES_THAT_HAVE_A_DIFFERENT_STORAGE_TYPE );
			GANDER_ASSERT( index < m_parameters.size(), "Index is out of bounds." );
			GANDER_ASSERT( parameter.rows() == m_parameters[index].rows && parameter.cols() == m_parameters[index].cols, "The parameter to set is a different size to the data to place it in." );

			m_serializedParameters.segment( m_parameters[index].firstElementIndex, parameter.size() ) = VectorXType::Map( parameter.data(), parameter.size() );
		}

		const std::string &parameterName( unsigned int index ) const
		{
			GANDER_ASSERT( index < m_parameters.size(), "Index is out of bounds." );
			return m_parameterNames[index];
		}

		const VectorXType &parameters() const 
		{
			return m_serializedParameters;
		}

		VectorXType &parameters()
		{
			return m_serializedParameters;
		}

	protected :

		struct Parameter
		{
			int rows, cols;
			int firstElementIndex;
		};
	
		std::vector< std::string > m_parameterNames;
		std::vector< Parameter > m_parameters;
		VectorXType m_serializedParameters;

};

}; // namespace Gander

#endif
