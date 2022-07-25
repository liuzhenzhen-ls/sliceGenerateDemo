#pragma once
#include"ManagedObject.h"
#include"Core.h"

using namespace System;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

namespace Wrapper {
	public ref class GenerateSliceAlgorithm:public ManagedObject<Core::SliceCache>
	{
		// TODO: 在此处为此类添加方法。
	public :
		GenerateSliceAlgorithm();
		~GenerateSliceAlgorithm();

		void SetVolumeData(IntPtr volPtr, cli::array<int>^ dims, cli::array<float>^ spacing);
		void SetSliceData(IntPtr imagePtr, cli::array<int>^ dims);
		int UpdateSlice(cli::array<float>^ text, int thickness);
		int UpdateSlice(IntPtr textPtr,int thickness);

		void Clear();
	};
}
