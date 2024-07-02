#include "stdafx.h"

int IniGetProfileStr(const char * const profile, char * AppName, char * KeyName, char * KeyVal)
{
	char appname[32] = {0}, keyname[32] = {0};
	char buf[128], *pValueOut;
	FILE *fp;
	int found=0;
	if( (fp=fopen( profile,"r" )) == NULL )
		return -1;
	fseek( fp, 0, SEEK_SET );
	sprintf(appname,"[%s]", AppName);
	memset(keyname, 0, sizeof(keyname));
	while(!feof(fp) && fgets(buf, 128, fp) != NULL)
	{
		if(found == 0)
		{
			if(buf[0] != '[')
			{
				continue;
			}
			else if (strncmp(buf, appname, strlen(appname)) == 0)
			{
				found = 1;
				continue;
			}
		}
		else if(found == 1)
		{
			if(buf[0] == '#')
			{
				continue;
			}
			else if (buf[0] == '[')
			{
				break;
			}
			else
			{
				if((pValueOut = (char*)strchr(buf, '=')) == NULL)
					continue;
				memset(keyname, 0, sizeof(keyname));
				sscanf(buf, "%[^=]", keyname);
				if(strcmp(keyname, KeyName) == 0)
				{
					sscanf(++pValueOut, "%[^\n]", KeyVal);
					found = 2;
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}
	fclose(fp);
	if(found == 2)
		return 0;
	else
		return -1;
}


int IniGetProfileInt(const char * const profile, char * AppName, char * KeyName)
{
	char appname[32] = {0}, keyname[32] = {0}, KeyVal[32] = {0};
	char buf[128],*pValueOut;
	FILE *fp;
	int found=0;
	if( (fp=fopen( profile,"r" ))==NULL )
		return -1;
	fseek( fp, 0, SEEK_SET );
	sprintf(appname,"[%s]", AppName);
	memset(keyname, 0, sizeof(keyname));
	while(!feof(fp) && fgets(buf, 128, fp) != NULL)
	{
		if(found == 0)
		{
			if(buf[0] != '[')
			{
				continue;
			}
			else if (strncmp(buf, appname, strlen(appname)) == 0)
			{
				found = 1;
				continue;
			}
		}
		else if(found == 1)
		{
			if(buf[0] == '#')
			{
				continue;
			}
			else if (buf[0] == '[')
			{
				break;
			}
			else
			{
				if((pValueOut = (char*)strchr(buf, '=')) == NULL)
					continue;
				memset(keyname, 0, sizeof(keyname));
				sscanf(buf, "%[^=]", keyname);
				if(strcmp(keyname, KeyName) == 0)
				{
					//sscanf(++pValueOut, "%[^\r]", KeyVal);
					sscanf(++pValueOut, "%s", KeyVal);
					found = 2;
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}
	fclose(fp);
	if(found == 2)
		return atoi(KeyVal);
	else
		return -1;
}

char* AddBackslashEnd(char* pszPath)
{
	if(pszPath[strlen(pszPath) - 1] != '\\')
		pszPath[strlen(pszPath)] = '\\';

	return pszPath;
}

void GetTime(TIME *stTime)
{
	time_t tmpcal_ptr;  
	struct tm *tmp_ptr = NULL; 
	memset(stTime, 0x00, sizeof(TIME));
#ifdef WIN32
	time(&tmpcal_ptr);
#elif STM32F10X_MD
	// tmpcal_ptr = RTC_GetCounter() + 8 * 60 * 60;		//读取RTC的CNT，获取当前的秒计数器
	RTC_TimeTypeDef sTime = {0};
  	RTC_DateTypeDef sDate = {0};
	HAL_StatusTypeDef hal_stat = HAL_ERROR;
	hal_stat = HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  	if(hal_stat != HAL_OK)
  	{
  	  return;
  	}
  	hal_stat = HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  	if(hal_stat != HAL_OK)
  	{
  	  return;
  	}
	tmp_ptr = (struct tm*)malloc(sizeof(struct tm));
	tmp_ptr->tm_year = sDate.Year;
	tmp_ptr->tm_mon = sDate.Month;
	tmp_ptr->tm_mday = sDate.Date;
	tmp_ptr->tm_hour = sTime.Hours;
	tmp_ptr->tm_min = sTime.Minutes;
	tmp_ptr->tm_sec = sTime.Seconds;
#else
	struct timeval temp;
	gettimeofday( &temp, NULL );
	stTime->nMilliseconds = temp.tv_usec;
	tmp_ptr = localtime(&tmpcal_ptr); 
#endif

#ifdef STM32F10X_MD
	stTime->nYear = 1900+tmp_ptr->tm_year;
	stTime->nMonth = tmp_ptr->tm_mon;
	stTime->nMilliseconds = HAL_GetTick();
#else	
	stTime->nYear = 1900+tmp_ptr->tm_year;
	stTime->nMonth = 1+tmp_ptr->tm_mon;
#endif
	stTime->nDay =  tmp_ptr->tm_mday;
	stTime->nHour = tmp_ptr->tm_hour;
	stTime->nMinute = tmp_ptr->tm_min;
	stTime->nSecond =  tmp_ptr->tm_sec;
#ifdef STM32F10X_MD
	free(tmp_ptr);
#endif

}
void SubTime(int nDay, char* szTimeString)
{
	struct tm* timeinfo = {0};  
	char szFile[64] = {0};
	int nSec = nDay * 24 * 60 * 60; 

	time_t curTime;  
	time(&curTime);  

	 
	curTime -= nSec;  

	/*! ��ȡ��ǰϵͳʱ��*/   
	timeinfo = localtime(&curTime);
	memset(szFile, 0, sizeof szFile);  
	/*!  ��װʱ��*/  
	sprintf(szFile, "%04d%02d%02d",   
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);  
	strcpy(szTimeString, szFile);
}
