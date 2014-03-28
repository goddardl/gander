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

#include "Gander/Math.h"

namespace Gander
{

/// The input to the RANSAC algorithm is a set of observed data values, a parameterized model which can explain or be fitted to the observations,
/// and some confidence parameters.
/// RANSAC achieves its goal by iteratively selecting a random subset of the original data. These data are hypothetical inliers and this hypothesis is then tested as follows:
/// 1) A model is fitted to the sample of hypothetical inliers, i.e. all free parameters of the model are fitted from this sample.
/// 2) All other data are then tested against the fitted model and, those points that fit the estimated model well are considered as part of the consensus set.
/// 3) The estimated model is reasonably good if sufficiently many points have been classified as part of the consensus set.
/// 4) Afterwards, the model may be improved by reestimating it using all members of the consensus set.
/// 5) This procedure is repeated a fixed number of times, each time producing either a model which is rejected because too few points are part of the consensus set, 
///    or a refined model together with a corresponding consensus set size. In the latter case, we keep the refined model if its consensus set is larger than the
///    previously saved model.
class RANSAC
{

public:
	
	/// Basic constructor.
	/// @param modelPoints The number of points required by the model. 
	/// @param maxBasicSolutions The number of models given to test.
	RANSAC( int modelPoints, int maxBasicSolutions );
    virtual ~RANSAC();

	/// Runs the RANSAC algorithm.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model(s) to be estimated.
	/// @param initialMask An optional mask vector which should be the same size as the number of points if used. It should indicate whether a
	///                    set of points should be considered an inlier. This vector will be populated by the RANSAC algorithm with the resulting inliers.  
	/// @return Whether the estimator was successful or not.
	bool operator()( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model, std::vector<bool> &initialMask );
	/// Derived classes should override this method to refine the result of the computed model and return true if it was successful.
	/// A common use for this method would be to implement a non-linear minimizer such as Lenvenberg-Marquardt to reduce the error.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model(s) to be refined.
	/// @param maxIters The maximum number of iterations to perform when minimizing the error of the model.
    virtual bool refine( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model, int maxIters ) { return true; }
	
	//! @name Parameter Accessors
	/// The setter and getter methods for the RANSAC algorithm.
	//////////////////////////////////////////////////////////////
	//@{
	/// Sets the seed of the random sampler which selects points.
	/// @param seed The value to set the seed to.
    inline void setSeed( int seed ) { m_seed = seed; }
	/// Gets the seed of the random sampler which selects points.
	/// @return The seed.
    inline int getSeed() const { return m_seed; }
    /// Sets the maximum number of iterations of the RANSAC algorithm.
	/// @param iterations The maximum number of iterations to set.
	void setMaxIterations( int iterations ) { m_maxIters = iterations; };
	/// Returns the maximum number of iterations that the RANSAC algorithm will be run.
	/// @return The maximum number of iterations.
    int getMaxIteration() const { return m_maxIters; }
    /// Sets the maximum number of iterations to be passed to the refinement algorithm upon completion of the first stage of RANSAC.
	/// @param iterations The maximum number of iterations to be passed to the refinement algorithm.
    void setMaxRefineIterations( int iterations ) { m_maxRefineIters = iterations; };
	/// Returns the maximum number of iterations that the refinement algorithm will be run for.
	/// @return The maximum number of iterations.
    int getMaxRefineIteration() const { return m_maxRefineIters; }
    void setConfidence( double confidence ) { m_confidence = confidence; };
    double getConfidence() const { return m_confidence; }
    /// Sets the threshold used to define when the datum (the points) are considered inliers of the model.
	/// @param threshold The threshold to set.
	void setThreshold( double threshold ) { m_threshold = threshold; };
	/// Returns the threshold that is used to define when the datum (the points) are considered inliers of the model.
	/// @return The threshold.
    double getThreshold() const { return m_threshold; }
	//@}
	
protected:
	
	/// Derived classes should implement this method to produce a model matrix. If multiple model matrices are produced then the number of rows of the model matrix should be 
	/// multiplied accordingly so that numberOfRows = modelRowSize*numberOfModels with the models stacked on top of each other and the number of models returned.
	/// For example, if the model is a 3x3 matrix and 2 models are created then the size of the model matrix should be (3*2, 3) such that a model 'X' can be returned
	/// by accessing a block of model:
	/// modelX = model.block( x * model.rows(), 0, model.rows(), model.cols() ).
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be estimated.
	/// @return The number of models that were created and stored within 'model'.
	virtual int runKernel( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &model ) = 0;
	/// Should be implemented by derived classes to calculate the error of the given model(s) with the set of points. 
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be tested.
	/// @param error A vector of error values, one for each pair of points. 
	/// @return The average error of the model.
	virtual double computeModelError( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &model, Eigen::VectorXd &error ) = 0;
	/// Aquires a subset of points at random, checks them for validity and returns them.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row from which to sample.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row from which to sample.
	/// @param pointsSample1 The first returned subset of points. 
	/// @param pointsSample2 The second returned subset of points.
	/// @param maxAttempts The number of tries at acquiring a good sample set before it fails.
	/// @return Whether the aquisition was successful.
	virtual bool subset( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &pointsSample1, 
			Eigen::MatrixXd &pointsSample2, unsigned int maxAttempts = 1000 );
	/// Returns the number of inliers from the point sets using the specified model. A vector of the errors for each pair of points is also returned
	/// along with the average error and a vector of masks that indicate whether the corresponding pair of points are inliers or not.
	/// @param points1 The first matrix of points stored in column major order with each point stored in a row.
	/// @param points2 The second matrix of points stored in column major order with each point stored in a row.
	/// @param model The model to be tested.
	/// @param error A vector of error values, one for each pair of points. 
	/// @param avgError The average error.
	/// @param mask A vector of flags, one per pair of points, indicating whether the corresponding pair of points are inliers to the given model.
	/// @return The number of inliers.
	virtual int findInliers( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &model,
			Eigen::VectorXd &error, double &avgError, std::vector<bool> &mask );

private:
	
	inline double round( double num ) const { return (num > 0.0) ? floor( num + 0.5 ) : ceil( num - 0.5 ); }
	inline int random( int maxValue ) const { return rand() % maxValue; }
	
	/// Returns the validity of a chosen set of sample points.
	virtual bool checkSubset( const Eigen::MatrixXd &pointSamples, unsigned int nSamples );
	/// Updates the number of iterations to use.
	int updateNumberOfIterations( double ep, int max_iters );

    int m_seed;
    unsigned int m_modelPoints; // The number of points to sample for the model.
    int m_maxBasicSolutions;
	int m_nInliers;
	double m_confidence;
	int m_maxIters;
	int m_maxRefineIters;
	double m_threshold;

};

}; // namespace Gander

#endif
