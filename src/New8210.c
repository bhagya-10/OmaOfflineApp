#include "global.h"

#ifdef _POS_TYPE_8210

#include "New8210.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pthread.h>
int    icc_fd;
void *glbEpp;
int  g_epp_Port_Num;

pthread_t PreDialTid=-1,InitCommTid=-1;

unsigned char Bmp_Display[] = "logo.png";
unsigned char Bmp_Prn[] = "logo_p.png";

static  int  ProcPreDial(void);
static  timer_t timerid; 
void CheckPinPad(void)
{
	errno = ENODEV;
	return ;
}

static int check_process(void)//检测启动相同应用程序的次数
{
	DIR	*dir;
	struct dirent *dirent;
	int ret;	
	int pid=0,execpt_pid=0;
	char link_file[1024],link_target[1024], self[1024];	

	execpt_pid = getpid();
	snprintf(link_file, sizeof(link_file)-1, "/proc/%d/exe", execpt_pid);//读取exe连接文件
	ret = readlink(link_file, self, sizeof(self)); //self为自己的文件
	if (ret >= 0)
		self[ret] = '\0';

	if ((dir = opendir("/proc/"))) {
		while ((dirent = readdir(dir))) {
			pid = strtol(dirent->d_name, NULL, 10);//提取PID
			if (pid < 2 || pid == execpt_pid) {
				pid = 0;
				continue;
			}
				
			snprintf(link_file, sizeof(link_file)-1, "/proc/%s/exe", dirent->d_name);//读取exe连接文件
			ret = readlink(link_file, link_target, sizeof(link_target));
			if (ret >= 0) {//表示此软链接合法		
				link_target[ret] = '\0';
				if( !strcmp(link_target, self) ) {//表明指向chat则为chat的PID
					break;
				}
			} 
			pid = 0;
		}
	}

	closedir(dir);
	return pid;
}


int BootInit(int argc, char *argv[])
{
	IDirectFBFont *font1;
#ifdef _POS_TYPE_6210
	DFBColor bgColor={0xC0,0xFF,0xFF,0xFF};
	DFBColor fgColor={0xF0,0x00,0x00,0x00};
#else
	DFBColor bgColor={0xF0,0xFF,0xFF,0xFF};
	DFBColor fgColor={0xF0,0x00,0x00,0x00};
#endif
	struct kbdset stdbCfg;
	int	   big_font_size = 30,font_size32= 32, small_font_size = 28;
	int   ibacklight_set_time_bak,iRet=0;
	int     iScreenwidth, iScreenheight;
	unsigned char   ucMaxZHChars = 16;

	iRet = check_process();
	if (iRet!=0)
	{
		return 1;
	}
	application_init(&argc, &argv);	

	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucMaxZHChars = 18;

		// 创建28号字体为了显示卡号超过18位
		font1 =dfb_get_font( small_font_size, 0);
		if(font1){
			dfb_destroy_font(font1);
		}
		dfb_create_font("/usr/share/fonts/wqy-microhei.ttf",  small_font_size, 0);

		font1 = dfb_get_font( small_font_size/2, 0);
		if(font1){
			dfb_destroy_font(font1);
		}
		dfb_create_font("/usr/share/fonts/wqy-microhei.ttf",       small_font_size/2, 0);
		font1 =dfb_get_font( big_font_size, 0);
		if(font1){
			dfb_destroy_font(font1);
		}
		dfb_create_font("/usr/share/fonts/wqy-microhei.ttf",  big_font_size, 0);
		font1 = dfb_get_font( big_font_size/2, 0);
		if(font1){
			dfb_destroy_font(font1);
		}
		dfb_create_font("/usr/share/fonts/wqy-microhei.ttf",       big_font_size/2, 0);
		lcdSetFont("/usr/share/fonts/wqy-microhei.ttf", "GBK", 0,  big_font_size, 0);
		font1 =dfb_get_font( font_size32, 0);
		if(font1){
			dfb_destroy_font(font1);
		}
		dfb_create_font("/usr/share/fonts/arialnb.ttf",  font_size32, 0);
	}

	if (ucMaxZHChars != 16)
	{
		lcdSetFgColor(fgColor);
		lcdSetBgColor(bgColor);
		lcdDrBgLogo(0, 0, 320, 240, "bg_image.png");
	}

	sys_kbd_get(&stdbCfg);
	stdbCfg.beep_enable = 1;
	stdbCfg.beep_freq = 2800;
	stdbCfg.beep_time = 20;
	stdbCfg.backlight = 60;
	stdbCfg.feed_enable = 1;
	sys_kbd_set(&stdbCfg);

	__ICCARD_EXCHANGE_APDU = iccCardExchangeAPDU;
	sysGetbacklighttime(&ibacklight_set_time_bak);
	if (ibacklight_set_time_bak == -1)
	{
		sysSetbacklighttime(60); // 常亮
	}

	ped_set_pin_input_region(0, 105, 320, 43);

	/*忽略SIGPIPE信号,避免sokect接收的时候返回EINTR这个错误，程序退出*/
	signal(SIGPIPE,SIG_IGN);

	// 初始化定时器
	Init_timer();
	return 0;
}

