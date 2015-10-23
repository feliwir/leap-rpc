#pragma once

//used in our gamehandler
class Player
{
public:
	enum Gesture
	{
		NONE = 0,
		ROCK,
		PAPER,
		SCISSOR
	};

	inline void SetGesture(Gesture g)
	{
		m_cur = g;
	}

	inline Gesture GetGesture()
	{
		return m_cur;
	}

private:
	Gesture m_cur;

};