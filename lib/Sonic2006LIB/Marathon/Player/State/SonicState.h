#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
    namespace Player
    {
        namespace State
        {
            enum SonicState
            {
                SonicState_Wait = 0,
                SonicState_Walk = 1,
                SonicState_Run = 2,
                SonicState_Fall = 3,
                SonicState_Jump = 4,
                SonicState_Brake = 5,
                SonicState_QuickTurn = 6,
                SonicState_Dead = 7,
                SonicState_Damage = 8,
                SonicState_Push = 0x9,
                SonicState_Damage2 = 0xA,
                SonicState_Grind = 0xB,
                SonicState_Edge = 0xC,
                SonicState_EdgeTemplateCommonFall = 0xD,
                SonicState_EdgeTemplateCommonJump = 0xE,
                SonicState_Ottotto = 0xF,
                SonicState_SpringTemplateType7310 = 0x10,
        
                SonicState_CommonSpringTemplate7320000000 = 0x12,
                SonicState_DashPanel = 0x13,
                SonicState_WideSpring = 0x11,
                SonicState_SpringTemplateTypeC = 0x14,
                SonicState_Goal = 0x15,
                SonicState_Invincible = 0x16,
                SonicState_Wind = 0x17,
                SonicState_Landing = 0x18,
                SonicState_DontMove = 0x19,
                SonicState_HoldObjectTemplate_571000 = 0x1A,
                SonicState_HoldObjectTemplate_622000 = 0x1D,
                SonicState_ChainJump = 0x1E,
                SonicState_RainbowRing = 0x1F,
                SonicState_WallLanding = 0x20,
                SonicState_OnPathColli = 0x21,
             
                SonicState_Talk = 0x24,
                SonicState_PsiCatched = 0x25,
                SonicState_CommonSpringTemplate732000000 = 0x26,
                SonicState_Stun = 0x27,
                SonicState_Rodeo = 0x28,
                SonicState_Balancer = 0x29,
                SonicState_WaterSlider = 0x2A,
                SonicState_Amigo = 0x2F,
                SonicState_AmigoChase = 0x30,
                SonicState_GemThrow = 0x2D,
                SonicState_GemSnipe = 0x2E,
         
                SonicState_BlastJump = 0x23,
                SonicState_HomingSmash = 0x3F,
                SonicState_Homing = 0x40,
                SonicState_HomingAfter = 0x41,
                SonicState_Sliding = 0x42,
                SonicState_Attack = 0x43,
                SonicState_SpinDash = 0x44,
                SonicState_Bound = 0x45,
                SonicState_LightDash = 0x46,
                SonicState_MachSpeed = 0x47,
                SonicState_TornadoGround = 0x48,
                SonicState_TornadoAir = 0x49,
                SonicState_SonicSuper = 0x4B
            };
        }
    }
}