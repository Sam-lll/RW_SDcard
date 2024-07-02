#include "stdafx.h"

char g_logdir[260];
int g_log_enable;
int g_logfile_stamp;
int g_auto_delete;
int g_auto_delete_keep_days;
FILE *g_logfile;			// for WIN32
FILE* g_fp;			        // for LINUX
/**********for STM32**********/
FATFS g_fs;
FIL g_fil;
FILINFO g_fno;
volatile FRESULT f_ret;
/*****************************/
#define MAX_STRING_LEN 512
char msg_buff[MAX_STRING_LEN];

void ZssLog(unsigned char* pszSubName)
{
	const char *tmp = "C:\\zsslog.ini";
	char logdir[260] = {0};
	int auto_delete = 0;
	int keep_days = 0;
	g_log_enable = 0;

#ifdef WIN32
	if (access(tmp, 0) != -1)
	{
		if(IniGetProfileInt(tmp, "ZssConfig", "EnableLog") != -1)
		{
			IniGetProfileStr(tmp, "ZssConfig", "LogDir_WinSNMPDll", logdir);
			sprintf(logdir, "%s\\%s", logdir, pszSubName);
			auto_delete = IniGetProfileInt(tmp, "ZssConfig", "EnableAutoDelete");
			keep_days = IniGetProfileInt(tmp, "ZssConfig", "AutoDeleteKeepDays") < 90 ? IniGetProfileInt(tmp, "ZssConfig", "AutoDeleteKeepDays") : 90;
			Open(logdir, auto_delete, keep_days);
		}
	}
#elif STM32F10X_MD
	sprintf(logdir, "%s", pszSubName);
	Open(logdir, auto_delete, keep_days);
#endif
}

