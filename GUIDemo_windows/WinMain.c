// Windows ͷ�ļ�:
#include <windows.h>
#include "GUI.h"
#include "LCDConf.h"

// ȫ�ֱ���:
HINSTANCE hInst;	  //��ǰʵ��

void LCDSIM_Init(void);  //LCDSIM.c��



//������Ϣ����ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void MainTask(void);
DWORD WINAPI GUI_Thead(LPVOID lpParamter)  //GUI�̺߳���
{
	MainTask();  //GUI Demo
	return 0;
}

//������	
int WINAPI WinMain(HINSTANCE hInstance,
								 HINSTANCE	hPrevInstance,
								 LPSTR lpCmdLine,
								 int  nShowCmd) 
{ 
	char *cName = "uCGUI-windows test";
	WNDCLASSEX wc; 
	HWND hWnd; 
	MSG Msg;
	HANDLE GUI_ThreadID;

	short win_x_size, win_y_size, pos_x, pos_y;

	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //	ͨ������������һ����ɫ�ı���������������ΪNULL�����������Ȥ��
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);//���������ʽ
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInstance;   //��ǰ����ľ����hInstance����ϵͳ�����ݵ�
	wc.lpfnWndProc = WndProc;  //	���ڴ�����̵Ļص�������
	wc.lpszClassName =(LPSTR)cName;   //	����������֡�
	wc.lpszMenuName = NULL;  //	Ŀ¼����������
	wc.style = CS_HREDRAW | CS_VREDRAW;  

	wc.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	wc.style        = 0;

	hInst = hInstance;

	win_x_size = LCD_XSIZE + 15;  //�����С
	win_y_size = LCD_YSIZE + 38;
	pos_x        = (GetSystemMetrics(SM_CXSCREEN ) - win_x_size) / 2;  //����λ��
	pos_y        = (GetSystemMetrics(SM_CYSCREEN ) - win_y_size) / 2;

	RegisterClassEx(&wc);  //��ϵͳ��ע��

	hWnd = CreateWindow(cName, cName,
									      WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_THICKFRAME),   //�̶���С
										  pos_x, pos_y,
										  win_x_size, win_y_size,
										  NULL,NULL,hInstance,NULL);
	if(hWnd == NULL) 
	{
		MessageBox(NULL, "There's an Error", "ErrorTitle", MB_ICONEXCLAMATION | MB_OK); 
		return FALSE; 
	} 
	

	//GUI_Init();  //��ʼ��GUI
	//GUI_SetBkColor(GUI_BLUE); 
	//GUI_Clear();
	//GUI_DispString("Hellow World!");
	//GUI_DrawCircle(200, 120, 30);


	ShowWindow(hWnd, nShowCmd);//	��ʾ����
	//ShowWindow(hWnd, SW_HIDE);                                      //���ش���
	UpdateWindow(hWnd); 

	SetTimer(hWnd, 1, 30, NULL);  //����һ����ʱ��  30ms
	GUI_ThreadID = CreateThread(NULL, 0, GUI_Thead, NULL, 0, NULL);  //����GUI�߳�
	CloseHandle(GUI_ThreadID);   //�ر��߳̾������


	//��Ϣ��ѭ������
	while(GetMessage(&Msg, NULL, 0 ,0)) 
	{ 
		TranslateMessage(&Msg);  //������Ϣ
		DispatchMessage(&Msg);  //	������Ϣ
	} 
	return Msg.message; 
} 

void UpdataShow(HDC _hdc);
void Touch_Pressed(int x, int y);
void Touch_Release(void);
//������Ϣ����ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_TIMER:  //��ʱ���¼����� ˢ����ʾ

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hdc = GetDC(hWnd);
		UpdataShow(hdc);

		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		Touch_Pressed(LOWORD(lParam), HIWORD(lParam));  //��������  ����Ϊ��ʱ����ڴ����е�λ��
		break;

	case WM_LBUTTONUP:
		Touch_Release();  //�����ͷ�
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);  //ȡ����ʱ��
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}