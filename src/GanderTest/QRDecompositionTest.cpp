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

#include <iostream>

#include "GanderTest/QRDecompositionTest.h"
#include "boost/test/floating_point_comparison.hpp"
#include "boost/test/test_tools.hpp"

#include "Gander/Common.h"

#include "Eigen/Dense"

using namespace Gander;
using namespace Gander::Test;
using namespace boost;
using namespace boost::unit_test;

#if 0

CV_IMPL void
cvRQDecomp3x3( const CvMat *matrixM, CvMat *matrixR, CvMat *matrixQ,
               CvMat *matrixQx, CvMat *matrixQy, CvMat *matrixQz,
               CvPoint3D64f *eulerAngles)
{
    double matM[3,3], R[3,3], matQ[3,3];
    CvMat M = cvMat(3, 3, CV_64F, matM);
    CvMat R = cvMat(3, 3, CV_64F, R);
    CvMat Q = cvMat(3, 3, CV_64F, matQ);
    double z, c, s;

    /* Validate parameters. */
    CV_Assert( CV_IS_MAT(matrixM) && CV_IS_MAT(matrixR) && CV_IS_MAT(matrixQ) &&
        matrixM->cols == 3 && matrixM->rows == 3 &&
        CV_ARE_SIZES_EQ(matrixM, matrixR) && CV_ARE_SIZES_EQ(matrixM, matrixQ));

    cvConvert(matrixM, &M);

    /* Find Givens rotation Q_x for x axis (left multiplication). */
    /*
         ( 1  0  0 )
    Qx = ( 0  c  s ), c = m33/sqrt(m32^2 + m33^2), s = m32/sqrt(m32^2 + m33^2)
         ( 0 -s  c )
    */
    s = matM[2,1];
    c = matM[2,2];
    z = 1./sqrt(c * c + s * s + std::numeric_limits<double>::epsilon());
    c *= z;
    s *= z;

    double _Qx[3,3] = { {1, 0, 0}, {0, c, s}, {0, -s, c} };
    CvMat Qx = cvMat(3, 3, CV_64F, _Qx);

    cvMatMul(&M, &Qx, &R);
    assert(fabs(R[2,1]) < std::numeric_limits<float>::epsilon());
    R[2,1] = 0;

    /* Find Givens rotation for y axis. */
    /*
         ( c  0 -s )
    Qy = ( 0  1  0 ), c = m33/sqrt(m31^2 + m33^2), s = -m31/sqrt(m31^2 + m33^2)
         ( s  0  c )
    */
    s = -R[2,0];
    c = R[2,2];
    z = 1./sqrt(c * c + s * s + std::numeric_limits<double>::epsilon());
    c *= z;
    s *= z;

    double _Qy[3,3] = { {c, 0, -s}, {0, 1, 0}, {s, 0, c} };
    CvMat Qy = cvMat(3, 3, CV_64F, _Qy);
    cvMatMul(&R, &Qy, &M);

    assert(fabs(matM[2,0]) < std::numeric_limits<float>::epsilon());
    matM[2,0] = 0;

    /* Find Givens rotation for z axis. */
    /*
         ( c  s  0 )
    Qz = (-s  c  0 ), c = m22/sqrt(m21^2 + m22^2), s = m21/sqrt(m21^2 + m22^2)
         ( 0  0  1 )
    */

    s = matM[1,0];
    c = matM[1,1];
    z = 1./sqrt(c * c + s * s + std::numeric_limits<double>::epsilon());
    c *= z;
    s *= z;

    double _Qz[3,3] = { {c, s, 0}, {-s, c, 0}, {0, 0, 1} };
    CvMat Qz = cvMat(3, 3, CV_64F, _Qz);

    cvMatMul(&M, &Qz, &R);
    assert(fabs(R[1,0]) < std::numeric_limits<float>::epsilon());
    R[1,0] = 0;

    // Solve the decomposition ambiguity.
    // Diagonal entries of R, except the last one, shall be positive.
    // Further rotate R by 180 degree if necessary
    if( R[0,0] < 0 )
    {
        if( R[1,1] < 0 )
        {
            // rotate around z for 180 degree, i.e. a rotation matrix of
            // [-1,  0,  0],
            // [ 0, -1,  0],
            // [ 0,  0,  1]
            R[0,0] *= -1;
            R[0,1] *= -1;
            R[1,1] *= -1;

            _Qz[0,0] *= -1;
            _Qz[0,1] *= -1;
            _Qz[1,0] *= -1;
            _Qz[1,1] *= -1;
        }
        else
        {
            // rotate around y for 180 degree, i.e. a rotation matrix of
            // [-1,  0,  0],
            // [ 0,  1,  0],
            // [ 0,  0, -1]
            R[0,0] *= -1;
            R[0,2] *= -1;
            R[1,2] *= -1;
            R[2,2] *= -1;

            cvTranspose( &Qz, &Qz );

            _Qy[0,0] *= -1;
            _Qy[0,2] *= -1;
            _Qy[2,0] *= -1;
            _Qy[2,2] *= -1;
        }
    }
    else if( R[1,1] < 0 )
    {
        // ??? for some reason, we never get here ???

        // rotate around x for 180 degree, i.e. a rotation matrix of
        // [ 1,  0,  0],
        // [ 0, -1,  0],
        // [ 0,  0, -1]
        R[0,1] *= -1;
        R[0,2] *= -1;
        R[1,1] *= -1;
        R[1,2] *= -1;
        R[2,2] *= -1;

        cvTranspose( &Qz, &Qz );
        cvTranspose( &Qy, &Qy );

        _Qx[1,1] *= -1;
        _Qx[1,2] *= -1;
        _Qx[2,1] *= -1;
        _Qx[2,2] *= -1;
    }

    // calculate the euler angle
    if( eulerAngles )
    {
        eulerAngles->x = acos(_Qx[1,1]) * (_Qx[1,2] >= 0 ? 1 : -1) * (180.0 / CV_PI);
        eulerAngles->y = acos(_Qy[0,0]) * (_Qy[2,0] >= 0 ? 1 : -1) * (180.0 / CV_PI);
        eulerAngles->z = acos(_Qz[0,0]) * (_Qz[0,1] >= 0 ? 1 : -1) * (180.0 / CV_PI);
    }

    /* Calulate orthogonal matrix. */
    /*
    Q = QzT * QyT * QxT
    */
    cvGEMM( &Qz, &Qy, 1, 0, 0, &M, CV_GEMM_A_T + CV_GEMM_B_T );
    cvGEMM( &M, &Qx, 1, 0, 0, &Q, CV_GEMM_B_T );

    /* Save R and Q matrices. */
    cvConvert( &R, matrixR );
    cvConvert( &Q, matrixQ );

    if( matrixQx )
        cvConvert(&Qx, matrixQx);
    if( matrixQy )
        cvConvert(&Qy, matrixQy);
    if( matrixQz )
        cvConvert(&Qz, matrixQz);
}

