#include <GL/glut.h>
#include "GameScene.h"

const int INIT_WINDOW_WIDTH = 1024;
const int INIT_WINDOW_HEIGHT = 768;
const char TITLE[] = "NoGo";

void Display();
void Idle();
void Reshape(int new_w, int new_h);
void Timer(int id);
void OnKeyBoard(unsigned char key, int x, int y);
void OnMouse(int button, int state, int x, int y);
void OnSpecialKey(int key, int x, int y);

GameScene *pGameScene;
int win_w, win_h;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	pGameScene->Display();

	glutSwapBuffers();
}

void Idle()
{
	/*if (timer_setted == false)
	{
		if (UpdateScreen != NULL)
			(*UpdateScreen)();
		Display();
	}*/
	//Display();
}

void Reshape(int new_w, int new_h)
{
	glutReshapeWindow(win_w, win_h);
	//win_width = new_w;
	//win_height = new_h;

	//nowsce.Update();
}

void Timer(int id)
{
	/*switch (id)
	{
	case TIMER_UPDATE:
		if (UpdateScreen != NULL)
			(*UpdateScreen)();
		Display();
		glutTimerFunc(time_to_update, &Timer, TIMER_UPDATE);
		break;
	default:
		break;
	}*/
}

void OnKeyBoard(unsigned char key, int x, int y)
{
	/*int tx, ty;
	CtoGui::ChangeXY(x, y, tx, ty);
	if (KeyProcess != NULL)
		KeyProcess(key, ty, tx);*/
}

void OnSpecialKey(int key, int x, int y)
{
	/*int tx, ty;
	CtoGui::ChangeXY(x, y, tx, ty);
	if (SpecialProcess != NULL)
		SpecialProcess(key, ty, tx);*/
}

void OnMouse(int button, int state, int x, int y)
{
	/*int tx, ty;
	CtoGui::ChangeXY(x, y, tx, ty);
	if (MouseProcess != NULL)
		MouseProcess(button, state, ty, tx);*/
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

	pGameScene = new GameScene();
	pGameScene->Init();

	glutDisplayFunc(&Display);
	glutIdleFunc(&Idle);
	glutReshapeFunc(&Reshape);
	glutKeyboardFunc(&OnKeyBoard);
	glutSpecialFunc(&OnSpecialKey);
	glutMouseFunc(&OnMouse);

	glutMainLoop();

	return 0;
}
