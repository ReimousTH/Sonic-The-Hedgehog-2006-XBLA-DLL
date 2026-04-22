#pragma once

#include <Marathon.inl>
#include <Marathon.inl>
#include <Boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <Sox/RefCountObject.h>


namespace Sonicteam
{
	class ArcFileLoaderE3;
	class ArcFileHandleE3;

#pragma pack(push, 4)
	MARATHON_XBLA_CLASS __declspec(align(4)) class ArcManager
	{
	public:
		
		ArcManager()
		{

		}
		~ArcManager()
		{

		}
	public:

		boost::shared_ptr<ArcFileLoaderE3> m_pFileLoaderE3; 
		MARATHON_INSERT_PADDING(0x14);
		std::map<std::string, REF_TYPE(Sonicteam::SoX::ArcHandle)> m_mspHandles; //0x28
	
	};
#pragma pack(pop)
};

