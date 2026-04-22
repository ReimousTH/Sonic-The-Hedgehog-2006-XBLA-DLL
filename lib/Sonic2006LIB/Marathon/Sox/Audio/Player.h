#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Audio
        {
            class Cue;

            class Player
            {
            public:
                MARATHON_DESTRUCTION_H(Player);
                virtual void Func4() = 0;
                virtual void Func8() = 0;
                virtual void FuncC() = 0;
                virtual void Func10() = 0;
                virtual uint32_t GetSoundBankID(const char* soundBank) = 0;
                virtual void Func18() = 0;
                virtual uint32_t GetSoundCueID(uint32_t soundBankID,const char* soundCue) = 0;
                virtual void Func20() = 0;
                virtual void Func24() = 0;
                virtual void Func28() = 0;
                virtual void Func2C() = 0;
                virtual SoX::RefSharedPointer<Cue> Play(uint32_t soundCueID) = 0;
                virtual void Func34() = 0;
                virtual void Func38() = 0;
                virtual void Func3C() = 0;
                virtual void Func40() = 0;
                virtual void Func44() = 0;
                virtual void Func48() = 0;
                virtual void Func4C() = 0;
            };

            MARATHON_ASSERT_OFFSETOF(Player, m_pVftable, 0x00);
            MARATHON_ASSERT_SIZEOF(Player, 4);
        }
    }
}