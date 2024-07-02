#pragma once

// dll exports interface, you must use /DZSSLOG_EXPORTS in makefile.
#ifdef ZSSLOG_EXPORTS
#define ZSSLOG_API __declspec(dllexport)
#else
#define ZSSLOG_API __declspec(dllimport)
#endif

enum EnumType{
	CX_LOG_MESSAGE = 0,
	CX_LOG_WARNING,
	CX_LOG_EXCEPTION,
	CX_LOG_ERROR,
	CX_LOG_DEBUG
};

#ifndef zssLogINFO
#	define zssLogINFO	LogInfo
#endif

#ifndef zssLogERROR
#	define zssLogERROR	LogError
#endif

#ifndef zssLogWARN
#	define zssLogWARN	LogWarn
#endif

#ifndef zssLogDEBUG
#	ifdef _DEBUG
#		define zssLogDEBUG				LogDebug
#	else // _DEUBG
#		if _MSC_VER <1300
#			define zssLogDEBUG			((void)0)
#		else
#			define zssLogDEBUG          (__noop)
#		endif
#	endif
#endif
//
//
//
int 	Open(const char* log_base_dir, int enable_auto_delete, int auto_delete_keep_days);
void 	Close();
char*	LogInfo(const char* lpszFormat, ...);
char* 	LogError(const char* lpszFormat, ...);
char* 	LogWarn(const char* lpszFormat, ...);
char* 	LogDebug(const char* lpszFormat, ...);
char* 	LogException(const char* lpszFormat, ...);
void 	ZssLog(unsigned char* pszSubName);
char* 	Logv(int type, const char* lpszFormat, va_list arg);
void 	CheckChangeFile(/*const char* &st*/ int curr_stamp, int day_sec);
void 	WriteRecord(const char *line);
void 	CleanFiles();