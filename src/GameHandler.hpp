#pragma once
#include <Leap.h>
#include <chrono>
#include <random>
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
		PRESENT,
		UNKNOWN
	};

	enum Result
	{
		NONE = 0,
		ROCK ,
		PAPER,
		SCISSOR
	};

	enum AIBehaviour
	{
		RNG = 0,
		HARD,
	};


	static AIBehaviour aiBehaviour;
	static State cState;

	static void Update(const Leap::Frame& frame);
	static Result playerResult;
	static Result botResult;

    static int playerPts;
    static int botPts;
private:
	static Result GetBotResult();
	static void GivePoint();

	static double acc;
	static int missingFrames;
	static std::random_device rd;
	static std::mt19937 rne;
    static std::uniform_int_distribution<int> uniform_dist;

 	static bool isRock(const Leap::Hand& hand);
	static bool isPaper(const Leap::Hand& hand);
	static bool isScissor(const Leap::Hand& hand);
	static std::chrono::time_point<std::chrono::high_resolution_clock> start;
};