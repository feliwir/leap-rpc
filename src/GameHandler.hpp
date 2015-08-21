#pragma once
#include <Leap.h>

#include "Player.hpp"

class Handler
{
public:
	enum State
	{
		WAITING = 0,
		PREPARE1,
		PREPARE2,
		PREPARE3,
		PREPARE4,
		EVALUATE,
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
};