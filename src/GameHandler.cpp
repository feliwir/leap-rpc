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
			auto movement = hand.palmVelocity().dot(Vector::down());;
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
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			
			auto fingers = hand.fingers();
			for(const auto& finger : fingers)
			{
				std::cout<< finger.type() << " - " << hand.wristPosition().distanceTo(finger.stabilizedTipPosition()) << std::endl;
				std::cout<< hand.wristPosition() << " - " << finger.stabilizedTipPosition() << std::endl;
			}

			if(isScissor(hand))
				cState = SCISSOR;
			else if(isPaper(hand))
				cState = PAPER;
			else if(isRock(hand))
				cState = ROCK;
			else
				cState = UNKNOWN;
		}
		break;
		default:
			break;
	}
}

bool Handler::isRock(const Leap::Hand& hand)
{
	std::cout << "TEST ROCK" << std::endl;
	auto fingers = hand.fingers();
	auto wristPos = hand.wristPosition();

	for(const auto& finger : fingers)
	{		
		auto tipPos = finger.stabilizedTipPosition();
		auto distance = tipPos.distanceTo(wristPos);
		
		if(distance>80.0f && finger.type() != 0)
		{
       		std::cout<< finger.type() << " - " << distance << std::endl;
       		return false;
       	}
	}

	return true;
}


bool Handler::isPaper(const Leap::Hand& hand)
{
	std::cout << "TEST PAPER" << std::endl;
    auto fingers = hand.fingers();
	auto wristPos = hand.wristPosition();

    for(const auto& finger : fingers)
    {        
       	auto tipPos = finger.stabilizedTipPosition();
       	auto distance = tipPos.distanceTo(wristPos);
       	
       	if(distance<100.0f && finger.type() != 0)
       	{
       		std::cout<< finger.type() << " - " << distance << std::endl;
       		return false;
       	}

    }

    return true;
}

bool Handler::isScissor(const Leap::Hand& hand)
{
	std::cout << "TEST SCISSOR" << std::endl;
    auto fingers = hand.fingers();
    auto wristPos = hand.wristPosition();
    Leap::Finger index, middle;

    for(const auto& finger : fingers)
    {
        auto type = finger.type();
        auto tipPos = finger.stabilizedTipPosition();
        auto distance = tipPos.distanceTo(wristPos);

        if(type==Leap::Finger::TYPE_INDEX || type==Leap::Finger::TYPE_MIDDLE)
        {
            if(distance<80.0f)
            {
                std::cout<< finger.type() << " - " << distance << std::endl;
                return false;
            }
        }
        else
        {
            if(distance>80.0f && finger.type() != 0)
            {
            	std::cout<< finger.type() << " - " << distance << std::endl;
                return false;
            }
        }
    }

    return true;
}