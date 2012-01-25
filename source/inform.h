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

	Inform(const HINSTANCE, const HWND, Debug*);

	void InformUser(int) const ;
	void InformUser(int, const TCHAR*) const;

	void LoadResourceString(UINT, const LPWSTR, int) const;

	int ShellExec(const wstring&, const wstring&) const;

};
