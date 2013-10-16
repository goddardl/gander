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
#ifndef __GANDER_RANSAC_H__
#define __GANDER_RANSAC_H__

#include <vector>
#include "Eigen/Eigen"
#include "Eigen/Dense"
#include "Eigen/Core"

namespace Gander
{

class RANSAC
{

public:

	RANSAC( int modelPoints, int maxBasicSolutions );
    virtual ~RANSAC();
	
	/// Derived classes should implement this method to produce a model matrix. If multiple model matrices are produced then the number of rows of the model matrix should be 
	/// multiplied accordingly so that numberOfRows = modelRowSize*numberOfModels, the models stacked on top of each other and the number of models returnsed. For example, if
	/// the model is a 3x3 matrix and 2 models are created then the size of the model matrix should be (3*2, 3) such that a model 'X' can be returned by accessing a block of
	/// model:
	/// modelX = model.block( x * model.rows(), 0, model.rows(), model.cols() ).
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be estimated.
	/// @return The number of models that were created and stored within 'model'.
	virtual int runKernel( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model ) = 0;
	/// Runs the RANSAC algorithm.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be estimated.
	/// @return Whether the estimator was successful or not.
	virtual bool run( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model,
		std::vector<bool> &initialMask, double threshold, double confidence = 0.99, int maxIters = 2000 );
	/// Derived classes should override this method to refine the result of the computed model and return true if it was successful.
	/// A common use for this method would be to implement a non-linear minimizer such as Lenvenberg-Marquardt to reduce the error.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be refined.
	/// @param maxIters The maximum number of iterations to perform when minimizing the error of the model.
    virtual bool refine( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model, int maxIters ) { return true; }
	/// Sets the seed of the random sampler which selects points.
	/// @param seed The value to set the seed to.
    virtual void setSeed( int seed );
	/// Gets the seed of the random sampler which selects points.
	/// @return The seed.
    virtual int getSeed() const;

protected:

	virtual double computeReprojError( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &model, Eigen::VectorXd &error ) = 0;
	virtual int findInliers( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &model,
		Eigen::VectorXd &error, std::vector<bool> &mask, double threshold );
	virtual bool getSubset( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &pointsSample1, 
		Eigen::MatrixXd &pointsSample2, int maxAttempts = 1000 );
	virtual bool checkSubset( const Eigen::MatrixXd &pointSamples, int nSamples );

private:
	
	double round( double num ) const;
	int random( int maxValue ) const;
	int updateNumberOfIterations( double p, double ep, int model_points, int max_iters );

    int m_seed;
    int m_modelPoints; // The number of points to sample for the model.
    int m_maxBasicSolutions;

};

}; // namespace Gander

#endif