INT32 PedWriteMasterKey(UINT32 Mode, UINT16  srcKeyID, UINT16 destKeyID, UINT16 KeyLen, UINT8 *KeyBuf)
{
	int iRet;
	iRet = ped_write_master_key(Mode,srcKeyID,destKeyID, KeyLen, KeyBuf);
	return (iRet ? errno : 0);
}

INT32 PedWritePinKey(UINT32 Mode, UINT16  MasterKeyID, UINT16 PinKeyID, UINT16 PinKeyLen, UINT8 *PinKey)
{
	int iRet;
	iRet = ped_write_pin_key(MasterKeyID, PinKeyID, PinKeyLen, PinKey);
	return (iRet ? errno : 0);
}

INT32 PedWriteMacKey(UINT32 Mode, UINT16  MasterKeyID, UINT16 MacKeyID, UINT16 MacKeyLen, UINT8 *MacKey)
{
	int iRet;
	iRet = ped_write_mac_key(MasterKeyID, MacKeyID, MacKeyLen, MacKey);
	return (iRet ? errno : 0);
}

INT32  PedGetPin(UINT16 PinKeyID, UINT8 *ExpectPinLen, UINT8 *CardNo, INT32 Mode, UINT8 *PinBlock)
{
	int iRet;
	iRet = ped_get_pin(PinKeyID,Mode,(const char*)ExpectPinLen,(const char*)CardNo,PinBlock);
	return (iRet ? errno : 0);
}

INT32 PedGetMac(UINT16 MacKeyID, UINT8 *DataIn, UINT16 InLen, INT32 Mode, UINT8 *MacOut)
{
	int iRet;
	iRet = ped_get_mac(MacKeyID,Mode,(UINT32)InLen,(const char*)DataIn,MacOut);
	return (iRet ? errno : 0);
}

INT32 PedTDEA(UINT32 Mode, INT32 KeyType, UINT16  KeyID, UINT32 DataLen, UINT8 *InBuf, UINT8 *OutBuf)
{
	int iRet;
	char sIv[100];
	memset(sIv,0,sizeof(sIv));
	iRet = ped_tdea(Mode, KeyType, KeyID, (UINT8 *)sIv, DataLen, InBuf, OutBuf);
	return (iRet ? errno : 0);
}

int32_t PedGetSensitiveTimer(int32_t SensitiveType)
{
	return ped_get_sensitive_timer(SensitiveType);
}


int iccCardExchangeAPDU(int fd, uint32_t sendlen, const void *senddata, uint32_t *recvlen, void *recvdata)
{
	return iccard_exchange_apdu(fd, sendlen, senddata, recvlen, recvdata);
}

void PrnBigFontChnData(void)
{
	int  iRet;
	
	iRet = prnSetFont("/usr/share/fonts/PosSong-24.ttf", "GB18030", 0, 24, SET_PRIMARY_FONT);
	if( iRet==ERROR )
	{
		lcdClrLine(2, 7);	
		lcdDisplay(0, 2, DISP_CFONT, "FONT NOT EXIST ");
		lcdFlip();
		kbGetKey();
		return ;
	}	
	return ;	
}

void PrnBigEngData(void)
{
	int  iRet;
	
	iRet = prnSetFont("/usr/share/fonts/arial.ttf", "GB18030", 0, 30, SET_PRIMARY_FONT);
	if( iRet==ERROR )
	{
		lcdClrLine(2, 7);	
		lcdDisplay(0, 2, DISP_CFONT, "FONT NOT EXIST ");
		lcdFlip();
		kbGetKey();
		return ;
	}	
	return ;	
}

