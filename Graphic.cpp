#include "Graphic.h"
#include <cstdio>

GLint LoadTexture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLint last_texture_ID;
	GLuint texture_ID = 0;

	FILE* pFile;
	fopen_s(&pFile, file_name, "rb");
	if (pFile == 0)
		return 0;

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, 54, SEEK_SET);

	GLint line_bytes = width * 3;
	while (line_bytes % 4 != 0)
		++line_bytes;
	total_bytes = line_bytes * height;

	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	GLint mx;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mx);
	if (!((width & (width - 1)) != 0)
		|| !((width & (height - 1)) != 0)
		|| width > mx
		|| height > mx)
	{
		const GLint new_width = 256;
		const GLint new_height = 256;
		GLint new_line_bytes, new_total_bytes;
		GLubyte* new_pixels = 0;

		new_line_bytes = new_width * 3;
		while (new_line_bytes % 4 != 0)
			++new_line_bytes;
		new_total_bytes = new_line_bytes * new_height;

		new_pixels = (GLubyte*)malloc(new_total_bytes);
		if (new_pixels == 0)
		{
			free(pixels);
			fclose(pFile);
			return 0;
		}

		gluScaleImage(GL_RGB,
			width, height, GL_UNSIGNED_BYTE, pixels,
			new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

		free(pixels);
		pixels = new_pixels;
		width = new_width;
		height = new_height;
	}

	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	free(pixels);
	return texture_ID;
}

void DrawTexture(Rect pos, GLint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2d(pos.lx, pos.ry);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2d(pos.rx, pos.ry);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2d(pos.rx, pos.ly);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2d(pos.lx, pos.ly);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawCircleTexture(Point pos, double radius, double text_radius, GLint texture)
{
	const double M_PI = acos(-1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < CircleSegment; i++)
	{
		glTexCoord2d(0.5 + text_radius * cos(2 * M_PI * i / CircleSegment), 0.5 + text_radius * sin(2 * M_PI * i / CircleSegment));
		glVertex2d(pos.x - radius * cos(2 * M_PI * i / CircleSegment), pos.y - radius * sin(2 * M_PI * i / CircleSegment));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawTransCircle(Point pos, double radius, double R, double G, double B, double A)
{
	const double M_PI = acos(-1);
	glEnable(GL_BLEND);
	glBegin(GL_TRIANGLE_FAN);
	glColor4d(R, G, B, A);
	for (int i = 0; i < CircleSegment; i++)
	{
		glVertex2d(pos.x - radius * cos(2 * M_PI * i / CircleSegment), pos.y - radius * sin(2 * M_PI * i / CircleSegment));
	}
	glEnd();
	glDisable(GL_BLEND);
}

void DrawString(const char* str, const char* font, int size, int x, int y, double R, double G, double B)
{
	SelectFont(size, GB2312_CHARSET, font);
	glColor3d(R, G, B);
	glRasterPos2d(x, y);

	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint  list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

void SelectFont(int size, int charset, const char* face)
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_BOLD, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
