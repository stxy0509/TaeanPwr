#pragma once


//#define	SD_FILENAME_DEFER	"1:deferedMsg.dat"

//-----------------------------------------------
#ifdef __K_SDC_C__
//-----------------------------------------------

//drive
#define DRIVE_NAND 		0
#define DRIVE_SDCARD 	1



//-----------------------------------------------
#else
//-----------------------------------------------

extern int is_sdc_mounted;
extern FATFS fs;


//-----------------------------------------------
#endif
//-----------------------------------------------



#define SDC_INSERTED	0
#define SDC_EJECTED		1


u32 sdc_read_detectPin(void);
void set_sdc_mount(int a_mount);

void sdc_saveSendMsg(char *a_msg);
void sdc_saveDataToFile(char *a_fname, char *a_msg, u32 *fsz);
void PrintFileSystem();
FRESULT print_files (char* path);
void chkDataFiles(void);


void clearDeferedMsgFile(void);
void sdc_saveDeferedMsg(char *a_msg);
void saveDeferredMsgFile(void);

void readDeferedMsgFile(void);
void sdc_mount(void);

void sdc_set_time_tag(int a_tag);

void sdc_clear(void);


