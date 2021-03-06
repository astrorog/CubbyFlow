/*************************************************************************
> File Name: GridSystemData3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D grid system data.
> Created Time: 2017/08/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Grid/GridSystemData3.h>
#include <Core/Utils/Factory.h>
#include <Core/Utils/FlatbuffersHelper.h>

#include <Flatbuffers/generated/GridSystemData3_generated.h>

namespace CubbyFlow
{
	GridSystemData3::GridSystemData3() :
		GridSystemData3({ 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 })
	{
		// Do nothing
	}

	GridSystemData3::GridSystemData3(const Size3& resolution, const Vector3D& gridSpacing, const Vector3D& origin)
	{
		m_velocity = std::make_shared<FaceCenteredGrid3>();
		m_advectableVectorDataList.push_back(m_velocity);
		m_velocityIdx = 0;
		
		Resize(resolution, gridSpacing, origin);
	}

	GridSystemData3::GridSystemData3(const GridSystemData3& other)
	{
		Resize(other.m_resolution, other.m_gridSpacing, other.m_origin);

		for (auto& data : other.m_scalarDataList)
		{
			m_scalarDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_vectorDataList)
		{
			m_vectorDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_advectableScalarDataList)
		{
			m_advectableScalarDataList.push_back(data->Clone());
		}
		for (auto& data : other.m_advectableVectorDataList)
		{
			m_advectableVectorDataList.push_back(data->Clone());
		}

		assert(m_advectableVectorDataList.size() > 0);

		m_velocity = std::dynamic_pointer_cast<FaceCenteredGrid3>(m_advectableVectorDataList[0]);

		assert(m_velocity != nullptr);

		m_velocityIdx = 0;
	}

	GridSystemData3::~GridSystemData3()
	{
		// Do nothing
	}

	void GridSystemData3::Resize(const Size3& resolution, const Vector3D& gridSpacing, const Vector3D& origin)
	{
		m_resolution = resolution;
		m_gridSpacing = gridSpacing;
		m_origin = origin;

		for (auto& data : m_scalarDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_vectorDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_advectableScalarDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
		for (auto& data : m_advectableVectorDataList)
		{
			data->Resize(resolution, gridSpacing, origin);
		}
	}

	Size3 GridSystemData3::GetResolution() const
	{
		return m_resolution;
	}

	Vector3D GridSystemData3::GetGridSpacing() const
	{
		return m_gridSpacing;
	}

	Vector3D GridSystemData3::GetOrigin() const
	{
		return m_origin;
	}

	BoundingBox3D GridSystemData3::GetBoundingBox() const
	{
		return m_velocity->BoundingBox();
	}

	size_t GridSystemData3::AddScalarData(const ScalarGridBuilder3Ptr& Builder, double initialVal)
	{
		size_t attrIdx = m_scalarDataList.size();
		m_scalarDataList.push_back(Builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData3::AddVectorData(const VectorGridBuilder3Ptr& Builder, const Vector3D& initialVal)
	{
		size_t attrIdx = m_vectorDataList.size();
		m_vectorDataList.push_back(Builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData3::AddAdvectableScalarData(const ScalarGridBuilder3Ptr& Builder, double initialVal)
	{
		size_t attrIdx = m_advectableScalarDataList.size();
		m_advectableScalarDataList.push_back(Builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	size_t GridSystemData3::AddAdvectableVectorData(const VectorGridBuilder3Ptr& Builder, const Vector3D& initialVal)
	{
		size_t attrIdx = m_advectableVectorDataList.size();
		m_advectableVectorDataList.push_back(Builder->Build(GetResolution(), GetGridSpacing(), GetOrigin(), initialVal));
		return attrIdx;
	}

	const FaceCenteredGrid3Ptr& GridSystemData3::GetVelocity() const
	{
		return m_velocity;
	}

	size_t GridSystemData3::GetVelocityIndex() const
	{
		return m_velocityIdx;
	}

	const ScalarGrid3Ptr& GridSystemData3::GetScalarDataAt(size_t idx) const
	{
		return m_scalarDataList[idx];
	}

	const VectorGrid3Ptr& GridSystemData3::GetVectorDataAt(size_t idx) const
	{
		return m_vectorDataList[idx];
	}

	const ScalarGrid3Ptr& GridSystemData3::GetAdvectableScalarDataAt(size_t idx) const
	{
		return m_advectableScalarDataList[idx];
	}

	const VectorGrid3Ptr& GridSystemData3::GetAdvectableVectorDataAt(size_t idx) const
	{
		return m_advectableVectorDataList[idx];
	}

	size_t GridSystemData3::GetNumberOfScalarData() const
	{
		return m_scalarDataList.size();
	}

	size_t GridSystemData3::GetNumberOfVectorData() const
	{
		return m_vectorDataList.size();
	}

	size_t GridSystemData3::GetNumberOfAdvectableScalarData() const
	{
		return m_advectableScalarDataList.size();
	}

	size_t GridSystemData3::GetNumberOfAdvectableVectorData() const
	{
		return m_advectableVectorDataList.size();
	}

	void GridSystemData3::Serialize(std::vector<uint8_t>* buffer) const
	{
		flatbuffers::FlatBufferBuilder builder(1024);

		auto resolution = CubbyFlowToFlatbuffers(m_resolution);
		auto gridSpacing = CubbyFlowToFlatbuffers(m_gridSpacing);
		auto origin = CubbyFlowToFlatbuffers(m_origin);

		std::vector<flatbuffers::Offset<fbs::ScalarGridSerialized3>> scalarDataList;
		std::vector<flatbuffers::Offset<fbs::VectorGridSerialized3>> vectorDataList;
		std::vector<flatbuffers::Offset<fbs::ScalarGridSerialized3>> advScalarDataList;
		std::vector<flatbuffers::Offset<fbs::VectorGridSerialized3>> advVectorDataList;

		SerializeGrid(&builder, m_scalarDataList, fbs::CreateScalarGridSerialized3, &scalarDataList);
		SerializeGrid(&builder, m_vectorDataList, fbs::CreateVectorGridSerialized3, &vectorDataList);
		SerializeGrid(&builder, m_advectableScalarDataList, fbs::CreateScalarGridSerialized3, &advScalarDataList);
		SerializeGrid(&builder, m_advectableVectorDataList, fbs::CreateVectorGridSerialized3, &advVectorDataList);

		auto gsd = fbs::CreateGridSystemData3(
			builder, &resolution, &gridSpacing, &origin, m_velocityIdx,
			builder.CreateVector(scalarDataList),
			builder.CreateVector(vectorDataList),
			builder.CreateVector(advScalarDataList),
			builder.CreateVector(advVectorDataList));

		builder.Finish(gsd);

		uint8_t* buf = builder.GetBufferPointer();
		size_t size = builder.GetSize();

		buffer->resize(size);
		memcpy(buffer->data(), buf, size);
	}

	void GridSystemData3::Deserialize(const std::vector<uint8_t>& buffer)
	{
		auto gsd = fbs::GetGridSystemData3(buffer.data());

		Resize(
			FlatbuffersToCubbyFlow(*gsd->resolution()),
			FlatbuffersToCubbyFlow(*gsd->gridSpacing()),
			FlatbuffersToCubbyFlow(*gsd->origin()));

		m_scalarDataList.clear();
		m_vectorDataList.clear();
		m_advectableScalarDataList.clear();
		m_advectableVectorDataList.clear();

		DeserializeGrid(gsd->scalarData(), Factory::BuildScalarGrid3, &m_scalarDataList);
		DeserializeGrid(gsd->vectorData(), Factory::BuildVectorGrid3, &m_vectorDataList);
		DeserializeGrid(gsd->advectableScalarData(), Factory::BuildScalarGrid3, &m_advectableScalarDataList);
		DeserializeGrid(gsd->advectableVectorData(), Factory::BuildVectorGrid3, &m_advectableVectorDataList);

		m_velocityIdx = static_cast<size_t>(gsd->velocityIdx());
		m_velocity = std::dynamic_pointer_cast<FaceCenteredGrid3>(m_advectableVectorDataList[m_velocityIdx]);
	}
}