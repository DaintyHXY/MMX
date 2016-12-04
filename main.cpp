/* 
   Name: WinAnim
   Author: Brook Miles
   Description: Making an animation in windows
*/

#include <windows.h>

static char g_szClassName[] = "MyWindowClass";
static HINSTANCE g_hInst = NULL;

const UINT idTimer1 = 1;
UINT nTimerDelay = 10;

HBITMAP hbmBall, hbmMask;
BITMAP bm;

int ballX, ballY;
int deltaX, deltaY;

int deltaValue = 4;

/*void EraseBall(HDC hdc)
{
   RECT rc;
   rc.left = ballX;
   rc.top = ballY;
   rc.right = ballX + bm.bmWidth;
   rc.bottom = ballY + bm.bmHeight;
   FillRect(hdc, &rc, (HBRUSH)(COLOR_BTNFACE+1));
}*/

void DrawBall(HDC hdc)
{
   HDC hdcMemory;
   hdcMemory = CreateCompatibleDC(hdc);

   SelectObject(hdcMemory, hbmMask);
   BitBlt(hdc, ballX, ballY, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCAND);

   SelectObject(hdcMemory, hbmBall);
   BitBlt(hdc, ballX, ballY, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCPAINT);

   DeleteDC(hdcMemory);
}

int Mmx(LPBYTE Picture1,LPBYTE Picture2,LPBYTE Picture,int intWidth,int intHeight,int RGB_Bit,int i){
	int x,y;
	LPDWORD temp1,temp2,temp;
	int fade_Rate=1*128;
	WORD fade1[4],fade2[4];
	fade1[0]=fade1[1]=fade1[2]=fade1[3]=32767-fade_Rate;
	fade2[0]=fade2[1]=fade2[2]=fade2[3]=fade_Rate;
	for(y=0;y<intHeight;y++){
		temp1=(LPDWORD)(Picture1+intWidth*RGB_Bit/8*y);
		temp2=(LPDWORD)(Picture2+intWidth*RGB_Bit/8*y);
		temp=(LPDWORD)(Picture+intWidth*RGB_Bit/8*y);
		for(x=0;x<intWidth;x++){
			__asm{
				Result_pixel=A_pixel*fade+B_pixel*(1-fade)=(A-B)*fade+B;
				pxor  mm7,mm7
				movq  mm2,[fade1]
				movq  mm3,[fade2]
				mov   esi,[temp1]
				mov   edx,[temp2]
				mov   edi,[temp]
				movd  mm0,[esi]
				movd  mm1,[edx]
				punpcklbw mm0,mm7
				punpcklbw mm1,mm7
				pmulhw mm0,mm2
				pmulhw mm1,mm3
				paddw  mm0,mm1
				packuswb mm0,mm7
				movd   [edi],mm0
				
				
			}
			temp1++;
			temp2++;
			temp++;
		}
	}
	__asm EMMS
	return 0;
}

int Mmx2(LPBYTE Picture1,LPBYTE Picture2,LPBYTE Picture,int intWidth,int intHeight,int RGB_Bit,int i){
	int x,y;
	LPBYTE temp1,temp2,temp;
	BYTE fade_Rate=(BYTE)i;
	BYTE fade1,fade2;
	fade1=255-fade_Rate;
	fade2=fade_Rate;
	for(y=0;y<intHeight;y++){
	temp1=(LPBYTE)(Picture1+intWidth*RGB_Bit/8*y); 
	temp2=(LPBYTE)(Picture2+intWidth*RGB_Bit/8*y);
	temp=(LPBYTE)(Picture+intWidth*RGB_Bit/8*y);
	for(x=0;x<3*intWidth;x++){
		__asm{
			mov dh,[fade1]
			mov dl,[fade2]
			mov ecx,[temp1]
			mov bh,[ecx]
			mov esi,[temp2]
			mov bl,[esi]
			mov al,dh
			mul bh
			mov edi,[temp]
			mov [edi],ah
			mov al,dl
			mul bl
			add ah,[edi]
			mov [edi],ah
			
		}
		temp1++;
		temp2++;
		temp++;
	}
 }
 __asm EMMS
 return0;
}

