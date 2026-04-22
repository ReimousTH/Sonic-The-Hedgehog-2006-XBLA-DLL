#pragma once

#include <Core2/NLua.h>
#include <Patch/DebugUI/DebugUI.h>
#include <Patch/ArcFix/ArcFix.h>
#include <Patch/RestoreRetaiRenderFunctions/RestoreGameimp.h>
#include <Patch/RestoreRetaiRenderFunctions/RestoreLUAFunctions.h>
#include <Patch/RestoreRetaiRenderFunctions/RestoreMainMenuTask.h>
#include <Patch/RestoreRetaiRenderFunctions/RestoreSonicContext.h>
#include <Patch/RestoreRetaiRenderFunctions/RestoreThreading.h>
#include <SoX/FileSystemARC.h>

namespace Patch
{
	class Globals
	{
	public:
		static NLua ms_CoreFile;
	};

	NLua Globals::ms_CoreFile;

	void INSTALL()
	{
		
		if (Globals::ms_CoreFile.GetBool("Debug//ms_EnableDebugUI"))
			DebugUI::INSTALL();
		
		if (Globals::ms_CoreFile.GetBool("Patch//ms_ArcFix"))
		{
			ArcFix::Globals::ms_Arcs = Globals::ms_CoreFile.GetTableOfString("ArcFix_ArcList");
			ArcFix::INSTALL();		
		}

		RestoreGameimp::INSTALL();
		RestoreLUAFunctions::INSTALL();
		RestoreMainMenuTask::INSTALL();
		//RestoreSonicContext::INSTALL();
		RestoreThreading::INSTALL();
		
	}
}