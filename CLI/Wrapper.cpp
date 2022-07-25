#include "pch.h"

#include "Wrapper.h"

namespace Wrapper {
	GenerateSliceAlgorithm::GenerateSliceAlgorithm() :ManagedObject(new Core::SliceCache())
	{
		Console::WriteLine("Creating a slicecache object to generateSlice!");
	}

	/*int GenerateSliceAlgorithm::getVolumeSlice(IntPtr dims, IntPtr spacings, IntPtr volumeData, IntPtr textureCoodinate, int thickness,IntPtr slice)
	{
		int* dim = reinterpret_cast<int*>(dims.ToPointer());
		float* spacing = reinterpret_cast<float*>(spacings.ToPointer());
		unsigned short* data = reinterpret_cast<unsigned short*>(volumeData.ToPointer());
		float* textureData= reinterpret_cast<float*>(textureCoodinate.ToPointer());
		unsigned short* retslice = reinterpret_cast<unsigned short*>(slice.ToPointer());

		int ret=m_Instance->getVolumeSlice(dim, spacing,data,textureData,thickness,retslice);

		return ret;
	}*/


	/*
	* set volume data
	*/
	void GenerateSliceAlgorithm::SetVolumeData(IntPtr volPtr, cli::array<int>^ dims, cli::array<float>^ spacing)
	{
		try
		{
			//unsigned short* voldata = reinterpret_cast<unsigned short*>(volPtr.ToPointer());

			int* d = new int[dims->Length];
			float* s = new float[spacing->Length];

			for (int i = 0; i < dims->Length; i++)
			{
				d[i] = dims[i];
			}

			for (int j = 0; j < spacing->Length; j++)
			{
				s[j] = spacing[j];
			}

			m_Instance->SetVolumeData(reinterpret_cast<unsigned short*>(volPtr.ToPointer()), d, s);

			delete[] s;
			delete[] d;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}

	}

	/*
	* set return slice data
	*/
	void GenerateSliceAlgorithm::SetSliceData(IntPtr imagePtr, cli::array<int>^ dims)
	{
		try
		{
			//unsigned short* retslice = reinterpret_cast<unsigned short*>(imagePtr.ToPointer());

			int* d = new int[dims->Length];
			for (int i = 0; i < dims->Length; i++)
			{
				d[i] = dims[i];
			}

			m_Instance->SetSliceData(reinterpret_cast<unsigned short*>(imagePtr.ToPointer()), d);

			delete[] d;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}

	int GenerateSliceAlgorithm::UpdateSlice(cli::array<float>^ textureCoordinate, int thickness)
	{
		try 
		{
			float* text = new float[textureCoordinate->Length];
			for (int i = 0; i < textureCoordinate->Length; i++)
			{
				text[i] = textureCoordinate[i];
			}

			int ret = m_Instance->UpdateSlice(text, thickness);

			delete[] text;

			return ret;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}

	int GenerateSliceAlgorithm::UpdateSlice(IntPtr text, int thickness)
	{
		try
		{
			//float* textdata = reinterpret_cast<float*>(text.ToPointer());

			int ret = m_Instance->UpdateSlice(reinterpret_cast<float*>(text.ToPointer()), thickness);

			//delete[] textdata;
			return ret;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}

	GenerateSliceAlgorithm::~GenerateSliceAlgorithm()
	{
		//m_Instance->clear();
	}

	Void GenerateSliceAlgorithm::Clear()
	{
		m_Instance->clear();
	}
}