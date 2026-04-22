#include "Debug.h"
#include <xtl.h>

bool Debug::ms_EnableDebug = true; 
bool Debug::ms_EnableDebugBreakpoint = true; 
bool Debug::ms_EnableDebugUI = true;
bool Debug::ms_EnableDebugUI_LOG = true;
bool Debug::ms_EnableDebugFile = true;
bool Debug::ms_EnableNULLLoader = true;
SYSTEMTIME Debug::ms_BootSystemTime;
uint32_t Debug::ms_DeviceID = true;
CRITICAL_SECTION Debug::ms_MsgCriticalSection;
CRITICAL_SECTION Debug::ms_BreakpointCriticalSection;
std::vector<CMessage> Debug::ms_Messages;
std::map<const char*, std::map<uint32_t, bool>, DLL_STD_MAP_CONST_CHAR_COMPARE> Debug::ms_Breakpoints;
std::vector<void(*)()> Debug::ms_DebugUIDrawFunctions;
float Debug::ms_MessageOffset;

bool Logging::ms_LoggingArcLoading = true;
bool Logging::ms_LoggingFileArcLoading = true;
bool Logging::ms_LoggingFileGraphicsNodeConstructor = true;
bool Logging::ms_LoggingThreadDestructor = true;
bool Logging::ms_LoggingThreadJoin = true;
bool Logging::ms_LoggingCloseHandle = true;
bool Logging::ms_LoggingThreadConstructor = true;
bool Logging::ms_LoggingFindGraphicsNode = true;
bool Logging::ms_LoggingGameImpClearActors = true;


void Debug::Initialize()
{
	InitializeCriticalSection(&Debug::ms_MsgCriticalSection);
	InitializeCriticalSection(&Debug::ms_BreakpointCriticalSection);
	AddMessage("Debug::Initialize");
}


