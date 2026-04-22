#ifndef SONICTEAM__FILELOADERARCT
#define SONICTEAM__FILELOADERARCT

#include <Sox/Defs.h>
#include <Sox/IFileLoader.h>
#include <Sox/FileHandleARC.h>
#include <System/Singleton.h>
#include <System/CreateStatic.h>
#include <boost/shared_ptr.hpp>
#include <Marathon.inl>
#include <string>
#include <map>

namespace Sonicteam
{

	class ArcManager;

	//this is the best that i can think
	class FileLoaderARC :Sonicteam::SoX::IFileLoader, public SSINGLETON(FileLoaderARC)
	{

	public:
		FileLoaderARC();
		~FileLoaderARC();
		MARATHON_DESTRUCTION_H(FileLoaderARC);;
		virtual boost::shared_ptr<Sonicteam::SoX::IFileHandle> LoadFile(std::string & filepath, boost::function<bool(void*, unsigned __int64)> function);

		boost::shared_ptr<ArcManager> m_spArcManager;
		std::string m_RootPath;
		std::map<std::string, REF_TYPE(Sonicteam::SoX::ArcHandle)> m_mspHandles; //0x28
	};
};
#endif