int Open(const char* log_base_dir, int enable_auto_delete, int auto_delete_keep_days)
{
	char pszIt[260] = {0};
	int i = 0, n = 0;
	TIME stTime;
	char tmp[260] = {0};
	int day_sec;
	if(0 != strlen((char*)log_base_dir))
	{
		char szPath[260] = {0};
		char* pszName = strcpy(szPath, log_base_dir);
		AddBackslashEnd(pszName);
		strcpy(pszIt, pszName);
		for (i = 0; pszIt[i]; i++)
		{
			if(pszIt[i] == '\\' || pszIt[i] == '/')
			{
				pszIt[i] = '\0';
#ifdef WIN32
				if(_access(pszIt, 0) == -1)
#elif STM32F10X_MD
                if((f_ret = f_stat(pszIt, &g_fno)) != FR_OK)
#else
				if(access(pszIt, 0) == -1)
#endif
				{
					/*_get_errno(&n);*/
#ifdef WIN32
					if(_mkdir(pszIt) == -1)
#elif STM32F10X_MD
					char str[32];
					itoa(f_ret, str, 10);
					USART_SendFormatString("f_stat error, f_ret: %d\r\n", f_ret);

					f_ret = f_mount(&g_fs, "", 0);
					if(f_ret != FR_OK){
						USART_SendFormatString("f_mount error, f_ret: %d\r\n", f_ret);
						return 0;
					}
					if((f_ret = f_mkdir(pszIt)) != FR_OK)
#else
					if(mkdir(pszIt, 0755) == -1)
#endif
					{
#ifdef STM32F10X_MD
						USART_SendFormatString("f_mkdir error, f_ret: %d\r\n", f_ret);
#endif
						/*_get_errno(&n)*/;
					}
				}
						
				pszIt[i] = '\\'; 
			}
		}
		strcpy(g_logdir, log_base_dir);
	}
	g_auto_delete = enable_auto_delete;
	g_auto_delete_keep_days = auto_delete_keep_days;

	GetTime(&stTime);
	g_logfile_stamp = stTime.nYear * 10000 + stTime.nMonth * 100 + stTime.nDay;
	day_sec = stTime.nHour * 3600 + stTime.nMinute * 60 + stTime.nSecond;

	sprintf(tmp, "%s\\%04d%02d%02d.log", g_logdir, stTime.nYear, stTime.nMonth, stTime.nDay);
#ifdef WIN32
	g_logfile = fopen(tmp, "a+");                                                                    ////////////////////////////////////////////////////////
	if(g_logfile == NULL)
		return 0;
#elif STM32F10X_MD
	f_ret = f_open(&g_fil, tmp, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if(f_ret != FR_OK){
		f_mkdir("f_open error");
		return 0;
	}
#else
	g_fp = fopen(tmp, "a+");                                                                    ////////////////////////////////////////////////////////
	if(g_fp == NULL)
		return 0;
#endif
	g_log_enable = 1;

	return 1;
	
}

void Close()
{
	if (g_log_enable)
	{
		if (g_logfile != NULL)
#ifdef WIN32
			fclose(g_logfile);
#elif STM32F10X_MD
			f_close(&g_fil);
#else
			fclose(g_fp);
#endif
		g_log_enable = 0;
	}
}

char* LogInfo(const char* lpszFormat, ...)
{
	va_list arg;
	if (!g_log_enable)
		return;
	
	va_start(arg, lpszFormat);
	char* ret_str = Logv(CX_LOG_MESSAGE, lpszFormat, arg);
	va_end(arg);

	return ret_str;
}

char* LogError(const char* lpszFormat, ...)
{
	va_list arg;
	if (!g_log_enable)
		return;
	
	va_start(arg, lpszFormat);
	char* ret_str = Logv(CX_LOG_ERROR, lpszFormat, arg);
	va_end(arg);

	return ret_str;
}

char* LogWarn(const char* lpszFormat, ...)
{
	va_list arg;
	if (!g_log_enable)
		return;
	
	va_start(arg, lpszFormat);
	char* ret_str = Logv(CX_LOG_WARNING, lpszFormat, arg);
	va_end(arg);

	return ret_str;
}

char* LogDebug(const char* lpszFormat, ...)
{
	va_list arg;
	if (!g_log_enable)
		return;
	
	va_start(arg, lpszFormat);
	char* ret_str = Logv(CX_LOG_DEBUG, lpszFormat, arg);
	va_end(arg);

	return ret_str;
}

char* LogException(const char* lpszFormat, ...)
{
	va_list arg;
	if (!g_log_enable)
		return;
	
	va_start(arg, lpszFormat);
	char* ret_str = Logv(CX_LOG_EXCEPTION, lpszFormat, arg);
	va_end(arg);

	return ret_str;
}

char* Logv(int type, const char* lpszFormat, va_list arg)
{
	static const char* szType[] = {"Message", "Warning", "Exception", "Error  ", "Debug  "};
	int stamp;
	int day_sec;
	TIME st;
	char szBuffer[MAX_STRING_LEN];
	// char prefix[256]; 
	
	GetTime(&st);
	stamp = st.nYear * 10000 + st.nMonth * 100 + st.nDay;
	day_sec = st.nHour * 3600 + st.nMinute * 60 + st.nSecond;

	CheckChangeFile( stamp, day_sec);
 	// if (g_fp == NULL)
 	// 	return;
	// vsnprintf(szBuffer, strlen(szBuffer), lpszFormat, arg);	//	error format, strlen(szBuffer) maybe always 0.
	vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, arg);
	//ATL::CT2A sz(szBuffer);

	memset(msg_buff, 0, sizeof(msg_buff));
	sprintf(msg_buff, "%04d-%02d-%02d %02d:%02d:%02d.%03d-%07d %s ():%s\r\n", 
		st.nYear, st.nMonth, st.nDay, st.nHour, st.nMinute, st.nSecond, (st.nMilliseconds - st.nMilliseconds % 100) / 100 % 1000, st.nMilliseconds,
		szType[type], szBuffer);
	
	// WriteRecord(prefix);
	WriteRecord(msg_buff);
	// WriteRecord("\n");
#ifdef WIN32
	fflush(g_logfile);
#elif STM32F10X_MD
	f_sync(&g_fil);
#else
	fflush(g_fp);
#endif
	return msg_buff;
}

