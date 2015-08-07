#include "Application.hpp"
#include <iostream>

bool Application::Initialize()
{
	m_winWidth = 800;
	m_winHeight = 600;
	m_window.create(sf::VideoMode(m_winWidth,m_winHeight),"LEAP");

	if(!m_window.isOpen())
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}

	if (!m_font.loadFromFile("data/Drifttype.ttf"))
	{
	    std::cout << "Failed to load game font!" << std::endl;
		return false;
	}

	if(!m_controller.isConnected())
	{
		std::cout << "Please make sure your LEAP is connected!" << std::endl;
		return false;   
	}

	if(!m_controller.addListener(m_listener))
	{
		std::cout << "Failed to add custom listener to controller!" << std::endl;
		return false;
	}
	return true;
}

void Application::Run()
{
	sf::Text text;
	text.setFont(m_font);
	text.setCharacterSize(50);
	text.setColor(sf::Color::Red);

	while (m_window.isOpen())
	{
	   // Event processing
	   sf::Event event;
	   while (m_window.pollEvent(event))
	   {
	       // Request for closing the window
	       if (event.type == sf::Event::Closed)
	           m_window.close();
	   }

	   // Clear the whole window before rendering a new frame
	   m_window.clear();

	   switch(Handler::cState)
	   {
	   		case Handler::WAITING:
	   			text.setString("Waiting!!");
	   			break;
	   		default:
	   			text.setString("Unknown State");
	   			break;
	   };

	   auto textBounds = text.getLocalBounds();
	   auto textPos = sf::Vector2f(m_winWidth/2-textBounds.width/2,
	   							   m_winHeight/2-textBounds.height/2);

	   text.setPosition(textPos);
	   m_window.draw(text);
	   // End the current frame and display its contents on screen
	   m_window.display();
	}
    Release();
}

void Application::Release()
{
	if(m_window.isOpen())
		m_window.close();

	m_controller.removeListener(m_listener);
}