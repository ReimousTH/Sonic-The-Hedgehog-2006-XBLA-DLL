Debug = 
{
	ms_EnableDebug = false,
	ms_EnableDebugFile = false,
	ms_EnableNULLLoader = false,
	ms_EnableDebugUI = false,
	ms_EnableDebugUI_LOG = false,
}

Logging = 
{
	ms_LoggingArcLoading = false,
	ms_LoggingFileArcLoading = false,
	ms_LoggingFileGraphicsNodeConstructor = false,
	ms_LoggingThreadConstructor = false,
	ms_LoggingThreadDestructor = false,
	ms_LoggingThreadJoin = false,
	ms_LoggingCloseHandle= false,
	ms_LoggingGameImpClearActors = false,
	ms_LoggingFindGraphicsNode = false
}

Patch = 
{
	ms_ArcFix = true
}

Config = 
{
	UnlockAllUpgrades = true,
	TransistionToMainMenu = true,
	UseRetailArchives = true
}
-- This Allow to specify which arcs you want to load
-- archives.lua

Retail_Archives = 
{
	"game:\\patch/boot.arc", 
	"game:\\patch/cache_fixed.arc", 
	"game:\\patch/scripts_sonicsuper.arc",
	--"game:\\xenon/archives/cache.arc",  -- not supported correctly
    "game:\\xenon/archives/enemy.arc", 
    "game:\\xenon/archives/event.arc",
    "game:\\xenon/archives/game.arc",
    "game:\\xenon/archives/human.arc",
    "game:\\xenon/archives/object.arc",
    "game:\\xenon/archives/particle.arc",
    "game:\\xenon/archives/player.arc",
    "game:\\xenon/archives/scripts.arc",
    "game:\\xenon/archives/shader.arc",
    --"game:\\xenon/archives/shader_lt.arc",
    "game:\\xenon/archives/sound.arc",
    "game:\\xenon/archives/stage.arc",
    "game:\\xenon/archives/system.arc",
    "game:\\xenon/archives/text.arc",
    "game:\\win32/archives/enemy_data.arc",
    "game:\\win32/archives/event_data.arc",
    "game:\\win32/archives/particle_data.arc",
    "game:\\win32/archives/player_amy.arc",
    "game:\\win32/archives/player_blaze.arc",
    "game:\\win32/archives/player_common.arc",
    "game:\\win32/archives/player_knuckles.arc",
    "game:\\win32/archives/player_omega.arc",
    "game:\\win32/archives/player_princess.arc",
    "game:\\win32/archives/player_rouge.arc",
    "game:\\win32/archives/player_shadow.arc",
    "game:\\win32/archives/player_silver.arc",
    "game:\\win32/archives/player_sonic.arc",
    "game:\\win32/archives/player_supershadow.arc",
    "game:\\win32/archives/player_supersilver.arc",
    "game:\\win32/archives/player_supersonic.arc",
    "game:\\win32/archives/player_tails.arc",
    "game:\\win32/archives/radarmap.arc",
    "game:\\win32/archives/sprite.arc",
    "game:\\win32/archives/stage_aqa_b.arc",
    "game:\\win32/archives/stage_aqa_a.arc",
    "game:\\win32/archives/stage_boss_dr1_dtd.arc",
    "game:\\win32/archives/stage_boss_dr1_wap.arc",
    "game:\\win32/archives/stage_boss_dr2.arc",
    "game:\\win32/archives/stage_boss_dr3.arc",
    "game:\\win32/archives/stage_boss_iblis02.arc",
    "game:\\win32/archives/stage_boss_iblis03.arc",
    "game:\\win32/archives/stage_boss_mefi01.arc",
    "game:\\win32/archives/stage_boss_mefi02.arc",
    "game:\\win32/archives/stage_boss_rct.arc",
    "game:\\win32/archives/stage_boss_solaris.arc",
    "game:\\win32/archives/stage_csc_a.arc",
    "game:\\win32/archives/stage_csc_b.arc",
    "game:\\win32/archives/stage_csc_c.arc",
    "game:\\win32/archives/stage_csc_e.arc",
    "game:\\win32/archives/stage_csc_f.arc",
    "game:\\win32/archives/stage_csc_iblis01.arc",
    "game:\\win32/archives/stage_dtd_a.arc",
    "game:\\win32/archives/stage_dtd_b.arc",
    "game:\\win32/archives/stage_e0003.arc",
    "game:\\win32/archives/stage_e0009.arc",
    "game:\\win32/archives/stage_e0010.arc",
    "game:\\win32/archives/stage_e0012.arc",
    "game:\\win32/archives/stage_e0021.arc",
    "game:\\win32/archives/stage_e0022.arc",
    "game:\\win32/archives/stage_e0023.arc",
    "game:\\win32/archives/stage_e0026.arc",
    "game:\\win32/archives/stage_e0028.arc",
    "game:\\win32/archives/stage_e0031.arc",
    "game:\\win32/archives/stage_e0104.arc",
    "game:\\win32/archives/stage_e0105.arc",
    "game:\\win32/archives/stage_e0106.arc",
    "game:\\win32/archives/stage_e0120.arc",
    "game:\\win32/archives/stage_e0125.arc",
    "game:\\win32/archives/stage_e0206.arc",
    "game:\\win32/archives/stage_e0214.arc",
    "game:\\win32/archives/stage_e0216.arc",
    "game:\\win32/archives/stage_e0221.arc",
    "game:\\win32/archives/stage_e0304.arc",
    "game:\\win32/archives/stage_flc_a.arc",
    "game:\\win32/archives/stage_flc_b.arc",
    "game:\\win32/archives/stage_kdv_a.arc",
    "game:\\win32/archives/stage_kdv_b.arc",
    "game:\\win32/archives/stage_kdv_c.arc",
    "game:\\win32/archives/stage_kdv_d.arc",
    "game:\\win32/archives/stage_rct_a.arc",
    "game:\\win32/archives/stage_rct_b.arc",
    "game:\\win32/archives/stage_tpj_a.arc",
    "game:\\win32/archives/stage_tpj_b.arc",
    "game:\\win32/archives/stage_tpj_c.arc",
    "game:\\win32/archives/stage_twn_a.arc",
    "game:\\win32/archives/stage_twn_b.arc",
    "game:\\win32/archives/stage_twn_c.arc",
    "game:\\win32/archives/stage_twn_d.arc",
    "game:\\win32/archives/stage_wap_a.arc",
    "game:\\win32/archives/stage_wap_b.arc",
    "game:\\win32/archives/stage_wvo_a.arc",
    "game:\\win32/archives/stage_wvo_b.arc"
}

