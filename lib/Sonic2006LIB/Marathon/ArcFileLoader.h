#pragma once

#include <Marathon.inl>
#include <Marathon.inl>
#include <boost/shared_ptr.hpp>

namespace Sonicteam
{

	MARATHON_XBLA_CLASS class ArcFileLoaderE3
	{
	public:
		virtual void Destroy()
		{

		}

		virtual boost::shared_ptr<ArcFileHandleE3> LoadHandle(std::string& path)
		{
			return  boost::shared_ptr<ArcFileHandleE3>((ArcFileHandleE3*)0);
		}

		ArcFileLoaderE3()
		{

		}
		~ArcFileLoaderE3()
		{

		}
	public:

		void* m_pBuffer;
	};
};

