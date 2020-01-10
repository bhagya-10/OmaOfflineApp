#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "tmsdemo.h"
#include "seos.h"


const char* menu[] = {
		"NEW8210 SEOS DEMO",
		"1.Display",
		"2.Display Arabic",
		"3.Key Board",
		"4.Touch Screen",
		"5.Printer",
		"6.Contact IC",
		"7.Contactless IC",
		"8.Magnetic Card",

		"1.Uart",
		"2.Modem",
		"3.GPRS",
		"4.Socket Server",
		"5.Socket Client",
		"6.SSL Server",
		"7.SSL Client",
		"8.Bar Scanner",

		"1.Mul Thread",
		"2.nfc test",
		"3.modem sync",
		"4.tms demo"
};
int DispMenu(const char * menu[], int iLines, int iTitle, int iStartKey, int iEndKey, int iTimeOut);
extern void Display_Demo(void);
extern void Display_Demo_Arabic(void);
extern void key_board_demo(void);
extern void Printer_Demo(void);
extern void ContactSmartCard_Demo(void);
extern void single_item_nfc(void );

extern void ContactlessSmartCard_Demo(void);
extern void magneticstrip_demo(void);
extern void uart_demo(void);

extern void socket_server_demo(void);
extern void socket_client_demo(void);
extern void ssl_server_demo(void);
extern void ssl_client_demo(void);
extern void modem_demo(void);
extern void gprs_demo(void);
extern void touch_demo(void);
extern void bar_scanner_demo(void);
extern int ThreadInit(void);
extern int nfc_test(void);
extern int demo_for_modem_sync(void);
extern int tms_demo(void);

#define CL_BACK_GROUND_PIC_FILE "cl_bk_pic.jpg"
DFBColor COLOR_BLACK={0xF0,0xFF,0xFF,0xFF};
DFBColor COLOR_WITE={0xF0,0x00,0x00,0x00};
#define font_file_arabic  "/usr/share/fonts/arial.ttf"
//#define font_file_arabic  "/usr/share/fonts/vietland.ttf"

int main(int argc, char *argv[]){
	int retval = 0;
	int selected=0;
	DFBColor bgColor={0xF0,0xFF,0xFF,0xFF};
	DFBColor fgColor={0xF0,0x00,0x00,0x00};

	application_init(&argc, &argv);

	lcdSetFgColor(fgColor);
	lcdSetBgColor(bgColor);
	lcdDrBgLogo(0, 0, 320, 240, "bg_image.png");

	sys_backlight_set_time(30);
	kbFlush();

	dfb_create_font(font_file_arabic,20,0);
	dfb_create_font(font_file_arabic,24,0);
	dfb_create_font(font_file_arabic,28,0);
	dfb_create_font(font_file_arabic,32,0);


	lcdSetFont(font_file_arabic, "UTF-8", 0,  20, 0);

	while (selected>=0){
		selected = DispMenu(menu,21,1,KEY0, KEY8, 60);
		switch (selected){
			case 0:
				Display_Demo();
				break;
			case 1:
				Display_Demo_Arabic();
				break;
			case 2:
				key_board_demo();
				break;
			case 3:
				touch_demo();
				break;
			case 4:
				Printer_Demo();
				break;
			case 5:
				ContactSmartCard_Demo();
				break;
			case 6: 
				ContactlessSmartCard_Demo();
				break;
			case 7:
				magneticstrip_demo();
				break;
			case 8:
				uart_demo();
				break;
			case 9:
				modem_demo();
				break;
			case 10:
				gprs_demo();
				break;
			case 11:
				socket_server_demo();
				break;
			case 12:
				socket_client_demo();
				break;
			case 13:
				ssl_server_demo();
				break;
			case 14:
				ssl_client_demo();
				break;
			case 15:
				bar_scanner_demo();
				break;
			case 16:
				ThreadInit();
				break;
			case 17:
				nfc_test();
				break;
			case 18:
				demo_for_modem_sync();
				break;
			case 19:
				tms_demo();
				break;
			default:
				break;
		}
	}

	return retval;
}

int DispMenu(const char * menu[], int iLines, int iTitle, int iStartKey, int iEndKey, int iTimeOut)
{
	int screens;
	int start_line;
	int start_item;
	int line_per_screen,max_line_screen;
	int cur_screen;
	int i;
	int t;
	int key;
	char  szBuffer[100];

	max_line_screen = 9;

	if( iTitle )
	{
		start_item = 1;
//		screens = (iLines-2)/ 3 + 1;
		screens = (iLines-2)/(max_line_screen-1) + 1;
		start_line = 2;
//		line_per_screen = 3;
		line_per_screen = max_line_screen-1;

	}
	else
	{
		start_item = 0;
//		screens = (iLines-1)/4 + 1;
		screens = (iLines-1)/max_line_screen + 1;
		start_line = 0;
//		line_per_screen = 4;
		line_per_screen = max_line_screen;
	}

	cur_screen = 0;

	while(1)
	{
		if( cur_screen<screens-1 )
			lcdSetIcon(ICON_DOWN, OPENICON);
		else
			lcdSetIcon(ICON_DOWN, CLOSEICON);
		if( cur_screen>0 )
			lcdSetIcon(ICON_UP, OPENICON);
		else
			lcdSetIcon(ICON_UP, CLOSEICON);

		//lcdClrLine(start_line, 7);
		lcdCls();
		lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY, (char *)menu[0]);
		for(i=0; i<line_per_screen; i++)
		{
			t = i + line_per_screen*cur_screen + start_item;
			if( t<iLines )
			{
				memset(szBuffer,0,sizeof(szBuffer));
				if (max_line_screen == 6)
				{

					lcdDisplay(0, (i*2+start_line), DISP_CFONT, (char *)szBuffer);
				}
				else
				{
					lcdDisplay(0, (i*2+start_line), DISP_CFONT, (char *)menu[t]);
				}
			}
		}
		lcdFlip();
		key = kbGetKeyMs(iTimeOut*1000);
//		if( key==KEY_CANCEL )
		if( key==KEY_CANCEL || key == KEY_TIMEOUT )
		{
			lcdSetIcon(ICON_DOWN, CLOSEICON);
			lcdSetIcon(ICON_UP,   CLOSEICON);
			return key;
		}
		else if( key==KEY_ENTER || key==KEY_DOWN )
		{
			if( cur_screen>=screens-1 )
				cur_screen = 0;
			else
				cur_screen++;
		}
		else if( key==KEY_UP )
		{
			if( cur_screen<=0 )
				cur_screen = screens-1;
			else
				cur_screen--;
		}
		else if( key>=iStartKey && key<=iEndKey )
		{
			lcdSetIcon(ICON_DOWN, CLOSEICON);
			lcdSetIcon(ICON_UP,   CLOSEICON);

			switch (key){
			case KEY1:
				key = 1;
				break;
			case KEY2:
				key = 2;
				break;
			case KEY3:
				key = 3;
				break;
			case KEY4:
				key = 4;
				break;
			case KEY5:
				key = 5;
				break;
			case KEY6:
				key = 6;
				break;
			case KEY7:
				key = 7;
				break;
			case KEY8:
				key = 8;
				break;
			default:
				break;
		}
		return 8*cur_screen+key-1;
	}
		//else
			//sysBeep();
	}
}
