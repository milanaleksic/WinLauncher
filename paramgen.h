#pragma once

#include "inform.h"
#include "config_reader.h"
#include "debug.h"

class ParameterGenerator {

private:

	const Inform* _inform;

	bool _wellKnownSubfolderFound;

	Debug* _debug;
	
	wostringstream _startParams ;

	ConfigReader* _configReader ;

	void AppendDebugInformation() ;
	void AppendJarLibraries() ;
	void AppendExtraParameters() ;
	void AppendSplashParameter() ;
	void AppendMainClassParameter() ;
	void AppendProgramArgsParameter() ;

	void ChangeDirectoryIfSomeWellKnownFolderIsFound() ;

	int ReadConfigFile() ;

	int ExecuteProgram() ;

	void CreateJavaStartParameters() ;

	void SleepIfRequestedFromOutside() ;

	wstring GetExecutorName() ;

public:

	ParameterGenerator(const HINSTANCE, const HWND) ;

	~ParameterGenerator() ;

	int StartApplication(void) ;

};