void PrnSmallFontData(void)
{
	int  iRet;
	
	iRet = prnSetFont("/usr/share/fonts/PosSong-16.ttf", "GB18030", 0, 16, SET_PRIMARY_FONT);
	if( iRet==ERROR )
	{
		lcdClrLine(2, 7);	
		lcdDisplay(0, 2, DISP_CFONT, "FONT NOT EXIST");
		lcdFlip();
		kbGetKey();
		return ;
	}	
	return ;	
}

void PrnSmallEngData(void)
{
	PrnSmallFontData();
}

void lcdDrLogo(INT32 X, INT32 Y, UINT8 *pucBitMap)
{
	lcdDrawPicture(X,Y,144,90, (char*)pucBitMap);
}

INT32  sysGetPowerStatus(void)
{
	return 9;
}

int sysSetbacklighttime(int keep)
{
	return sys_backlight_set_time(keep);
}

int sysGetbacklighttime(int *keep)
{
	return sys_backlight_get_time(keep);
}

int EppLight(unsigned long dwMode, unsigned long dwTimeMs)
{
	errno = ENODEV;
	return -1;
}

int EppDisplayLogo(int X, int Y, const uint8_t *pbyBmpLogoIn)
{
	errno = ENODEV;
	return -1;
}

int EppDisplayString(int X, int Y, int iMode, const void *str, int iStrLen)
{
	errno = ENODEV;
	return -1;
}

int EppClearScreen(void)
{
	errno = ENODEV;
	return -1;
}

int EppSetPinInputTimeout(unsigned long wTimeout100Ms)
{
	errno = ENODEV;
	return -1;
}

int EppGetPin(uint32_t byKeyType, uint32_t byKeyIndex, uint32_t byDisplayMode,
            uint32_t byMode, const uint8_t *pbyCardInfo, const uint8_t *pbyLenInfo, uint8_t *pbyPinBlockOut)
{
	errno = ENODEV;
	return -1;
}

int EppRestoreDefaultIdleLogo(void)
{
	errno = ENODEV;
	return -1;
}

int EppGetMac(uint32_t byKeyType, uint32_t byKeyIndex, const uint8_t *pbyMacDataIn,
                        unsigned long wMacLen, uint32_t byMode, uint8_t *pbyMacOut)
{
	errno = ENODEV;
	return -1;
}

int EppDownloadAppKey(uint32_t byKeyType, uint32_t byMode, uint32_t byKeyIndex,
    const uint8_t *pbyKeyDataIn, uint32_t byKeyLen, uint32_t byMasterKeyIndex)
{
	errno = ENODEV;
	return -1;
}

int EppClose(void)
{
	errno = ENODEV;
	return -1;
}

int EppBeep(unsigned long dwFrequency, unsigned long dwTimeMs)
{
	errno = ENODEV;
	return -1;
}

int EppClearAKey(uint32_t byKeyType, uint32_t byKeyIndex)
{
	errno = ENODEV;
	return -1;
}



int CheckConnectPinPad(uint8_t ucFlag)
{
	return 0;
}


void   AdjustMenuContext(const char *pszInStr, char *pszOutStr)
{
	int  index,ilen;

	if (pszInStr==NULL || pszOutStr==NULL)
	{
		return;
	}

	ilen = strlen(pszInStr);
	for (index=1;index<ilen; index++)
	{
		if (pszInStr[index]>='0' && pszInStr[index] <= '9')
		{
			break;
		}
	}

	memcpy(pszOutStr,"  ",2);//先空二格
	if (index>=ilen)
	{
		strcpy(pszOutStr+2,pszInStr);
	}
	else
	{
		memcpy(pszOutStr+2,pszInStr,index);
		memcpy(pszOutStr+index+2,"        ",8);
		strcpy(pszOutStr+index+10,pszInStr+index);
	}
}


int ThreadPreDial(void)
{
	int err=0;

	SetPreDialDisplayFlag(0); //显示--7110需要测试
	if (PreDialTid == -1 )
	{	
		if((err = pthread_create(&PreDialTid,NULL,(void *)ProcPreDial,NULL)) < 0)//创建预拨号
		{
			perror("pthread_create");
		}
	}

	return err;
}


int  ProcPreDial(void)
{
	int iRet;

	pthread_detach(PreDialTid);//使线程分离出来，当这个线程执行完成任务后释放资源
	iRet = PreDial();
	PreDialTid = -1;
	return iRet;
}

