#include "pch.h"

#include <Array/Array3.h>
#include <Array/ArrayAccessor3.h>

using namespace CubbyFlow;

TEST(ArrayAccessor3, Constructors)
{
	double data[60];
	for (int i = 0; i < 60; ++i)
	{
		data[i] = static_cast<double>(i);
	}

	ArrayAccessor3<double> acc(Size3(5, 4, 3), data);

	EXPECT_EQ(5u, acc.Size().x);
	EXPECT_EQ(4u, acc.Size().y);
	EXPECT_EQ(3u, acc.Size().z);
	EXPECT_EQ(data, acc.Data());
}

TEST(ArrayAccessor3, Iterators)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.Accessor();

	float cnt = 1.f;
	for (float& elem : acc)
	{
		EXPECT_FLOAT_EQ(cnt, elem);
		cnt += 1.f;
	}

	cnt = 1.f;
	for (const float& elem : acc)
	{
		EXPECT_FLOAT_EQ(cnt, elem);
		cnt += 1.f;
	}
}

TEST(ArrayAccessor3, ForEach)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.Accessor();

	size_t i = 0;
	acc.ForEach([&](float val)
	{
		EXPECT_FLOAT_EQ(acc[i], val);
		++i;
	});
}

TEST(ArrayAccessor3, ForEachIndex)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.Accessor();

	acc.ForEachIndex([&](size_t i, size_t j, size_t k)
	{
		size_t idx = i + (4 * (j + 3 * k)) + 1;
		EXPECT_FLOAT_EQ(static_cast<float>(idx), acc(i, j, k));
	});
}

TEST(ArrayAccessor3, ParallelForEach)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.Accessor();

	acc.ParallelForEach([&](float& val)
	{
		val *= 2.f;
	});

	acc.ForEachIndex([&](size_t i, size_t j, size_t k)
	{
		size_t idx = i + (4 * (j + 3 * k)) + 1;
		float ans = 2.f * static_cast<float>(idx);
		EXPECT_FLOAT_EQ(ans, acc(i, j, k));
	});
}

TEST(ArrayAccessor3, ParallelForEachIndex)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.Accessor();

	acc.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
	{
		size_t idx = i + (4 * (j + 3 * k)) + 1;
		EXPECT_FLOAT_EQ(static_cast<float>(idx), acc(i, j, k));
	});
}


TEST(ConstArrayAccessor3, Constructors)
{
	double data[60];
	for (int i = 0; i < 60; ++i)
	{
		data[i] = static_cast<double>(i);
	}

	// Construct with ArrayAccessor3
	ArrayAccessor3<double> acc(Size3(5, 4, 3), data);
	ConstArrayAccessor3<double> cacc(acc);

	EXPECT_EQ(5u, cacc.Size().x);
	EXPECT_EQ(4u, cacc.Size().y);
	EXPECT_EQ(3u, cacc.Size().z);
	EXPECT_EQ(data, cacc.Data());
}

TEST(ConstArrayAccessor3, Iterators)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.ConstAccessor();

	float cnt = 1.f;
	for (const float& elem : acc)
	{
		EXPECT_FLOAT_EQ(cnt, elem);
		cnt += 1.f;
	}
}

TEST(ConstArrayAccessor3, ForEach)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.ConstAccessor();

	size_t i = 0;
	acc.ForEach([&](float val)
	{
		EXPECT_FLOAT_EQ(acc[i], val);
		++i;
	});
}

TEST(ConstArrayAccessor3, ForEachIndex)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.ConstAccessor();

	acc.ForEachIndex([&](size_t i, size_t j, size_t k)
	{
		size_t idx = i + (4 * (j + 3 * k)) + 1;
		EXPECT_FLOAT_EQ(static_cast<float>(idx), acc(i, j, k));
	});
}

TEST(ConstArrayAccessor3, ParallelForEachIndex)
{
	Array3<float> arr1(
	{
		{
			{ 1.f,  2.f,  3.f,  4.f },
			{ 5.f,  6.f,  7.f,  8.f },
			{ 9.f, 10.f, 11.f, 12.f }
		},
		{
			{ 13.f, 14.f, 15.f, 16.f },
			{ 17.f, 18.f, 19.f, 20.f },
			{ 21.f, 22.f, 23.f, 24.f }
		}
	});
	auto acc = arr1.ConstAccessor();

	acc.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
	{
		size_t idx = i + (4 * (j + 3 * k)) + 1;
		EXPECT_FLOAT_EQ(static_cast<float>(idx), acc(i, j, k));
	});
}