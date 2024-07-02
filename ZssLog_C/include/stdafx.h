#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#ifdef WIN32
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
#elif STM32F10X_MD
	// #include <stm32f10x_rtc.h>
	#include "stm32f1xx_hal.h"
	#include "main.h"
	#include "usr_timer.h"
	#include <fatfs.h>
	#include <unistd.h>
	#include <sys/time.h>
	extern RTC_HandleTypeDef hrtc;
	extern volatile uint32_t msTicks;
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif

//#include <dirent.h>
// #include "ff.h"
#include "Zss.ZssLog_C.h"
#include "Util.h"