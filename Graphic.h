#pragma once
#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define MAIN_MENU 1
#define IN_GAME 2
#define GAME_WHITE_WIN 3
#define GAME_BLACK_WIN 4

const int CircleSegment = 50;

struct Point
{
	int x, y;
	Point() { x = y = 0; }
	Point(int a, int b) :x(a), y(b) {}
	bool operator == (const Point& t)const
	{
		return x == t.x && y == t.y;
	}
	Point operator + (const Point& t)const
	{
		return Point(x + t.x, y + t.y);
	}
};

struct Rect
{
	int lx, ly, rx, ry;
	Rect() { lx = ly = rx = ry = 0; }
	Rect(int a, int b, int c, int d) :lx(a), ly(b), rx(c), ry(d) {}
	bool inRect(Point a)
	{
		if (lx <= a.x && a.x <= rx && ly <= a.y && a.y <= ry)
			return true;
		return false;
	}
};

GLint LoadTexture(const char* file_name);
void DrawTexture(Rect pos, GLint texture);
void DrawCircleTexture(Point pos, double radius, double text_radius, GLint texture);
void DrawTransCircle(Point pos, double radius, double R, double G, double B, double A);
void DrawString(const char* str, const char* font, int size, int x, int y, double R, double G, double B);
void SelectFont(int size, int charset, const char* face);
