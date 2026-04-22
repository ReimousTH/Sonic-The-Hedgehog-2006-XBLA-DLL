#pragma once

#include <xtl.h>
#include <string>
#include <typeinfo.h>
#include <vector>
#include <map>
#include <sstream>
#include <Core2/Utility.h>

typedef unsigned int uint32_t;

struct CMessage 
{

	float lerp2(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	std::string Data;
	int Count;
	float FrameTime;
	DWORD DefaultColor;
	DWORD TriggerColor;
	DWORD CurrentColor;
	bool IsAnimation;

	void StartAnimation(float duration = 1.0f) {
		FrameTime = 1.0f;
		IsAnimation = true;
	}

	void Lerp(float delta) {
		if (!IsAnimation) return;

		FrameTime -= delta;

		if (FrameTime <= 0.0f) {
			FrameTime = 0.0f;
			IsAnimation = false;
			CurrentColor = DefaultColor; // Reset to default when done
			return;
		}


		BYTE def_r = (DefaultColor >> 16) & 0xFF;
		BYTE def_g = (DefaultColor >> 8) & 0xFF;
		BYTE def_b = DefaultColor & 0xFF;

		BYTE sel_r = (TriggerColor >> 16) & 0xFF;
		BYTE sel_g = (TriggerColor >> 8) & 0xFF;
		BYTE sel_b = TriggerColor & 0xFF;

		CurrentColor = D3DCOLOR_ARGB(
			255,
			static_cast<BYTE>(lerp2(sel_r, def_r, 1.0f - FrameTime)),
			static_cast<BYTE>(lerp2(sel_g, def_g, 1.0f - FrameTime)),
			static_cast<BYTE>(lerp2(sel_b, def_b, 1.0f - FrameTime))
		);
	}

	CMessage() 
	{
		DefaultColor = D3DCOLOR_ARGB(255, 0, 255, 255); // Cyan
		TriggerColor = D3DCOLOR_ARGB(255, 255, 0, 0);   // Red (example)
		CurrentColor = DefaultColor;
	}
};

struct DebugBreakPointData
{
	uint32_t line;
	bool isEnabled;
};

class Debug
{
public:
	static bool ms_EnableDebug;

	static bool ms_EnableDebugUI;

	static bool ms_EnableDebugUI_LOG;

	static bool ms_EnableDebugFile;

	static bool ms_EnableDebugBreakpoint;

	static uint32_t ms_DeviceID;

	static SYSTEMTIME ms_BootSystemTime;

	static std::vector<CMessage> ms_Messages;

	static std::map<const char*, std::map<uint32_t, bool>,DLL_STD_MAP_CONST_CHAR_COMPARE> ms_Breakpoints;

	static float ms_MessageOffset;

	static bool ms_EnableNULLLoader;

	static CRITICAL_SECTION ms_MsgCriticalSection;

	static CRITICAL_SECTION ms_BreakpointCriticalSection;

	static std::vector<void(*)()> ms_DebugUIDrawFunctions;

	static void Initialize();

};

class Logging
{
public:
	static bool ms_LoggingArcLoading;
	static bool ms_LoggingFileArcLoading;
	static bool ms_LoggingFileGraphicsNodeConstructor;
	static bool ms_LoggingThreadDestructor;
	static bool ms_LoggingThreadJoin;
	static bool ms_LoggingCloseHandle;
	static bool ms_LoggingThreadConstructor;
	static bool ms_LoggingFindGraphicsNode;
	static bool ms_LoggingGameImpClearActors;
};

template <typename T>
XOVERLAPPED* HardwareMessage(LPCWSTR Title, T value, XOVERLAPPED* m_Overlapped)
{

	MESSAGEBOX_RESULT result;
	LPCWSTR g_pwstrButtonsX_INL[1] = { L"------------OK----------------" };
	std::wstringstream ss;
	ss << std::hex << value;
	XShowMessageBoxUI(0, Title, ss.str().c_str(), 1, g_pwstrButtonsX_INL, 1, XMB_ERRORICON, &result, m_Overlapped);
	return m_Overlapped;
}

template <typename T>
void HardwareMessage(LPCWSTR Title, T value)
{
	XOVERLAPPED overlap;
	overlap.hEvent = CreateEvent(0, false, false, 0);
	HardwareMessage(Title, value, &overlap);
	WaitForSingleObject(overlap.hEvent, INFINITE);
	CloseHandle(overlap.hEvent);
}

extern void AddMessage(const char* fmt_str, ...);
extern void AddMessage2(bool enableLogging,const char* fmt_str, ...);
extern void HardwareBreakpoint(const char* _file, size_t _line, const char* fmt_str, ...);
extern void AddDebugUIFunction(void(*)());

struct RTTI
{
	uint32_t signature;
	uint32_t offset;
	uint32_t cdOffset;
	std::type_info* pTypeDescriptor;
	void* pClassDescriptor;
};

extern const char* GetClassName(void* pClass);
