#include "GameScene.h"
#include <GL/glut.h>

GameScene::GameScene() :checkerBoard(Rect(0, 0, 900, 900))
{
}

void GameScene::Init()
{
	checkerBoard.Init();
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	checkerBoard.Draw();
}