void AddMessage(std::string& str)
{
	if (!Debug::ms_EnableDebug) return;

	if (Debug::ms_EnableDebugFile)
	{
		// 1. Initialize content data
		XCONTENT_DATA contentData = { 0 };
		lstrcpyW(contentData.szDisplayName, L"log");
		strcpy_s(contentData.szFileName, "log.txt");
		contentData.dwContentType = XCONTENTTYPE_SAVEDGAME;
		contentData.DeviceID = Debug::ms_DeviceID;  // Must be valid!

		HRESULT hr = XContentCreate(0, "save", &contentData, XCONTENTFLAG_OPENALWAYS, NULL, NULL, NULL);

		char bootTimeBuffer[64];
		sprintf(bootTimeBuffer, "[%02d_%02d_%02d_%03d] - ",
			Debug::ms_BootSystemTime.wHour, Debug::ms_BootSystemTime.wMinute, Debug::ms_BootSystemTime.wSecond, Debug::ms_BootSystemTime.wMilliseconds);

		std::string logName = "save:\\" + std::string(bootTimeBuffer) + "log.txt";

		HANDLE hFile = CreateFile(logName.c_str(),
			GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,  // Or CREATE_ALWAYS to overwrite
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			// Get current Xbox 360 system time
			SYSTEMTIME sysTime;
			GetSystemTime(&sysTime);

			// Format time string
			char timeBuffer[64];
			sprintf(timeBuffer, "[%02d:%02d:%02d.%03d] - ",
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

			// Combine time + message
			std::string log_string = timeBuffer + str + "\r\n";

			// Seek to end for appending
			SetFilePointer(hFile, 0, NULL, FILE_END);

			DWORD dwWritten;
			WriteFile(hFile, log_string.c_str(), log_string.size(), &dwWritten, NULL);
			CloseHandle(hFile);
		}
		XContentClose("save", NULL);
	}

	if (!Debug::ms_EnableDebugUI)
		return;

	if (!Debug::ms_EnableDebugUI_LOG)
		return;

	EnterCriticalSection(&Debug::ms_MsgCriticalSection); // Lock

	bool find = false;
	int cc = 0;
	for (std::vector<CMessage>::iterator it = Debug::ms_Messages.begin(); it != Debug::ms_Messages.end(); it++) {
		if (it->Data == str) {
			it->Count++;
			it->StartAnimation();
			find = true;
			CMessage msg = *it;
			if ((Debug::ms_Messages.size() - cc) > 10)
			{
				it = Debug::ms_Messages.erase(it);
				Debug::ms_Messages.push_back(msg);
			}
			break;
		}
		cc++;
	}

	//
	if (!find)
	{
		CMessage _n;
		_n.Count = 1;
		_n.Data = str;
		_n.StartAnimation();
		Debug::ms_Messages.push_back(_n);
	}

	LeaveCriticalSection(&Debug::ms_MsgCriticalSection); // Unlock

}

void AddMessage(const char* fmt_str, ...)
{
	if (!Debug::ms_EnableDebug) return;

	va_list args;
	va_start(args, fmt_str);
	int size = vsnprintf(NULL, 0, fmt_str, args);
	va_end(args);

	if (size <= 0) return;


	char* buffer = new char[size + 1];


	va_start(args, fmt_str);
	vsnprintf(buffer, size + 1, fmt_str, args);
	va_end(args);

	std::string result(buffer);
	delete[] buffer;


	AddMessage(result);
}

void AddMessage2(bool enableLogging, const char* fmt_str, ...)
{
	if (!enableLogging || !Debug::ms_EnableDebug)
		return;

	va_list args;
	va_start(args, fmt_str);
	int size = vsnprintf(NULL, 0, fmt_str, args);
	va_end(args);

	if (size <= 0) return;


	char* buffer = new char[size + 1];


	va_start(args, fmt_str);
	vsnprintf(buffer, size + 1, fmt_str, args);
	va_end(args);

	std::string result(buffer);
	delete[] buffer;

	AddMessage(result);
}

void HardwareBreakpoint(const char* _file, size_t _line, const char* fmt_str, ...)
{
	if (!Debug::ms_EnableDebug)
		return;

	EnterCriticalSection(&Debug::ms_BreakpointCriticalSection);

	std::map<const char*, std::map<uint32_t, bool>, DLL_STD_MAP_CONST_CHAR_COMPARE>::iterator it = Debug::ms_Breakpoints.find(_file);

	if (it == Debug::ms_Breakpoints.end())
	{
		Debug::ms_Breakpoints[_file] = std::map<uint32_t, bool>();
	}

	std::map<uint32_t, bool>::iterator jt = Debug::ms_Breakpoints[_file].find((uint32_t)_line);

	bool isFirstTime = false;
	bool isDisabled = false;

	if (jt == Debug::ms_Breakpoints[_file].end())
	{
		Debug::ms_Breakpoints[_file][(uint32_t)_line] = true;
		isFirstTime = true;
	}
	else
		isDisabled = !Debug::ms_Breakpoints[_file][(uint32_t)_line];


	if (!isDisabled || isFirstTime)
	{
		va_list args;
		va_start(args, fmt_str);
		int size = _vscprintf(fmt_str, args);
		va_end(args);

		if (size <= 0)
		{
			LeaveCriticalSection(&Debug::ms_BreakpointCriticalSection);
			return;
		}

		char* buffer = new char[size + 1];

		va_start(args, fmt_str);
		vsnprintf(buffer, size + 1, fmt_str, args);
		va_end(args);

		std::string result(buffer);
		delete[] buffer;

		std::wstring wResult(result.begin(), result.end());

		XOVERLAPPED xOverlap;
		memset(&xOverlap, 0, sizeof(xOverlap));
		xOverlap.hEvent = CreateEvent(0, false, false, 0);

		std::wstringstream title;
		title << L"Breakpoint [" << _file << L":" << _line << L"]";

		std::wstringstream message;
		message << L"File: " << _file << L"\nLine: " << _line << L"\n\n";
		message << wResult;

		MESSAGEBOX_RESULT xResult;
		LPCWSTR g_pwstrButtonsX_INL[2] = { L"Continue", L"Disable" };
		XShowMessageBoxUI(0, title.str().c_str(), message.str().c_str(), 2, g_pwstrButtonsX_INL, 0, XMB_ERRORICON, &xResult, &xOverlap);

		WaitForSingleObject(xOverlap.hEvent, INFINITE);

		if (xResult.dwButtonPressed == 1)
			Debug::ms_Breakpoints[_file][(uint32_t)_line] = false;

		CloseHandle(xOverlap.hEvent);

	}
	LeaveCriticalSection(&Debug::ms_BreakpointCriticalSection);
}

void AddDebugUIFunction(void(*function)())
{
	Debug::ms_DebugUIDrawFunctions.push_back(function);
}

const char* GetClassName(void* pClass)
{
	RTTI* rtti = *(RTTI**)(*(uint32_t*)pClass - sizeof(void*));
	return rtti->pTypeDescriptor->name();
}
