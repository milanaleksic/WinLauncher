#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include "inform.h"
#include "debug.h"
#include <map>

using namespace std;

class ConfigReader {

private:

	const Inform* _inform;

	Debug* _debug;

	map<wstring, wstring> _parameterMap;

	wstring TrimString(const wstring&);

	bool LineIsAComment(const wstring&);

	void DoProcessReadBuffer(int, int, const char*) ;

	void ChangeDirectoryIfSomeWellKnownFolderIsFound();

public:

	ConfigReader(const Inform*, Debug*);
	
	wstring NewExtractConfig(const wstring&);

	int ReadConfigFile();

};

#endif _CONFIG_READER_H_