int ThreadJoinInitModule(void)
{
	int err=0;
	void *tret;
	
	if (InitCommTid != -1)
	{
		lcdClrLine(2, 7);
		lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, " PROCESSING....");
		lcdDisplay(0, 5, DISP_CFONT|DISP_MEDIACY, " PLS WAIT...   ");
		lcdFlip();
		err = pthread_join(InitCommTid,&tret);//退出初始化
		if (err == 0)
		{
			InitCommTid = -1;
		}
		else if (err<0)
		{
			perror("pthread_join");
		}		
	}
	return err;
}

int ThreadInitModule(void)
{
	int   err=0;
	int   iDevType = OPT_DEVWNET;
	
	// 8210的ppp拨号函数的设备名称和原来的不同，需要区分GPRS,CDMA
	if (gstCurAcq.stTxnCommCfg.ucCommType == CT_CDMA)
		iDevType = PPP_DEV_CDMA;
	else
		iDevType = PPP_DEV_GPRS;
	
	err = PPPCheck(iDevType);
	if (err==0 || err == -NET_ERR_LINKOPENING) //PPP_LINK_BUILDING和NET_ERR_LINKOPENING值相同
	{
		return 0;
	}
	
	printf("intit: %d \n",gstCurAcq.stTxnCommCfg.ucCommType);
	if (InitCommTid == -1)
	{
		if((err = pthread_create(&InitCommTid,NULL,(void *)CommInitModule,&gstCurAcq.stTxnCommCfg)) < 0)//创建初始化
		{
			perror("pthread_create");
		}
	}
	return err;
}


void timer_thread(union sigval v)  
{  
	sys_delay_to_suspend();  
} 


int    Init_timer(void)
{ 
    struct sigevent evp;  
    memset(&evp, 0, sizeof(struct sigevent));       //清零初始化  
	
    evp.sigev_value.sival_int = 111;            //也是标识定时器的，这和timerid有什么区别？回调函数可以获得  
    evp.sigev_notify = SIGEV_THREAD;            //线程通知的方式，派驻新线程  
    evp.sigev_notify_function = timer_thread;       //线程函数地址  
	
    if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)  
    {  
        perror("fail to timer_create");  
        exit(-1);  
    } 

	return 0;
}

int start_timer(time_t sec, suseconds_t usec)
{	
	struct itimerspec value;

	if (timerid < 0){
		return -1;
	}
			
	// 启动定时器 
    value.it_interval.tv_sec = sec;  
    value.it_interval.tv_nsec = usec;  
    value.it_value.tv_sec = sec;  
    value.it_value.tv_nsec = usec;

	if (timer_settime(timerid, 0, &value, NULL) == -1)  
    {  
        perror("fail to timer_settime");  
        exit(-1);  
    } 

    return 0;
}

int stop_timer(void)
{	
	struct itimerspec value;
	
	if (timerid < 0){
		return -1;
	}
	
	// 启动定时器 
    value.it_interval.tv_sec = 0;  
    value.it_interval.tv_nsec = 0;  
    value.it_value.tv_sec = 0;  
    value.it_value.tv_nsec = 0;
	
	if (timer_settime(timerid, 0, &value, NULL) == -1)  
    {  
        perror("fail to timer_settime");  
        exit(-1);  
    } 
	
    return 0;
}

#if 0
int start_timer(time_t sec, suseconds_t usec)
{
	pid_t pid;
	
	if ((pid = fork()) == 0) {
		struct sigaction sa;
		struct itimerval value;
		
		/* 设置信号处理函数 */
		SETSIG(sa, SIGALRM,  sigroutine, 0);
		
		/* 启动定时器 */
		value.it_value.tv_sec	  = sec;
		value.it_value.tv_usec	  = usec;
		value.it_interval.tv_sec  = sec;
		value.it_interval.tv_usec = usec;
		setitimer(ITIMER_REAL, &value, NULL);
		
		while (1) pause();
	}
	
	return pid;
}

int stop_timer(int timer)
{
	pid_t w;
	int   status;
	
	kill(timer, SIGTERM);
	do {
		w = waitpid(timer, &status, WUNTRACED | WCONTINUED);
		if (w == -1) {
			perror("waitpid");
			break;
		}
		
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	
	return 0;
}
#endif

#endif
//end of line







