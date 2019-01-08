#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100
#define MAP_HEIGHT 50
#define MAP_WIDTH 100
#define Length 20

enum mapType {
	none,
	water,
	wall,
	start,
	end,
};

struct st_Point {
	int x;
	int y;
};

struct st_Node {
	st_Point point;
	int g;
	int h;
	int f;
	st_Node* pParent = nullptr;
};

bool operator<(st_Node lValue, st_Node rValue) {
	return lValue.f > rValue.f;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FindPath(st_Point start, st_Point end, st_Point *next);
BOOL NearNodeMake(st_Node* pNode);
BOOL NodeMake(st_Point* pPoint, st_Node *pParent);


void DrawMap(HDC hDc);
void DrawOpenList(HDC hDc);
void DrawCloseList(HDC hDc);