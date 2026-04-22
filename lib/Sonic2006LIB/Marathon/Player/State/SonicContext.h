#pragma once
#include "CommonContext.h"
#include "..\Gauge\SonicGauge.h"
#include "..\IExportPostureRequestFlag.h"
#include "..\IExportWeaponRequestFlag.h"
#include "..\Score.h"
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <xtl.h>
#include <Marathon.inl>

enum PostureFlag
{
	// The player is grounded.
	PostureFlag_Grounded = 0x01,

	// The player is brushing against a wall.
	PostureFlag_WallSide = 0x08,

	// The player is head on against a wall.
	PostureFlag_WallFront = 0x10,

	// The player is grinding on a rail.
	PostureFlag_RailGrind = 0x40,

	// The player is in the intermediate state between jumping and falling.
	PostureFlag_FallIntermediate = 0x100,

	// The player is falling.
	PostureFlag_Fall = 0x200,

	// The player is on water collision.
	PostureFlag_Water = 0x800,

	// The player is light dashing.
	PostureFlag_LightDash = 0x4000,

	// The player is rotating in a non-forward direction.
	PostureFlag_QuickRotate = 0x8000,

	// The player is on tentative collision.
	PostureFlag_Tentative = 0x10000,

	// The player is water sliding.
	PostureFlag_WaterSlide = 0x20000,

	// The player is on grass collision.
	PostureFlag_Grass = 0x100000,

	// The player is on dirt collision.
	PostureFlag_Dirt = 0x200000,

	// The player is on stone collision.
	PostureFlag_Stone = 0x400000,

	// The player is on sand collision.
	PostureFlag_Sand = 0x1000000
};

namespace Sonicteam
{
	namespace Player
	{
		namespace State
		{

			class SonicContext:public CommonContext
			{
			public:

				enum GemSprite : uint32_t
				{
					GemSprite_Green = 1,
					GemSprite_Red,
					GemSprite_Blue,
					GemSprite_White,
					GemSprite_Sky,
					GemSprite_Yellow,
					GemSprite_Purple,
					GemSprite_Super
				};

				enum Gem : uint32_t
				{
					Gem_Blue = 1,
					Gem_Red,
					Gem_Green,
					Gem_Purple,
					Gem_Sky,
					Gem_White,
					Gem_Yellow,
					Gem_Super
				};

				MARATHON_INSERT_PADDING(4);
				be<GemSprite> m_CurrentGemSprite;
				uint8_t m_HomingLockOn;
				uint8_t m_DisablePlayerMovement;
				uint8_t m_AntigravityHitBox;
				uint8_t m_Field23B;
				uint8_t m_BoundAttackHitBox;
				uint8_t m_Field240;
				uint8_t m_Shrink;
				uint8_t m_ThunderGuard;
				uint8_t m_Tornado;
				uint8_t m_FPS;
				uint8_t m_ThrowGem;
				uint8_t m_SlowTime;
				uint8_t m_MachAura;
				uint8_t m_GemsEnabled;
				uint8_t m_Field246;
				uint8_t m_Field247;
				Gem m_CurrentGem;
				float c_homing_spd;
				float c_homing_brake;
				float c_sliding_time;
				float c_spindash_spd;
				float c_spindash_time;
				float c_bound_jump_spd_0;
				float c_bound_jump_spd_1;
				float c_lightdash_speed;
				float c_boundjump_jmp;
				float c_boundjump_block;
				float c_attack_brake;
				float c_sliding_speed_min;
				float c_sliding_speed_max;
				float c_homing_smash_charge;
				float c_custom_action_slow_time;
				float c_custom_action_machspeed_acc;
				float c_custom_action_machspeed_time;
				float c_scale_run_speed_max;
				float c_scale_run_acc;
				float c_scale_walk_speed_max;
				float c_scale_jump_speed;
				float c_scale_jump_block;

				MARATHON_DESTRUCTION_H(SonicContext);




			};

		}

	}
}

