#include "stdafx.h"
#include "debug.h"
#include "shellapi.h"
#include <iomanip>
#include <stdarg.h>

Debug::Debug() : _debugTurnedOn(false) {
	int paramCount[] = { 0 };
	_buffer = NULL;
	wchar_t** programArgsArray = CommandLineToArgvW(GetCommandLine(), &paramCount[0]);
	for (int i=1; i<paramCount[0]; i++) {
		if (wcscmp(programArgsArray[i], L"debug") == 0) {
			_debugTurnedOn = true;
			_buffer = _output.rdbuf();
			_buffer->open(FILENAME_DEBUG_OUTPUT, ios::out | ios::trunc);
		}
	}
}

Debug::~Debug() {
	if (_buffer != NULL && _buffer->is_open())
		_buffer->close();
}

void Debug::Log(int cntArgs, ...) {
	if (!_debugTurnedOn)
		return;

	va_list list;
	va_start(list, cntArgs);

	_output << Time() << L"DEBUG: ";
	for (int i=0; i<cntArgs; i++) {
		_output << va_arg(list, TCHAR*);
	}

	va_end(list);
	
	_output << endl;
}

void Debug::Log(const wstring& logLine) {
	if (_debugTurnedOn)
		_output << Time() << L"DEBUG: " << logLine << endl;
}

bool Debug::IsDebugOn() {
	return _debugTurnedOn;
}

wstring Debug::Time() const {
	// using native Windows API date since STL does not use millisecond precision
	SYSTEMTIME st;
    GetSystemTime(&st);

    wstringstream ss;
	ss << setw(2) << setfill(L'0') << st.wHour << L':'
		<< setw(2) << setfill(L'0') << st.wMinute << L':'
		<< setw(2) << setfill(L'0') << st.wSecond << L'.'
		<< setw(3) << setfill(L'0') << st.wMilliseconds << L' ';
	return ss.str();
}
