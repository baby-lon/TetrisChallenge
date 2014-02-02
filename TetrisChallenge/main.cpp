#include <windows.h>
#include "Tetris.h"

CTetris* Game = NULL;

#define TIMER_MOVE 1

LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;	

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		SetTimer(hwnd , TIMER_MOVE , 50 , NULL);
		return 0;
	case WM_TIMER:
	{
		int timer_id = wp;
		switch(timer_id){
			case TIMER_MOVE:
			{
				Game->Move();
				Game->Draw();
				InvalidateRect(hwnd , NULL , FALSE);
			}
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch(wp){
			case VK_RIGHT:
				Game->MovePiece(1);
				Game->Draw();
				InvalidateRect(hwnd , NULL , FALSE);
			break;
			case VK_LEFT:
				Game->MovePiece(-1);
				Game->Draw();
				InvalidateRect(hwnd , NULL , FALSE);
				break;
			case VK_UP:
				Game->RotatePiece();
				Game->Draw();
				InvalidateRect(hwnd , NULL , FALSE);
				break;
			case VK_DOWN:
				Game->SpeedUp(true);
				InvalidateRect(hwnd , NULL , FALSE);
				break;
		}
		break;
	}
	case WM_KEYUP:
	{
		switch(wp){
			case VK_DOWN:
				Game->SpeedUp(false);
				InvalidateRect(hwnd , NULL , FALSE);
				break;
		}
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd , &ps);
		Game->BitBlt(hdc);
		EndPaint(hwnd , &ps);
		return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance ,
			PSTR lpCmdLine , int nCmdShow ) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;
	
	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("Tetris");

	if (!RegisterClass(&winc)){
		return -1;
	}

	Game = new CTetris();
	int fix_w = 16;
	int fix_h = 38;
	hwnd = CreateWindow(
			TEXT("TETRIS") , TEXT("Tetris"),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MINIMIZE,
			CW_USEDEFAULT , CW_USEDEFAULT,
			Game->GetBitmapWidth()+fix_w , Game->GetBitmapHeight() + fix_h,
			NULL , NULL , hInstance , NULL
	);

	if (hwnd == NULL){
		return -1;
	}

	Game->CreateBuffer(hInstance, hwnd);
	Game->Draw();

	while(GetMessage(&msg , NULL , 0 , 0)){
		DispatchMessage(&msg);
	}

	delete Game;
	Game = NULL;
	return msg.wParam;
}
