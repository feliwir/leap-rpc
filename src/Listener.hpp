#pragma once
#include <Leap.h>


class LeapListener : public Leap::Listener 
{
private:
	enum State
	{
		PREPARE = 0,
		RESULT = 1,
	};
public:
    virtual void onConnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);


 	bool isFist(const Leap::Hand& hand);
	bool isHand(const Leap::Hand& hand);
private:
	Leap::Frame m_last;
	State m_cState;
};

