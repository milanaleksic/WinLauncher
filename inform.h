#pragma once

#include "debug.h"

using namespace std;

class Inform {

private:

	// za upotrebu od strane InformUser i pristup resursima aplikacije
	HINSTANCE _hInst ;
	HWND _hWindow ;

	Debug* _debug ;

public:

	Inform(HINSTANCE, HWND, Debug*);

	void InformUser(int) ;
	void InformUser(int, const TCHAR*) ;

	void LoadResourceString(UINT, LPWSTR, int) ;

	int ShellExec(wstring&, wstring&, bool) ;

};