void Test_MMX(int intWidth,int intHeight,int RGB_Bit,int Time){
	__try{
		_asm EMMS
		
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	HDC hDC;
	LPBITMAPINFO lpInfo;
	int intSize,i;
	LPBYTE lpBufl,lpBuf2,lpBuf;
	HEILE Picture1,Picture2;
	Picture1=_lopen("test3.bmp",OF_READ);
	Picture2=_lopen("test4.bmp",OF_READ);
	intSize=GetFileSize((HANDLE)Picture1,NULL);
	lpBuf1(LPBYTE)LocalAlloc(LPTR,inSize);
	lpBuf2(LPBYTE)LocalAlloc(LPTR,inSize);
	lpBuf(LPBYTE)LocalAlloc(LPTR,inSize);
	_lread(Picture1,lpBuf1,intSize);
	_lread(Picture2,lpBuf2,intSize);
	_lclose(Picture1);
	_lclose(Picture2);
	memcpy(lpBuf,lpBuf1,intSize);
	lpInfo=(LPBITMAPINFO)(lpBuf+0x0e);
	hDC=GetDC(NULL);
	For(i=0;i<255;i++){
		Mmx(lpBuf1+0x60,lpBuf2+0x60,lpBuf+0x60,intWidth,intHeight,RGB_Bit,i);
		SetDIBitsToDevice(hDC,300,150,intWidth,intHeight,0,0,0,480,lpBuf+0x60,lpInfo,DIB_RGB_COLORS);
		
	}
	LocalFree(lpBuf1);
	LocalFree(lpBuf2);
	ReleaseDC(NULL,hDC);
	
	
	
}
using namespace std;
int _tmain(int argc,__TCHAR*argv[]){
	DWORD start_time=GetTickCount();
	Test_MMX(640,480,24,8);
	DWORD end_time=GetTickCount();
	cout<<"使用MMX用时："<<(end_time-start_time)<<"ms!"<<endl;
	system("pause");
	return 0;
}

/*void UpdateBall(HWND hwnd)
{
   RECT rc; 
   GetClientRect(hwnd, &rc);

   ballX += deltaX;
   ballY += deltaY;

   if(ballX < 0){
      ballX = 0;
      deltaX = deltaValue;
   }
   else if(ballX + bm.bmWidth > rc.right){
      ballX = rc.right - bm.bmWidth;
      deltaX = -deltaValue;
   }
   if(ballY < 0){
      ballY = 0;
      deltaY = deltaValue;
   }
   else if(ballY + bm.bmHeight > rc.bottom){
      ballY = rc.bottom - bm.bmHeight;
      deltaY = -deltaValue;
   }
}*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
   switch(Message)
   {
      case WM_CREATE:
         hbmBall = LoadBitmap(g_hInst, "BALLBMP");
         hbmMask = LoadBitmap(g_hInst, "MASKBMP");
         if(!hbmBall || !hbmMask){
            MessageBox(hwnd, "Load of resources failed.", "Error",
               MB_OK | MB_ICONEXCLAMATION);
            return -1;
         }

         GetObject(hbmBall, sizeof(bm), &bm);
         SetTimer(hwnd, idTimer1, nTimerDelay, NULL);

         ballX = 0;
         ballY = 0;
         deltaX = deltaValue;
         deltaY = deltaValue;

      break;
     /* case WM_TIMER:
         if(hbmBall && hbmMask)
         {
            HDC hdcWindow;
            hdcWindow = GetDC(hwnd);

            EraseBall(hdcWindow);
            UpdateBall(hwnd);
            DrawBall(hdcWindow);

            ReleaseDC(hwnd, hdcWindow);
         }
      break;*/
      case WM_PAINT:
         if(hbmBall && hbmMask)
         {
            PAINTSTRUCT ps;
            HDC hdcWindow;
            hdcWindow = BeginPaint(hwnd, &ps);

            DrawBall(hdcWindow);
            
            EndPaint(hwnd, &ps);
         }
      break;
      case WM_CLOSE:
         DestroyWindow(hwnd);
      break;
      case WM_DESTROY:
         KillTimer(hwnd, idTimer1);
         
         DeleteObject(hbmBall);
         DeleteObject(hbmMask);
         PostQuitMessage(0);
      break;
      default:
         return DefWindowProc(hwnd, Message, wParam, lParam);
   }
   return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpCmdLine, int nCmdShow)
{
   WNDCLASSEX WndClass;
   HWND hwnd;
   MSG Msg;

   g_hInst = hInstance;

   WndClass.cbSize        = sizeof(WNDCLASSEX);
   WndClass.style         = 0;
   WndClass.lpfnWndProc   = WndProc;
   WndClass.cbClsExtra    = 0;
   WndClass.cbWndExtra    = 0;
   WndClass.hInstance     = g_hInst;
   WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
   WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
   WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
   WndClass.lpszMenuName  = NULL;
   WndClass.lpszClassName = g_szClassName;
   WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

   if(!RegisterClassEx(&WndClass))
   {
      MessageBox(0, "Window Registration Failed!", "Error!",
         MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
      return 0;
   }

   hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      g_szClassName,
      "A Bitmap Program",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
      NULL, NULL, g_hInst, NULL);

   if(hwnd == NULL)
   {
      MessageBox(0, "Window Creation Failed!", "Error!",
         MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
      return 0;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   while(GetMessage(&Msg, NULL, 0, 0))
   {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
   }
   return Msg.wParam;
}

