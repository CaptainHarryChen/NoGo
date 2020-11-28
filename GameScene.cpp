#include "GameScene.h"
#include <GL/glut.h>

GameScene::GameScene(int width, int height) :scene_width(width), scene_height(height)
{
	pRuler = new GameRule;
	pCheckerBoard = new CheckerBoard(Rect(0, 0, 900, 900), pRuler);
	pRuler->Register(pCheckerBoard);
}

void GameScene::Init()
{
	pCheckerBoard->Init();
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	pCheckerBoard->Draw();
}

void GameScene::OnMouseMove(int x, int y)
{
}

void GameScene::OnMouseClick(int button, int state, int x, int y)
{
	if (pCheckerBoard->in(Point(x, y)))
		pCheckerBoard->OnMouseClick(Point(x, y));
}
