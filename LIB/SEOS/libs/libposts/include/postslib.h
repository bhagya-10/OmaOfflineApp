/****************************************************************************
NAME
	postslib.h - 电子签名

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming		2013.05.10		- add precompile process
****************************************************************************/

#ifndef _POS_TS_LIB_H
#define _POS_TS_LIB_H

#include <directfb.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum {
	LANGUAGE_CHN = 0,   // 中文
	LANGUAGE_ENG,       // 英文
};

enum {
	IMAGE_BMP = 0,   // bmp单色文件格式
	IMAGE_PNG,       // png 文件格式
	IMAGE_JBG,
};

//签名模式控制
#define	 SIGN_	0x01   //

// 电子签名传入参数结构
typedef struct _tagST_TS_ELEC_SIGNATURE
{
	unsigned int        ulTimeout;            // 超时时间
	unsigned int        ullineTimes;          // 画线的粗细 1-7
	unsigned char       ucLanguage;           // 显示的语言
	unsigned char       ucImageType;          // 图片的格式
	char                szPrompt[100];        // 电子签名板提示信息－－
	char                szTxnInfoCode[40];    // 电子签名交易特征码
	char                szImagefileName[100]; // 签名后保存图片的名称
	int  (*bitmap_handle)(const unsigned char *bitmap, int image_width,   //像素处理
			int image_height, int channel, char *out_file);
}ST_TS_ELEC_SIGNATURE;


/*设置签名模式*/  //0x01 强制签名 0x02 取消无效
int PosTsSetMode(int mode);

/*设置字体*/
int PosTsSetFont(IDirectFBFont *font, int mode);

// 电子签名 返回值 0 成功 -3 超时 -2用户取消 -1其他错误
int GetElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo);

// 将屏幕截图转换成png文件格式保存
int pixel_plot_png(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *pngfile);

// 将屏幕截图转换成bmp文件格式保存
int  pixel_plot_bmp(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *bmpfile);

// 将屏幕截图转换成最小有效区域bmp文件格式保存
int  pixel_plot_min_bmp(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *bmpfile);

/*截屏转成jbg*/
int pixel_plot_jbg(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *jbgfile);

// 将单色bmp文件压缩成jbg格式文件
int bmp2jbg(char *bmpfile, const char *jbgfile);

int bmp2png(const char *bmpfile, const char *pngfile);

/*单色bmp文件转成jbg格式并存放在缓存区*/
int bmp_encode_jbg_buffer(const char *bmpfile, unsigned char *buf, int buf_len, int *out_len);

int jbg2pbm(const char *jbgfile, const char *pbmfile);

int jbg2png(const char *jbgfile, const char *pngfile);

int SetElecSignTimeOut(uint16_t usTimeOutMs);

int GetModeElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo,uint8_t SignMode);

int ShakeHands(void);

int posts_get_libversion(char *pszVer);
int PosGetElecPixelSize(void);
void cl_led_posts_support(uint8_t IsSupport);//签名部分支持非接屏幕灯


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // end of file

