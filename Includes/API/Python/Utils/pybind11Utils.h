/*************************************************************************
> File Name: pybind11Utils.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: pybind11 utility functions for CubbyFlow Python API.
> Created Time: 2018/01/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_PYBIND11_UTILS_H
#define CUBBYFLOW_PYTHON_PYBIND11_UTILS_H

#include <Core/Math/Quaternion.h>
#include <Core/Point/Point2.h>
#include <Core/Point/Point3.h>
#include <Core/Size/Size2.h>
#include <Core/Size/Size3.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace CubbyFlow
{
	inline Size2 TupleToSize2(pybind11::tuple tuple)
	{
		Size2 ret;

		if (tuple.size() == 2)
		{
			for (size_t i = 0; i < 2; ++i)
			{
				ret[i] = tuple[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Size2 TupleToSize2(pybind11::list list)
	{
		Size2 ret;

		if (list.size() == 2)
		{
			for (size_t i = 0; i < 2; ++i)
			{
				ret[i] = list[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Size3 TupleToSize3(pybind11::tuple tuple)
	{
		Size3 ret;

		if (tuple.size() == 3)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				ret[i] = tuple[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Size3 TupleToSize3(pybind11::list list)
	{
		Size3 ret;

		if (list.size() == 3)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				ret[i] = list[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Point2UI TupleToPoint2UI(pybind11::tuple tuple)
	{
		Point2UI ret;

		if (tuple.size() == 2)
		{
			for (size_t i = 0; i < 2; ++i)
			{
				ret[i] = tuple[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Point2UI TupleToPoint2UI(pybind11::list list)
	{
		Point2UI ret;

		if (list.size() == 2)
		{
			for (size_t i = 0; i < 2; ++i)
			{
				ret[i] = list[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Point3UI TupleToPoint3UI(pybind11::tuple tuple)
	{
		Point3UI ret;

		if (tuple.size() == 3)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				ret[i] = tuple[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	inline Point3UI TupleToPoint3UI(pybind11::list list)
	{
		Point3UI ret;

		if (list.size() == 3)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				ret[i] = list[i].cast<size_t>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return {};
	}

	inline pybind11::tuple Size2ToTuple(const Size2& size)
	{
		return pybind11::make_tuple(size.x, size.y);
	}

	inline pybind11::tuple Size3ToTuple(const Size3& size)
	{
		return pybind11::make_tuple(size.x, size.y, size.z);
	}

	template <typename T, size_t N>
	inline Vector<T, N> TupleToVector(pybind11::tuple tuple)
	{
		Vector<T, N> ret;

		if (tuple.size() == N)
		{
			for (size_t i = 0; i < N; ++i)
			{
				ret[i] = tuple[i].cast<T>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	template <typename T, size_t N>
	inline Vector<T, N> TupleToVector(pybind11::list list)
	{
		Vector<T, N> ret;

		if (list.size() == N)
		{
			for (size_t i = 0; i < N; ++i)
			{
				ret[i] = list[i].cast<T>();
			}
		}
		else
		{
			throw std::invalid_argument("Invalid size.");
		}

		return ret;
	}

	template <typename T>
	inline Quaternion<T> TupleToQuaternion(pybind11::tuple tuple)
	{
		Quaternion<T> ret;

		for (size_t i = 0; i < 4; ++i)
		{
			ret[i] = tuple[i].cast<T>();
		}

		return ret;
	}

	template <typename T>
	inline Quaternion<T> TupleToQuaternion(pybind11::list list)
	{
		Quaternion<T> ret;

		for (size_t i = 0; i < 4; ++i)
		{
			ret[i] = list[i].cast<T>();
		}

		return ret;
	}

	inline Vector2F TupleToVector2F(pybind11::tuple tuple)
	{
		return TupleToVector<float, 2>(tuple);
	}

	inline Vector2F TupleToVector2F(pybind11::list list)
	{
		return TupleToVector<float, 2>(list);
	}

	inline Vector3F TupleToVector3F(pybind11::tuple tuple)
	{
		return TupleToVector<float, 3>(tuple);
	}

	inline Vector3F TupleToVector3F(pybind11::list list)
	{
		return TupleToVector<float, 3>(list);
	}

	inline Vector4F TupleToVector4F(pybind11::tuple tuple)
	{
		return TupleToVector<float, 4>(tuple);
	}

	inline Vector4F TupleToVector4F(pybind11::list list)
	{
		return TupleToVector<float, 4>(list);
	}

	inline QuaternionF TupleToQuaternionF(pybind11::tuple tuple)
	{
		return TupleToQuaternion<float>(tuple);
	}

	inline QuaternionF TupleToQuaternionF(pybind11::list list)
	{
		return TupleToQuaternion<float>(list);
	}

	inline Vector2D TupleToVector2D(pybind11::tuple tuple)
	{
		return TupleToVector<double, 2>(tuple);
	}

	inline Vector2D TupleToVector2D(pybind11::list list)
	{
		return TupleToVector<double, 2>(list);
	}

	inline Vector3D TupleToVector3D(pybind11::tuple tuple)
	{
		return TupleToVector<double, 3>(tuple);
	}

	inline Vector3D TupleToVector3D(pybind11::list list)
	{
		return TupleToVector<double, 3>(list);
	}

	inline Vector4D TupleToVector4D(pybind11::tuple tuple)
	{
		return TupleToVector<double, 4>(tuple);
	}

	inline Vector4D TupleToVector4D(pybind11::list list)
	{
		return TupleToVector<double, 4>(list);
	}

	template <typename T>
	inline pybind11::tuple Vector2ToTuple(const Vector<T, 2>& vec)
	{
		return pybind11::make_tuple(vec.x, vec.y);
	}

	template <typename T>
	inline pybind11::tuple Vector3ToTuple(const Vector<T, 3>& vec)
	{
		return pybind11::make_tuple(vec.x, vec.y, vec.z);
	}

	template <typename T>
	inline pybind11::tuple Vector4ToTuple(const Vector<T, 4>& vec)
	{
		return pybind11::make_tuple(vec.x, vec.y, vec.z, vec.w);
	}

	inline QuaternionD TupleToQuaternionD(pybind11::tuple tuple)
	{
		return TupleToQuaternion<double>(tuple);
	}

	inline QuaternionD TupleToQuaternionD(pybind11::list list)
	{
		return TupleToQuaternion<double>(list);
	}

	inline Size2 ObjectToSize2(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Size2>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Size2>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToSize2(pybind11::tuple(obj));
		}
		
		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToSize2(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to Size2.");
	}

	inline Size3 ObjectToSize3(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Size3>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Size3>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToSize3(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToSize3(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to Size3.");
	}

	inline Point2UI ObjectToPoint2UI(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Point2UI>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Point2UI>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToPoint2UI(pybind11::tuple(obj));
		}
		
		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToPoint2UI(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to Point2UI.");
	}

	inline Point3UI ObjectToPoint3UI(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Point3UI>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Point3UI>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToPoint3UI(pybind11::tuple(obj));
		}
		
		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToPoint3UI(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to Point3UI.");
	}

	inline Vector2F ObjectToVector2F(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector2F>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector2F>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector2F(pybind11::tuple(obj));
		}
		
		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector2F(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to Vector2F.");
	}

	inline Vector2D ObjectToVector2D(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector2D>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector2D>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector2D(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector2D(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to Vector2D.");
	}

	inline Vector3F ObjectToVector3F(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector3F>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector3F>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector3F(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector3F(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to Vector3F.");
	}

	inline Vector3D ObjectToVector3D(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector3D>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector3D>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector3D(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector3D(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to Vector3D.");
	}

	inline Vector4F ObjectToVector4F(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector4F>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector4F>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector4F(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector4F(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to Vector4F.");
	}

	inline Vector4D ObjectToVector4D(const pybind11::object& obj)
	{
		if (pybind11::isinstance<Vector4D>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<Vector4D>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector4D(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToVector4D(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to Vector4D.");
	}

	inline QuaternionF ObjectToQuaternionF(const pybind11::object& obj)
	{
		if (pybind11::isinstance<QuaternionF>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<QuaternionF>();
		}

		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToQuaternionF(pybind11::tuple(obj));
		}

		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToQuaternionF(pybind11::list(obj));
		}

		throw std::invalid_argument("Cannot convert to QuaternionF.");
	}

	inline QuaternionD ObjectToQuaternionD(const pybind11::object& obj)
	{
		if (pybind11::isinstance<QuaternionD>(static_cast<pybind11::handle>(obj)))
		{
			return obj.cast<QuaternionD>();
		}
		
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToQuaternionD(pybind11::tuple(obj));
		}
		
		if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			return TupleToQuaternionD(pybind11::list(obj));
		}
		
		throw std::invalid_argument("Cannot convert to QuaternionD.");
	}
}

#endif