void CheckChangeFile(/*TIME *st*/int curr_stamp, int day_sec)
{
	
	if (curr_stamp != g_logfile_stamp) 
	{
		TIME st;
		char tmp[260];
	#ifdef WIN32
		fclose(g_logfile);
		g_logfile = NULL;
	#elif STM32F10X_MD
		// f_close(&g_fil);
	#else
		fclose(g_fp);
		g_fp = NULL;
	#endif
		GetTime(&st);
		g_logfile_stamp = curr_stamp;
		
		sprintf(tmp, "%s\\%04d%02d%02d.log", g_logdir, st.nYear, st.nMonth, st.nDay);
#ifdef WIN32
// 		g_logfile = CreateFile(tmp, 
// 			GENERIC_READ | GENERIC_WRITE,
// 			FILE_SHARE_READ,
// 			NULL,
// 			OPEN_ALWAYS,
// 			FILE_ATTRIBUTE_NORMAL,
// 			NULL);
// 		if (g_logfile != INVALID_HANDLE_VALUE)
// 		{
// 			SetFilePointer(g_logfile, 0, NULL, FILE_END);
// 		}
#elif STM32F10X_MD
		f_ret = f_mount(&g_fs, "", 0);
		if(f_ret != FR_OK){
			return;
		}
		f_ret = f_open(&g_fil, tmp, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		if(f_ret != FR_OK){
			return;
		}
#else
		g_fp = fopen(tmp, "a+");
#endif
		if (g_auto_delete)
		{
			CleanFiles();
		}
	}
}

void WriteRecord(const char *line)
{
	int nWrite = 0;
	int len = line ? strlen(line) : 0;
	if (len)
	{
		unsigned long ulWritten = 0;
#ifdef WIN32
		WriteFile(g_logfile, line, len, &ulWritten, NULL);
#elif STM32F10X_MD
		// f_ret = f_open(&g_fil, tmp, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		// if(f_ret != FR_OK){
		// 	f_mkdir("f_open error");
		// 	return;
		// }
		f_ret = f_lseek(&g_fil, g_fil.fsize);
		if(f_ret != FR_OK){
			return;
		}
		f_ret = f_write(&g_fil, line, len, &ulWritten);
		if(f_ret != FR_OK){
			return;
		}
		// f_close(&g_fil);
		// f_sync(&g_fil);
#else
		nWrite = fwrite(line, len, 1, g_fp);
#endif
	}
}

static int rdn(int y, int m, int d) 
{
	if (m < 3)
		y--, m += 12;
	return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
}

static int get_date_distance(int y1, int m1, int d1, int y2, int m2, int d2)
{
	return rdn(y1,m1,d1) - rdn(y2,m2,d2);
}

void CleanFiles()
{
	long	hFind;
	char	TempPath[260] = {0};
	int		i = 0;
	char	time[64] = {0};
	int year, month, day;
	sprintf(TempPath, "%s\\*.log", g_logdir);

	for(i = 0; i < 100; i++)
	{
		SubTime(i, time);
		
		
		if (sscanf(time, "%04d%02d%02d", &year, &month, &day) == 3) 
		{
			int curr_year = g_logfile_stamp / 10000;
			int curr_month = (g_logfile_stamp /100) % 100;
			int curr_day = g_logfile_stamp % 100;
			if (get_date_distance(curr_year, curr_month, curr_day, year, month, day) > g_auto_delete_keep_days)
			{
				char szFile[260];
				strcpy(szFile, g_logdir);
				strcat(szFile, "\\");
				strcat(szFile, time);
				strcat(szFile, ".log");
				remove(szFile);
			}
		}
		
	}
	
}