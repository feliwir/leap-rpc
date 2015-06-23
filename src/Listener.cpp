#include "Listener.hpp"
using namespace Leap;

void LeapListener::onConnect(const Controller& controller) 
{
    std::cout << "Connected" << std::endl;
}

void LeapListener::onFrame(const Controller& controller) 
{
    std::cout << "Frame available" << std::endl;
}

