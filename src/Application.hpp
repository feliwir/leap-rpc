#pragma once
#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "Listener.hpp"
#include "GameHandler.hpp"

class Application
{
public:
	bool Initialize();
	void Run();
	void Release();
	void Present();
private:
	sf::RenderWindow m_window;
	LeapListener m_listener;
	Leap::Controller m_controller;
	sf::Font m_font;
	int16_t m_winWidth;
	int16_t m_winHeight;
};