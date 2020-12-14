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
	search_step = 0;
}

void GameAI::Run()
{
	int cur_step = 0;
	Node* cur = root;
	Point tmp;
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
				cur = root->son[player_move.x][player_move.y];
				if (cur == NULL)
					cur = new Node(root, player_move);
				cur_step = 0;
				root->son[player_move.x][player_move.y] = NULL;
				root->Release();
				delete root;
				root = cur;
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
		//search
		if (cur_step < MAX_SEARCH_STEP)
		{
			cur->NextOp();
			if (cur->Finish())
			{
				if (cur != root)//以后修改
				{
					cur->last_op = Point(-1, 9);
					cur = cur->father;
					cur_step--;
				}
			}
			else
			{
				if (cur->son[cur->last_op.x][cur->last_op.y] == NULL)
					cur->son[cur->last_op.x][cur->last_op.y] = new Node(cur, cur->last_op);
				tmp = cur->last_op;
				cur->NextOp();
				cur = cur->son[tmp.x][tmp.y];
				cur->value = -1;
				cur_step++;
			}
		}
		else
		{
			cur->value = cur->Evaluate(color);
			std::cerr << "Finish search:" << cur->value << std::endl;
			cur->father->UpdateValue(cur->value, color == cur->moveColor());
			cur = cur->father;
			cur_step--;
		}
		
		//处理移动
		mv_lock.lock();
		if (need_move == true && cur->Finish()&&cur==root)//clock() - start_time >= 900 )
		{
			ai_move = root->FindMaxValue();
			cur = root->son[ai_move.x][ai_move.y];
			if (cur == NULL)
				cur = new Node(root, ai_move);
			cur_step = 0;
			root->son[ai_move.x][ai_move.y] = NULL;
			root->Release();
			delete root;
			root = cur;
			need_move = false;
			ready_move = true;
		}
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

