#pragma once
#include <Leap.h>
#include <chrono>
#include "Player.hpp"

class Handler
{
public:
	enum State
	{
		MENU1 = 0,
		MENU2,
		MENU3,
		WAITING,
		PREPARE1,
		PREPARE2,
		PREPARE3,
		PREPARE4,
		EVALUATE,
		EVALUATE2,
		ROCK,
		PAPER,
		SCISSOR,
		UNKNOWN
	};


	static State cState;

	static void Update(const Leap::Frame& frame);

private:
	static double acc;
	static int missingFrames;

 	static bool isRock(const Leap::Hand& hand);
	static bool isPaper(const Leap::Hand& hand);
	static bool isScissor(const Leap::Hand& hand);
	static std::chrono::time_point<std::chrono::high_resolution_clock> start;
};