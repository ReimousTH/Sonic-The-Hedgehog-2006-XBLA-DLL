#ifndef Sonicteam__SOX__Graphics__FRAMEBUFFEROBJECT
#define Sonicteam__SOX__Graphics__FRAMEBUFFEROBJECT

#include "Defs.h"
#include <Sox/IResource.h>
#include <Sox/RefCountObject.h>
#include <Marathon.inl>
#include <Sox/Graphics/Xenon/TextureXenon.h>



namespace Sonicteam{
	namespace SoX{
		namespace Graphics{
			class FrameBufferObject:public Sonicteam::SoX::RefCountObject{

				MARATHON_DESTRUCTION_H(FrameBufferObject);;

			};
		};
	};
};
#endif
