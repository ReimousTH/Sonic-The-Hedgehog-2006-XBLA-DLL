#pragma once
#include <xtl.h>
#include <Core2/Hook.h>
#include <d3d9.h>
#include <xgraphics.h>
#include <Sox/ApplicationXenon.h>
#include <stdio.h>
#include <D3DX9Shader.h>
#include <Core2/DebugFont.h>
#include <DocMarathonImp.h>

namespace Patch
{
    namespace DebugUI
    {
        void RenderDebugUI(D3DDevice* pDevice, double delta);

        void INSTALL();
    }
}