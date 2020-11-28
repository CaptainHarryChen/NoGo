#pragma once
#include <GL/glut.h>

struct Rect
{
	int lx, ly, rx, ry;
	Rect() {}
	Rect(int a, int b, int c, int d) :lx(a), ly(b), rx(c), ry(d) {}
};

GLint LoadTexture(const char* file_name);
void DrawTexture(Rect pos, GLint texture);