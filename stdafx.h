#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define WINLAUNCHER_VERSION_MAJOR 1
#define WINLAUNCHER_VERSION_MINOR 0

#include <windows.h>
#include <io.h>
#include <string.h>
#include <sstream>
#include <iostream>

#define MAX_LOADSTRING 2048
#define MAX_PARAMSTRING 4096
#define MAX_LEN_CONFIG 8192

#define CONFIG_EXECUTOR L"executor"
#define CONFIG_MAIN_CLASS L"mainClass"
#define CONFIG_SHOW_SPLASH L"showSplash"
#define CONFIG_EXTRA_PARAMS L"extra"
#define CONFIG_PROGRAM_ARGS L"programArgs"

#define FILENAME_DEBUG_OUTPUT "debug-output.log"
#define FILENAME_DEBUG_STDOUT L"debug-stdout.log"
#define FILENAME_DEBUG_STDERR L"debug-stderr.log"