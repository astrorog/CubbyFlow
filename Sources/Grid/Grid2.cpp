/*************************************************************************
> File Name: Grid2.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D cartesian grid structure.
> Created Time: 2017/07/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Grid/Grid2.h>
#include <Utils/Serial.h>

namespace CubbyFlow
{
	Grid2::Grid2()
	{
		// Do nothing
	}

	Grid2::~Grid2()
	{
		// Do nothing
	}

	const Size2& Grid2::Resolution() const
	{
		return m_resolution;
	}

	const Vector2D& Grid2::Origin() const
	{
		return m_origin;
	}

	const Vector2D& Grid2::GridSpacing() const
	{
		return m_gridSpacing;
	}

	const BoundingBox2D& Grid2::BoundingBox() const
	{
		return m_boundingBox;
	}

	Grid2::DataPositionFunc Grid2::CellCenterPosition() const
	{
		Vector2D h = m_gridSpacing;
		Vector2D o = m_origin;
		
		return [h, o](size_t i, size_t j)
		{
			return o + h * Vector2D(i + 0.5, j + 0.5);
		};
	}

	void Grid2::ForEachCellIndex(const std::function<void(size_t, size_t)>& func) const
	{
		SerialFor(
			ZERO_SIZE, m_resolution.x,
			ZERO_SIZE, m_resolution.y,
			[this, &func](size_t i, size_t j)
		{
			func(i, j);
		});
	}

	void Grid2::ParallelForEachCellIndex(const std::function<void(size_t, size_t)>& func) const
	{
		ParallelFor(
			ZERO_SIZE, m_resolution.x,
			ZERO_SIZE, m_resolution.y,
			[this, &func](size_t i, size_t j)
		{
			func(i, j);
		});
	}

	bool Grid2::HasSameShape(const Grid2& other) const
	{
		return m_resolution.x == other.m_resolution.x
			&& m_resolution.y == other.m_resolution.y
			&& Similar(m_gridSpacing.x, other.m_gridSpacing.x)
			&& Similar(m_gridSpacing.y, other.m_gridSpacing.y)
			&& Similar(m_origin.x, other.m_origin.x)
			&& Similar(m_origin.y, other.m_origin.y);
	}

	void Grid2::SetSizeParameters(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin)
	{
		m_resolution = resolution;
		m_origin = origin;
		m_gridSpacing = gridSpacing;

		Vector2D resolutionD = Vector2D(
			static_cast<double>(resolution.x),
			static_cast<double>(resolution.y));

		m_boundingBox = BoundingBox2D(origin, origin + gridSpacing * resolutionD);
	}

	void Grid2::SwapGrid(Grid2* other)
	{
		std::swap(m_resolution, other->m_resolution);
		std::swap(m_gridSpacing, other->m_gridSpacing);
		std::swap(m_origin, other->m_origin);
		std::swap(m_boundingBox, other->m_boundingBox);
	}

	void Grid2::SetGrid(const Grid2& other)
	{
		m_resolution = other.m_resolution;
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_boundingBox = other.m_boundingBox;
	}
}