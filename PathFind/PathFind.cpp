// PathFind.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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
st_Point g_Start;
st_Point g_End;
std::list<st_Node *> openList;
std::list<st_Node *> closeList;


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


	g_Start.x = 10;
	g_Start.y = 20;

	map[g_Start.y][g_Start.x] = start;

	g_End.x = 50;
	g_End.y = 20;

	map[g_End.y][g_End.x] = end;


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
	case WM_LBUTTONUP:
		{

			if (drawMode == start) {
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

				map[g_Start.y][g_Start.x] = none;
				g_Start.x = xpos;
				g_Start.y = ypos;

				HDC dc = GetDC(hWnd);

				DrawMap(dc);

				ReleaseDC(hWnd, dc);
			}
			else if (drawMode == end) {
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

				map[g_End.y][g_End.x] = none;
				g_End.x = xpos;
				g_End.y = ypos;

				HDC dc = GetDC(g_hWnd);

				DrawMap(dc);

				ReleaseDC(hWnd, dc);
			}

		}
		break;
	case WM_LBUTTONDBLCLK:
		for (auto i = openList.begin(); i != openList.end(); ++i) {
			delete (*i);
		}

		for (auto i = closeList.begin(); i != closeList.end(); ++i) {
			delete (*i);
		}

		openList.clear();
		closeList.clear();

		//FindPath(g_Start, g_End);
		JumpPointSearch(g_Start, g_End);

		break;
	case WM_RBUTTONUP:
		{
			for (auto i = openList.begin(); i != openList.end(); ++i) {
				delete (*i);
			}

			for (auto i = closeList.begin(); i != closeList.end(); ++i) {
				delete (*i);
			}

			openList.clear();
			closeList.clear();

			InvalidateRect(hWnd, NULL, true);
		}
	case WM_MOUSEMOVE:
		{
			if (wParam != MK_LBUTTON)
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
			
			if (drawMode == start)
				break;
			else if (drawMode == end)
				break;
		
			map[ypos][xpos] = drawMode;

			InvalidateRect(hWnd, NULL, false);
		}
		break;

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

			case 0x34: //4
				drawMode = start;
				break;

			case 0x35: //5
				drawMode = end;
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

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

BOOL FindPath(st_Point start, st_Point end)
{
	NodeMake(start, nullptr);

	st_Node* t;

	while (1) {
		if (openList.size() == 0)
			return false;

		t = PopMin(openList);

		for (auto i = openList.begin(); i != openList.end(); ++i) {
			if (t->point == (*i)->point) {
				openList.erase(i);
				break;
			}
		}

		closeList.push_back(t);

		if (NearNodeMake(t))
			break;

		HDC hdc = GetDC(g_hWnd);

		DrawOpenList(hdc);
		DrawCloseList(hdc);

		ReleaseDC(g_hWnd, hdc);
	}

	HDC dc = GetDC(g_hWnd);

	DrawOpenList(dc);
	DrawCloseList(dc);

	HPEN linePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	HPEN oldP = (HPEN)SelectObject(dc, linePen);

	t = PopMin(openList);

	MoveToEx(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2, NULL);
	while (t->pParent != nullptr) {
		LineTo(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);
		t = (t->pParent);
	}
	LineTo(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);

	DrawStart(dc);
	DrawEnd(dc);

	SelectObject(dc, oldP);

	DeleteObject(linePen);
	ReleaseDC(g_hWnd, dc);

	return true;
}

BOOL NearNodeMake(st_Node * pNode)
{
	st_Point p;
	p.x = pNode->point.x;
	p.y = pNode->point.y + 1;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x;
	p.y = pNode->point.y - 1;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x + 1;
	p.y = pNode->point.y;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x - 1;
	p.y = pNode->point.y;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x - 1;
	p.y = pNode->point.y - 1;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x - 1;
	p.y = pNode->point.y + 1;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x + 1;
	p.y = pNode->point.y + 1;
	if (NodeMake(p, pNode))
		return true;

	p.x = pNode->point.x + 1;
	p.y = pNode->point.y - 1;
	if (NodeMake(p, pNode))
		return true;

	return false;
}

