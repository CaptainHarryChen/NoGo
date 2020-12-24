#include <GL/glut.h>
#include "Graphic.h"
#include "SaveManager.h"
#include "GameAI_MCTS.h"
#include "GameScene.h"


void GameScene::StartGame(Color c)
{
	col_human = c;
	col_ai = c == Color::BLACK ? Color::WHITE : Color::BLACK;

	if (pRuler != nullptr)
		delete pRuler;
	pRuler = new GameRule;

	if (pAI != nullptr)
		delete pAI;
	pAI = new GameAI_MCTS(col_ai);

	pCheckerBoard->Init(c, pRuler, pAI);
	pAI->SetBeginningState();
	pAI->Start();
	game_state = IN_GAME;
	pMenuBoard->SetGameState(game_state);
	pStartBlack->SetText(Text("重新开始：黑棋", "楷体", 22));
	pStartWhite->SetText(Text("重新开始：白棋", "楷体", 22));

	msg_send = false;

	if (!msg_send && pRuler->moveColor() == col_ai)
	{
		pAI->SendGameMessage();
		msg_send = true;
	}
}

GameScene::GameScene(int width, int height) :scene_width(width), scene_height(height)
{
	pAI = nullptr;
	pRuler = nullptr;

	pCheckerBoard = new CheckerBoard(Rect(0, 0, 900, 900));
	pMenuBoard = new MenuBoard(Rect(900, 0, 1200, 900));
	pStartBlack = new Button(Rect(950, 425, 1150, 475), Text("开始游戏：黑棋", "楷体", 22), Point(5, 30));
	pStartWhite = new Button(Rect(950, 500, 1150, 550), Text("开始游戏：白棋", "楷体", 22), Point(5, 30));
	pSave = new Button(Rect(950, 575, 1150, 625), Text("保存游戏", "楷体", 30), Point(25, 35));
	pRead = new Button(Rect(950, 650, 1150, 700), Text("读取游戏", "楷体", 30), Point(25, 35));

	game_state = MAIN_MENU;
	pMenuBoard->SetGameState(game_state);
	col_human = Color::BLACK;
	col_ai = Color::WHITE;

	msg_send = false;
}

GameScene::~GameScene()
{
	if (this->pAI != nullptr)
		delete this->pAI;
	if (this->pRuler != nullptr)
		delete this->pRuler;

	delete this->pCheckerBoard;
	delete this->pMenuBoard;
	delete this->pStartBlack;
	delete this->pStartWhite;
	delete this->pSave;
	delete this->pRead;
}

void GameScene::Init()
{
	//pCheckerBoard->Init();
	pMenuBoard->Init();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	pCheckerBoard->Draw();
	pMenuBoard->Draw();
	pStartBlack->Draw();
	pStartWhite->Draw();
	pSave->Draw();
	pRead->Draw();

	glutSwapBuffers();
}

void GameScene::Idle()
{
	if (game_state == IN_GAME)
	{
		if (pRuler->moveColor() == col_ai)
		{
			Point a;
			if (pAI->GetMove(a) == true)
			{
				pRuler->setPiece(a.x, a.y, col_ai);
				pCheckerBoard->setPiece(a.x, a.y, col_ai);
				int tmp = pRuler->isOver();
				if (tmp)
				{
					game_state = tmp;
					pMenuBoard->SetGameState(game_state);
				}
				msg_send = false;
			}
		}
	}
	Display();
}

void GameScene::OnMouseMove(int x, int y)
{
	if (game_state == IN_GAME)
	{
		if (pCheckerBoard->in(Point(x, y)))
			pCheckerBoard->SetMousePos(Point(x, y));
		else
			pCheckerBoard->SetMousePos(Point(-1, -1));
		pSave->OnMouse(Point(x, y));
		pRead->OnMouse(Point(x, y));
	}
	else
	{
		pCheckerBoard->SetMousePos(Point(-1, -1));
	}
	if (game_state != PAUSE)
	{
		pStartBlack->OnMouse(Point(x, y));
		pStartWhite->OnMouse(Point(x, y));
	}
}

void GameScene::OnMouseClick(int button, int state, int x, int y)
{
	std::cerr << "Mouse click at pos (" << x << "," << y << ")" << std::endl;
	
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			if (game_state == IN_GAME)
			{
				if (pCheckerBoard->in(Point(x, y)))
					pCheckerBoard->OnMouseClick(Point(x, y));

				int tmp = pRuler->isOver();
				if (tmp)
				{
					game_state = tmp;
					pMenuBoard->SetGameState(game_state);
				}
				else if (!msg_send && pRuler->moveColor() == col_ai)
				{
					pAI->SendGameMessage();
					msg_send = true;
				}

				if (pSave->OnClick(Point(x, y), state))
				{
					wchar_t path[MAX_PATH];
					GetCurrentDirectory(MAX_PATH, path);
					int tmp = game_state;
					game_state = PAUSE;
					std::string str = GetSaveFile(TEXT("存档文件(*.save)\0*.save\0所有文件(*.*)\0*.*"), TEXT("打开存档"), path);
					game_state = tmp;
				}
				if (pRead->OnClick(Point(x, y), state))
				{
					wchar_t path[MAX_PATH];
					GetCurrentDirectory(MAX_PATH, path);
					int tmp = game_state;
					game_state = PAUSE;
					std::string str = GetOpenFile(TEXT("存档文件(*.save)\0*.save\0所有文件(*.*)\0*.*"), TEXT("打开存档"), path);
					game_state = tmp;
				}
			}
			if (game_state != PAUSE)
			{
				if (pStartBlack->OnClick(Point(x, y), state))
					StartGame(Color::BLACK);
				if (pStartWhite->OnClick(Point(x, y), state))
					StartGame(Color::WHITE);
			}
		}
		else if (state == GLUT_DOWN)
		{
			if (game_state != PAUSE)
			{
				pStartBlack->OnClick(Point(x, y), state);
				pStartWhite->OnClick(Point(x, y), state);
				pSave->OnClick(Point(x, y), state);
				pRead->OnClick(Point(x, y), state);
			}
		}
	}
}
