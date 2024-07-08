#pragma once
#include <ff.h>

void SetExtFlag(int flag);
int GetExtFlag(void);
void SetlogFlag(int flag);
int GetlogFlag(void);
void SetStartFlag(int flag);
int GetStartFlag(void);
void SetBrakeFlag(int flag);
int GetBrakeFlag(void);
void SetPauseFlag(int flag);
int GetPauseFlag(void);
void SetIncreaseFlag(int flag);
int GetIncreaseFlag(void);
void SetDecreaseFlag(int flag);
int GetDecreaseFlag(void);

void ListFilesAndDirectories(const char* startDir);
void SendLogContent(const char* logPath);
void DiskReset(FIL fil, FATFS fs, const char* logPath);
void WriteFile2Disk(const unsigned char* const strpath, const unsigned char* const strfile);
void GetFiles(const char* path);