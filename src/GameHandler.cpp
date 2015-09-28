#include "GameHandler.hpp"
#include "Utility.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace Leap;
Handler::State Handler::cState = Handler::MENU1;
double Handler::acc = 0.0f;
int Handler::missingFrames = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Handler::start;
std::random_device Handler::rd;
std::mt19937 Handler::rne(rd());
std::uniform_int_distribution<int> Handler::uniform_dist(1,3);
int Handler::playerPts = 0;
int Handler::botPts = 0;
Handler::Result Handler::playerResult = NONE;
Handler::Result Handler::botResult = NONE;
Handler::AIBehaviour Handler::aiBehaviour = RNG;

void Handler::Update(const Frame& frame)
{

	auto hand = frame.hands().frontmost();
	
	switch(cState)
	{
		case MENU1:
		break;
		case MENU2:
		break;
		case MENU3:
		break;
		case WAITING:
		case PREPARE2:
		case PREPARE4:
		{
			auto movement = hand.palmVelocity().dot(Vector::down());;
			if(movement>MIN_MOVEMENT)
			{
				acc += movement;
			}
			else if(abs(movement)<MIN_MOVEMENT && acc >= TURN_DISTANCE)
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
			if(movement>MIN_MOVEMENT)
			{
				acc += movement;
			}
			else if(abs(movement)<MIN_MOVEMENT && acc >= TURN_DISTANCE)
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
			start = std::chrono::high_resolution_clock::now();
			cState = EVALUATE2;
		}
		break;
		case EVALUATE2:
		{
			auto end = std::chrono::high_resolution_clock::now();
			if(!((end-start)>std::chrono::milliseconds(500)))
			{
				std::cout << "WATING" << std::endl;
				break;
			}

			std::cout << hand.confidence() << std::endl;

			if(hand.confidence()<0.45f)
			{
				std::cout << "UNCONFIDENT" << std::endl;
				break;
			}

			auto fingers = hand.fingers();
			for(const auto& finger : fingers)
			{
				std::cout<< finger.type() << " - " << hand.wristPosition().distanceTo(finger.stabilizedTipPosition()) << std::endl;
				std::cout<< hand.wristPosition() << " - " << finger.stabilizedTipPosition() << std::endl;
			}

			cState = PRESENT;

			if(isScissor(hand))
				playerResult = SCISSOR;
			else if(isPaper(hand))
				playerResult = PAPER;
			else if(isRock(hand))
				playerResult = ROCK;
			else
			{
				cState = UNKNOWN;
				break;
			}

			botResult = GetBotResult();
			std::cout << "Player did choose " << playerResult << std::endl;
			std::cout << "Bot did choose " << botResult << std::endl;
			GivePoint();

			std::cout << "Player Points: " << playerPts << std::endl;
			std::cout << "Bot Points: " << botPts << std::endl;
		}
		break;
		default:
			break;
	}

	if(!hand.isValid() && (cState != MENU1 && cState != MENU2 && cState != MENU3))
	{
		++missingFrames;
		if(missingFrames>TOLERATED_MISSING)
		{
			std::cout << "GOTO MENU" << std::endl;
			playerPts = 0;
			botPts = 0;
			cState = MENU1;
		}
		return;
	}

	missingFrames = 0;
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
        else if(type==Leap::Finger::TYPE_PINKY)
        {
            if(distance>95.0f)
            {
            	std::cout<< finger.type() << " - " << distance << std::endl;
                return false;
            }
        }
        else if(type==Leap::Finger::TYPE_RING)
        {
        	if(distance>110.0f)
            {
            	std::cout<< finger.type() << " - " << distance << std::endl;
                return false;
            }
        }
    }

    return true;
}

Handler::Result Handler::GetBotResult()
{
	if(aiBehaviour==RNG)
	{
		return (Result)(uniform_dist(rne));
	}
	else
	{
		return (playerResult==SCISSOR)	? ROCK : (playerResult==ROCK) 
										? PAPER: SCISSOR;
	}
}

void Handler::GivePoint()
{
	if ((playerResult==SCISSOR && botResult==PAPER)||
	   	(playerResult==PAPER && botResult==ROCK)||
		(playerResult==ROCK && botResult==SCISSOR))
	{
		++playerPts;
		std::cout << "Give player a point" << std::endl;	
	}
	else if ((botResult==SCISSOR && playerResult==PAPER)||
	   	(botResult==PAPER && playerResult==ROCK)||
		(botResult==ROCK && playerResult==SCISSOR))
	{
		++botPts;
		std::cout << "Give bot a point" << std::endl;	
	}		
}