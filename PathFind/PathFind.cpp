﻿// PathFind.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PathFind.h"



// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd;

BYTE map[MAP_HEIGHT][MAP_WIDTH];
mapType drawMode;
std::priority_queue<st_Node> openList;
std::priority_queue<st_Node> closeList;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATHFIND));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_PATHFIND);
	wcex.lpszClassName = L"test";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(L"test", L"그래프그리기", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// TODO: 여기에 코드를 입력합니다.

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_MOUSEMOVE:
		{
			if(wParam != MK_LBUTTON)
				return DefWindowProc(hWnd, message, wParam, lParam);

			int xpos = GET_X_LPARAM(lParam) / Length;
			int ypos = GET_Y_LPARAM(lParam) / Length;

			if (xpos >= MAP_WIDTH)
				return DefWindowProc(hWnd, message, wParam, lParam);
			if (xpos < 0)
				return DefWindowProc(hWnd, message, wParam, lParam);

			if (ypos >= MAP_HEIGHT)
				return DefWindowProc(hWnd, message, wParam, lParam);
			if (ypos < 0)
				return DefWindowProc(hWnd, message, wParam, lParam);

			map[ypos][xpos] = drawMode;

			InvalidateRect(hWnd, NULL, false);
		}

	case WM_KEYDOWN:
		{
			switch (wParam) {
			case 0x31: // 1key
				drawMode = none;
				break;

			case 0x32: // 2key
				drawMode = water;
				break;

			case 0x33:  // 3key
				drawMode = wall;
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

			DrawMap(hdc);
			DrawOpenList(hdc);
			DrawCloseList(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawMap(HDC hDc)
{
	HPEN nonePen, waterPen, wallPen, startPen, endPen;
	HBRUSH noneB, waterB, wallB, startB, endB;


	nonePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	noneB = CreateSolidBrush(RGB(255, 255, 255));
	waterPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 255));
	waterB = CreateSolidBrush(RGB(100, 100, 255));
	wallPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
	wallB = CreateSolidBrush(RGB(80, 80, 80));
	startPen = CreatePen(PS_SOLID, 1, RGB(100, 255, 100));
	startB = CreateSolidBrush(RGB(100, 255, 100));
	endPen = CreatePen(PS_SOLID, 1, RGB(255, 100, 100));
	endB = CreateSolidBrush(RGB(255, 100, 100));


	HPEN oldPen = (HPEN)SelectObject(hDc, nonePen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, noneB);

	for(int i=0;i<MAP_HEIGHT;++i){
		for (int j = 0; j < MAP_WIDTH; ++j) {
			switch (map[i][j]) {
			case none:
				SelectObject(hDc, nonePen);
				SelectObject(hDc, noneB);
				Rectangle(hDc, j*Length, i*Length, j*Length + Length, i*Length + Length);
				break;

			case water:
				SelectObject(hDc, waterPen);
				SelectObject(hDc, waterB);
				Rectangle(hDc, j*Length, i*Length, j*Length + Length, i*Length + Length); 
				break;

			case wall:
				SelectObject(hDc, wallPen);
				SelectObject(hDc, wallB);
				Rectangle(hDc, j*Length, i*Length, j*Length + Length, i*Length + Length);
				break;

			case start:
				SelectObject(hDc, startPen);
				SelectObject(hDc, startB);
				Rectangle(hDc, j*Length, i*Length, j*Length + Length, i*Length + Length);
				break;

			case end:
				SelectObject(hDc, endPen);
				SelectObject(hDc, endB);
				Rectangle(hDc, j*Length, i*Length, j*Length + Length, i*Length + Length);
				break;

			default:
				break;
			}
		}
	}

	SelectObject(hDc, oldPen);
	SelectObject(hDc, oldBrush);

	DeleteObject(nonePen);
	DeleteObject(waterPen);
	DeleteObject(wallPen);
	DeleteObject(startPen);
	DeleteObject(endPen);

	DeleteObject(noneB);
	DeleteObject(waterB);
	DeleteObject(wallB);
	DeleteObject(startB);
	DeleteObject(endB);
}

void DrawOpenList(HDC hDc)
{
	HPEN openPen;
	HBRUSH openB;

	openPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 100));
	openB = CreateSolidBrush(RGB(255, 255, 100));

	HPEN oldP = (HPEN)SelectObject(hDc, openPen);
	HBRUSH oldB = (HBRUSH)SelectObject(hDc, openB);

	std::priority_queue<st_Node> tmp = openList;

	for (int i = 0; i < tmp.size(); ++i) {
		st_Node t = tmp.top();

		Rectangle(hDc, t.point.x * Length, t.point.y*Length, t.point.x*Length + Length, t.point.y*Length + Length);

		tmp.pop();
	}

	SelectObject(hDc, oldP);
	SelectObject(hDc, oldB);

	DeleteObject(openPen);
	DeleteObject(openB);
}

void DrawCloseList(HDC hDc)
{
	HPEN closePen;
	HBRUSH clodeB;

	closePen = CreatePen(PS_SOLID, 1, RGB(100, 255, 255));
	clodeB = CreateSolidBrush(RGB(100, 255, 255));

	HPEN oldP = (HPEN)SelectObject(hDc, closePen);
	HBRUSH oldB = (HBRUSH)SelectObject(hDc, clodeB);

	std::priority_queue<st_Node> tmp = closeList;

	for (int i = 0; i < tmp.size(); ++i) {
		st_Node t = tmp.top();

		Rectangle(hDc, t.point.x * Length, t.point.y*Length, t.point.x*Length + Length, t.point.y*Length + Length);

		tmp.pop();
	}

	SelectObject(hDc, oldP);
	SelectObject(hDc, oldB);

	DeleteObject(closePen);
	DeleteObject(clodeB);

}

