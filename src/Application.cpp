#include "Application.hpp"
#include "GameHandler.hpp"
#include <iostream>

bool Application::Initialize()
{
	m_winWidth = 800;
	m_winHeight = 600;

	while(!m_controller.isConnected())
	{}

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

	if(!m_controller.addListener(m_listener))
	{
		std::cout << "Failed to add custom listener to controller!" << std::endl;
		return false;
	}
	return true;
}

void Application::Run()
{
	sf::Text text,lblStart,lblQuit,lblOptions;
	text.setFont(m_font);
	text.setCharacterSize(50);
	text.setColor(sf::Color::Red);

	lblStart.setFont(m_font);
	lblStart.setString("Start - Standard");
	lblStart.setPosition(150.0f,100.0f);
	lblStart.setCharacterSize(50);
	lblOptions.setFont(m_font);
	lblOptions.setString("Start - Hardcore");
	lblOptions.setPosition(150.0f,200.0f);
	lblOptions.setCharacterSize(50);
	lblQuit.setFont(m_font);
	lblQuit.setString("Quit");
	lblQuit.setPosition(150.0f,300.0f);
	lblQuit.setCharacterSize(50);

	while (m_window.isOpen())
	{
	   // Event processing
	   sf::Event event;
	   while (m_window.pollEvent(event))
	   {
	       // Request for closing the window
	       if (event.type == sf::Event::Closed)
	           m_window.close();

	       if (event.type == sf::Event::KeyPressed)
	       {
				if (event.key.code == sf::Keyboard::Return)
				{
					if(Handler::cState==Handler::MENU1)
						Handler::cState=Handler::WAITING;
					else if(Handler::cState==Handler::MENU2)
						Handler::cState=Handler::WAITING;
					else if(Handler::cState==Handler::MENU3)
						m_window.close();
				}
				else if(event.key.code == sf::Keyboard::Up)
				{
					if(Handler::cState==Handler::MENU2)
						Handler::cState=Handler::MENU1;
					else if(Handler::cState==Handler::MENU3)
						Handler::cState=Handler::MENU2;
				}
				else if(event.key.code == sf::Keyboard::Down)
				{
					if(Handler::cState==Handler::MENU1)
						Handler::cState=Handler::MENU2;
					else if(Handler::cState==Handler::MENU2)
						Handler::cState=Handler::MENU3;
				}
	       }
	   }

	   // Clear the whole window before rendering a new frame
	   m_window.clear();

	   switch(Handler::cState)
	   {
	   		case Handler::MENU1:
	   			lblStart.setColor(sf::Color::Red);
	   			lblOptions.setColor(sf::Color::White);
	   			lblQuit.setColor(sf::Color::White);
	   			break;
   			case Handler::MENU2:
   				lblStart.setColor(sf::Color::White);
	   			lblOptions.setColor(sf::Color::Red);
	   			lblQuit.setColor(sf::Color::White);
   				break;
   			case Handler::MENU3:
   				lblStart.setColor(sf::Color::White);
	   			lblOptions.setColor(sf::Color::White);
	   			lblQuit.setColor(sf::Color::Red);		
	   			break;
	   		case Handler::WAITING:
	   			text.setString("Waiting!!");
	   			break;
	   		case Handler::PREPARE1:
	   		case Handler::PREPARE2:
	   			text.setString("Rock!!");
	   			break;
	   		case Handler::PREPARE3:
	   		case Handler::PREPARE4:
	   			text.setString("Paper!!");
	   			break;
	   		case Handler::EVALUATE:
	   		case Handler::EVALUATE2:
	   			text.setString("Scissor!!");
	   			break;
	   		case Handler::ROCK:
	   			text.setString("R!!");
	   			break;
	   		case Handler::PAPER:
	   			text.setString("P!!");
	   			break;
	   		case Handler::SCISSOR:
	   			text.setString("S!!");
	   			break;
	   		default:
	   			text.setString("Unknown State");
	   			break;
	   };


	   if(Handler::cState==Handler::MENU1||Handler::cState==Handler::MENU2
	   	||Handler::cState==Handler::MENU3)
	   {
	   		m_window.draw(lblStart);
			m_window.draw(lblOptions);
   			m_window.draw(lblQuit);
	   }
	   else
	   {
			auto textBounds = text.getLocalBounds();
			auto textPos = sf::Vector2f(m_winWidth/2-textBounds.width/2,
									   m_winHeight/2-textBounds.height/2);

			text.setPosition(textPos);
			m_window.draw(text);
	   }

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