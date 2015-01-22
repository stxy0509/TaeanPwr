#define __K_SDC_C__

#include "k_includes.h"
//#include "..\SDK2.0\include\fatfs\ff.h"


#define SDC_INSERTED	0
#define SDC_EJECTED		1
#define SDC_DISKERROR	2

#define SDC_chkCardInPort()     ((*R_GP2ILEV) & (1<<3))

FATFS fs;

int is_sdc_mounted = 0;
int fgSDC_diskFail = 0;


void sdc_addHistoryItem(U8 a_itemNo);
FRESULT scan_files (char* path);


//void sdc_setTagMounted(int a_tag)
void sdc_set_time_tag(int a_tag)
{
    if (sdc_read_detectPin()!=SDC_INSERTED)
    {
        return;
    }

    {
        u32 fsz;
        char t_item[130];


        rtc_isSecUpdate();
        sprintf(t_item,"\r\n%02d%02d%02d-%02d:%02d:%02d ",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
        switch(a_tag)
        {
            case SDC_BOOT_TAG:
                strcat(t_item,"---- boot ");
                strcat(t_item, KOGA_VER_STRING);
                break;

            // case SDC_MOUNT_TAG:
            //     strcat(t_item,"---->> SD mounted ");
            //     break;

            case SDC_TIME_TAG:
            default:
                break;
        }
        strcat(t_item,"\r\n");

        sdc_saveDataToFile(FN_AIO,      t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_CT3919,   t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_DCS,      t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_ATM1,     t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_ATM2,     t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_HMP155,   t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_PTB210,   t_item, &fsz);    delayms(5);
        sdc_saveDataToFile(FN_SHOCK,    t_item, &fsz);    delayms(5);
        if (a_tag != SDC_TIME_TAG)
        {
            sdc_saveDataToFile(FN_GPS,      t_item, &fsz);    delayms(5);
            sdc_saveDataToFile(FN_MOSE,     t_item, &fsz);    delayms(5);
            // sdc_saveDataToFile(FN_SEND,     t_item, &fsz);    delayms(5);
            sdc_saveDataToFile(FN_HISTORY,  t_item, &fsz);    delayms(5);
        }
    }

}

//-----------------------------------------------------------------------------
void sdc_mount(void)
{
	if (SDC_chkCardInPort()==0)	//inserted
	{
		debugstring("\r\nSD Card check...\r\n");

		if (f_mount(DRIVE_SDCARD,&fs) != FR_OK)
		{
			debugstring("Mount Error");
			set_sdc_mount(2);   // mount fail
            fgSDC_diskFail = 1;
		}
		else
		{
			set_sdc_mount(1);   // mounted
            fgSDC_diskFail = 0;
            delayms(500);

            sdc_set_time_tag(SDC_MOUNT_TAG);
            //sdc_setTagMounted();
		}
	}
    else
    {
        set_sdc_mount(0);
        fgSDC_diskFail = 0;
    }
}


//-----------------------------------------------------------------------------
void sdc_unmount(void)
{
	if (SDC_chkCardInPort()==1)	// ejected
	{
		debugstring("\r\n SD-Card is unmounted...\r\n");

        f_mount(DRIVE_SDCARD, 0);
	}
    set_sdc_mount(0);
}


//-----------------------------------------------------------------------------
void sdc_addHistoryItem(U8 a_itemNo)
{
    FRESULT  res;
    FIL fsrc;
    FIL* fp = &fsrc;

    rtcTime t1;
    char t_item[30];
    int bw;


    //history.dat
    res = f_open(fp, FN_HISTORY,FA_OPEN_ALWAYS|FA_WRITE);
    if (res != FR_OK)
    {
        debugprintf("faild f_open : res : %d\r\n",res);
        fgSDC_diskFail = 1;
        return;
    }

    rtc_gettime(&t1);
    switch (a_itemNo)
    {
    case 1:	//PON
        sprintf(t_item,"%02d%02d%02d-%02d%02d%02d-PON\r\n",(t1.year)%100,t1.mon,t1.day,t1.hour,t1.min,t1.sec);
        f_lseek(fp, (fp->fsize));
        f_write(fp,t_item,strlen(t_item),&bw);
        break;
    }
    f_close(fp);
}

void sdc_saveSendMsg(char *a_msg)
{
    FRESULT  res;
    FIL fsrc;
    FIL* fp = &fsrc;

    char t_item[MSG_LENGTH-1];
    int bw;

    res = f_open(fp,FN_SEND,FA_OPEN_ALWAYS|FA_WRITE);
    if (res != FR_OK)
    {
        debugprintf("faild f_open : res : %d\r\n",res);
        fgSDC_diskFail = 1;
        return;
    }

    // // 해운대
    // //10B100FF1204051610353759912694484124093181005007032761020906270735033706530002FFFF01887FFFFFFFFFFFFFFFFFFF
    // res = f_open(fp,FN_SEND,FA_OPEN_ALWAYS|FA_WRITE);
    // if (res != FR_OK)
    // {
    //     debugprintf("faild f_open : res : %d\r\n",res);
    //     fgSDC_diskFail = 1;
    //     return;
    // }

   strcpy(t_item,a_msg);
   strcat(t_item,"\r\n");
   f_lseek(fp, (fp->fsize));
   f_write(fp,t_item,strlen(t_item),&bw);
    f_close(fp);
}



void sdc_saveDeferedMsg(char *a_msg)
{
/*    FRESULT  res;
    FIL fsrc;
    FIL* fp = &fsrc;

    char t_item[MSG_LENGTH];
    int bw;



    //history.dat
    res = f_open(fp,SD_FILENAME_DEFER,FA_OPEN_ALWAYS|FA_WRITE);
    if (res != FR_OK)
    {
        debugprintf("faild f_open : res : %d\r\n",res);
        fgSDC_diskFail = 1;
        return;
    }

	strcpy(t_item,a_msg);
	strcat(t_item,"\r\n");
	f_lseek(fp, (fp->fsize));
	f_write(fp,t_item,strlen(t_item),&bw);
    f_close(fp);
*/}

void readDeferedMsgFile(void)
{
/*    FRESULT  res;
    FIL fsrc;
    FIL* fp = &fsrc;
    char buf[MSG_LENGTH-1];
#define MSG_LEN

    //PRINTLINE;
    res = f_open(fp,SD_FILENAME_DEFER,FA_OPEN_ALWAYS|FA_READ);
    if (res != FR_OK)
    {
        debugprintf("faild f_open : res : %d\r\n",res);
        fgSDC_diskFail = 1;
        return;
    }
    for (;;)
    {
        f_gets(buf, (MSG_LENGTH-1), fp);

        debugprintf("%d : %s",strlen(buf),buf);

        if (strlen(buf)>60)
        {
            strcpy(s_msg,buf);
            q_putData();
        }
        else
        {
            break;
        }
    }

    f_close(fp);
*/}


//-----------------------------------------------------------------------------
void sdc_saveDataToFile(char *a_fname, char *a_msg, u32 *fsz)
{
    FRESULT  res;
    FIL fsrc;
    FIL* fp = &fsrc;

    char t_item[100];
    u32 bw;

    u32 sz_msg;

    strcpy(t_item,"1:");
    strcat(t_item,a_fname);

    //
    res = f_open(fp,t_item, FA_OPEN_ALWAYS|FA_WRITE);   delayms(5);
    if (res != FR_OK)
    {
        // PRINTLINE;
        debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
        fgSDC_diskFail = 1;
        return;
    }

	f_lseek(fp, (fp->fsize));   delayms(5);
    sz_msg = strlen(a_msg);
	f_write(fp, a_msg, sz_msg, &bw);   delayms(5);

    *fsz = (u32)(fp->fsize);
    f_close(fp);   delayms(5);
}

//-----------------------------------------------------------------------------
//FRESULT scan_files (char* path)
FRESULT print_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN+1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {
                char dirname[_MAX_LFN+1];
                if(fn[0]!='.')
                {
                    sprintf(dirname,"%s/%s",path,fn);
                    // debugprintf("%s\r\n", dirname);
                    wifiprintf("%s\r\n", dirname);
                    //print_files(dirname);
                }
            } else {
                // debugprintf("%s/%s\t%10d\r\n", path, fn,fno.fsize);
                wifiprintf("%s/%s\t%10d\r\n", path, fn,fno.fsize);
            }
        }
    }
    else{
        debugprintf("path(%s) not found\r\n",path);
        wifiprintf("path(%s) not found\r\n",path);
    }
    return res;

