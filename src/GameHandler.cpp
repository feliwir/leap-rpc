#include "GameHandler.hpp"
#include "Utility.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace Leap;
Handler::State Handler::cState = Handler::WAITING;
double Handler::acc = 0.0f;
int Handler::missingFrames = 0;

void Handler::Update(const Frame& frame)
{

	auto hand = frame.hands().frontmost();
	
	if(!hand.isValid())
	{
		++missingFrames;
		if(missingFrames>1000)
			cState = WAITING;
		return;
	}

	missingFrames = 0;

	switch(cState)
	{
		case WAITING:
		case PREPARE2:
		case PREPARE4:
		{
			auto movement = hand.palmVelocity().dot(Vector::down());
			std::cout << movement << std:: endl;
			if(movement>100)
			{
				acc += movement;
			}
			else if(abs(movement)<100 && acc >= 10000)
			{
				std::cout << "NEXT STATE" << std::endl;
				cState = (State)((int)cState + 1);
				acc = 0;
			}
			else
			{
				acc = 0;
			}
		}
		break;
		case PREPARE1:
		case PREPARE3:
		{
			auto movement = hand.palmVelocity().dot(Vector::up());
			std::cout << movement << std:: endl;
			if(movement>100)
			{
				acc += movement;
			}
			else if(abs(movement)<100 && acc >= 20000)
			{
				std::cout << "NEXT STATE" << std::endl;
				cState = (State)((int)cState + 1);
				acc = 0;
			}
			else

			{
				acc = 0;
			}
		}
		break;
		case EVALUATE:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			
			if(isScissor(hand))
				cState = SCISSOR;
			else if(isRock(hand))
				cState = ROCK;
			else if(isPaper(hand))
				cState = PAPER;
			else
				cState = UNKNOWN;
		}
		break;
		default:
		std::cout << "Idle " << std::endl;
		break;
	}
}

bool Handler::isRock(const Leap::Hand& hand)
{
	auto fingers = hand.fingers();
	auto center = hand.palmPosition();

	for(const auto& finger : fingers)
	{
		
		auto tip = finger.tipPosition();
		if(tip.distanceTo(center)>60.0f)
			return false;
	}

	return true;
}


bool Handler::isPaper(const Leap::Hand& hand)
{
    auto fingers = hand.fingers();
	auto center = hand.palmPosition();

    for(const auto& finger : fingers)
    {        
       	auto tip = finger.tipPosition();

       	if(finger.type()==Finger::TYPE_PINKY || finger.type()==Finger::TYPE_THUMB)
       	{
       		if(tip.distanceTo(center)<55.0f)
			{
				std::cout << finger.type()<< " - " <<tip.distanceTo(center) << std::endl;
				return false;
			}
       	}
       	else
       	{
			if(tip.distanceTo(center)<65.0f)
			{
				std::cout << finger.type()<< " - " <<tip.distanceTo(center) << std::endl;
				return false;
			}
		}
    }

    return true;
}

bool Handler::isScissor(const Leap::Hand& hand)
{
    auto fingers = hand.fingers();
    auto center = hand.palmPosition();
    Leap::Finger index, middle;

    for(const auto& finger : fingers)
    {
        auto type = finger.type();
        auto tip = finger.tipPosition();

        if(type==Leap::Finger::TYPE_INDEX)
        {
            index = finger;
            if(tip.distanceTo(center)<50.0f)
            {
                std::cout << tip.distanceTo(center) << std::endl;
                return false;
            }
        }
        else if(type==Leap::Finger::TYPE_MIDDLE)
        {
            middle = finger;
            if(tip.distanceTo(center)<80.0f)
            {
                std::cout << tip.distanceTo(center) << std::endl;
                return false;
            }
        }
        else
        {
            if(tip.distanceTo(center)>80.0f)
                return false;
        }
    }

    if(Util::ToDeg(middle.direction().angleTo(index.direction()))<5.0f)
    {    
        std::cout << Util::ToDeg(middle.direction().angleTo(index.direction())) << std::endl;
        return false;
    }

    return true;
}