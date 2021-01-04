#include <GL/glut.h>
#include "GameScene.h"

const int INIT_WINDOW_WIDTH = 1024;
const int INIT_WINDOW_HEIGHT = 768;
const int SCENE_WIDTH = 1200;
const int SCENE_HEIGHT = 900;
const char TITLE[] = "NoGo";

void ChangeXY(int x, int y, int& resx, int& resy);

void Display();
void Idle();
void Reshape(int new_w, int new_h);
void Timer(int id);
void OnKeyBoard(unsigned char key, int x, int y);
void OnMouseClick(int button, int state, int x, int y);
void OnSpecialKey(int key, int x, int y);

GameScene* pGameScene;
int win_w, win_h;

void Display()
{
	pGameScene->Display();
}

void Idle()
{
	pGameScene->Idle();
}

void Reshape(int new_w, int new_h)
{
	glutReshapeWindow(win_w, win_h);
}

void Timer(int id)
{
}

void OnKeyBoard(unsigned char key, int x, int y)
{
}

void OnSpecialKey(int key, int x, int y)
{
}

void OnMouseMove(int x, int y)
{
	int tx, ty;
	ChangeXY(x, y, tx, ty);
	pGameScene->OnMouseMove(ty, tx);
}

void OnMouseClick(int button, int state, int x, int y)
{
	int tx, ty;
	ChangeXY(x, y, tx, ty);
	pGameScene->OnMouseClick(button, state, ty, tx);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	int cx = GetSystemMetrics(SM_CXFULLSCREEN), cy = GetSystemMetrics(SM_CYFULLSCREEN);
	win_w = INIT_WINDOW_WIDTH;
	win_h = INIT_WINDOW_HEIGHT;
	glutInitWindowPosition((cx - win_w) / 2, (cy - win_h) / 2);
	glutInitWindowSize(win_w, win_h);
	glutCreateWindow(TITLE);

	pGameScene = new GameScene(SCENE_WIDTH, SCENE_HEIGHT);
	pGameScene->Init();

	glutDisplayFunc(&Display);
	glutIdleFunc(&Idle);
	glutReshapeFunc(&Reshape);
	glutKeyboardFunc(&OnKeyBoard);
	glutSpecialFunc(&OnSpecialKey);
	glutMouseFunc(&OnMouseClick);
	glutPassiveMotionFunc(&OnMouseMove);

	glutMainLoop();

	return 0;
}

void ChangeXY(int x, int y, int& resx, int& resy)
{
	resy = (int)(1.0 * x * SCENE_WIDTH / win_w);
	resx = (int)(1.0 * y * SCENE_HEIGHT / win_h);
}