XBLA_Archives = 
{
	"game:\\xenon/archives/title0.arc",
	"game:\\xenon/archives/kdv_a0.arc",
	"game:\\xenon/archives/kdv_a1.arc",
	"game:\\xenon/archives/kdv_a2.arc",
	"game:\\xenon/archives/kdv_a3.arc",
	"game:\\xenon/archives/kdv_a4.arc",
}

ArcFix_ArcList = XBLA_Archives

if (Config.UnlockAllUpgrades == true) then WriteVirtualBytes("0x8216C668","60000000") end
if (Config.TransistionToMainMenu == true) then WriteVirtualBytes("0x82445714","39600006") end
if (Config.UseRetailArchives == true) then ArcFix_ArcList = Retail_Archives end





-- WriteVirtualBytes("0x8257ED94","60000000")
-- WriteVirtualBytes("0x8257EDA0","60000000")
-- WriteVirtualBytes("0x8257EDAC","60000000")
-- WriteVirtualBytes("0x8257EDB8","60000000")
-- WriteVirtualBytes("0x8257EDC4","60000000")
-- WriteVirtualBytes("0x8255A020","38800001")



-- 82185E40 → 821859BC → 82185748 → 82184FE4 → 8216E174 → 82167780 → 82581314 → 821952B4 → 82195314 → 82195CE8 → 8219FE14 → 821B64D0 → 821870E8 → 8216020C → 82237B54 → 8223463C → 82618FB8

-- 824F5298 → 824F5298 → 824F5280 → 82445218 → 8215B8C8 → 8215B53C → 8215B160 → 8215ACC4 → 8214C54C → 82147758 → 824F53F4 → 8216BD04 → 8216B134 → 8216BAE8 → 821744C4 → 821873A0 → 821876F0 → 8214020C → 821FD3CC → 821FD394 → 821FC45C → 82582C30

-- JoinThread WAS CALLED BEFORE RESTART IN RETAIL 

-- 82507128
-- 82507d90
-- 825078f4
-- 82507648

--WriteVirtualBytes("0x8248D3E0","60000000")
-- WriteVirtualBytes("0x8248D3A4","60000000")



--WriteVirtualBytes("0x8203E86C","8236D2F8")
--WriteVirtualBytes("0x8203E870","8236D2F8")
--WriteVirtualBytes("0x8203E874","8236D2F8")
-- WriteVirtualBytes("0x8203E878","8236D2F8")
--WriteVirtualBytes("0x8203E87C","8236D2F8")
--WriteVirtualBytes("0x8203E880","8236D2F8")
-- WriteVirtualBytes("0x8203E884","8236D2F8")
--WriteVirtualBytes("0x8203E888","8236D2F8")
--WriteVirtualBytes("0x8203E88C","8236D2F8")
--WriteVirtualBytes("0x8203E890","8236D2F8")
--WriteVirtualBytes("0x821550A8","60000000") -- Disable Live Handle