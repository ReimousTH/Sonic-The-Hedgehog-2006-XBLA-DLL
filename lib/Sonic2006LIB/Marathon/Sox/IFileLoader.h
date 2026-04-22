#ifndef SONICTEAM__SOX__IFILELOADER
#define SONICTEAM__SOX__IFILELOADER

#include <Sox/Defs.h>
#include <Sox/IResource.h>
#include <Sox/RefCountObject.h>
#include <boost/function.hpp>	
#include <boost/shared_ptr.hpp>	



namespace Sonicteam{
	namespace SoX{
		

		
		//this is the best that i can think
		class IFileLoader
		{

		public:	
			IFileLoader();
			~IFileLoader();
			MARATHON_DESTRUCTION_H(IFileLoader);;
			virtual boost::shared_ptr<Sonicteam::SoX::IFileHandle> LoadFile(std::string& filepath, boost::function<bool(void*, unsigned __int64)> function) = 0;

		};

	};
};
#endif