#endif


void givensDecomposeQR3x3( Eigen::Matrix3d M, Eigen::Matrix3d &R, Eigen::Matrix3d &Q, Eigen::Matrix3d &Qx, Eigen::Matrix3d &Qy, Eigen::Matrix3d &Qz )
{
    double z, c, s;
	
	// Find Givens rotation Qx for x axis (left multiplication).
    //
    //      ( 1  0  0 )
    // Qx = ( 0  c -s ), c = m33/sqrt(m32^2 + m33^2), s = m32/sqrt(m32^2 + m33^2)
	//      ( 0  s  c )
	//
	s = M(2,1);
    c = M(2,2);
    z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
    c *= z;
    s *= z;
	Qx << 1, 0, 0, 0, c, s, 0, -s, c;

	R = M * Qx;	
	BOOST_ASSERT( fabs( R(2,1) ) < std::numeric_limits<double>::epsilon() );
    R(2,1) = 0.;
	
    // Find Givens rotation for y axis.
    //
    //      ( c  0  s )
    // Qy = ( 0  1  0 ), c = m33/sqrt(m31^2 + m33^2), s = -m31/sqrt(m31^2 + m33^2)
    //      (-s  0  c )
    //
    s = -R(0,2);
    c = R(2,2);
    z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
    c *= z;
    s *= z;

	Qy << c, 0, s, 0, 1, 0, -s, 0, c;
	M = R * Qy;

    BOOST_ASSERT( fabs( M(2,0) ) < std::numeric_limits<float>::epsilon() );
    M(0,2) = 0;

    // Find Givens rotation for z axis.
    //
    //      ( c -s  0 )
    // Qz = ( s  c  0 ), c = m22/sqrt(m21^2 + m22^2), s = m21/sqrt(m21^2 + m22^2)
    //      ( 0  0  1 )
    //
    s = M(0,1);
    c = M(1,1);
    z = 1. / sqrt( c * c + s * s + std::numeric_limits<double>::epsilon() );
    c *= z;
    s *= z;
    
	Qz << c, -s, 0, s, c, 0, 0, 0, 1;
	R = M * Qz;
    
	BOOST_ASSERT( fabs( R(0,1) ) < std::numeric_limits<float>::epsilon() );
    R(0,1) = 0.;

	std::cerr << "--------------------- " << std::endl;
    std::cerr << R << std::endl;
	// Solve the decomposition ambiguity.
    // Diagonal entries of R, except the last one, shall be positive.
    // Further rotate R by 180 degree if necessary
    if( R(0,0) < 0 )
    {
        if( R(1,1) < 0 )
        {
            // Rotate around z by 180 degrees.
			// I.E. A rotation matrix of:
            // [-1,  0,  0],
            // [ 0, -1,  0],
            // [ 0,  0,  1]
            R(0,0) *= -1;
            R(1,0) *= -1;
            R(1,1) *= -1;

            Qz(0,0) *= -1;
            Qz(1,0) *= -1;
            Qz(0,1) *= -1;
            Qz(1,1) *= -1;
        }
        else
        {
            // Rotate around Y by 180 degrees.
			// I.E. A rotation matrix of:
            // [-1,  0,  0],
            // [ 0,  1,  0],
            // [ 0,  0, -1]
            R(0,0) *= -1;
            R(2,0) *= -1;
            R(2,1) *= -1;
            R(2,2) *= -1;

			Qz.transpose();

            Qy(0,0) *= -1;
            Qy(2,0) *= -1;
            Qy(0,2) *= -1;
            Qy(2,2) *= -1;
        }
    }
    else if( R(1,1) < 0 )
    {
        // Rotate around X by 180 degrees.
		// I.E. A rotation matrix of:
        // [ 1,  0,  0],
        // [ 0, -1,  0],
        // [ 0,  0, -1]
        R(1,0) *= -1;
        R(2,0) *= -1;
        R(1,1) *= -1;
        R(2,1) *= -1;
        R(2,2) *= -1;

		Qy.transpose();
		Qz.transpose();

        Qx(1,1) *= -1;
        Qx(2,1) *= -1;
        Qx(1,2) *= -1;
        Qx(2,2) *= -1;
    }

    // calculate the euler angle
        std::cerr << acos( Qx(1,1) ) * ( Qx(2,1) >= 0 ? 1 : -1) * (180.0 / M_PI) << std::endl;
		std::cerr << acos( Qy(0,0) ) * ( Qy(0,2) >= 0 ? 1 : -1 ) * (180.0 / M_PI) << std::endl;
        std::cerr << acos( Qz(0,0) ) * ( Qz(1,0) >= 0 ? 1 : -1 ) * (180.0 / M_PI) << std::endl;

/*
    // Calulate orthogonal matrix.
    // Q = QzT * QyT * QxT
    cvGEMM( &Qz, &Qy, 1, 0, 0, &M, CV_GEMM_A_T + CV_GEMM_B_T );
    cvGEMM( &M, &Qx, 1, 0, 0, &Q, CV_GEMM_B_T );

    // Save R and Q matrices.
    cvConvert( &R, matrixR );
    cvConvert( &Q, matrixQ );

		*/
}

