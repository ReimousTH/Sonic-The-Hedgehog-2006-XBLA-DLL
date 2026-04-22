#ifndef SONICTEAM__SOX__ARCHANDLE
#define SONICTEAM__SOX__ARCHANDLE

#include <xtl.h>
#include <Sox/Defs.h>
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <Sox/IResource2.h>
#include <Sox/Misc/LinkSoxNode.h>
#include <Sox/ArcFile.h>
#include <Sox/ArcHandleMgr.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <Marathon.inl>



namespace Sonicteam
{
	class ArcFileHandleE3;
	class ArcManager;

	namespace SoX
	{


		

		class ArcHandle :public Sonicteam::SoX::RefCountObject
		{

		public:
			ArcHandle();
			~ArcHandle();
			MARATHON_DESTRUCTION_H(ArcHandle);

			ArcManager* m_Field8;
			std::string m_ArcFilePath; // game:\common\Resources_scripts_4P.arc 0x94  (+)
			char IsFixedArchive;
			char IsDownloadContent;
			char m_Field2A; //padding
			char m_Field2B; //padding
			Sonicteam::SoX::ArcFile m_ArcFile; //0x2C
			boost::shared_ptr<Sonicteam::ArcFileHandleE3> m_Field44;
			uint32_t m_Field48;
			uint32_t m_Field4C;
			uint32_t m_Field50;

		};
	};
};

#endif