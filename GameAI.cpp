#include "GameAI.h"
#include <cstdlib>
#include <ctime>

GameAI::GameAI(Color col)
{
	color = col;
	start_time = 0;
	pMain = NULL;
	need_move = false;
	ready_move = false;
	root = NULL;
}

void GameAI::Search(Node* u, int step = 0)
{
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
			if (tmp[i][j] != SPACE)
				u->setPiece(i, j, tmp[i][j]);
	Node v = *u;
	v.step++;
	Color mv = u->moveColor();
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if (u->isLegal(i, j, mv))
			{
				v.A[i][j] = mv;
				Search(&v, step + 1);
				if ((flag && v.value > u->value) || (!flag && v.value < u->value))
					u->value = v.value, u->bestop = Point(i, j);
				v.A[i][j] = SPACE;
			}
}

void GameAI::Run()
{
	for (;;)
	{
		//Process Message
		for (;;)
		{
			msg_lock.lock();
			if (qmsg.empty())
			{
				msg_lock.unlock();
				break;
			}
			Message message = qmsg.front();
			qmsg.pop();
			msg_lock.unlock();
			if (message == END)
				break;
			else if (message == MOVE)
			{
				root->setPiece(player_move.x, player_move.y, root->moveColor());
			}
			else if (message == CALC)
			{
				start_time = clock();
				mv_lock.lock();
				need_move = true;
				ready_move = false;
				mv_lock.unlock();
			}
		}
		mv_lock.lock();
		//�����ƶ�
		if (need_move == true)
		{
			mv_lock.unlock();
			Search(root);
			ai_move = root->bestop;
			root->setPiece(ai_move.x, ai_move.y, root->moveColor());
			mv_lock.lock();
			need_move = false;
			ready_move = true;
			mv_lock.unlock();
		}
		else
			mv_lock.unlock();
	}
}

void GameAI::SetBeginningState()
{
	root = new Node();
}
void GameAI::SetBeginningState(const Color A[9][9])
{
	root = new Node();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] != SPACE)
				root->setPiece(i, j, A[i][j]);
}

void GameAI::Start()
{
	srand((unsigned int)time(0));
	pMain = new std::thread(&GameAI::Run, this);
}

void GameAI::End()
{
	msg_lock.lock();
	qmsg.push(END);
	msg_lock.unlock();
	pMain->join();
}

void GameAI::SendMoveMessage()
{
	msg_lock.lock();
	qmsg.push(CALC);
	msg_lock.unlock();
}

bool GameAI::GetMove(Point& res)
{
	mv_lock.lock();
	if (!ready_move)
	{
		mv_lock.unlock();
		return false;
	}
	res = ai_move;
	ready_move = false;
	mv_lock.unlock();
	return true;
}

void GameAI::PlayerMove(Point p)
{
	msg_lock.lock();
	qmsg.push(MOVE);
	player_move = p;
	msg_lock.unlock();
}

