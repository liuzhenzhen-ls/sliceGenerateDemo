#include<iostream>
#include<assert.h>
#include"slicecache.h"
#include<sstream>
#include<objbase.h>

namespace Core {

	SliceCache::SliceCache(Processor* owner, size_t cacheSize)
		: owner_(owner)
		, cacheSize_(cacheSize)
		, currentBackgroundThread_(0)
		, tmpCache_()
	{
		assert(owner_, "null pointer passed");//断言函数，直接用c++的不用tgt库中的
	}

	SliceCache::SliceCache() {}
	SliceCache::~SliceCache() {
		clear();
	}

	size_t SliceCache::getCacheSize() const {
		return cacheSize_;
	}

	//设置缓存list<CAcheEntry>的大小，使用递归，只要缓存长度大于设置长度，删除最后一个元素，其中一个元素中内存释放掉
	void SliceCache::setCacheSize(size_t cacheSize) {
		cacheSize_ = cacheSize;
		while (slices_.size() > cacheSize_) {
			delete slices_.back().slice_;
			slices_.pop_back();
		}
	}


	void SliceCache::clear() {
		/*boost::lock_guard<boost::mutex> lock(cacheAccessMutex_)*/;

		//if (currentBackgroundThread_ && currentBackgroundThread_->isRunning())
		//    currentBackgroundThread_->interruptAndJoin();
		//delete currentBackgroundThread_;
		//currentBackgroundThread_ = 0;

		for (std::list<CacheEntry>::iterator it = slices_.begin(); it != slices_.end(); ++it) {
			assert(it->slice_, "cache entry does not store slice");
			delete it->slice_;
		}
		if (slices_.size() > 0)
		{
			slices_.clear();
		}

		/*delete tmpCache_.slice_;
		tmpCache_.slice_ = 0;*/

	/*	delete[] volume;
		delete[] retslice;*/
	}

	//==================================

	void SliceCache::SetSliceData(unsigned short* imagePtr, int* dims)
	{
		if (imagePtr != nullptr)
		{
			retslice = imagePtr;
			sliceDim.x = dims[0];
			sliceDim.y = dims[1];
		}
	}

	void SliceCache::SetVolumeData(unsigned short* volData, int* dim, float* spacing)
	{
		dims.x = dim[0];
		dims.y = dim[1];
		dims.z = dim[2];
		tgt::vec3 spacings(spacing[0], spacing[1], spacing[2]);
		volume = new Volume(dims, spacings, volData);
	}

	int SliceCache::UpdateSlice(float* texture, int thickness)
	{
		try
		{
			assert(volume, "null pointer passed");
			std::string hash = getHash(volume, texture, thickness);
			int size[2] = { sliceDim.x,sliceDim.y };
			int ret = 0;

			ret = SliceHelper::GetVolumeSlice(volume, texture, size, thickness, retslice);
			return ret;


			//hash 值获取已有切片
			/*if (!findSliceInCache(hash, true))
			{
				ret = SliceHelper::GetVolumeSlice(volume, texture, size, thickness, retslice);

				if (retslice)
				{
					addSliceToCache(retslice, hash);
					cleanupCache();
					assert(slices_.size() <= cacheSize_, "invalid cache size");
					ret = 1;
				}
				else
				{
					ret = -1;
				}
			}
			else
			{
				memcpy((char*)retslice, findSliceInCache(hash, true), sliceDim.x * sliceDim.y * sizeof(unsigned short));

				if (retslice)
				{
					ret = 1;
				}
				else
				{
					ret = -1;
				}
			}

			return ret;*/
		}
		catch (std::exception ex)
		{
			return -1;
		}
	}

	//==================================
	// private

	void SliceCache::addSliceToCache(unsigned short* slice, const std::string& hash, bool finishedSlice) const {
		/* boost::lock_guard<boost::mutex> lock(cacheAccessMutex_);*/

		assert(slice, "null pointer passed");
		assert(!hash.empty(), "hash is empty");

		// store unfinished or "not" cached slices to be able to delete them.
		if (!finishedSlice || (cacheSize_ == 0)) {
			delete tmpCache_.slice_;
			tmpCache_.slice_ = slice;
			tmpCache_.hash_ = hash;
		}
		else { //add slice to cache list
			CacheEntry entry;
			entry.hash_ = hash;
			entry.slice_ = slice;
			slices_.push_front(entry);
		}
	}


