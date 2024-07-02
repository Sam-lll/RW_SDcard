#include "stdafx.h"



// #ifndef zssLogINFO
// #	define zssLogINFO	LogInfo
// #endif

// #ifndef zssLogERROR
// #	define zssLogERROR	LogError
// #endif

// #ifndef zssLogWARN
// #	define zssLogWARN	LogWarn
// #endif

// #ifndef zssLogDEBUG
// #	ifdef _DEBUG
// #		define zssLogDEBUG				LogDebug
// #	else // _DEUBG
// #		if _MSC_VER <1300
// #			define zssLogDEBUG			((void)0)
// #		else
// #			define zssLogDEBUG          (__noop)
// #		endif
// #	endif
// #endif

int main()
{
	char *buf = "string";
	unsigned char *str = (unsigned char *)"test";
	ZssLog(str);
// 	while(1)
// 	{
		zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
		zssLogDEBUG("<%s  %d> %s(buf: %s)", __FILE__, __LINE__, __FUNCTION__, buf);
		zssLogERROR("<%s  %d> %s(buf: %s)", __FILE__, __LINE__, __FUNCTION__, buf);
		zssLogWARN("<%s  %d> %s(buf: %s)", __FILE__, __LINE__, __FUNCTION__, buf);
// 	}

	return 0;
}