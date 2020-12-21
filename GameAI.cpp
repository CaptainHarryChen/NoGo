#include "GameAI.h"
#include <thread>

GameAI::GameAI(Color color)
{
	this->color = color;

}

GameAI::~GameAI(){}

void GameAI::WaitNextMove(Point& res)
{
	while (!GetMove(res))
		std::this_thread::yield();
}
