#pragma once

#include <Marathon.inl>
#include <Marathon.inl>
#include <Sox/ArcHandle.h>
#include <Sox/RefCountObject.h>
#include <boost/function.hpp>
#include <string>

namespace Sonicteam
{
	MARATHON_XBLA_CLASS class ArcFileLoaderE3;

#pragma pack(push, 4)
	MARATHON_XBLA_CLASS __declspec(align(4)) class ArcFileHandleE3
	{
	public:
		virtual void Destroy()
		{

		}
		virtual void* GetHandle()
		{
		 return m_Field24;
			 
		}
		virtual uint64_t GetSize()
		{
			return m_ParentList;
		}

		virtual bool ReadArc()
		{
			return m_Field5;
		}

		virtual bool ReadArcHeader()
		{
			return m_Field5;
		}

		ArcFileHandleE3()
		{

		}
		~ArcFileHandleE3()
		{

		}
	public:

		uint8_t m_Field4;
		uint8_t m_Field5;
		uint8_t m_Field6;
		uint8_t m_Field7;
		std::string m_Path;

		void* m_Field24;
		uint64_t m_ParentList;
		ArcFileLoaderE3* m_pFileLoaderE3;
	
	};
#pragma pack(pop)
};

