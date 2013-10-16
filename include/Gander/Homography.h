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
#ifndef __GANDER_HOMOGRAPHY_H__
#define __GANDER_HOMOGRAPHY_H__

#include <vector>

#include "Gander/Math.h"

namespace Gander
{

/// Builds a homography that transforms a set of 2D points from one to the other.
/// Two matrices of 4 points must be supplied where each column is a point and each row a component of the point.
/// The point matrices must be of the same size with each column a pair of corresponding points. E.G: point1.col(x) maps to point2.col(x).
/// A minimum of 4 points must be supplied. If more than 4 points are supplied, the homography is refined using Levenberg-Marquardt.
/// @param points1 The matrix of 2D points in the first image.
/// @param points2 The matrix of 2D points in the second image.
/// @param H The computed homography.
/// @param reprojectionErrorThreshold The reprojection error below which a point can be classified as an inlier.
/// @return Whether the homography was successful or not.
bool computePlaneToPlaneHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H,
	std::vector<bool> &mask, double reprojectionErrorThreshold = 1e-5 );

/// Minimizes the error of a homography transform using Levenberg-Merquardt.
/// The algorithm is intolerant to outliers.
/// @param points1 The list of 2D points in the first image.
/// @param points2 The list of 2D points in the second image.
/// @param H The input and output homography to refine.
/// @return Whether the refinement was successful or not.
bool refineHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H, int maxIters );

/// Sets 'error' to a list of error metrics, one for each point correspondence and returns the average error.
double computePlaneToPlaneHomographyError( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, const Eigen::MatrixXd &H, Eigen::VectorXd &error );

/// Builds a homography that transforms a set of 2D points from one to the other.
/// Two matrices of 4 points must be supplied where each column is a point and each row a component of the point.
/// The point matrices must be of the same size with each column a pair of corresponding points. E.G: point1.col(x) maps to point2.col(x).
/// @param points1 The matrix of 2D points in the first image.
/// @param points2 The matrix of 2D points in the second image.
/// @param H The computed homography.
/// @return Whether the homography was successful or not.
bool compute4PointPlaneToPlaneHomography( const Eigen::MatrixXd &points1, const Eigen::MatrixXd &points2, Eigen::MatrixXd &H );

}; // namespace Gander

#endif
