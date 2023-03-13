#include <windows.h>
#include <memory.h>
#include <math.h>

#include "LCD.h"
#include "GUI.h"
#include "LCDConf.h"

unsigned int LCD_Buffer[LCD_YSIZE][LCD_XSIZE];
BITMAPINFO bmp; 

void LCDSIM_Init(void) 
{
	//��ʼ��λͼ�ṹ	
	ZeroMemory(&bmp,   sizeof(BITMAPINFO));  

	bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmp.bmiHeader.biWidth = LCD_XSIZE;   //λͼ���
	bmp.bmiHeader.biHeight = -LCD_YSIZE;  //λͼ�߶�  //��ͼ���ǵ�����ʾ��ʱ�򣬰�biHeight��Ϊ��Ӧ�ĸ�ֵ
	bmp.bmiHeader.biPlanes   =   1;   
	bmp.bmiHeader.biBitCount   =   32;   //һ������32Bit
	bmp.bmiHeader.biCompression   =   BI_RGB; 
}


void UpdataShow(HDC _hdc)
{
	SetDIBitsToDevice(_hdc,
								0, 0,  //����Ļ�е���ʼ����
								LCD_XSIZE, LCD_YSIZE,  //��ʾ����Ļ�Ŀ�,��
								0 , 0,   //λͼ���½ǵ�����
								0,  //DIB�е���ʼɨ����
								LCD_YSIZE,  // DIBɨ������Ŀ
								(BYTE*)LCD_Buffer,  //λͼ��������ʼָ��,��BYTE*����
								&bmp,  //λͼ��BITMAPINFOָ��
								DIB_RGB_COLORS);  //��ɫʹ������
}

void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)  
{
#if 0
	int R = (Index & 0xF800) >> 11;
	int G = (Index & 0x07E0) >> 5;
	int B = Index & 0x001F;

	LCD_Buffer[y][x] = R << 3 << 16 | G << 2 << 8 | B << 3;
#endif

#if (LCD_BITSPERPIXEL == 8)
	unsigned char R = 1.164 * (Index - 16);
	LCD_Buffer[y][x] = R << 16 | R << 8 | R ;
#elif (LCD_BITSPERPIXEL == 16) && (LCD_FIXEDPALETTE == 565)
	LCD_Buffer[y][x] = ((Index & 0xF800) << 3 >> 11 << 16) | ((Index & 0x07E0) << 2 >> 5 << 8) | (Index & 0x1F)<<3;
#else if (LCD_BITSPERPIXEL == 1)
	if (Index == 0)
	{
		LCD_Buffer[y][x] = 0x00000000;
	}
	else
	{
		LCD_Buffer[y][x] = 0x00FFFFFF;
	}
#endif
}


void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)
{

}


int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)
{
	return LCD_Buffer[y][x];
}

void LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color, int LayerIndex) 
{

}



GUI_PID_STATE Touch_Status;
void Touch_Pressed(int x, int y)  //��������  x,y Ϊ�����ǵ�����ڴ����е�����
{
	Touch_Status.Pressed = 1;
	Touch_Status.x = x;
	Touch_Status.y = y;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}

void Touch_Release(void)  //�����ͷ�
{
	Touch_Status.Pressed = 0;
	Touch_Status.x = -1;
	Touch_Status.y = -1;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}




