#pragma once

#include "../MessageReceiver.h"
#include <SoX/RefCountObject.h>
#include <xtl.h>
#include <Sox/Misc/List.h>
#include <Sox/Memory/IDestructible.h>
#include <Sox/Memory/DEFINITIONS.hpp>

#include <Marathon.inl>
#include <Sox/LinkNode.h>
#include "Defs.h"

#define FrameObserverStepType void*

namespace Sonicteam
{
	namespace SoX
	{
		namespace Graphics
		{
			class Frame;

			class FrameObserver
			{

			public:
				FrameObserver();
				~FrameObserver(void);

				virtual void ObserveFrameStep(FrameObserverStepType)
				{

				}
				MARATHON_DESTRUCTION_H(FrameObserver);;

				SoX::LinkNode<FrameObserver> m_lnFrameObserver;
				Frame* m_pFrameGPStandard;

				void OnRNodeHClear(); //
			};
		}

	}
}
