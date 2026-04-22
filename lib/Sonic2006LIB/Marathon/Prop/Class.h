


#ifndef Sonicteam_Prop_Class
#define Sonicteam_Prop_Class

#include <Marathon.inl>
#include "PropClass.h"
#include "AllocatedData.h"

#include <Sox/RefCountObject.h>
#include <xtl.h>


namespace Sonicteam { namespace Prop {





	struct ClassPropParamInfo{
	public:
		const char ParamName[0x10];
		unsigned int ParamType;
		unsigned int ParamFlag; // Unknown Param
	};


	struct ClassPropParamData{
	public:
		const char* ClassName;
		unsigned int ClassParamInfoCount;
		ClassPropParamInfo* ClassParamInfo; //array
	};


	class Class:public Sonicteam::SoX::RefCountObject{

	public:
		Class(ClassPropParamData* ClassPropData,REF_TYPE(Sonicteam::Prop::AllocatedData) ClassPropAllocatedData );
		~Class();
		MARATHON_DESTRUCTION_H(Class);
		ClassPropParamData* ClassPropData; // 0x8
		REF_TYPE(Sonicteam::Prop::AllocatedData) ClassPropAllocatedData; //0xC
	};


}};

#endif

