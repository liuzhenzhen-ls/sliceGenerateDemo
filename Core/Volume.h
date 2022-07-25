#pragma once
#include<iostream>
#include"vector.h"

using namespace tgt;

namespace Core {
	/*template<typename T>*/
	class  Volume
	{
	public:
		 Volume();
		 Volume(svec3 dim, vec3 spacing,unsigned short* data);
		~ Volume();
		size_t calPos(const svec3& pos) const;
		unsigned short getVoxelValue(size_t x, size_t y, size_t z)const;
		unsigned short getVoxelValue(const svec3& pos)const;
		unsigned short getVoxelValue(size_t value) const;
		float getVoxelNormalized(size_t x, size_t y, size_t z)const;
		float GetVolumeLinear(const vec3& pos) const;

		unsigned short* GetData() ;
		vec3 getSpacing() const;
		svec3 getDimensions() const;
		
	private:
		unsigned short* volData_;
		vec3 spacing_;
		svec3 dimensions_;
	};
}
