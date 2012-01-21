#include "stdafx.h"
#include "Resource.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <mbstring.h>
#include "config_reader.h"

ConfigReader::ConfigReader(Inform* inform, Debug* debug) {
	_inform = inform;
	_debug = debug;
}

int ConfigReader::ReadConfigFile() {
	// prelazimo u direktorijum startup (za dev)
	// ukoliko ga nema pokusavamo da predjemo u bin (za deploy)
	// inace, informisemo korisnika o gresci
	if (!SetCurrentDirectory(TEXT("startup"))) {
		if (!SetCurrentDirectory(TEXT("bin"))) {
			wchar_t* t = new wchar_t[500];
			GetCurrentDirectory(500,t);
			
			_inform->InformUser(IDS_BINNOTEXISTING, t);

			delete[] t;
			return 1;
		}
	}

	int fh;
	int noBytesRead;
	int returnValue = 0;
	
	_debug->Log(1, L"------- Configuration File Parsing");

	returnValue = _sopen_s( &fh, ".launcher", _O_RDWR, _SH_DENYNO, _S_IWRITE );

	if (returnValue != 0)
		return returnValue;

	int fileSize = _filelength(fh);

	char *textAscii = new char[fileSize];
	memset(textAscii, 0x00, fileSize);
	
	// ne prepoznaje se pravilno kada se fajl zakljucan iz Jave (tj. ne vrati se problem)
	// tako da se fajl otvori validno, ali se sigurno nista ne moze procitati iz fajla
	// sto mi prepoznajemo kao "-1", sto nije jedan od prijavljenih kodova 
	// koji moze da vrati _sopen_s
	if( !returnValue && (( noBytesRead = _read( fh, textAscii, fileSize ) ) <= 0) )
		returnValue = -1;

	// zatvaramo fajl
	if (fh != -1)
		_close( fh );

	if (returnValue == 0)
		DoProcessReadBuffer(noBytesRead, fileSize, textAscii);
	
	delete [] textAscii;

	return returnValue;
}

void ConfigReader::DoProcessReadBuffer(int noBytesRead, int fileSize, char *textAscii) {
	wchar_t *_text = new wchar_t[fileSize];
	memset(_text, 0x00, fileSize);

	for (int i=0; i<noBytesRead; i++)
		_text[i] = _mbbtombc(textAscii[i]);
	for (int i=noBytesRead; i<fileSize; i++)
		_text[i] = 0x00;

	_debug->Log(3, L"Complete file read: [", _text, L"]");

	wistringstream is(_text, wistringstream::in);
	wstring line;
	while(!is.eof()) {
		getline(is, line);
		if (line.length() != 0) {
			if (LineIsAComment(line)) {
				_debug->Log(3, L"Line [", line.data(), L"] is comment, ommitting");
				continue;
			}
			wstring::size_type pos = line.find(L'=');
			if (pos == wstring::npos) {
				_debug->Log(3, L"Line [", line.data(), L"] is not a proper parameter, ommitting");
				continue;
			}

			wstring paramName = TrimString(line.substr(0, pos));
			wstring paramValue = TrimString(line.substr(pos+1, line.length()));
			_debug->Log(7, L"Line [", line.data(), L"] deduced as a proper parameter: [", paramName.data(), L"]=[", paramValue.data(), L"]");

			_parameterMap[paramName] = paramValue;
		}
	}
	delete [] _text;
}

bool ConfigReader::LineIsAComment(wstring& str) {
	wstring::iterator iter;
	for (iter = str.begin() ; iter < str.end(); iter++)
	{
		if (*iter == L' ' || *iter == L'\t')
			continue;
		if (*iter == L'#')
			return true;
		return false;
	}
	return false;
}

wstring ConfigReader::TrimString(wstring& str) {
	if (str.length() == 0)
		return str;
	int begin = 0, end = str.length()-1;
	while (str[begin] == L' ')
		begin++;
	while (str[end] == L' ')
		end--;
	if (begin<end)
		return str.substr(begin, end-begin+1);
	else
		return str;
}

wstring ConfigReader::NewExtractConfig(wstring& configName) {
	if (_parameterMap.size() == 0) {
		//TODO: throw exception (after introducing exceptions :)
	}
	return _parameterMap[configName];
}
