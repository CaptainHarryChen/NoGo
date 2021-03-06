#include "CheckerBoard.h"
#include <cstring>

CheckerBoard::CheckerBoard(Rect _pos)
{
	pos = _pos;
	memset(img_board, 0, sizeof img_board);
	img_black = img_white = 0;
	pRuler = nullptr;
	pAI = nullptr;
	col_human = Color::BLACK;
	col_ai = Color::WHITE;
	memset(board, 0, sizeof board);
	mouse_pos = Point(-1, -1);

	img_board[0] = LoadTexture("img//checkerboard//board0.bmp");
	img_board[1] = LoadTexture("img//checkerboard//board1.bmp");
	img_board[2] = LoadTexture("img//checkerboard//board2.bmp");
	img_board[3] = LoadTexture("img//checkerboard//board3.bmp");
	img_board[4] = LoadTexture("img//checkerboard//board4.bmp");
	img_board[5] = LoadTexture("img//checkerboard//board5.bmp");
	img_board[6] = LoadTexture("img//checkerboard//board6.bmp");
	img_board[7] = LoadTexture("img//checkerboard//board7.bmp");
	img_board[8] = LoadTexture("img//checkerboard//board8.bmp");
	img_board[9] = LoadTexture("img//checkerboard//board9.bmp");
	img_black = LoadTexture("img//piece//blackpiece.bmp");
	img_white = LoadTexture("img//piece//whitepiece.bmp");
}

void CheckerBoard::Init(Color c_human, GameRule* p, GameAI* pai)
{
	col_human = c_human;
	col_ai = c_human == Color::BLACK ? Color::WHITE : Color::BLACK;
	pRuler = p;
	pAI = pai;
	memset(board, 0, sizeof board);
}

void CheckerBoard::SetBoard(Color tboard[9][9])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			board[i][j] = tboard[i][j];
}

void CheckerBoard::Draw()
{
	int w = (pos.rx - pos.lx) / 9, h = (pos.ry - pos.ly) / 9;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			Rect dr = Rect(j * w, i * h, (j + 1) * w, (i + 1) * h);
			int id = 0;
			if (i == 0 && j == 0)
				id = 1;
			else if (i == 0 && j == 8)
				id = 2;
			else if (i == 8 && j == 8)
				id = 3;
			else if (i == 8 && j == 0)
				id = 4;
			else if (i == 8)
				id = 5;
			else if (j == 8)
				id = 6;
			else if (i == 0)
				id = 7;
			else if (j == 0)
				id = 8;
			else
				id = 9;
			DrawTexture(dr, img_board[id]);
		}
	for(int i=0;i<9;i++)
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == Color::BLACK)
				DrawCircleTexture(Point(j * w + w / 2, i * h + h / 2), (int)(0.46 * w), 0.46, img_black);
			else if (board[i][j] == Color::WHITE)
				DrawCircleTexture(Point(j * w + w / 2, i * h + h / 2), (int)(0.46 * w), 0.46, img_white);
		}
	if (mouse_legal)
		DrawTransCircle(Point(mouse_pos.y * w + w / 2, mouse_pos.x * h + h / 2), (int)(0.46 * w), 0.0, 1.0, 0.0, 0.5);
	else
		DrawTransCircle(Point(mouse_pos.y * w + w / 2, mouse_pos.x * h + h / 2), (int)(0.46 * w), 1.0, 0.0, 0.0, 0.5);
}

void CheckerBoard::SetMousePos(Point u)
{
	if (in(u))
	{
		int w = (pos.rx - pos.lx) / 9, h = (pos.ry - pos.ly) / 9;
		int y = u.x / w, x = u.y / h;
		mouse_pos = Point(x, y);
		mouse_legal = pRuler->isLegal(x, y, col_human) && pRuler->moveColor() == col_human;
	}
	else
		mouse_pos = Point(-1, -1);
}

void CheckerBoard::OnMouseClick(Point a)
{
	int w = (pos.rx - pos.lx) / 9, h = (pos.ry - pos.ly) / 9;
	int y = a.x / w, x = a.y / h;
	if (pRuler->isLegal(x, y, col_human) && pRuler->moveColor() == col_human)
	{
		pRuler->setPiece(x, y, col_human);
		pAI->PlayerMove(Point(x,y));
		board[x][y] = col_human;
	}
}

bool CheckerBoard::in(Point a)
{
	return pos.inRect(a);
}

void CheckerBoard::setPiece(int x, int y, Color col)
{
	board[x][y] = col;
}
