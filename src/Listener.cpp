#include "Listener.hpp"
#include "Utility.hpp"
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
			if(isRock(hand))
			{
				std::cout << "Hand forms rock!" << std::endl;
			}
            else if(isPaper(hand))
            {
                std::cout << "Hand forms paper!" << std::endl;
            }
            else if(isScissor(hand))
            {
                std::cout << "Hand forms scissor!" << std::endl;
            }
    	}
    	
    }

    m_last = frame;
}


bool LeapListener::isRock(const Leap::Hand& hand)
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


bool LeapListener::isPaper(const Leap::Hand& hand)
{
    auto fingers = hand.fingers();
    auto direction = hand.direction();

    for(const auto& finger : fingers)
    {        
        auto fingerDir = finger.direction();
        
        //THRESHOLD is a random float value (0.4f)
        if(Util::ToDeg(direction.angleTo(fingerDir))>20.0f)
        {    
            std::cout << Util::ToDeg(direction.angleTo(fingerDir)) << std::endl;
            return false;
        }
    }

    return true;
}

bool LeapListener::isScissor(const Leap::Hand& hand)
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