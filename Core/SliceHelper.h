//#ifndef VRN_VOLUMESLICEHELPER_H
//#define VRN_VOLUMESLICEHELPER_H

#include"volume.h"
#include"vector.h"
#include<iostream>
#include<vector>

using namespace std;
namespace Core {

    //class TriangleMeshGeometryNormal;
    //Helper class for the extraction of slices from a volume.

    class SliceHelper {

    public:
        static int GetVolumeSlice(const Volume* volume, const float* texture, int sliceSize[], int thickness,unsigned short* retSlice);
    };
//#endif
}