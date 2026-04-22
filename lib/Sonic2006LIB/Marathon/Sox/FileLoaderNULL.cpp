#include "FileLoaderNULL.h"


using namespace Sonicteam::SoX;


boost::shared_ptr<Sonicteam::SoX::IFileHandle> FileLoaderNULL::LoadFile(std::string& filepath, boost::function<bool(void*, unsigned __int64)> function)
{
    return boost::shared_ptr<Sonicteam::SoX::IFileHandle>();
}



FileLoaderNULL::FileLoaderNULL()
{

}

FileLoaderNULL::~FileLoaderNULL()
{

}

DESTRUCTION_CPP(FileLoaderNULL);