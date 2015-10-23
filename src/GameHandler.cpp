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

//This function is called by the Leap SDK
//One frame contains all data that was collected
void Handler::Update(const Frame& frame)
{
	auto hand = frame.hands().frontmost();
	
	//State machine
	switch(cState)
	{
		//The menu is controlled by the Application class
		case MENU1:
		break;
		case MENU2:
		break;
		case MENU3:
		break;
		//The up & down movement of our hand during the game
		case WAITING:
		case PREPARE2:
		case PREPARE4:
		{
			//check if the hand is moving down
			auto movement = hand.palmVelocity().dot(Vector::down());;
			if(movement>MIN_MOVEMENT)
			{
				acc += movement;
			}
			//when the hand is stoping to move go to the next state
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
			//check if the hand is moving up
			auto movement = hand.palmVelocity().dot(Vector::up());
			if(movement>MIN_MOVEMENT)
			{
				acc += movement;
			}
			//when the hand is stoping to move go to the next state
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
		//start our evaluation timer and go to the next state
		case EVALUATE:
		{
			start = std::chrono::high_resolution_clock::now();
			cState = EVALUATE2;
		}
		break;
		case EVALUATE2:
		{
			//wait until the timer has stopped, so the user has time to
			//do his gesture
			auto end = std::chrono::high_resolution_clock::now();
			if(!((end-start)>std::chrono::milliseconds(500)))
			{
				std::cout << "WATING" << std::endl;
				break;
			}

			//if the sdk isn't confident about this frane, skip this one
			//and try the next one
			std::cout << hand.confidence() << std::endl;
			if(hand.confidence()<0.45f)
			{
				std::cout << "UNCONFIDENT" << std::endl;
				break;
			}

			//get all our fingers and print them out for debugging
			//purposes
			auto fingers = hand.fingers();
			for(const auto& finger : fingers)
			{
				std::cout<< finger.type() << " - " << hand.wristPosition().distanceTo(finger.stabilizedTipPosition()) << std::endl;
				std::cout<< hand.wristPosition() << " - " << finger.stabilizedTipPosition() << std::endl;
			}

			cState = PRESENT;

			//check what motion this is
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

			//get the choice of the computer controlled enemy
			botResult = GetBotResult();
			std::cout << "Player did choose " << playerResult << std::endl;
			std::cout << "Bot did choose " << botResult << std::endl;
			//Decide who won this round
			GivePoint();

			std::cout << "Player Points: " << playerPts << std::endl;
			std::cout << "Bot Points: " << botPts << std::endl;
		}
		break;
		default:
			break;
	}

	//when not in menu we check if the hand is missing. In case it is we go back to 
	//the main menu
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

//check if all fingers are close to the palm
//ignore the thumb
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

//check if all fingers are far away from the palm
//ignore the thumb
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

//check if index and middle finger are far from the palm
//ring finger and pinky should be close to the pakm
//ignore the thumb
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

//check if we are playing on random or hardcore
Handler::Result Handler::GetBotResult()
{
	//return a random result
	if(aiBehaviour==RNG)
	{
		return (Result)(uniform_dist(rne));
	}
	//return the correct result to win
	else
	{
		return (playerResult==SCISSOR)	? ROCK : (playerResult==ROCK) 
										? PAPER: SCISSOR;
	}
}

//compare player choice and bot choice
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