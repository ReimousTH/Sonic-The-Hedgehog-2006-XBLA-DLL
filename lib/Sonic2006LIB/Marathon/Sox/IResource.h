#ifndef SONICTEAM__SOX__IRESOURCE
#define SONICTEAM__SOX__IRESOURCE

#include <string>
#include <Sox/Defs.h>
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <Sox/ResourceManager.h>


namespace Sonicteam{
	namespace SoX{
		class IResource:public Sonicteam::SoX::RefCountObject
		{
		public:
			//size0x60
			IResource(void);
			~IResource();
			//virtual ~IResource(void);
			MARATHON_DESTRUCTION_H(IResource);;



			//bool	
			virtual bool ResourceLoad(void* File,size_t size) = 0; // FileArrayPtr,FileSizeMb
			virtual int ResouceUnk02(); //return 0
			virtual std::string GetResourceName(std::string file_name) = 0;
			virtual int ResourceType(); //return 1




			//used for boost::bind
			bool ResourceLoadFinal(void* File,unsigned __int64 size);


			//fields
			std::string ManagerName; //0x8 .lua
			std::string ResourceName; //0x24 .lua
			std::string ResourceStr3; //0x40 .lua (ResourceArcName? After Load?)
			std::string ResourceStr4; //0x5c

			char IsInResourceManager; //78
			void SetResourceManagerMeta(unsigned int* i,std::string& m,std::string& r);

		};
	}
}

#endif