namespace Gander
{

namespace Test
{

struct QRDecompositionTest
{
	/// A simple test that demonstrates the use of the EigenQR module.
	/// QR decomposition is used throughout Gander but is most commonly
	/// used for decomposing a pose matrix into the calibration and 
	/// transformation components.
	void testHouseholderQR()
	{
		try
		{
			Eigen::Matrix3d C;
			C << 1.94444, 0, 0, 0, 2.1875, 0, 0.0141111, 0.127, 1.;
		
			Eigen::Matrix3d rx;
			rx = Eigen::AngleAxisd( 50*(M_PI/180.), Eigen::Vector3d( 0,0,1 ) );
			Eigen::Matrix3d ry;
			ry = Eigen::AngleAxisd( 90*(M_PI/180.), Eigen::Vector3d( 0,1,0 ) );
			Eigen::Matrix3d rz;
			rz = Eigen::AngleAxisd( 270*(M_PI/180.), Eigen::Vector3d( 1, 0, 0) );
			Eigen::Matrix3d rotation = rz * ry * rx;
				
			Eigen::Vector3d translation;
			translation << 1, 2, 3;

			Eigen::MatrixXd proj( 3, 4 );
			proj << C*rotation, translation;
		
		{
		
			Eigen::Matrix3d A;
			A << 12, -51, 4, 6, 167, -68, -4, 24, -41;
			Eigen::Matrix3d R;
			Eigen::Matrix3d Q;
			Eigen::Matrix3d Qx, Qy, Qz;
			givensDecomposeQR3x3( A, R, Q, Qx, Qy, Qz );
			//std::cerr << "R1\n" << rotation << "\n";
			//std::cerr << "R2\n" << R << "\n";
			std::cerr << "Qx\n" << Qx << "\n";
			std::cerr << "Ry\n" << ry << "\n";
			std::cerr << "Qy\n" << Qy << "\n";
			std::cerr << "Rz\n" << rz << "\n";
			std::cerr << "Qz\n" << Qz << "\n";
}
//			std::cerr << translation << std::endl;
//			std::cerr << proj << std::endl;
			
			Eigen::Matrix3d Q = proj.block( 0, 0, 3, 3 ).inverse();
//			std::cerr << "\nQ:\n" << Q << "\n";
			Eigen::ColPivHouseholderQR< Eigen::Matrix3d > qr( Q );
			Eigen::Matrix3d B = qr.matrixQR().triangularView< Eigen::Upper >();
			Eigen::Vector3d t = B*proj.col(3).head(3);
			
			Eigen::Matrix3d R = qr.matrixQ().inverse();
			Eigen::Matrix3d A = B.inverse();
//			std::cerr << "\nB:\n" << B << "\n";
//			std::cerr << "\nA:\n" << A << "\n";
//			A /= A( 2, 2 );
			std::cerr << "\nR1:\n" << rotation << "\n";
			std::cerr << "\nR2:\n" << R << "\n";
			Eigen::Vector3d p( 0, 1, 0 );
			std::cerr << rotation * p << std::endl << std::endl;
//			std::cerr << "\n" << A << "\n";
//			std::cerr << "\n" << t << "\n";
		
//			std::cerr << "A*R\n" << A*R << "\n";
//			std::cerr << "\nP:\n" << proj << "\n";
		}
		catch ( std::exception &e ) 
		{
			BOOST_WARN( !e.what() );
			BOOST_CHECK( !"Exception thrown during QRDecompositionTest." );
		}
	}
};

struct QRDecompositionTestSuite : public boost::unit_test::test_suite
{

	QRDecompositionTestSuite() : boost::unit_test::test_suite( "QRDecompositionTestSuite" )
	{
		boost::shared_ptr<QRDecompositionTest> instance( new QRDecompositionTest() );
		add( BOOST_CLASS_TEST_CASE( &QRDecompositionTest::testHouseholderQR, instance ) );
	}
};

void addQRDecompositionTest( boost::unit_test::test_suite *test )
{
	test->add( new QRDecompositionTestSuite( ) );
}

} // namespace Test

} // namespace Gander

