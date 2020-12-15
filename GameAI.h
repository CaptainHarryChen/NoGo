#pragma once
#include <thread>
#include <mutex>
#include <ctime>
#include <queue>
#include "Graphic.h"
#include "GameRule.h"
#include "Node.h"

#include "safe_queue.h"

#define MAX_SEARCH_STEP 3

class GameAI
{
	std::thread* pMain;
	std::mutex mv_lock;

	enum class Message { END, MOVE, CALC };

	threadsafe_queue <Message> qmsg;
	Point player_move, ai_move;
	Color color;
	clock_t start_time;
	bool need_move, ready_move;

	Node* root;

	void Search(Node* u, int step);
	void Run();
	
public:
	GameAI(Color col);

	void SetBeginningState();
	void SetBeginningState(const Color A[9][9]);
	void Start();
	void End();
	void SendMoveMessage();
	bool GetMove(Point &res);
	void PlayerMove(Point p);
};

