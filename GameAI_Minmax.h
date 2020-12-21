#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include <ctime>
#include <queue>
#include "Graphic.h"
#include "GameRule.h"
#include "MinMaxNode.h"
#include "GameAI.h"
#include "safe_queue.h"

#define MAX_SEARCH_STEP 4

class GameAI_Minmax : public GameAI
{
	std::thread* pMain;
	std::mutex mv_lock;

	threadsafe_queue <Message> qmsg;
	Point player_move, ai_move;
	//Color color;
	clock_t start_time;
	std::atomic_bool need_move, ready_move;

	MinMaxNode * root;

	Point Search(MinMaxNode* u, int step = 0, double alpha = -1e100, double beta = 1e100);
	void Run();
	
public:
	GameAI_Minmax(Color col);
	~GameAI_Minmax();

	void SetBeginningState();
	void SetBeginningState(const Color A[9][9]);
	void Start();
	void End();
	void SendGameMessage(Message = Message::CALC);
	bool GetMove(Point &res);
	void PlayerMove(Point p);
};

