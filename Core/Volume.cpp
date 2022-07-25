#include"volume.h"

namespace Core
{
	Volume::Volume() {}

	Volume::~Volume() 
	{
		delete[] volData_;
	}
	
	Volume::Volume(svec3 dim, vec3 spa, unsigned short* data)
	{
		dimensions_ = dim;
		spacing_ = spa;
		volData_ = data;
	}

	size_t Volume::calPos(const svec3& pos) const
	{
		//we need to work with size_t to avoid problems when working with very large datasets
		return pos.z * dimensions_.x * dimensions_.y + pos.y * dimensions_.x + pos.x;
	}

	unsigned short Volume::getVoxelValue(size_t x, size_t y, size_t z) const
	{
		return getVoxelValue(svec3(x, y, z));
	}

	unsigned short Volume::getVoxelValue(const svec3& pos) const
	{
		return getVoxelValue(calPos(pos));
	}

	unsigned short Volume::getVoxelValue(size_t value) const
	{
		return volData_[value];
	}

	float Volume::getVoxelNormalized(size_t x, size_t y, size_t z) const
	{
		unsigned short val = getVoxelValue(x, y, z);
	    //return val/65535.f;
		return val;
	}


	float Volume::GetVolumeLinear(const vec3& pos) const
	{
		vec3 posAbs = tgt::max(pos/* - 0.5f*/, vec3(0.0f));
		vec3 p = posAbs - floor(posAbs); // get decimal part
		svec3 llb = svec3(posAbs);
		svec3 urf = svec3(ceil(posAbs));
		urf = tgt::min(urf, dimensions_ - svec3(1)); // clamp so the lookups do not exceed the dimensions
		llb = tgt::min(llb, dimensions_ - svec3(1)); // dito

		/*
			interpolate linearly
		*/
		return  getVoxelNormalized(llb.x, llb.y, llb.z) * (1.f - p.x) * (1.f - p.y) * (1.f - p.z) // llB
			+ getVoxelNormalized(urf.x, llb.y, llb.z) * (p.x) * (1.f - p.y) * (1.f - p.z) // lrB
			+ getVoxelNormalized(urf.x, urf.y, llb.z) * (p.x) * (p.y) * (1.f - p.z) // urB
			+ getVoxelNormalized(llb.x, urf.y, llb.z) * (1.f - p.x) * (p.y) * (1.f - p.z) // ulB
			+ getVoxelNormalized(llb.x, llb.y, urf.z) * (1.f - p.x) * (1.f - p.y) * (p.z) // llF
			+ getVoxelNormalized(urf.x, llb.y, urf.z) * (p.x) * (1.f - p.y) * (p.z) // lrF
			+ getVoxelNormalized(urf.x, urf.y, urf.z) * (p.x) * (p.y) * (p.z) // urF
			+ getVoxelNormalized(llb.x, urf.y, urf.z) * (1.f - p.x) * (p.y) * (p.z);// ulF
    }

	unsigned short* Volume::GetData()
	{
		return volData_;
	}

	vec3 Volume::getSpacing() const
	{
		return spacing_;
	}

	svec3 Volume::getDimensions() const
	{
		return dimensions_;
	}
}