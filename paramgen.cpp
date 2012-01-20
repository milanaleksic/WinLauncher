#include "stdafx.h"
#include "Resource.h"
#include <errno.h>
#include "paramgen.h"
#include "debug.h"

ParameterGenerator::ParameterGenerator(HINSTANCE hInst, HWND hWindow) {
	_debug = new Debug();
	_debug->Log(1, L"WinLauncher starting up");
	_inform = new Inform(hInst, hWindow, _debug);
	_configReader = new ConfigReader(_inform, _debug);
}

ParameterGenerator::~ParameterGenerator() {
	delete _debug;
	delete _inform;
	delete _configReader;	
}

/*
Ako hoces da informises korisnika o necemu (kada nije greska u obradi), koristi:
InformUser(IDS_INFO, <tekst koji hoces da posaljes>);
*/
int ParameterGenerator::StartApplication(void) {
	int configFileRead = _configReader->ReadConfigFile();
	if (configFileRead != 0)
		return configFileRead;
	SleepIfRequestedFromOutside();
	CreateJavaStartParameters();
	return ExecuteProgram();
}

// ako se zeli, moguce je odloziti startovanje aplikacije tako sto se (putem environmenta)
// programu prosledi odgovarajuci par key + value, gde je value vreme u ms za cekanje
void ParameterGenerator::SleepIfRequestedFromOutside() {
	wchar_t* buff = new wchar_t[60];

	size_t size = 60;
	memset(buff, 0x00, 60);
	TCHAR envParam[MAX_LOADSTRING];
	_inform->LoadResourceString(IDS_WIN_LAUNCHER_ENV_PARAM_WAIT_ON_STARTUP, envParam, MAX_LOADSTRING);
	_wdupenv_s(&buff, &size, envParam);
	if (size != 0) {
		int interval = _wtoi(buff);
		Sleep(interval);
	}

	delete[] buff;
}

// citamo koji executor koristimo (default je javaw)
wstring ParameterGenerator::GetExecutorName() {
	wstring key = CONFIG_EXECUTOR;
	return _configReader->NewExtractConfig(key);
}

int ParameterGenerator::ExecuteProgram() {
	wstring value = GetExecutorName();
	if (value.empty())
		return 1;
	
	return _inform->ShellExec(value, _startParams.str(), _debug->IsDebugOn());
}

void ParameterGenerator::AppendDebugInformation() {
	if (!_debug->IsDebugOn())
		return;
	
	wstring executor = GetExecutorName();
	if (executor.empty())
		return;
	
	_startParams <<  L" /C \"" <<  executor.data() <<  L"\"";
	_startParams <<  L" >..\\" << FILENAME_DEBUG_STDOUT << L" 2>..\\" << FILENAME_DEBUG_STDERR;
	if (_debug->IsDebugOn())
		_debug->Log(2, L"Debug included. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::AppendJarLibraries() {
	struct _wfinddata_t finddata;
	intptr_t hFile = _wfindfirst(L"..\\lib\\*.jar", &finddata);

	if (hFile == -1)
		_inform->InformUser(IDS_NOTFOUND);

	_startParams <<  L" -cp .";
	do {
		_startParams <<  L";..\\lib\\" <<  finddata.name;
	} while( _wfindnext( hFile, &finddata ) == 0 );

	if (_debug->IsDebugOn())
		_debug->Log(2, L"JAR libraries included. Current startParams: ", _startParams.str().data());

	_findclose(hFile);
}

// default value: show
void ParameterGenerator::AppendSplashParameter() {
	bool showSplash = true;
	wstring key(CONFIG_SHOW_SPLASH);
	wstring value = _configReader->NewExtractConfig(key);
	if (value == L"false")
		return ;

	struct _wfinddata_t finddata;
	intptr_t hFile = _wfindfirst(L"*.jpg", &finddata);

	if (hFile != -1) {
		_startParams <<  L" -splash:" << finddata.name;
		_findclose(hFile);
	}

	if (_debug->IsDebugOn())
		_debug->Log(2, L"Splash included since JPG file has been found. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::AppendMainClassParameter() {
	wstring key(CONFIG_MAIN_CLASS);
	wstring value = _configReader->NewExtractConfig(key);

	if (value.length() > 0) {
		_startParams <<  L" " <<  value.data();
	} else {
		TCHAR szMainClassName[MAX_LOADSTRING];
		_inform->LoadResourceString(IDS_MAINCLASSNAME, szMainClassName, MAX_LOADSTRING);
		_startParams <<  szMainClassName;
	}

	if (_debug->IsDebugOn())
		_debug->Log(2, L"Main class included. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::AppendExtraParameters() {
	wstring key(CONFIG_EXTRA_PARAMS);
	wstring value = _configReader->NewExtractConfig(key);

	if (value.length() > 0) {
		_startParams << L" " << value.data();
	}

	if (_debug->IsDebugOn())
		_debug->Log(2, L"Extra params included. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::AppendProgramArgsParameter() {
	wstring key(CONFIG_PROGRAM_ARGS);
	wstring value = _configReader->NewExtractConfig(key);

	if (value.length() > 0) {
		_startParams << L" " << value.data();
	}

	if (_debug->IsDebugOn())
		_debug->Log(2, L"Program args included. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::CreateJavaStartParameters() {
	_debug->Log(1, L"------- Parameters generation proceeding");

	// UKOLIKO JE DEBUG MOD, POTREBNO JE DODATI PRE I POSLE EKSTRA PARAMETARA
	// DODATNE PARAMETRE ZA CMD PROGRAM, KAKO BI SE PRAVILNO OBRADILO
	// PREUSMERAVANJE ULAZA/IZLAZA
	AppendDebugInformation();
	
	// BIBLIOTEKE u LIB poddirektorijumu
	AppendJarLibraries();
	
	// SPLASH DIREKTORIJUM
	AppendSplashParameter();

	// CITANJE DODATNIH PARAMETARA ZA JVM
	AppendExtraParameters();

	// IME GLAVNE KLASE SE CITA IZ RESURSA UKOLIKO NE POSTOJI PODESAVANJE!
	AppendMainClassParameter();

	// DODAVANJE PARAMETARA SAMOG PROGRAMA
	AppendProgramArgsParameter();
}

int ParameterGenerator::ReadConfigFile() {
	int result = _configReader->ReadConfigFile();

	switch (result) {
		case -1:
			_inform->InformUser(IDS_ERROR_ALREADY_ACTIVE);
			break;
		case EACCES:
			_inform->InformUser(IDS_ERRORACCESS);
			break;
		case ENOENT:
			_inform->InformUser(IDS_ERRORNOTFOUND);
			break;
	}

	return result;
}
