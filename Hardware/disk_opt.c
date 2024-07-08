#include "disk_opt.h"
#include "stdafx.h"
#include <assert.h>

extern FILINFO g_fno;
extern FRESULT f_ret;
extern FATFS g_fs;
extern FIL g_fil;
extern uint8_t TxBuffer[TxBuffer_Size];
char* fn;
int extract_flag;
int getlog_flag;
int unlock_flag;
int unlock_flag_step2;
int start_flag;
int brake_flag;
int pause_flag;
int increase_flag;
int decrease_flag;

#if _USE_LFN
	char lfn[64];
#endif

#define MAX_DIRS 10
#define DIR_NAME_LEN 64
char* dirQueue[MAX_DIRS];
char path[DIR_NAME_LEN];

void SetExtFlag(int flag) {
    extract_flag = flag;
}

int GetExtFlag(void) {
    return extract_flag;
}

void SetlogFlag(int flag) {
    getlog_flag = flag;
}

int GetlogFlag(void) {
    return getlog_flag;
}

void SetStartFlag(int flag) {
    start_flag = flag;
}

int GetStartFlag(void) {
    return start_flag;
}

void SetBrakeFlag(int flag) {
    brake_flag = flag;
}

int GetBrakeFlag(void) {
    return brake_flag;
}

void SetPauseFlag(int flag) {
    pause_flag = flag;
}

int GetPauseFlag(void) {
    return pause_flag;
}
void SetIncreaseFlag(int flag) {
    increase_flag = flag;
}

int GetIncreaseFlag(void) {
    return increase_flag;
}

void SetDecreaseFlag(int flag) {
    decrease_flag = flag;
}

int GetDecreaseFlag(void) {
    return decrease_flag;
}

void ListFilesAndDirectories(const char* startDir) {
	zssLogINFO("<%s  %d> %s(startDir: %s)LOG_IN", __FILE__, __LINE__, __FUNCTION__, startDir);
    DIR dir;
    int queueStart = 0, queueEnd = 0;
    dirQueue[queueEnd++] = strdup(startDir); // Add start directory to queue
    // dirQueue[queueEnd++] = startDir; // Add start directory to queue
    while (queueStart < queueEnd) {
        // Get next directory from queue
        char* currentDir = dirQueue[queueStart++];
        if (f_opendir(&dir, currentDir) == FR_OK) {
            while (1) {
                zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
                if (f_readdir(&dir, &g_fno) != FR_OK || g_fno.fname[0] == 0) {
                    zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
                    break;
                }
                if (g_fno.fname[0] == '.' && g_fno.fname[1] == 0) continue; // Ignore "." (current directory)
                if (g_fno.fname[0] == '.' && g_fno.fname[1] == '.' && g_fno.fname[2] == 0) continue; // Ignore ".." (parent directory)
                sprintf(path, "%s/%s", currentDir, g_fno.fname);
                if (g_fno.fattrib & AM_DIR) {
                    // It's a directory, add it to the queue
                    if (queueEnd < MAX_DIRS) {
                        zssLogINFO("<%s  %d> %s(path: %s)", __FILE__, __LINE__, __FUNCTION__, path);
                        dirQueue[queueEnd++] = strdup(path);
                    } else {
                        zssLogINFO("<%s  %d> %s(Directory queue is full)", __FILE__, __LINE__, __FUNCTION__);
                        // printf("Directory queue is full, skipping: %s\n", path);
						USART_SendString("Directory queue is full\r\n");
                    }
                } else {
                    // It's a file, print its name
                    // printf("File: %s\n", path);
					zssLogINFO("<%s  %d> %s(file: %s)", __FILE__, __LINE__, __FUNCTION__, path);
					USART_SendString(path);
					USART_SendString("\r\n");
                }
            }
            zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
            f_closedir(&dir);
        }
        zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
        // Free the memory allocated for the directory name
        free(currentDir);
        zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "");
    }
    USART_SendString("Please choose the file to extract: ");
	zssLogINFO("<%s  %d> %s(arg: )LOG_OUT", __FILE__, __LINE__, __FUNCTION__);
}
// An highlighted block

