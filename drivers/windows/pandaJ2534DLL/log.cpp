#include "stdafx.h"
#include "shlwapi.h"
#include <Shlobj.h> 
#include "synchronize.h"
#include <string>
#include "log.h"

#pragma comment(lib, "shlwapi")

static Mutex logger_mutex;
static char log_path[MAX_PATH];

void log_init(const char *file_name) {
	logger_mutex.lock();
	SHGetFolderPathA(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, log_path);
	PathAppendA(log_path, file_name);
	logger_mutex.unlock();
}

void logA(const char* format, ...)
{
	logger_mutex.lock();
	char buf[1024];
	va_list args;
	va_start(args, format);
	vsprintf_s(buf, format, args);
	va_end(args);
	char path[MAX_PATH];
	FILE *fp = NULL;
	fopen_s(&fp, log_path, "a+");
	std::string str(buf);
	str += '\n';
	fputs(str.c_str(), fp);
	fclose(fp);
	logger_mutex.unlock();
}

