#ifndef SONICTEAM__SPANVERSE__MODELPLUGIN__MODELSTANDARD
#define SONICTEAM__SPANVERSE__MODELPLUGIN__MODELSTANDARD

#include "Defs.h"
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <boost/shared_ptr.hpp>
#include <Spanverse/ModelPlugin/ModelControl.h>

namespace Sonicteam{
	namespace Spanverse{
		namespace ModelPlugin{
			class ModelStandard:Sonicteam::Spanverse::ModelPlugin::ModelControl{
			public:
				ModelStandard();
				~ModelStandard();

				MARATHON_DESTRUCTION_H(ModelStandard);;



			};
		};
	};
};



#endif