/*
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        i = strlen(path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR)
            {
                debugprintf("[D]%s\r\n",fn);
            }
            else
            {
                debugprintf("%s/%s : \t\t %dbyte\r\n", path, fn,fno.fsize);
            }
        }
    }
    else
    {
        debugprintf("path(%s) not found:Error(%d)\r\n",path,res);
    }
    return res;
*/
}

//-----------------------------------------------------------------------------
void PrintFileSystem()
{
    //scan_files("1:");
}


//-----------------------------------------------------------------------------
void chkDataFiles(void)
{
    // do nothing...

}

//-----------------------------------------------------------------------------
void set_sdc_mount(int a_mount)
{
	is_sdc_mounted = a_mount;
}


int get_sdcMountStatus(void)
{
    /* retval:
            0: unmount
            1: mount
            2: mount fail
    */
	return ( is_sdc_mounted );
}

//-----------------------------------------------------------------------------
u32 sdc_read_detectPin(void)
{
	/*
        ret_val :
            0 SDC_INSERTED  : inserted
            1 SDC_EJECTED   : ejected
            2 SDC_DISKERROR : SD is inserted, but error status
    */

	u32 ret_val = SDC_EJECTED;

	if (SDC_chkCardInPort()==0)
	{   // inserted
        ret_val = SDC_INSERTED;

        if (fgSDC_diskFail == 1)
        {
            ret_val = SDC_DISKERROR;
        }
		else if (get_sdcMountStatus()==0)
		{
            sdc_mount();

            if (get_sdcMountStatus()==1)  // mounted
            {
                saveDeferredMsgFile();
            }
		}

	}
	else
	{   // ejected
        if (get_sdcMountStatus()==1)  // mounted
        {
            sdc_unmount();
        }
        fgSDC_diskFail = 0;
        ret_val = SDC_EJECTED;
	}

    return(ret_val);
}




