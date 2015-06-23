#pragma once
#include <Leap.h>


class LeapListener : public Leap::Listener 
{
    public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
};

