#ifndef VRN_SLICECACHE_H
#define VRN_SLICECACHE_H

#include<iostream>
#include"volume.h"
#include"slicehelper.h"
#include <list>
//#include<boost/thread/mutex.hpp>

//#include"backgroundthread.h"

using namespace std;

namespace Core {

class Processor;
class SliceCreationBackgroundThread;

/**
 * Fixed-size LRU cache for 2D volume slices. On each getVolumeSlice() call the SliceCache checks whether
 * a slice with the specified parameters is present in the cache and returns it, if this is the case.
 * If not, the slice is extracted from the underlying volume via the SliceHelper and stored in the cache.
 *
 * Uncomplete slices (see VolumeOctree) will be stored in the tmpCache.
 */
class SliceCache {
    friend class SliceCreationBackgroundThread;
public:
    SliceCache(Processor* owner, size_t cacheSize);
    SliceCache();
    ~SliceCache();

    size_t getCacheSize() const;
    void setCacheSize(size_t cacheSize);
    void clear();

    bool hasSliceInCache(const Volume* volume, const float* texture,int thickness) const;

    //2021.4.6 update
    void SetVolumeData(unsigned short* volPtr, int* dims, float* spacing);
    void SetSliceData(unsigned short* imagePtr, int* dims);
    int UpdateSlice(float* textureCoodinate, int thickness);
    

private:
    struct CacheEntry {
        std::string hash_;  //< consisting of the input volume hash and a hash of the slice parameters
        unsigned short* slice_;
        CacheEntry() : hash_(""), slice_(0) {}
    };

    void addSliceToCache(unsigned short* slice, const std::string& hash, bool finishedSlice = true) const;
    void cleanupCache() const;
    unsigned short* findSliceInCache(const std::string& hash, bool updateUsage) const;

    std::string getHash(const Volume* volume, const float* texture, int thickness) const;

    Processor* owner_;

    mutable std::list<CacheEntry> slices_;
    size_t cacheSize_;

    // Used to store the last unfinished slice. Needed to delete it, since it is not stored in slices_.
    // Also stores the last requested slice, if cacheSize is zero. Needed for destruction.
    mutable CacheEntry tmpCache_;
    
    //∂‡œﬂ≥Ã
    mutable SliceCreationBackgroundThread* currentBackgroundThread_;
    //mutable mutex cacheAccessMutex_; ///< mutex for synchronizing cache accesses
    
    //2021.4.6
    Volume* volume;
    unsigned short* retslice;
    tgt::svec3 dims;
    tgt::svec2 sliceDim;

};

//class SliceCreationBackgroundThread : public BackgroundThread {
//    friend class SliceCache;
//public:
//    SliceCreationBackgroundThread(Processor* processor, const SliceCache* sliceCache,
//           const Volume* volume, /*SliceAlignment alignment, size_t sliceIndex, int* shiftArray,
//            size_t levelOfDetail,*/ const std::string& hash);
//
//    SliceCreationBackgroundThread(Processor* processor, const SliceCache* sliceCache,
//        const Volume* volume, const float*texture, int thickness, const std::string& hash);
	
//protected:
//    virtual void threadMain();
//    const SliceCache* sliceCache_;
//    const Volume* volume_;
//    size_t sliceIndex_;
//    const float* texture_;
//    int thickness_;
//    int* shiftArray_;
//    size_t levelOfDetail_;
//    std::string hash_;
//};

} 

#endif
