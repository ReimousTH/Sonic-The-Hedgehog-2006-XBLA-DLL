#pragma once
#include "..\RefCountObject.h"
#include <xtl.h>
#include <Marathon.inl>

namespace Sonicteam{
	namespace SoX{
		namespace Physics{
			class Shape:public Sonicteam::SoX::RefCountObject
			{
			public:
				//size0x60
				Shape(void);
				~Shape(void);

				unsigned int SFlag; //Shape Type 
				unsigned int SVolume; // w*h*l * FLT_MIN //0xC

				

				MARATHON_DESTRUCTION_H(Shape);
				virtual void ShapeProceedUnk01(__out XMFLOAT4* u1,__out XMFLOAT4* u2) = 0;
				virtual void ShapeInitVolume(); // i guess


			};
		};
	}
}