//-----------------------------------------------------------------------------
void clearDeferedMsg(void)
{
/*	FRESULT  res;
	FIL fsrc;
	FIL* fp = &fsrc;

	//deferMsg.dat
	res = f_open(fp,SD_FILENAME_DEFER,FA_CREATE_ALWAYS|FA_WRITE);
	if (res != FR_OK)
	{
		debugprintf("faild f_open : res : %d\r\n",res);
        fgSDC_diskFail = 1;
		return;
	}
	debugprintf("deferMsg.dat is created.. : Ok.\r\n");
	f_close(fp);
*/}


//-----------------------------------------------------------------------------
void saveDeferredMsgFile(void)
{
/*	int num,i;

    //PRINTLINE;
    if (sdc_read_detectPin()==SDC_INSERTED)
    {
        clearDeferedMsg();
        num = is_q_dataNum();
        if (num>0)
        {
            i=0;
            for(;;)
            {
                q_getData(i);
                sdc_saveDeferedMsg(s_msg);
                i++;
                if (i>=num)
                {
                    break;
                }
            }
        }
    }
*/}


//-----------------------------------------------------------------------------
void clearDeferedMsgFile(void)
{
    if (sdc_read_detectPin()==SDC_INSERTED)
    {
        clearDeferedMsg();
    }
}






//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
#if 0
				// SDC test
				{
					int i;
					for (i=0; i<50000; i++)
					{
						debugprintf("%d\r\n",i);
						if (sdc_read_detectPin()==SDC_INSERTED)
						{
							sdc_saveDataToFile("rRdcp.dat","$WIXDR,A,034,D,1,S,5.0,M,1,C,17.3,C,0,H,69.4,P,0,P,1024.8,H,0,C,17.3,C,2,U,0.0,#,0,U,25.1,V,1*13\r\n");
							sdc_saveDataToFile("rAdb.dat","$PDTWL,POS,11,11,01,00,01,10.00,34,43.9136,N,128,22.6923,E,  25.1,0.9,1.2*19\r\n");
							sdc_saveDataToFile("rPtb210.dat","%#	  3919	  1259	     0.0243	    18.647\r\n");
							sdc_saveDataToFile("sentMsg.dat","%#	  3919	  1259	     0.0243	    18.647\r\n");
							sdc_saveDataToFile("deferMsg.dat","%#	  3919	  1259	     0.0243	    18.647\r\n");
							*R_WDCNT=WATCHDOG_V;
						}
					}
					debugstring("\r\n---> SDC TEST End...\r\n");
					drifterStage = 100;
					break;
				}
#endif
//-------------------------------------------------------------------------------------------------------------

