#pragma once
#include <Leap.h>


class LeapListener : public Leap::Listener 
{
    public:
    virtual void onConnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
};