BOOL NodeMake(st_Point pPoint, st_Node * pParent)
{
	if (pPoint.x < 0 || pPoint.x >= MAP_WIDTH)
		return false;
	if (pPoint.y < 0 || pPoint.y >= MAP_HEIGHT)
		return false;

	if (map[pPoint.y][pPoint.x] == wall)
		return false;


	for (auto i = openList.begin(); i != openList.end(); ++i) {
		if ((*i)->point == pPoint) {
			if ((*i)->g > pParent->g + G_Weight) {
				(*i)->g = pParent->g + G_Weight;
				(*i)->f = (*i)->g + (*i)->h;
				(*i)->pParent = pParent;

				return false;
			}
			else {
				return false;
			}
		}
	}

	for (auto i = closeList.begin(); i != closeList.end(); ++i) {
		if ((*i)->point == pPoint) {
			if ((*i)->g > pParent->g + G_Weight) {
				(*i)->g = pParent->g + G_Weight;
				(*i)->f = (*i)->g + (*i)->h;
				(*i)->pParent = pParent;

				openList.push_front((*i));

				closeList.erase(i);

				return false;
			}
			else {
				return false;
			}
		}
	}

	st_Node* newNode = new st_Node;

	newNode->point = pPoint;
	if (pParent == nullptr) {
		newNode->g = G_Weight;
	}
	else {
		if (map[pPoint.y][pPoint.x] == water) {
			newNode->g = pParent->g + G_Weight * WATER_Weight;
		}
		else {
			newNode->g = pParent->g + G_Weight;
		}
	}
	newNode->h = abs(pPoint.x - g_End.x)*H_Weight + abs(pPoint.y - g_End.y) * H_Weight;
	newNode->f = newNode->g + newNode->h;
	newNode->pParent = pParent;

	openList.push_front(newNode);

	if (newNode->point == g_End)
		return true;
	return false;
}

