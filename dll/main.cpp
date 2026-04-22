#include <xtl.h>
#include <Core2/Hook.h>
#include <Core2/Debug.h>
#include <Core2/Utility.h>
#include <Patch/Common.h>
#include <FileLoaderARC.h>
#include <SoX/FileLoaderNULL.h>
#include <SoX/FileSystemXenon.h>
#include <SoX/FileSystemArc.h>
#include <SoX/Perfomance.h>

typedef unsigned int uint32_t;

static bool CRT_INITIALIZED;

namespace boost 
{
    void throw_exception(const std::exception&) 
    {
    }
}

char* getenv(const char* name)
{
    // Return NULL for all environment variables
    // or implement minimal functionality if needed
    return NULL;
}

void SayHI()
{
    std::wstringstream wlog;
    wlog << "<<Core.exe>> [" << __DATE__ << " ; " << __TIME__ << "]" << " Patch List ";
    XOVERLAPPED overlap;
    overlap.hEvent = CreateEvent(0, false, false, 0);
    HardwareMessage(wlog.str().c_str(), "", &overlap);
    WaitForSingleObject(overlap.hEvent, INFINITE);
    CloseHandle(overlap.hEvent);
}


void main()
{

    SayHI();

    // Initialize Hook Data
    HookNew::SaveBuffer = new std::map<void*, std::vector<HookNew*>>();
    HookNew::SaveBufferVFT = new std::map<void*, std::vector<HookNew*>>();
    HookNew::UseEmulatedAddress = HookNew::IsEmulated();

    // Initialze 06 Static DATA
    Sonicteam::FileLoaderARC* pLoaderARC = &Sonicteam::FileLoaderARC::getInstance((void**)0x82C244C8, (void*)0x821455B8);
    *(Sonicteam::FileLoaderARC**)(0x82D2646C) = pLoaderARC;

    Sonicteam::SoX::FileLoaderNULL::getInstance((void**)0x82C24508, (void*)0x821457B0);
    Sonicteam::SoX::FileSystemXenon* pArcXenon = &Sonicteam::SoX::FileSystemXenon::getInstance((void**)0x82C24FFC, (void*)0x823349C0);
    Sonicteam::SoX::FileSystemArc* pArcSystem =  &Sonicteam::SoX::FileSystemArc::getInstance((void**)0x82D272C4, (void*)0x82594378);
    *(Sonicteam::SoX::FileSystemArc**)(0x82C24C24) = pArcSystem;


    //Unstable
    // Sonicteam::SoX::PerformanceFrequency::getInstance((void**)0x824F6EE0, (void*)0x82D26458);

    SYSTEMTIME sysTime;
    GetSystemTime(&Debug::ms_BootSystemTime);

    // Initialize Debug Data
    Debug::Initialize();
    Patch::Globals::ms_CoreFile.LuaOpen();
    Patch::Globals::ms_CoreFile.LoadFile(std::string("game:\\common\\Core.lua"));
    Debug::ms_EnableDebug = Patch::Globals::ms_CoreFile.GetBool(std::string("Debug//ms_EnableDebug"));
    Debug::ms_EnableDebugUI = Patch::Globals::ms_CoreFile.GetBool(std::string("Debug//ms_EnableDebugUI"));
    Debug::ms_EnableDebugFile = Patch::Globals::ms_CoreFile.GetBool(std::string("Debug//ms_EnableDebugFile"));
    Debug::ms_EnableNULLLoader = Patch::Globals::ms_CoreFile.GetBool(std::string("Debug//ms_EnableNULLLoader"));
    Debug::ms_EnableDebugUI_LOG = Patch::Globals::ms_CoreFile.GetBool(std::string("Debug//ms_EnableDebugUI_LOG"));

    Logging::ms_LoggingArcLoading = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingArcLoading"));
    Logging::ms_LoggingFileGraphicsNodeConstructor = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingFileGraphicsNodeConstructor"));
    Logging::ms_LoggingThreadConstructor = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingThreadConstructor"));
    Logging::ms_LoggingThreadDestructor = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingThreadDestructor"));
    Logging::ms_LoggingThreadJoin = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingThreadJoin"));
    Logging::ms_LoggingCloseHandle = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingCloseHandle"));
    Logging::ms_LoggingGameImpClearActors = Patch::Globals::ms_CoreFile.GetBool(std::string("Logging//ms_LoggingGameImpClearActors"));


    if (Debug::ms_EnableDebugFile)
    {
        ULARGE_INTEGER size = { 0 };
        XOVERLAPPED xov = { 0 };
        XShowDeviceSelectorUI(0, XCONTENTTYPE_SAVEDGAME, XCONTENTFLAG_FORCE_SHOW_UI, size, (PXCONTENTDEVICEID)&Debug::ms_DeviceID, &xov);
    }

    //HardwareBreakpoint(__FILE__, __LINE__, "test");
    Patch::INSTALL();
}

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {

    case DLL_PROCESS_ATTACH:
        main();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;

    }
    return TRUE;
}

static volatile long g_CrtInitLock = 0;
static bool g_CrtInitialized = false;

extern "C" HANDLE WINAPI GetProcessHeap()
{
    // Double-check lock pattern
    if (!g_CrtInitialized) {
        if (InterlockedCompareExchange(&g_CrtInitLock, 1, 0) == 0) {
            // We got the lock
            if (!g_CrtInitialized) { // Check again
                // Initialize CRT
                *(unsigned int*)0x82D358B0 = 0xFFFFFFFF;
                *(unsigned int*)0x82D358B4 = 0xFFFFFFFF;
                BranchTo(0x824AD098,int); // XapiInitProcess
                int rout = BranchTo(0x824ACEF0,int, 1); // XapiCallThreadNotifyRoutines
                BranchTo(0x824AC5F0,int, rout); // XapiPAL50Incompatible
                BranchTo(0x8260D1A8,int); // _mtinit
                BranchTo(0x824ACE78,int); // _rtinit
                BranchTo(0x824ACD98,int, 1); // _cinit

                g_CrtInitialized = true;
            }
            InterlockedExchange(&g_CrtInitLock, 0);
        }
        else {
            // Another thread is initializing, wait
            while (!g_CrtInitialized) {
                Sleep(0);
            }
        }
    }

    return (HANDLE) * (uint32_t*)0x82D2E568;
}
