#include "stdafx.h"
#include "WinLauncher.h"
#include "shellapi.h"
#include "paramgen.h"
#include <tchar.h>

HINSTANCE hInst;
HWND hWindow;

int EntryPoint() {
	ParameterGenerator* paramGenerator = new ParameterGenerator(hInst, hWindow);
	int ret = paramGenerator->StartApplication();
	delete paramGenerator;
	return ret;
}

// setting up the current directory to the one where EXE is positioned
void SetCurrentDirectoryToExePosition() {
	int paramCount[] = { 0 };
	wchar_t** programArgsArray = CommandLineToArgvW(GetCommandLine(),&paramCount[0]);
	wchar_t* programName = programArgsArray[0];


	// if you sent "nochangedir" argument to WinLauncher than there will be no changing of current directory
	for (int i=1; i<*paramCount; i++) {
		if (wcscmp(programArgsArray[i], PARAM_NOCHANGEDIR)==0)
			return;
	}
	
	wchar_t* programDirName = new wchar_t[wcslen(programName)];
	wmemset(programDirName, 0x00, wcslen(programName));

	wchar_t* end = wcsrchr(programName, '\\');

	wmemcpy(programDirName, programName, end-programName);

	SetCurrentDirectory(programDirName);

	delete[] programDirName;
}

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE, TCHAR*);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				InitInstance(HINSTANCE, TCHAR*);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	TCHAR szWindowClass[MAX_LOADSTRING];
	LoadString(hInstance, IDC_WINLAUNCHER, szWindowClass, MAX_LOADSTRING);
	
	MyRegisterClass(hInstance, szWindowClass);

	// Perform application initialization:
	if (!InitInstance (hInstance, szWindowClass))
	{
		return FALSE;
	}

	hInst  = hInstance;

	SetCurrentDirectoryToExePosition();

	int ret = EntryPoint();

	DestroyWindow(hWindow);

	return ret;
}

ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR* szWindowClass)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINLAUNCHER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINLAUNCHER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WINLAUNCHER));

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL InitInstance(HINSTANCE hInstance, TCHAR* szWindowClass)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable
	TCHAR szTitle[MAX_LOADSTRING];
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_DISABLED,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_HIDE);
	hWindow = hWnd;   

	return TRUE;
}
