#ifndef SONICTEAM__SOX__FILEHANDLEARC
#define SONICTEAM__SOX__FILEHANDLEARC

#include <Sox/Defs.h>
#include <Sox/IFileHandle.h>
#include <Sox/Misc/Aligment.hpp>
#include <Sox/ArcHandleMgr.h>
#include <Sox/RefCountObject.h>
#include <Sox/ArcHandle.h>

#include <iostream>
#include <string>
#include <Marathon.inl>

namespace Sonicteam{
	 class FileLoaderARC;;
	namespace SoX{

		
	

		//this is the best that i can think
		MARATHON_ALIGNAS_4 class FileHandleARC:public Sonicteam::SoX::IFileHandle
		{

		public:	
			FileHandleARC();
			FileHandleARC(std::string&,IFileHandle_BFUNCTION_TYPE);
			~FileHandleARC();
			MARATHON_DESTRUCTION_H(FileHandleARC);;
			virtual bool LoadHandle();
			virtual unsigned __int64 GetHandleSize();
			virtual void* GetHandleBuffer();
			void LoadFileFromArc(Sonicteam::SoX::ArcHandleSearchResource& res);
			


	
			static bool SearchChunk(Sonicteam::SoX::ArcHandleMgr* mgr,std::string& search_path,Sonicteam::SoX::ArcHandleSearchResource& search_chunk);
			static std::string GetARCChunkPath(Sonicteam::FileLoaderARC* mgr,std::string& path);
			static unsigned int SearchChunkEntryE1(Sonicteam::SoX::ArcFile* ArcFile, const char* path);
			static void DecompressZChunk(void* out_buffer,size_t out_size,void* c_buffer,size_t c_size);

			char IsLoaded; //4
			bool IsProcess; //5
			bool m_Field6; //5
			std::string m_Path; //8 (game:\xenon\registry.lub)
			uint32_t m_Field24;
			uint32_t m_Buffer; // 0x28
			uint32_t m_Field2C;
			uint64_t m_CompressedSize; // Compressed Size
			uint8_t m_IsCompressed;
			uint32_t m_Field3C;
			uint64_t m_DecompressedSize; // Decompressed Size
			boost::function<bool(void*, unsigned __int64)> m_Field48;
			REF_TYPE(Sonicteam::SoX::ArcHandle) m_spHandle; //54
			uint32_t m_Field58;
			uint32_t m_Field5C;
		};

	};
};
#endif