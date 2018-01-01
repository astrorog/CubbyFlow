/*************************************************************************
> File Name: FDMMGLinearSystem3-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Multigrid-syle 3-D linear system.
> Created Time: 2017/11/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_IMPL_H
#define CUBBYFLOW_FDM_MG_LINEAR_SYSTEM3_IMPL_H

namespace CubbyFlow
{
	template <typename T>
	void FDMMGUtils3::ResizeArrayWithCoarsest(const Size3& coarsestResolution,
		size_t numberOfLevels, std::vector<Array3<T>>* levels)
	{
		numberOfLevels = std::max(numberOfLevels, ONE_SIZE);

		levels->resize(numberOfLevels);

		// Level 0 is the finest level, thus takes coarsestResolution ^ numberOfLevels.
		// Level numberOfLevels - 1 is the coarsest, taking coarsestResolution.
		Size3 res = coarsestResolution;
		for (size_t level = 0; level < numberOfLevels; ++level)
		{
			(*levels)[numberOfLevels - level - 1].Resize(res);
			res.x = res.x << 1;
			res.y = res.y << 1;
			res.z = res.z << 1;
		}
	}

	template <typename T>
	void FDMMGUtils3::ResizeArrayWithFinest(const Size3& finestResolution,
		size_t maxNumberOfLevels, std::vector<Array3<T>>* levels)
	{
		Size3 res = finestResolution;
		size_t i = 1;

		for (; i < maxNumberOfLevels; ++i)
		{
			if (res.x % 2 == 0 && res.y % 2 == 0 && res.z % 2 == 0)
			{
				res.x = res.x >> 1;
				res.y = res.y >> 1;
				res.z = res.z >> 1;
			}
			else
			{
				break;
			}
		}

		ResizeArrayWithCoarsest(res, i, levels);
	}
}

#endif