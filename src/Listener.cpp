#include "Listener.hpp"
using namespace Leap;

void LeapListener::onConnect(const Controller& controller) 
{
    std::cout << "Connected" << std::endl;
    m_cState = PREPARE;
}

void LeapListener::onFrame(const Controller& controller) 
{
    std::cout << "Frame available" << std::endl;
    auto frame = controller.frame();
    if(!m_last.isValid())
    {
    	std::cout <<"First frame!" << std::endl;
    	m_last = frame;
    	return;
    }

    if(m_cState==PREPARE)
    {
    	auto hands = frame.hands();
    	if(hands.isEmpty())
    	{
    		std::cout << "No hand detected" << std::endl;
    		return;
    	}

    	for(auto& hand : hands)
    	{
			if(isFist(hand))
			{
				std::cout << "Hand forms fist!" << std::endl;
			}
            else if(isHand(hand))
            {
                std::cout << "Hand forms spread palm!" << std::endl;
            }
    	}
    	
    }

    m_last = frame;
}


bool LeapListener::isFist(const Leap::Hand& hand)
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



bool LeapListener::isHand(const Leap::Hand& hand)
{
    auto fingers = hand.fingers();
    auto direction = hand.direction();

    for(const auto& finger : fingers)
    {        
        auto fingerDir = finger.direction();
        
        //THRESHOLD is a random float value (0.4f)
        if(direction.angleTo(fingerDir)>0.4f)
        {    
            std::cout << direction.angleTo(fingerDir) << std::endl;
            return false;
        }
    }

    return true;
}