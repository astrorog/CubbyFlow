/*************************************************************************
> File Name: BLAS.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic BLAS operator wrapper class.
> Created Time: 2017/08/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BLAS_H
#define CUBBYFLOW_BLAS_H

namespace CubbyFlow
{
	//!
	//! \brief Generic BLAS operator wrapper class
	//!
	//! This class provides BLAS (Basic Linear Algebra Subprograms)-like set of
	//! operators for vector and matrix class. By default, it supports Vector<T, 2>,
	//! Vector<T, 3>, Vector<T, 4>, Matrix<T, 2, 2>, Matrix<T, 3, 3> and
	//! Matrix<T, 4, 4>. For custom vector/matrix classes, create a new BLAS class
	//! that conforms the function interfaces defined in this class. It will enable
	//! performing linear algebra routines (such as conjugate gradient) for the
	//! custom vector/matrix types.
	//!
	template <typename S, typename V, typename M>
	struct BLAS
	{
		using ScalarType = S;
		using VectorType = V;
		using MatrixType = M;

		//! Sets entire element of given vector \p result with scalar \p s.
		static void Set(ScalarType s, VectorType* result);

		//! Copies entire element of given vector \p result with other vector \p v.
		static void Set(const VectorType& v, VectorType* result);

		//! Sets entire element of given matrix \p result with scalar \p s.
		static void Set(ScalarType s, MatrixType* result);

		//! Copies entire element of given matrix \p result with other matrix \p v.
		static void Set(const MatrixType& m, MatrixType* result);

		//! Performs dot product with vector \p a and \p b.
		static ScalarType Dot(const VectorType& a, const VectorType& b);

		//! Performs ax + y operation where \p a is a matrix and \p x and \p y are vectors.
		static void AXPlusY(ScalarType a, const VectorType& x, const VectorType& y, VectorType* result);

		//! Performs matrix-vector multiplication.
		static void MVM(const MatrixType& m, const VectorType& v, VectorType* result);

		//! Computes residual vector (b - ax).
		static void Residual(const MatrixType& a, const VectorType& x, const VectorType& b, VectorType* result);

		//! Returns L2-norm of the given vector \p v.
		static ScalarType L2Norm(const VectorType& v);

		//! Returns L-inf-norm of the given vector \p v.
		static ScalarType LInfNorm(const VectorType& v);
	};
}

#include <Math/BLAS-Impl.h>

#endif