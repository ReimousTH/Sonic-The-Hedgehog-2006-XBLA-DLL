<h1>Sonic The Hedgehog 2006 XBLA (DLL)</h1>

<h1>How To Build (DLL)</h1>
<ul>
<li>Move Xbox 360 SDK to thirdparty/XENON_SDK</li>
<li>Wait for visual studio to configure cmake project</li>
<li>Switch to CMake Targets View</li>
<li>Project Sonic2006->DLL_BUILD, Build</li>
<li>Open out/build/xbox-360-release/Core.xex (if the build was successful) copy to xbla game folder</li>
</ul>

<h1>How To Patch xbla default.xex</h1>
<ul>
<li>You need xextool</li>
<li>xextool -e u -c b -o default-binary.xex default.xex</li>
<li>copy default-binary.xex to files/default.xex(yes rename it)</li>
<li>switch to CMake Targets View (in Visual Studio)</li>
<li>Project Sonic2006->patch_xex, Build</li>
<li>Open folder /files and copy everything(even patch folder) but default.xex and font_build and for_360_sdk to xbla game folder</li>
<li>Extra : thirdparty/patch_xex/CMakeLists.txt, find PATCH_CONFIG ( here patch config, patches/*.s (asm files)  )</li>
</ul>

<h1>Info</h1>
<ul>
<li>DLL_VIEW (only for IntelliSense) DO NOT BUILD IT</li>
<li>files/media (font files for debug log in-game)</li>
<li>files/common/Core.lua - DLL Config ...</li>
<li>Patch.ms_ArcFix = true/false - enable/disable arc system patch for xbla also allow to load more arcs if you need to</li>
<li>Config.UseRetailArchives = true/false - (only when Patch.ms_ArcFix = true) allows to load retail archives(you can copy all arcs from retail to xbla and just run it)</li>
<li>Config.TransistionToMainMenu = true/false - (only when Patch.ms_ArcFix = true and Config.UseRetailArchives = true) skips title screen and goes right to main menu)</li>
<li>Debug.ms_EnableDebug = true/false - enable debug features (you dont need it unless you building custom DLL) same goes for all options for Debug.</li>
<li>Logging.?? - (only when Debug.ms_EnableDebug = true and Debug.ms_EnableDebugFile = true)</li>
<li>Logging.ms_EnableDebugUI/ms_EnableDebugUI_LOG - just debug ui with log (do not use, too much memory, it would probably crash)</li>
<li>Where log files? - save folder (for xbox360/xenia) ...</li>
<li>xenia - default, documents/xenia/content/534587EA/00000001/log.txt/*.txt files</li>
</ul>

<h1>Info for XBLA</h1>
<ul>
<li>Game may crash, cuz its XBLA, did fixes as much it was possible</li>
<li>Game cutscene code for some reason does not work, but you can skip hit (hit start - plank black screen, or just infinity loading)</li>
<li>Game may crash(in-game) MainMenu->Extras->Audio Room/Theater Room, you need move .xncp to other path (.../sound/sound.xncp ??, here you all with yourself)</li>
<li>Going from hub->main_menu may crash game, because of background (Add to Core.lua -> WriteVirtualBytes("0x8248D3A4","60000000") )</li>
<li>Crisis City (fast section) may crash during goal/result -> main_menu</li>
<li>Radical Train also may crash near trains (sync issues)</li>
<li>If you want to improve perfomance, you need build DLL, then remove code for ....</li>
<li>// Disable Frame Syncronizer at all (may become unstable without it)</li>
<li>// No Frustrum Threads (may become unstable without it) </li>
<li>// No Frustrum Update Threads (may become unstable without it) </li>
<li>// Restore Boost Synchronization (may become unstable without it)</li>
</ul>

<h1>Info for Xbox 360 SDK</h1>
<ul>
<li>Before you even ran any .exe from sdk or even first build, for this to work on modern hardware/system, you need to copy (files/for_360_sdk) to thirdparty/XENON_SDK/bin/win32/</li>
<li>If you get an error during the build, that's normal (even if you created a manifest, sometimes Windows just ignores them) .../</li>
<li>sdbinst.exe "C:\path\to\force_link.xml" (files/for_360_sdk)</li>
<li>but you need modify .xml first for specific .exe (cl.exe, lib.exe inside thirdparty/XENON_SDK/bin/win32), specify .exe also path to it</li>
</ul>

<h1>Requirements</h1>
<ul>
<li>Xbox 360 SDK (copy to thirdparty/XENON_SDK)</li>
<li>Microsoft Visual C++ 2005 Redistributable Package</li>
<li>xextool ( for .xex )</li>
<li>Visual Studio 2019+</li>
<li>IDA 9.2.2+ for .idb</li>
<li><a href= "https://angelcode.com/products/bmfont/">Bitmap Font Generator</a></li>
</ul>

<h1>How to Run</h1>
<ul>
<li>Configure (game/media/Core.lua if you want)</li>
<li>Run default-core.xex</li>
<li>Make sure you have core.xex in game folder also all retail files(if you want to run into main menu)</li>
</ul>

</ul>
