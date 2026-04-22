#include "FileLoaderARC.h"


using namespace Sonicteam;


boost::shared_ptr<Sonicteam::SoX::IFileHandle> FileLoaderARC::LoadFile(std::string& filepath, boost::function<bool(void*, unsigned __int64)> function)
{
    return boost::shared_ptr<Sonicteam::SoX::IFileHandle>();
}



FileLoaderARC::FileLoaderARC()
{

}

FileLoaderARC::~FileLoaderARC()
{

}

DESTRUCTION_CPP(FileLoaderARC);