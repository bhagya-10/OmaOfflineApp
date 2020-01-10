/****************************************************************************
NAME
	postslib.h - ����ǩ��

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
	LANGUAGE_CHN = 0,   // ����
	LANGUAGE_ENG,       // Ӣ��
};

enum {
	IMAGE_BMP = 0,   // bmp��ɫ�ļ���ʽ
	IMAGE_PNG,       // png �ļ���ʽ
	IMAGE_JBG,
};

//ǩ��ģʽ����
#define	 SIGN_	0x01   //

// ����ǩ����������ṹ
typedef struct _tagST_TS_ELEC_SIGNATURE
{
	unsigned int        ulTimeout;            // ��ʱʱ��
	unsigned int        ullineTimes;          // ���ߵĴ�ϸ 1-7
	unsigned char       ucLanguage;           // ��ʾ������
	unsigned char       ucImageType;          // ͼƬ�ĸ�ʽ
	char                szPrompt[100];        // ����ǩ������ʾ��Ϣ����
	char                szTxnInfoCode[40];    // ����ǩ������������
	char                szImagefileName[100]; // ǩ���󱣴�ͼƬ������
	int  (*bitmap_handle)(const unsigned char *bitmap, int image_width,   //���ش���
			int image_height, int channel, char *out_file);
}ST_TS_ELEC_SIGNATURE;


/*����ǩ��ģʽ*/  //0x01 ǿ��ǩ�� 0x02 ȡ����Ч
int PosTsSetMode(int mode);

/*��������*/
int PosTsSetFont(IDirectFBFont *font, int mode);

// ����ǩ�� ����ֵ 0 �ɹ� -3 ��ʱ -2�û�ȡ�� -1��������
int GetElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo);

// ����Ļ��ͼת����png�ļ���ʽ����
int pixel_plot_png(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *pngfile);

// ����Ļ��ͼת����bmp�ļ���ʽ����
int  pixel_plot_bmp(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *bmpfile);

// ����Ļ��ͼת������С��Ч����bmp�ļ���ʽ����
int  pixel_plot_min_bmp(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *bmpfile);

/*����ת��jbg*/
int pixel_plot_jbg(const unsigned char *pixelbuf, int pixel_width, int pixel_height, int channel, char *jbgfile);

// ����ɫbmp�ļ�ѹ����jbg��ʽ�ļ�
int bmp2jbg(char *bmpfile, const char *jbgfile);

int bmp2png(const char *bmpfile, const char *pngfile);

/*��ɫbmp�ļ�ת��jbg��ʽ������ڻ�����*/
int bmp_encode_jbg_buffer(const char *bmpfile, unsigned char *buf, int buf_len, int *out_len);

int jbg2pbm(const char *jbgfile, const char *pbmfile);

int jbg2png(const char *jbgfile, const char *pngfile);

int SetElecSignTimeOut(uint16_t usTimeOutMs);

int GetModeElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo,uint8_t SignMode);

int ShakeHands(void);

int posts_get_libversion(char *pszVer);
int PosGetElecPixelSize(void);
void cl_led_posts_support(uint8_t IsSupport);//ǩ������֧�ַǽ���Ļ��


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // end of file