	void SliceCache::cleanupCache() const {
		/* boost::lock_guard<boost::mutex> lock(cacheAccessMutex_);*/

		while (slices_.size() > cacheSize_) {
			delete slices_.back().slice_;
			slices_.pop_back();
		}
		assert(slices_.size() <= cacheSize_, "invalid cache size");
	}

	unsigned short* SliceCache::findSliceInCache(const std::string& hash, bool updateUsage) const {
		/*boost::lock_guard<boost::mutex> lock(cacheAccessMutex_);*/

		assert(!hash.empty(), "hash is empty");
		for (std::list<CacheEntry>::iterator it = slices_.begin(); it != slices_.end(); ++it) {
			if (it->hash_ == hash) {
				CacheEntry entry = *it;
				if (updateUsage) { // move found entry to front
					slices_.erase(it);
					slices_.push_front(entry);
				}
				assert(entry.slice_, "cache entry does not contain slice");
				return entry.slice_;
			}
		}
		return 0;
	}

	bool SliceCache::hasSliceInCache(const Volume* volume, const float* texture, int thickness) const {
		return (findSliceInCache(getHash(volume, texture, thickness), false) != 0);
	}


	std::string SliceCache::getHash(const Volume* volume, const float* texture, int thickness) const
	{
		std::ostringstream configStr;
		configStr << texture;
		configStr << thickness;
		configStr << volume;
		configStr << thickness;

		string ret = "";

		ret = configStr.str();
		return ret;
	}


	//--------------------------------------------------------------------------
	  //Background Thread
	//--------------------------------------------------------------------------
	//SliceCreationBackgroundThread::SliceCreationBackgroundThread(Processor* processor, const SliceCache* sliceCache,
	//           const Volume* volume,/* SliceAlignment alignment, size_t sliceIndex, int* shiftArray,
	//            size_t levelOfDetail, */const std::string& hash)
	//    : BackgroundThread()
	//    , sliceCache_(sliceCache)
	//    , volume_(volume)
	//    /* , alignment_(alignment)
	//    , sliceIndex_(sliceIndex)
	//    , shiftArray_(0)
	//    , levelOfDetail_(levelOfDetail)*/
	//    , hash_(hash)
	//	, texture_(texture_)
	//	, thickness_(1)
	//    {
	//    /*if(shiftArray) {
	//        shiftArray_ = new int[volume->getNumChannels()];
	//        memcpy(&shiftArray_,&shiftArray,sizeof(int)*volume->getNumChannels());
	//    }*/
	//   /* assert(processor_, "null pointer passed as processor");*/
	//    assert(volume_, "null pointer passed as volume");
	//    assert(!hash_.empty(), "passed hash is empty");
	//}



	//SliceCreationBackgroundThread::SliceCreationBackgroundThread(Processor* processor, const SliceCache* sliceCache, const Volume* volume, const float* texture, int thickness, const std::string& hash)
	//    : BackgroundThread()
	//    , sliceCache_(sliceCache)
	//    , volume_(volume)
	//    , sliceIndex_(1)
	//    , shiftArray_(0)
	//    , texture_(texture)
	//    , thickness_(1)
	//    , levelOfDetail_(0)
	//    , hash_(hash)
	//{
	//    /*assert(processor_, "null pointer passed as processor");*/
	//    assert(volume_, "null pointer passed as volume");
	//    assert(!hash_.empty(), "passed hash is empty");
	//}



	//void SliceCreationBackgroundThread::threadMain()
	//	{
	//        
	//	
	//		/*if (alignment_ == UNALIGNED_PLANE)
	//		{
	//            slice = SliceHelper::getVolumeSlice(volume_, texture_, thickness_);
	//        }
	//        else
	//        {
	//            slice = SliceHelper::getVolumeSlice(volume_, alignment_, sliceIndex_, shiftArray_, levelOfDetail_);
	//            delete[] shiftArray_;
	//        }*/
	//
	//        //int ret = SliceHelper::GetVolumeSlice(volume, texture, size, thickness, retslice);
	//    
	//
	//   /* unsigned short* slice = nullptr;
	//    if (slice) {
	//        sliceCache_->addSliceToCache(slice, hash_);
	//    }*/
	//}

}