void DrawMap(HDC hDc)
{
	HPEN nonePen, waterPen, wallPen, startPen, endPen;
	HBRUSH noneB, waterB, wallB, startB, endB;


	nonePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	noneB = CreateSolidBrush(RGB(255, 255, 255));
	waterPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	waterB = CreateSolidBrush(RGB(100, 100, 255));
	wallPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	wallB = CreateSolidBrush(RGB(80, 80, 80));
	startPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	startB = CreateSolidBrush(RGB(100, 255, 100));
	endPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
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

	openPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	openB = CreateSolidBrush(RGB(255, 255, 100));

	HPEN oldP = (HPEN)SelectObject(hDc, openPen);
	HBRUSH oldB = (HBRUSH)SelectObject(hDc, openB);


	for (auto i = openList.begin(); i != openList.end(); ++i) {
		Rectangle(hDc, (*i)->point.x * Length, (*i)->point.y*Length, (*i)->point.x*Length + Length, (*i)->point.y*Length + Length);
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

	closePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	clodeB = CreateSolidBrush(RGB(100, 255, 255));

	HPEN oldP = (HPEN)SelectObject(hDc, closePen);
	HBRUSH oldB = (HBRUSH)SelectObject(hDc, clodeB);

	for (auto i = closeList.begin(); i != closeList.end(); ++i) {
		Rectangle(hDc, (*i)->point.x * Length, (*i)->point.y*Length, (*i)->point.x*Length + Length, (*i)->point.y*Length + Length);
	}

	SelectObject(hDc, oldP);
	SelectObject(hDc, oldB);

	DeleteObject(closePen);
	DeleteObject(clodeB);

}

void DrawStart(HDC hDc)
{
	HPEN startPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	HBRUSH startB = CreateSolidBrush(RGB(100, 255, 100));


	HPEN oldP = (HPEN)SelectObject(hDc, startPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, startB);


	Rectangle(hDc, g_Start.x*Length, g_Start.y*Length, g_Start.x*Length + Length, g_Start.y*Length + Length);


	SelectObject(hDc, oldP);
	SelectObject(hDc, oldBrush);

	DeleteObject(startPen);
	DeleteObject(startB);
}

void DrawEnd(HDC hDc)
{
	HPEN endPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	HBRUSH endB = CreateSolidBrush(RGB(255, 100, 100));


	HPEN oldP = (HPEN)SelectObject(hDc, endPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDc, endB);


	Rectangle(hDc, g_End.x*Length, g_End.y*Length, g_End.x*Length + Length, g_End.y*Length + Length);

	SelectObject(hDc, oldP);
	SelectObject(hDc, oldBrush);

	DeleteObject(endPen);
	DeleteObject(endB);
}

void DrawPath(HDC hDc)
{
	st_Node* t = nullptr;
	for (auto i = openList.begin(); i != openList.end(); ++i) {
		if ((*i)->point == g_End) {
			t = (*i);
			break;
		}
	}

	if (t == nullptr)
		return;

	DrawOpenList(hDc);
	DrawCloseList(hDc);

	HPEN linePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	HPEN oldP = (HPEN)SelectObject(hDc, linePen);

	MoveToEx(hDc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2, NULL);
	while (t->pParent != nullptr) {
		LineTo(hDc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);
		t = (t->pParent);
	}
	LineTo(hDc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);


	SelectObject(hDc, oldP);

	DeleteObject(linePen);
}

st_Node * PopMin(std::list<st_Node*> list)
{

	auto min = list.begin();

	for (auto i = list.begin(); i != list.end(); ++i) {
		if ((*min)->f > (*i)->f)
			min = i;
	}

	return (*min);
}

void JumpPointSearch(st_Point start, st_Point end)
{
	JPSNodeMake(start, nullptr);
	
	st_Node *t;

	while (1) {
		if (openList.size() == 0)
			return;

		t = PopMin(openList);

		if (t->point == g_End)
			break;

		for (auto i = openList.begin(); i != openList.end(); ++i) {
			if (t->point == (*i)->point) {
				openList.erase(i);
				break;
			}
		}

		SearchDirection(t);

		closeList.push_back(t);

		HDC hdc = GetDC(g_hWnd);

		DrawOpenList(hdc);
		DrawCloseList(hdc);

		ReleaseDC(g_hWnd, hdc);

	}


	HDC dc = GetDC(g_hWnd);

	DrawOpenList(dc);
	DrawCloseList(dc);

	HPEN linePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	HPEN oldP = (HPEN)SelectObject(dc, linePen);

	t = PopMin(openList);

	MoveToEx(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2, NULL);
	while (t->pParent != nullptr) {
		LineTo(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);
		t = (t->pParent);
	}
	LineTo(dc, t->point.x* Length + Length / 2, t->point.y* Length + Length / 2);

	DrawStart(dc);
	DrawEnd(dc);

	SelectObject(dc, oldP);

	DeleteObject(linePen);
	ReleaseDC(g_hWnd, dc);

}

void SearchDirection(st_Node * pNode)
{
	if (pNode == nullptr)
		return;

	if (pNode->pParent == nullptr) {
		CheckDirection(pNode, LL);
		CheckDirection(pNode, LD);
		CheckDirection(pNode, DD);
		CheckDirection(pNode, RD);
		CheckDirection(pNode, RR);
		CheckDirection(pNode, RU);
		CheckDirection(pNode, UU);
		CheckDirection(pNode, LU);

		return;
	}

	st_Point parentPoint = pNode->pParent->point;
	st_Point myPoint = pNode->point;

	if (myPoint.x < parentPoint.x  && myPoint.y == parentPoint.y) {
		CheckDirection(pNode, LL);

		if (map[myPoint.y - 1][myPoint.x] == wall) {
			CheckDirection(pNode, LU);
		}

		if (map[myPoint.y + 1][myPoint.x] == wall) {
			CheckDirection(pNode, LD);
		}
	}
	else if (myPoint.x > parentPoint.x  && myPoint.y == parentPoint.y) {
		CheckDirection(pNode, RR);

		if (map[myPoint.y - 1][myPoint.x] == wall) {
			CheckDirection(pNode, RU);
		}

		if (map[myPoint.y + 1][myPoint.x] == wall) {
			CheckDirection(pNode, RD);
		}
	}
	else if (myPoint.x == parentPoint.x && myPoint.y > parentPoint.y) {
		CheckDirection(pNode, DD);

		if (map[myPoint.y][myPoint.x + 1] == wall) {
			CheckDirection(pNode, RD);
		}

		if (map[myPoint.y][myPoint.x - 1] == wall) {
			CheckDirection(pNode, LD);
		}
	}
	else if (myPoint.x == parentPoint.x && myPoint.y < parentPoint.y) {
		CheckDirection(pNode, UU);

		if (map[myPoint.y][myPoint.x + 1] == wall) {
			CheckDirection(pNode, RU);
		}

		if (map[myPoint.y][myPoint.x - 1] == wall) {
			CheckDirection(pNode, LU);
		}
	}
	else if (myPoint.x < parentPoint.x && myPoint.y < parentPoint.y) {
		CheckDirection(pNode, LU);
		CheckDirection(pNode, UU);
		CheckDirection(pNode, LL);

		if (map[myPoint.y][myPoint.x + 1] == wall) {
			CheckDirection(pNode, RU);
		}
		if (map[myPoint.y + 1][myPoint.x] == wall) {
			CheckDirection(pNode, LD);
		}
	}
	else if (myPoint.x > parentPoint.x && myPoint.y < parentPoint.y) {
		CheckDirection(pNode, RU);
		CheckDirection(pNode, UU);
		CheckDirection(pNode, RR);

		if (map[myPoint.y][myPoint.x - 1] == wall) {
			CheckDirection(pNode, LU);
		}
		if (map[myPoint.y + 1][myPoint.x] == wall) {
			CheckDirection(pNode, RD);
		}
	}
	else if (myPoint.x < parentPoint.x && myPoint.y > parentPoint.y) {
		CheckDirection(pNode, LD);
		CheckDirection(pNode, DD);
		CheckDirection(pNode, LL);

		if (map[myPoint.y - 1][myPoint.x] == wall) {
			CheckDirection(pNode, LU);
		}
		if (map[myPoint.y][myPoint.x + 1] == wall) {
			CheckDirection(pNode, RD);
		}
	}
	else if (myPoint.x > parentPoint.x && myPoint.y > parentPoint.y) {
		CheckDirection(pNode, RD);
		CheckDirection(pNode, DD);
		CheckDirection(pNode, RR);

		if (map[myPoint.y - 1][myPoint.x] == wall) {
			CheckDirection(pNode, RU);
		}
		if (map[myPoint.y][myPoint.x - 1] == wall) {
			CheckDirection(pNode, LD);
		}
	}
}

void CheckDirection(st_Node * pParent, e_Direction eDirection)
{
	st_Point myPoint = pParent->point;
	st_Point *outP = new st_Point;


	if (Jump(myPoint, outP, eDirection)) {
		JPSNodeMake(*outP, pParent);
	}
	else {
		delete outP;
	}
}

BOOL Jump(st_Point point, st_Point * outP, e_Direction eDirection)
{
	if (point.x < 0 || point.x >= MAP_WIDTH)
		return false;
	if (point.y < 0 || point.y >= MAP_HEIGHT)
		return false; 
	
	if (map[point.y][point.x] == wall)
		return false;

	if (map[point.y][point.x] == end) {
		outP->x = point.x;
		outP->y = point.y;

		return true;
	}

	switch (eDirection) {
	case LL:
		while (1) {
			point.x--;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;

			if (map[point.y - 1][point.x] == wall && map[point.y - 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y + 1][point.x] == wall && map[point.y + 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y][point.x - 1] == wall) {
				return false;
			}

		}

		break;

	case RR:
		while (1) {
			point.x++;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;

			if (map[point.y - 1][point.x] == wall && map[point.y - 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y + 1][point.x] == wall && map[point.y + 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y][point.x + 1] == wall) {
				return false;
			}
		}

		break;
	case UU:
		while (1) {
			point.y--;

			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (map[point.y][point.x - 1] == wall && map[point.y - 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x + 1] == wall && map[point.y - 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y - 1][point.x] == wall) {
				return false;
			}
		}

		break;
	case DD:
		while (1) {
			point.y++;

			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (map[point.y][point.x - 1] == wall && map[point.y + 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x + 1] == wall && map[point.y + 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y + 1][point.x] == wall) {
				return false;
			}
		}

		break;

	case LD:
		while (1) {
			point.x--;
			point.y++;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;
			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (Jump(point, outP, LL)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}
			if (Jump(point, outP, DD)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}


			if (map[point.y - 1][point.x] == wall && map[point.y - 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x + 1] == wall && map[point.y + 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y + 1][point.x - 1] == wall) {
				return false;
			}
		}

		break;

	case LU:
		while (1) {
			point.x--;
			point.y--;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;
			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (Jump(point, outP, LL)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}
			if (Jump(point, outP, UU)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}


			if (map[point.y + 1][point.x] == wall && map[point.y + 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x + 1] == wall && map[point.y - 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y - 1][point.x - 1] == wall) {
				return false;
			}
		}

		break;

	case RU:
		while (1) {
			point.x++;
			point.y--;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;
			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (Jump(point, outP, RR)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}
			if (Jump(point, outP, UU)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}


			if (map[point.y + 1][point.x] == wall && map[point.y + 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x - 1] == wall && map[point.y - 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y - 1][point.x + 1] == wall) {
				return false;
			}
		}

		break;

	case RD:
		while (1) {
			point.x++;
			point.y++;

			if (point.x < 0 || point.x >= MAP_WIDTH)
				return false;
			if (point.y < 0 || point.y >= MAP_HEIGHT)
				return false;

			if (Jump(point, outP, RR)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}
			if (Jump(point, outP, DD)) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}


			if (map[point.y - 1][point.x] == wall && map[point.y - 1][point.x + 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x - 1] == wall && map[point.y + 1][point.x - 1] != wall) {
				outP->x = point.x;
				outP->y = point.y;
				return true;
			}

			if (map[point.y][point.x] == end) {
				outP->x = point.x;
				outP->y = point.y;

				return true;
			}

			if (map[point.y + 1][point.x + 1] == wall) {
				return false;
			}
		}
		break;
	default:
		return false;
	}

	return false;
}

