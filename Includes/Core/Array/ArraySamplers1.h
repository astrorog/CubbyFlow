/*************************************************************************
> File Name: ArraySamplers1.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 1-D nearest array sampler class.
> Created Time: 2017/04/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_SAMPLERS1_H
#define CUBBYFLOW_ARRAY_SAMPLERS1_H

#include <Core/Array/ArrayAccessor1.h>
#include <Core/Array/ArraySamplers.h>

#include <functional>

namespace CubbyFlow
{
	//!
	//! \brief 1-D nearest array sampler class.
	//!
	//! This class provides nearest sampling interface for a given 1-D array.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//!
	template <typename T, typename R>
	class NearestArraySampler<T, R, 1> final
	{
	public:
		static_assert(std::is_floating_point<R>::value, "Samplers only can be instantiated with floating point types");

		//!
		//! \brief      Constructs a sampler using array accessor, spacing between
		//!     the elements, and the position of the first array element.
		//!
		//! \param[in]  accessor    The array accessor.
		//! \param[in]  gridSpacing The grid spacing.
		//! \param[in]  gridOrigin  The grid origin.
		//!
		explicit NearestArraySampler(
			const ConstArrayAccessor1<T>& accessor,
			R gridSpacing,
			R gridOrigin);

		//! Copy constructor.
		NearestArraySampler(const NearestArraySampler& other);

		//! Returns sampled value at point \p pt.
		T operator()(R pt) const;

		//! Returns the nearest array index for point \p pt.
		void GetCoordinate(R pt, size_t* i) const;

		//! Returns a function object that wraps this instance.
		std::function<T(R)> Functor() const;

	private:
		R m_gridSpacing;
		R m_origin;
		ConstArrayAccessor1<T> m_accessor;
	};

	//! Type alias for 1-D nearest array sampler.
	template <typename T, typename R>
	using NearestArraySampler1 = NearestArraySampler<T, R, 1>;
	
	//!
	//! \brief 1-D linear array sampler class.
	//!
	//! This class provides linear sampling interface for a given 1-D array.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//!
	template <typename T, typename R>
	class LinearArraySampler<T, R, 1> final
	{
	public:
		static_assert(std::is_floating_point<R>::value, "Samplers only can be instantiated with floating point types");

		//!
		//! \brief      Constructs a sampler using array accessor, spacing between
		//!     the elements, and the position of the first array element.
		//!
		//! \param[in]  accessor    The array accessor.
		//! \param[in]  gridSpacing The grid spacing.
		//! \param[in]  gridOrigin  The grid origin.
		//!
		explicit LinearArraySampler(
			const ConstArrayAccessor1<T>& accessor,
			R gridSpacing,
			R gridOrigin);

		//! Copy constructor.
		LinearArraySampler(const LinearArraySampler& other);

		//! Returns sampled value at point \p pt.
		T operator()(R pt) const;

		//! Returns the indices of points and their sampling weight for given point.
		void GetCoordinatesAndWeights(R pt, size_t* i0, size_t* i1, T* weight0, T* weight1) const;

		//! Returns a function object that wraps this instance.
		std::function<T(R)> Functor() const;

	private:
		R m_gridSpacing;
		R m_origin;
		ConstArrayAccessor1<T> m_accessor;
	};

	//! Type alias for 1-D linear array sampler.
	template <typename T, typename R>
	using LinearArraySampler1 = LinearArraySampler<T, R, 1>;
	
	//!
	//! \brief 1-D cubic array sampler class.
	//!
	//! This class provides cubic sampling interface for a given 1-D array.
	//!
	//! \tparam T - The value type to sample.
	//! \tparam R - The real number type.
	//!
	template <typename T, typename R>
	class CubicArraySampler<T, R, 1> final
	{
	public:
		static_assert(std::is_floating_point<R>::value, "Samplers only can be instantiated with floating point types");

		//!
		//! \brief      Constructs a sampler using array accessor, spacing between
		//!     the elements, and the position of the first array element.
		//!
		//! \param[in]  accessor    The array accessor.
		//! \param[in]  gridSpacing The grid spacing.
		//! \param[in]  gridOrigin  The grid origin.
		//!
		explicit CubicArraySampler(
			const ConstArrayAccessor1<T>& accessor,
			R gridSpacing,
			R gridOrigin);

		//! Copy constructor.
		CubicArraySampler(const CubicArraySampler& other);

		//! Returns sampled value at point \p pt.
		T operator()(R pt) const;

		//! Returns a function object that wraps this instance.
		std::function<T(R)> Functor() const;

	private:
		R m_gridSpacing;
		R m_origin;
		ConstArrayAccessor1<T> m_accessor;
	};

	//! Type alias for 1-D cubic array sampler.
	template <typename T, typename R>
	using CubicArraySampler1 = CubicArraySampler<T, R, 1>;
}

#include <Core/Array/ArraySamplers1-Impl.h>

#endif