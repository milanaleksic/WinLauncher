#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include "inform.h"
#include "debug.h"
#include <map>

using namespace std;

class ConfigReader {

private:

	Inform* _inform;

	Debug* _debug;

	map<wstring, wstring> _parameterMap;

	wstring ConfigReader::TrimString(wstring& str);

	bool LineIsAComment(wstring& str);

public:

	ConfigReader(Inform*, Debug*);
	
	wstring NewExtractConfig(wstring& configName);

	int ReadConfigFile();

};

#endif _CONFIG_READER_H_