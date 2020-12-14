#pragma once
#include <thread>
#include <mutex>
#include <ctime>
#include <queue>
#include "Graphic.h"
#include "GameRule.h"
#include "Node.h"

#define MAX_SEARCH_STEP 5

class GameAI
{
	std::thread* pMain;
	std::mutex msg_lock,mv_lock;

	enum Message { END, MOVE, CALC };

	std::queue<Message> qmsg;
	Point player_move, ai_move;
	Color color;
	clock_t start_time;
	bool need_move, ready_move;
	int search_step;

	Node* root;

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

