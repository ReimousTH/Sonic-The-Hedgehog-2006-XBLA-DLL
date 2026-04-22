#pragma once
#ifndef CORE_DEBUG
#define CORE_DEBUG

#include <xtl.h>
#include <sstream>

#include <algorithm>
#include <vector>
#include <string>

#include <map>
#include <queue>
 


struct DMSG_UI;
struct DMSG;


extern std::map<DMSG_UI*,size_t> _Message_UI_;
extern std::vector<DMSG> _Message_;
extern size_t FPS_CAP;

class DebugOptions{
private: //prevent r22, causing null ptr 
	static bool AlwaysRunModLoaderAtStartup;
	static bool EnableDevStuff;
	static bool DebugLogV4HUD;  //(_cntlzw(DebugLogV4HUD) & 0x20) != 0; !DebugLogV4HUD (not operation weird)
	static bool userScrolled;
	static bool HideXNCP;
	static bool showlog;
	static bool showuimsg;
	static bool BreakPoint;
	static bool YKhronoTimeACC;
	



	//extra



	static double scrollOffset;
	static double scroll_acc_min;
	static double scroll_acc;

	public:
	static std::map<std::string,size_t> breakpoint_disabled;

	static CRITICAL_SECTION g_MessageCriticalSection;
	static bool USEMUTEX;




	static bool& GetAlwaysRunModLoaderAtStartup();
	static void SetAlwaysRunModLoaderAtStartup(bool value);

	static bool& GetEnableDevStuff();
	static void SetEnableDevStuff(bool value);

	static bool& GetDebugLogV4HUD();
	static void SetDebugLogV4HUD(bool value);


	
	static bool& GetuserScrolled();
	static void SetuserScrolled(bool value);

	static bool& GetHideXNCP();
	static void SetHideXNCP(bool value);


	static bool& Getshowlog();
	static void Setshowlog(bool value);

	static bool& Getshowuimsg();
	static void Setshowuimsg(bool value);

	static bool& GetBreakPoint();
	static void SetBreakPoint(bool value);


	
	static double& GetscrollOffset();
	static void SetscrollOffset(double value);


	static double& Getscroll_acc_min();
	static void Setscroll_acc_min(double value);

	static double& Getscroll_acc();
	static void Setscroll_acc(double value);



	static bool& GetYKhronoTimeACC();
	static void SetYKhronoTimeACC(bool value);


	static void Initilize();










};


//Change-Default Later






struct DMSG {

	float lerp2(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	std::string Data;
	size_t _c;
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
		if(!IsAnimation) return;

		FrameTime -= delta;

		if(FrameTime <= 0.0f) {
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

	DMSG() {
		DefaultColor = D3DCOLOR_ARGB(255, 0, 255, 255); // Cyan
		TriggerColor = D3DCOLOR_ARGB(255, 255, 0, 0);   // Red (example)
		CurrentColor = DefaultColor;
	}
};



struct DMSG_UI{
	std::wstring Data;
	DWORD pos_x;
	DWORD pos_y;
	DWORD CurrentColor;



public:

	void Zero(){
		ZeroMemory(this,sizeof(DMSG_UI));
	}

	void Release(){
		_Message_UI_.erase(_Message_UI_.find(this));
		delete this;
	}

	void SetColor(DWORD CurrentColor){
		this->CurrentColor = CurrentColor;
	}
	void SetPosX(DWORD posX){
		this->pos_x = posX;
	}
	void SetPosY(DWORD posY){
		this->pos_y = posY;
	}
	void SetPosXY(DWORD posX,DWORD posY){
		SetPosX(posX);
		SetPosY(posY);
	}
	void SetMSG(std::wstring& msg){
		this->Data = msg;

	}
	void SetMSG(const char* fmt_str,...){

		va_list args;
		va_start(args, fmt_str);
		int size = vsnprintf(NULL, 0, fmt_str, args);
		va_end(args);

		if (size <= 0) return;


		char* buffer = new char[size + 1];


		va_start(args, fmt_str); 
		vsnprintf(buffer, size + 1, fmt_str, args);
		va_end(args);




	}

};





void AddMessage(std::string& str);
void ClearMessages();
void AddMessage(const char* fmt_str,...);
void AddMessage2(volatile bool loggingEnable, const char* fmt_str, ...);
void HardwareBreakpoint(const char* _file,size_t _line,const char* fmt_str,...);



template <typename T>
void PushXenonMessage(LPCWSTR Title,T value)
{

	LPCWSTR g_pwstrButtonsX_INL[1] = { L"------------OK----------------" };

	std::wstringstream ss;
	ss <<std::hex << value;

	MESSAGEBOX_RESULT result;
	XOVERLAPPED m_Overlapped; 

	XShowMessageBoxUI(0,Title,ss.str().c_str(),1,g_pwstrButtonsX_INL,1,XMB_ALERTICON,&result,&m_Overlapped);

	
}

template <typename T>
XOVERLAPPED* PushXenonMessage(LPCWSTR Title,T value,XOVERLAPPED* m_Overlapped)
{

	MESSAGEBOX_RESULT result;
	LPCWSTR g_pwstrButtonsX_INL[1] = { L"------------OK----------------" };
	std::wstringstream ss;
	ss <<std::hex << value;
	XShowMessageBoxUI(0,Title,ss.str().c_str(),1,g_pwstrButtonsX_INL,1,XMB_ERRORICON,&result,m_Overlapped);
	return m_Overlapped;
}





template <typename T>
void PushXenonMessage(LPCWSTR Title,T value,MESSAGEBOX_RESULT* oute)
{
	LPCWSTR g_pwstrButtonsX_INL[1] = { L"------------OK----------------" };
	std::wstringstream ss;
	ss <<std::hex << value;

	XShowMessageBoxUI(0,Title,ss.str().c_str(),1,g_pwstrButtonsX_INL,1,XMB_ALERTICON,oute,NULL);

}



#endif