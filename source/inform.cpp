#include "stdafx.h"
#include "Resource.h"
#include "shellapi.h"
#include "inform.h"
#include "debug.h"

Inform::Inform(const HINSTANCE hInst, const HWND hWind, Debug* debug) {
	_hInst = hInst;
	_hWindow = hWind;
	_debug = debug;
}

void Inform::InformUser(int msgId) const {
	TCHAR szMessage[MAX_LOADSTRING];
	TCHAR szAppError[MAX_LOADSTRING];
	
	LoadResourceString(msgId, szMessage, MAX_LOADSTRING);
	LoadResourceString(IDS_APP_ERROR, szAppError, MAX_LOADSTRING);

	swprintf_s(szMessage, MAX_LOADSTRING, L"%s (%d)", szMessage, msgId);
	
	MessageBox(_hWindow, szMessage, szAppError, MB_ICONSTOP | MB_OK);
}

void Inform::InformUser(int msgId, const TCHAR* rest) const {
	TCHAR szMessage[MAX_LOADSTRING];
	TCHAR szAppError[MAX_LOADSTRING];
	
	LoadResourceString(msgId, szMessage, MAX_LOADSTRING);
	LoadResourceString(IDS_APP_ERROR, szAppError, MAX_LOADSTRING);

	wcscat_s(szMessage, MAX_LOADSTRING, rest);

	swprintf_s(szMessage, MAX_LOADSTRING, L"%s (%d)", szMessage, msgId);
	
	MessageBox(_hWindow, szMessage, szAppError, MB_ICONSTOP | MB_OK);
}

void Inform::LoadResourceString(UINT uID, const LPWSTR lpBuffer, int cchBufferMax) const {
	LoadString(_hInst, uID, lpBuffer, cchBufferMax);
}

int Inform::ShellExec(const wstring& executor, const wstring& startParams) const {
	_debug->Log(1, L"------- Shell Exec preparation proceeding");
	SHELLEXECUTEINFO info;
	info.hwnd = _hWindow;


	if (_debug->IsDebugOn()) {
		info.lpFile = L"cmd";
	} else {
		if (!executor.empty())
			info.lpFile = executor.data();
		else
			info.lpFile = DEFAULT_EXECUTOR;
	}

	const TCHAR* startParamsData = startParams.data();
	info.nShow = SW_SHOWNORMAL;
	info.cbSize = sizeof(info);
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpVerb = L"open";
	info.lpParameters = startParamsData;
	info.lpDirectory = NULL;		
	info.dwHotKey = NULL;
	info.hIcon = NULL;
	info.hMonitor = NULL;
	info.hProcess = NULL;
	info.hInstApp = NULL;		
	info.lpClass = NULL;
	info.lpIDList = NULL;

	_debug->Log(5, L"Executing program [", info.lpFile, L"] with parameters [", startParamsData, L"]");
	if (!ShellExecuteEx(&info)) {
		InformUser(IDS_COULDNOTSTART, startParamsData);
		return 1;
	}
	return 0;
}