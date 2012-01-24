#pragma once

#include "inform.h"
#include "config_reader.h"
#include "debug.h"

class ParameterGenerator {

private:

	const Inform* _inform;

	Debug* _debug;
	
	// polje koje cuva parametre (agregiraju se promene unutar ove varijable)
	wostringstream _startParams ;

	ConfigReader* _configReader ;

	// dodavanje individualnih parametara iz konfiguracionog fajla
	void AppendDebugInformation() ;
	void AppendJarLibraries() ;
	void AppendExtraParameters() ;
	void AppendSplashParameter() ;
	void AppendMainClassParameter() ;
	void AppendProgramArgsParameter() ;

	int ReadConfigFile() ;

	int ExecuteProgram() ;

	// kreiranje parametara (najbitnija metoda)
	void CreateJavaStartParameters() ;

	void SleepIfRequestedFromOutside() ;

	wstring GetExecutorName() ;

public:

	// konstruktor 
	ParameterGenerator(const HINSTANCE, const HWND) ;

	// podrazumevani destruktor
	~ParameterGenerator() ;

	// startovanje aplikacije (entry point za klijente)
	int StartApplication(void) ;

};
