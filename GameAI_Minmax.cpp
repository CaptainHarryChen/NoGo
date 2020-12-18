#include "GameAI_Minmax.h"
#include <cstdlib>
#include <ctime>

GameAI_Minmax::GameAI_Minmax(Color col) : GameAI(col)
{
	start_time = 0;
	pMain = nullptr;
	need_move = false;
	ready_move = false;
	root = nullptr;
}

GameAI_Minmax::~GameAI_Minmax()
{
	this->End();
	if (pMain != nullptr)
		delete pMain;
	if (root != nullptr)
		delete root;
}

void GameAI_Minmax::Search(Node* u, int step = 0)
{
	const int dd[8][2] = { {-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1} };
	if (step >= MAX_SEARCH_STEP)
	{
		u->value = u->Evaluate(color);
		//u->debug();
		//std::cerr << "Finish search:" << u->value << std::endl;
		return;
	}
	bool flag = u->moveColor() == color;
	if (flag)
		u->value = -1e100;
	else
		u->value = 1e100;
	memset(u->B, -1, sizeof u->B);
	static Color tmp[9][9];
	memcpy(tmp, u->A, sizeof tmp);
	memset(u->A, 0, sizeof u->A);
	u->dsu.Reset();
	u->step = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (tmp[i][j] != Color::SPACE)
				u->setPiece(i, j, tmp[i][j]);
	Node v = *u;
	v.step++;
	Color mv = u->moveColor();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			bool has_neighbor = false;
			for (int d = 0; d < 8; d++)
			{
				int x = i + dd[d][0], y = j + dd[d][1];
				if (x >= 0 && y >= 0 && x < 9 && y < 9 && u->A[x][y] != Color::SPACE)
					has_neighbor = true;
			}
			if (has_neighbor && u->isLegal(i, j, mv))
			{
				v.A[i][j] = mv;
				Search(&v, step + 1);
				if ((flag && v.value > u->value) || (!flag && v.value < u->value))
					u->value = v.value, u->bestop = Point(i, j);
				v.A[i][j] = Color::SPACE;
			}
		}
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (u->isLegal(i, j, mv))
			{
				v.A[i][j] = mv;
				Search(&v, step + 1);
				if ((flag && v.value > u->value) || (!flag && v.value < u->value))
					u->value = v.value, u->bestop = Point(i, j);
				v.A[i][j] = Color::SPACE;
				break;
			}
		}
}

void GameAI_Minmax::Run()
{
	bool bQuit = false;
	while (!bQuit)
	{
		//Process Message
		for (;;)
		{
			Message message;

			if (!this->qmsg.try_pop(message))
				break;

			if (message == Message::END)
			{
				bQuit = true;
				break;
			}
			else if (message == Message::MOVE)
			{
				root->setPiece(player_move.x, player_move.y, root->moveColor());
			}
			else if (message == Message::CALC)
			{
				start_time = clock();
				std::lock_guard<std::mutex> lg(this->mv_lock);
				need_move = true;
				ready_move = false;
			}
		}

		if (bQuit)
			break;

		//´¦ÀíÒÆ¶¯
		if (need_move)
		{
			Search(root);
			ai_move = root->bestop;
			root->setPiece(ai_move.x, ai_move.y, root->moveColor());

			std::lock_guard <std::mutex> lg(this->mv_lock);
			need_move = false;
			ready_move = true;
		}
		else
			std::this_thread::yield();
	}
}

void GameAI_Minmax::SetBeginningState()
{
	if (root != nullptr)
		delete root;
	root = new Node();
}
void GameAI_Minmax::SetBeginningState(const Color A[9][9])
{
	if (root != nullptr)
		delete root;
	root = new Node();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] != Color::SPACE)
				root->setPiece(i, j, A[i][j]);
}

void GameAI_Minmax::Start()
{
	srand((unsigned int)time(0));
	pMain = new std::thread(&GameAI_Minmax::Run, this);
}

void GameAI_Minmax::End()
{
	this->SendGameMessage(Message::END);
	pMain->join();
}

void GameAI_Minmax::SendGameMessage(Message msg)
{
	qmsg.push(msg);
}

bool GameAI_Minmax::GetMove(Point& res)
{
	if (!ready_move)
		return false;
	std::lock_guard<std::mutex> lg(this->mv_lock);
	res = ai_move;
	ready_move = false;
	return true;
}

void GameAI_Minmax::PlayerMove(Point p)
{
	//qmsg.push(Message::MOVE);
	this->SendGameMessage(Message::MOVE);
	player_move = p;
}

