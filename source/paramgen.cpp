#include "stdafx.h"
#include "Resource.h"
#include <errno.h>
#include "paramgen.h"
#include "debug.h"

ParameterGenerator::ParameterGenerator(HINSTANCE hInst, HWND hWindow) : _wellKnownSubfolderFound(false) {
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
	ChangeDirectoryIfSomeWellKnownFolderIsFound();
	int configFileRead = ReadConfigFile();
	if (configFileRead != 0)
		return configFileRead;
	SleepIfRequestedFromOutside();
	CreateJavaStartParameters();
	return ExecuteProgram();
}

void ParameterGenerator::ChangeDirectoryIfSomeWellKnownFolderIsFound() {
	if (_debug->IsDebugOn()) {
		wchar_t* currentDir = new wchar_t[MAX_PATH_LENGTH];
		memset(currentDir, 0x00, MAX_PATH_LENGTH);
		GetCurrentDirectory(MAX_PATH_LENGTH, currentDir);
		_debug->Log(2, L"Current directory is: ", currentDir);
		delete[] currentDir;
	}

	wchar_t* wellKnownDirectories[] = { L"startup", L"bin" };
	for (int i=0; i < sizeof(wellKnownDirectories)/sizeof(wellKnownDirectories[0]); i++) {
		if (SetCurrentDirectory(wellKnownDirectories[i])) {
			_wellKnownSubfolderFound = true;
			_debug->Log(2, L"Well-known subfolder found, setting startup dir to it: ", wellKnownDirectories[i]);
			return;
		}
	}
}

// ako se zeli, moguce je odloziti startovanje aplikacije tako sto se (putem environmenta)
// programu prosledi odgovarajuci par key + value, gde je value vreme u ms za cekanje
void ParameterGenerator::SleepIfRequestedFromOutside() {
	wchar_t* buff = new wchar_t[60];
	size_t size = 60;
	memset(buff, 0x00, 60);

	_wdupenv_s(&buff, &size, ENV_PARAM_WAITONSTARTUP);
	if (size != 0) {
		int interval = _wtoi(buff);
		Sleep(interval);
	}

	delete[] buff;
}

wstring ParameterGenerator::GetExecutorName() {
	wstring key = CONFIG_EXECUTOR;
	return _configReader->NewExtractConfig(key);
}

int ParameterGenerator::ExecuteProgram() {
	wstring executor = GetExecutorName();
	if (executor.empty())
		return 1;
	
	return _inform->ShellExec(executor, _startParams.str());
}

void ParameterGenerator::AppendDebugInformation() {
	if (!_debug->IsDebugOn())
		return;
	
	wstring executor = GetExecutorName();
	if (executor.empty())
		return;
	
	_startParams <<  L" /C \"" <<  executor.data() <<  L"\""
		<< L" >"  << (_wellKnownSubfolderFound ? L"..\\" : L"") << FILENAME_DEBUG_STDOUT 
		<< L" 2>" << (_wellKnownSubfolderFound ? L"..\\" : L"") << FILENAME_DEBUG_STDERR;
	if (_debug->IsDebugOn())
		_debug->Log(2, L"Debug included. Current startParams: ", _startParams.str().data());
}

void ParameterGenerator::AppendClasspath() {
	_startParams <<  L" -cp .";

	wstring extraClassPath = GetExtraClasspathDirParameter();
	if (extraClassPath.length()>0)
		_startParams <<  L";" << (_wellKnownSubfolderFound ? L"..\\" : L"") << extraClassPath;

	struct _wfinddata_t finddata;
	intptr_t hFile = _wellKnownSubfolderFound 
		? _wfindfirst(L"..\\lib\\*.jar", &finddata)
		: _wfindfirst(L"lib\\*.jar", &finddata);

	if (hFile == -1)
		_inform->InformUser(IDS_NOTFOUND);

	do {
		_startParams << L";" << (_wellKnownSubfolderFound ? L"..\\" : L"") << L"lib\\" <<  finddata.name;
	} while( _wfindnext( hFile, &finddata ) == 0 );

	if (_debug->IsDebugOn())
		_debug->Log(2, L"JAR libraries included. Current startParams: ", _startParams.str().data());

	_findclose(hFile);
}

// default value: show
void ParameterGenerator::AppendSplashParameter() {
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
	
	_startParams <<  L" "
		<< (value.length() > 0 ? value.data() : DEFAULT_MAINCLASS);

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

wstring ParameterGenerator::GetExtraClasspathDirParameter() {
	wstring key(CONFIG_EXTRA_CLASSPATH_DIR);
	wstring extraClassPath = _configReader->NewExtractConfig(key);

	if (extraClassPath.length() > 0) {
		if (_debug->IsDebugOn())
			_debug->Log(2, L"Extra Classpath dir included: ", extraClassPath.data());
		return extraClassPath;
	}
	return L"";
}

void ParameterGenerator::CreateJavaStartParameters() {
	_debug->Log(1, L"------- Parameters generation proceeding");
	AppendDebugInformation();
	AppendClasspath();
	AppendSplashParameter();
	AppendExtraParameters();
	AppendMainClassParameter();
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
