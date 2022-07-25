#include"slicehelper.h"
#include <fstream>

namespace Core {

	int SliceHelper::GetVolumeSlice(const Volume* volume, const float* texture, int sliceSize[], int thickness,unsigned short* retslice)
	{
		tgt::vec3 dims = volume->getDimensions();
		tgt::vec3 sp = volume->getSpacing();

		if (!volume)
		{
			return -1;
		}

		/* v4     y      v3
		 *        ^
		 *        |
		 *        o  -->x
		 * v1            v2
		 */

		tgt::vec3 v1 = vec3(dims.x * texture[0], dims.y * texture[1], dims.z * texture[2]);
		tgt::vec3 v2 = vec3(dims.x * texture[3], dims.y * texture[4], dims.z * texture[5]);
		tgt::vec3 v3 = vec3(dims.x * texture[6], dims.y * texture[7], dims.z * texture[8]);
		tgt::vec3 v4 = vec3(dims.x * texture[9], dims.y * texture[10], dims.z * texture[11]);

		tgt::ivec2 res(sliceSize[0], sliceSize[1]/*tgt::distance(v2, v1), tgt::distance(v4, v1)*/);

		tgt::vec3 xVec = v2 - v1;
		tgt::vec3 yVec = v4 - v1;

		tgt::vec3 origin = v1;

		memset(retslice,0,res.x*res.y*sizeof(unsigned short));

		tgt::vec3 fetchX = normalize(xVec);
		tgt::vec3 fetchY = normalize(yVec);
		tgt::vec3 fetchNorm = tgt::normalize(tgt::cross(fetchX, fetchY));
		tgt::vec3 fetchOrigin = origin + (0.5f * fetchX) + (0.5f * fetchY);

		int halfThick = thickness / 2;

#pragma omp parallel for	
		for (int x = 0; x < res.x; x++)
		{
			for (int y = 0; y < res.y; y++)
			{
				tgt::vec3 pos = fetchOrigin + ((float)x * fetchX) + ((float)y * fetchY);

				float valueFloat = 0.0f;

				for (int k = -halfThick; k < thickness - halfThick; ++k)
				{
					pos = pos + (float)k * fetchNorm;
					if (hand(greaterThanEqual(pos, tgt::vec3(0.0f))) && hand(lessThanEqual(pos, dims)))
					{
						valueFloat = volume->GetVolumeLinear(pos);
					}
				}

				retslice[y * res.x + x] = (valueFloat / thickness);
			}
		}

		return 1;
	}
}