BOOL JPSNodeMake(st_Point pPoint, st_Node * pParent)
{
	if (pPoint.x < 0 || pPoint.x >= MAP_WIDTH)
		return false;
	if (pPoint.y < 0 || pPoint.y >= MAP_HEIGHT)
		return false;

	if (map[pPoint.y][pPoint.x] == wall)
		return false;


	for (auto i = openList.begin(); i != openList.end(); ++i) {
		if ((*i)->point == pPoint) {
			int realG = pParent->g + (abs(pParent->point.x - pPoint.x) + abs(pParent->point.y - pPoint.y)) * G_Weight;

			if ((*i)->g > realG) {
				(*i)->g = realG;
				(*i)->f = (*i)->g + (*i)->h;
				(*i)->pParent = pParent;

				return false;
			}
			else {
				return false;
			}
		}
	}

	for (auto i = closeList.begin(); i != closeList.end(); ++i) {
		if ((*i)->point == pPoint) {
			int realG = pParent->g + (abs(pParent->point.x - pPoint.x) + abs(pParent->point.y - pPoint.y)) * G_Weight;

			if ((*i)->g > realG) {
				(*i)->g = realG;
				(*i)->f = (*i)->g + (*i)->h;
				(*i)->pParent = pParent;

				openList.push_front((*i));

				closeList.erase(i);

				return false;
			}
			else {
				return false;
			}
		}
	}

	st_Node* newNode = new st_Node;

	newNode->point = pPoint;
	if (pParent == nullptr) {
		newNode->g = G_Weight;
	}
	else {
		newNode->g = pParent->g + (abs(pParent->point.x - pPoint.x) + abs(pParent->point.y - pPoint.y)) * G_Weight;
	}
	newNode->h = abs(pPoint.x - g_End.x)*H_Weight + abs(pPoint.y - g_End.y) * H_Weight;
	newNode->f = newNode->g + newNode->h;
	newNode->pParent = pParent;

	openList.push_front(newNode);

	if (newNode->point == g_End)
		return true;

	return false;
}

