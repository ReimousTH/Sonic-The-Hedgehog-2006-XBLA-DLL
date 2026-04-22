#ifndef Sonicteam__SOX__Graphics__INSTANCEMODELCOMMON
#define Sonicteam__SOX__Graphics__INSTANCEMODELCOMMON



#include "Defs.h"
#include <Sox/IResource.h>
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <Sox/Graphics/InstanceModel.h>


namespace Sonicteam{
	namespace SoX{
		namespace Graphics{


	
			//:)
			class InstanceModelCommon:public Sonicteam::SoX::Graphics::InstanceModel 
			{

			public:
				InstanceModelCommon();
				~InstanceModelCommon();

				MARATHON_DESTRUCTION_H(InstanceModelCommon); ;





			};
		};
	}
}

#endif

