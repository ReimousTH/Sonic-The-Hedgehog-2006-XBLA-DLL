#ifndef SONICTEAM__GE1PE__REFCOUNTOBJECT
#define SONICTEAM__GE1PE__REFCOUNTOBJECT

#include <Marathon.inl>
#include <Sox/RefCountObject.h>



#define REF_TYPE_GE1PE(Type) Sonicteam::SoX::RefCountObjContainer<Type>


namespace Sonicteam{
	namespace GE1PE{


	
		//IT same as RefCountObject i think
		class RefCountObject:public Sonicteam::SoX::RefCountObject {
			MARATHON_DESTRUCTION_H(RefCountObject);;
		};

	


	}
}
#endif