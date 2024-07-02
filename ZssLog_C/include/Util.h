#pragma once

typedef struct  
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
	int nMilliseconds;

}TIME;

int IniGetProfileStr(const char * const profile, char * AppName, char * KeyName, char * KeyVal);
int IniGetProfileInt(const char * const profile, char * AppName, char * KeyName);
char* AddBackslashEnd(char* pszPath);
void GetTime(TIME *stTime);
void SubTime(int nDay, char* szTimeString);


