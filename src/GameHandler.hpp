#pragma once
#include "Player.hpp"

class Handler
{
public:
	enum State
	{
		WAITING = 0,
		PREPARE1,
		PREPARE2,
		PREPARE3
	};

	static State cState;

	static void Update();
};