void SendLogContent(const char* logPath) {
    assert(logPath != NULL);
    zssLogINFO("<%s  %d> %s(arg: %s)LOG_IN", __FILE__, __LINE__, __FUNCTION__, logPath);
    uint16_t bytesRead = 0;
    uint64_t filePos = 0;

    if((f_ret = f_close(&g_fil)) != FR_OK){
        USART_SendFormatString("Can't close file, ret: %d\r\n", f_ret);
	}
	if((f_ret = f_open(&g_fil, logPath, FA_OPEN_ALWAYS | FA_READ)) != FR_OK){
        USART_SendFormatString("Can't open file, ret: %d\r\n", f_ret);
        DiskReset(g_fil, g_fs, logPath);
	}

    else{
        do {
            // Read data from the file
            memset(TxBuffer, 0x00, sizeof(TxBuffer));
            if ((f_ret = f_read(&g_fil, TxBuffer, sizeof(TxBuffer), &bytesRead)) == FR_OK) {
                // Successfully read 'bytesRead' bytes into 'buffer'
                // Process the read data here...
                USART_SendData(TxBuffer, bytesRead);
            } else {
                // Handle read error here...
                USART_SendFormatString("Can't read file, ret: %d\r\n", f_ret);
                if(FR_DISK_ERR == f_ret){
                    USART_SendFormatString("DiskReset\r\n");
                    DiskReset(g_fil, g_fs, logPath);
                }
                break;
            }
            // Must manaul operate the file pointer to the next position, otherwise it will not read normally
	        if((f_ret = f_lseek(&g_fil, filePos)) != FR_OK){
		        USART_SendFormatString("Can't move file pointer, ret: %d\r\n", f_ret);
	        }
            // Increase the file pointer
            filePos += bytesRead;
        } while (bytesRead == sizeof(TxBuffer));  // Continue reading until a smaller chunk is read (end of file)
    }
    USART_SendFormatString("\r\nbytesRead: %d\r\n", bytesRead);
    USART_SendFormatString("BufferSize: %d\r\n", sizeof(TxBuffer));
    USART_SendString("End\r\n");
    // Close the file
    if((f_ret = f_close(&g_fil)) != FR_OK){
        USART_SendFormatString("Can't close file, ret: %d\r\n", f_ret);
	}
    // Open the file, restore the the log library write mode
	if((f_ret = f_open(&g_fil, logPath, FA_OPEN_ALWAYS | FA_WRITE | FA_READ)) != FR_OK){
		USART_SendFormatString("Can't open file, ret: %d\r\n", f_ret);
	}

    zssLogINFO("<%s  %d> %s(arg: )LOG_OUT", __FILE__, __LINE__, __FUNCTION__);
    return;
}

void DiskReset(FIL fil, FATFS fs, const char* logPath)
{
    // if((f_ret = f_close(&fil)) != FR_OK){
    //     USART_SendFormatString("Can't close file, ret: %d\r\n", f_ret);
	// }
    // if((f_ret = f_mount(NULL, "", 0)) != FR_OK){
    //     USART_SendFormatString("Can't umount disk, ret: %d\r\n", f_ret);
	// }
    // if((f_ret = f_mount(&fs, "", 1)) != FR_OK){
    //     USART_SendFormatString("Can't mount disk, ret: %d\r\n", f_ret);
	// }
    // if((f_ret = f_open(&fil, logPath, FA_OPEN_ALWAYS | FA_WRITE | FA_READ)) != FR_OK){
	// 	USART_SendFormatString("Can't open file, ret: %d\r\n", f_ret);
	// }
    DSTATUS diskStatus;
    uint8_t driveNumber = 0; // Change this to your drive number

    // Check the status of the disk
    diskStatus = disk_status(driveNumber);

    if (diskStatus & STA_NOINIT) {
        // Disk is not initialized, try to initialize it
        diskStatus = disk_initialize(driveNumber);

        if (diskStatus & STA_NOINIT) {
            // Disk initialization failed
            // Handle the error here...
        }
    }
}

void WriteFile2Disk(const unsigned char* const strpath, const unsigned char* const strfile)
{
	FIL fil;    /* File object */
	FRESULT fr; /* FatFs return code */
	/* Open the file, or if it does not exist, create a new file */
	fr = f_open(&fil, strpath, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (fr == FR_OK) {
	    uint8_t written;
	    /* Write the text to the file */
	    fr = f_write(&fil, strfile, strlen(strfile), &written);
	    if (fr == FR_OK && written == strlen(strfile)) {
	        /* Close the file */
			fr = f_sync(&fil);
	        if (fr == FR_OK) {
	            /* Sync the file system */
	        	fr = f_close(&fil);    
	            if (fr == FR_OK) {
	                printf("File written and synced successfully.\n");
	            } else {
	                printf("Failed to sync file.\n");
	            }
	        } else {
	            printf("Failed to close file.\n");
	        }
	    } else {
	        printf("Failed to write file.\n");
	    }
	} else {
	    printf("Failed to open/create file.\n");
	}
}

int GetCurrDir() {
    DIR dir;
    FRESULT res;

    // Mount the disk
    // res = f_mount(&fs, "", 1);
    // if (res != FR_OK) {
    //     // Handle mount error
    //     return -1;
    // }

    // Open the directory
    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        // Handle directory open error
        return -1;
    }

    // Read directory entries
    while (1) {
        res = f_readdir(&dir, &g_fno);
        if (res != FR_OK || g_fno.fname[0] == 0) {
            // End of directory or error occurred
            break;
        }

        // Print the file name
        printf("%s\n", g_fno.fname);
    }

    // Close the directory
    f_closedir(&dir);

    // Unmount the disk
    f_mount(NULL, "", 0);

    return 0;
}