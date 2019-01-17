#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100
#define MAP_HEIGHT 50
#define MAP_WIDTH 100
#define Length 20

#define G_Weight 10
#define H_Weight 10
#define WATER_Weight 2

enum mapType {
	none,
	water,
	wall,
	start,
	end,
};

enum e_Direction {
	LL,
	LD,
	DD,
	RD,
	RR,
	RU,
	UU,
	LU,
};

struct st_Point {
	int x;
	int y;
};

struct st_Node {
	st_Point point = { 0,0 };
	int g;
	int h;
	int f;
	st_Node* pParent = nullptr;
};

bool operator==(st_Point lValue, st_Point rValue) {
	return (lValue.x == rValue.x) && (lValue.y == rValue.y);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL FindPath(st_Point start, st_Point end);
BOOL NearNodeMake(st_Node* pNode);
BOOL NodeMake(st_Point pPoint, st_Node *pParent);


void DrawMap(HDC hDc);
void DrawOpenList(HDC hDc);
void DrawCloseList(HDC hDc);
void DrawStart(HDC hDc);
void DrawEnd(HDC hDc);
void DrawPath(HDC hDc);

st_Node* PopMin(std::list<st_Node *> list);


void JumpPointSearch(st_Point start, st_Point end);
void SearchDirection(st_Node *pNode);
void CheckDirection(st_Node *pParent, e_Direction eDirection);
BOOL Jump(st_Point point, st_Point *outP, e_Direction eDirection, HDC dc);
BOOL JPSNodeMake(st_Point pPoint, st_Node *pParent);
