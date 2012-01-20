#include "stdafx.h"
#include "Resource.h"
#include "shellapi.h"
#include "inform.h"
#include "debug.h"

Inform::Inform(HINSTANCE hInst, HWND hWind, Debug* debug) {
	_hInst = hInst;
	_hWindow = hWind;
	_debug = debug;
}

void Inform::InformUser(int msgId) {
	TCHAR szMessage[MAX_LOADSTRING];
	TCHAR szAppError[MAX_LOADSTRING];
	
	LoadResourceString(msgId, szMessage, MAX_LOADSTRING);
	LoadResourceString(IDS_APP_ERROR, szAppError, MAX_LOADSTRING);

	swprintf_s(szMessage, MAX_LOADSTRING, L"%s (%d)", szMessage, msgId);
	
	MessageBox(_hWindow, szMessage, szAppError, MB_ICONSTOP | MB_OK);
}

void Inform::InformUser(int msgId, const TCHAR* rest) {
	TCHAR szMessage[MAX_PARAMSTRING];
	TCHAR szAppError[MAX_LOADSTRING];
	
	LoadResourceString(msgId, szMessage, MAX_LOADSTRING);
	LoadResourceString(IDS_APP_ERROR, szAppError, MAX_LOADSTRING);

	wcscat_s(szMessage, MAX_PARAMSTRING, rest);

	swprintf_s(szMessage, MAX_PARAMSTRING, L"%s (%d)", szMessage, msgId);
	
	MessageBox(_hWindow, szMessage, szAppError, MB_ICONSTOP | MB_OK);
}

void Inform::LoadResourceString(UINT uID, LPWSTR lpBuffer, int cchBufferMax) {
	LoadString(_hInst, uID, lpBuffer, cchBufferMax);
}

int Inform::ShellExec(wstring& podesavanje, wstring& _startParams, bool debugMode) {
	_debug->Log(1, L"------- Shell Exec preparation proceeding");
	// priprema za WinAPI Shell komandu za pokretanje programa java(w)+parametri
	SHELLEXECUTEINFO info;
	info.hwnd = _hWindow;


	if (debugMode) {
		// DEBUG MOD RADA PROGRAMA GDE SE KREIRAJU STDOUT I STDERR FAJLOVI
		info.lpFile = L"cmd";

	} else {
		// NORMALAN MOD RADA PROGRAMA
		if (podesavanje.empty())
			info.lpFile = podesavanje.data();
		else
			info.lpFile = L"javaw";
	}

	const TCHAR* startParamsData = _startParams.data();
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

	// konacno, pokretanje programa
	_debug->Log(5, L"Executing program [", info.lpFile, L"] with parameters [", startParamsData, L"]");
	if (!ShellExecuteEx(&info)) {
		InformUser(IDS_COULDNOTSTART, startParamsData);
		return 1;
	}
	return 0;
}