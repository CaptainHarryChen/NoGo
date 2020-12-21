#include "GameAI.h"
#include <thread>

GameAI::GameAI(Color color)
{
	this->ai_color = color;

}

GameAI::~GameAI(){}

void GameAI::WaitNextMove(Point& res)
{
	while (!GetMove(res))
		std::this_thread::yield();
}
