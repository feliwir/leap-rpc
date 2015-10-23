#include "Listener.hpp"
#include "Utility.hpp"
#include "GameHandler.hpp"
using namespace Leap;

//called during initialization
void LeapListener::onConnect(const Controller& controller) 
{
    std::cout << "Connected" << std::endl;
    m_cState = PREPARE;
}

//call our gamehandler with the current frame
void LeapListener::onFrame(const Controller& controller) 
{
    auto frame = controller.frame();
    if(!m_last.isValid())
    {
    	std::cout <<"First frame!" << std::endl;
    	m_last = frame;
    	return;
    }

    Handler::Update(frame);

    m_last = frame;
}


