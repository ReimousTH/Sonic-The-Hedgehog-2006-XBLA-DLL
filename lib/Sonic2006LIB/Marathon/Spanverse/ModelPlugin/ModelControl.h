#ifndef SONICTEAM__SPANVERSE__MODELPLUGIN__MODELCONTROL
#define SONICTEAM__SPANVERSE__MODELPLUGIN__MODELCONTROL

#include "Defs.h"
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <boost/shared_ptr.hpp>
#include <Sox/Graphics/InstanceModelCommon.h>


namespace Sonicteam{
	namespace Spanverse{
		namespace ModelPlugin{
			class ModelControl{
			public:
				ModelControl();
				~ModelControl();

				REF_TYPE(Sonicteam::SoX::Graphics::Model) Model; //0x4
				MARATHON_DESTRUCTION_H(ModelControl);;
				virtual REF_TYPE(Sonicteam::SoX::Graphics::InstanceModelCommon) GetModelInstance1(); 
				virtual REF_TYPE(Sonicteam::SoX::Graphics::InstanceModelCommon) GetModelInstance2();
				virtual void ModelControl8();


			};


		};
	};
};



#endif