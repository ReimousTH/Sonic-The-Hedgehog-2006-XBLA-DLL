#ifndef SONICTEAM__FILELOADERARCNULL
#define SONICTEAM__FILELOADERARCNULL

#include <Sox/Defs.h>
#include <Sox/IFileLoader.h>
#include <System/Singleton.h>
#include <System/CreateStatic.h>
#include <boost/shared_ptr.hpp>
#include <Marathon.inl>
#include <string>
#include <map>

namespace Sonicteam
{
	namespace SoX
	{
		//this is the best that i can think
		class FileLoaderNULL :Sonicteam::SoX::IFileLoader, public SSINGLETON(FileLoaderNULL)
		{

		public:
			FileLoaderNULL();
			~FileLoaderNULL();
			MARATHON_DESTRUCTION_H(FileLoaderNULL);;
			virtual boost::shared_ptr<Sonicteam::SoX::IFileHandle> LoadFile(std::string & filepath, boost::function<bool(void*, unsigned __int64)> function);
			/// ... Fields?
		};
	};
};
#endif