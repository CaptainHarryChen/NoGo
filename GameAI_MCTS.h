#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include <ctime>
#include <queue>
#include "safe_queue.h"
#include "GameAI.h"
#include "Graphic.h"

const double Confidence = 2.0;
const int LeastVisitTime = 5;
const int RolloutStep = 10;

struct MCTS_Node;

struct cmp
{
	Point op;
	double ucb;
	
	cmp() { ucb = 0; }
	cmp(Point a, double b) :op(a), ucb(b) {}

	bool operator < (const cmp& t)const
	{
		return ucb < t.ucb;
	}
};

struct MCTS_Node
{
	double value;
	int n;
	std::priority_queue<cmp> que;
	MCTS_Node* son[9][9];

	MCTS_Node()
	{
		value = 0;
		n = 0;
		memset(son, 0, sizeof son);
	}

	~MCTS_Node()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (son[i][j] != nullptr)
					delete son[i][j];
	}

	double UCB()const
	{
		if (n == 0)
			return 1e100;
		return value / n + Confidence * sqrt(log(n) / n);
	}
};

class GameAI_MCTS :
	public GameAI
{
	std::thread* pMain;
	std::mutex mv_lock;

	threadsafe_queue <Message> qmsg;
	Point player_move, ai_move;
	clock_t start_time;
	std::atomic_bool need_move, ready_move;

	GameRule* board, * tboard;
	MCTS_Node* root;

	void Move(Point mv);
	double Rollout();
	double Search(MCTS_Node* cur);
	void Run();

public:
	GameAI_MCTS(Color col);
	~GameAI_MCTS();

	void SetBeginningState();
	void SetBeginningState(const Color A[9][9]);
	void Start();
	void End();
	void SendGameMessage(Message = Message::CALC);
	bool GetMove(Point& res);
	void PlayerMove(Point p);
};

