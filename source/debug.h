#pragma once

#include <fstream>

using namespace std;

class Debug {

private:

	wofstream _output;

	wfilebuf *_buffer;
	
	bool _debugTurnedOn;

	wstring Time() const;

public:

	Debug();

	~Debug();

	void Close();

	//TODO: izazovi problem prilikom upisa u log i provali zasto nakon toga ne radi dobro

	//TODO: ne podrzavas lokalno vreme (gleda UTC+0)

	void Log(int cntArgs, ...) ;
	
	void Log(const wstring&) ;
	
	bool IsDebugOn() ;

};
