#include "Application.hpp"
#include "GameHandler.hpp"
#include <iostream>

//This function will wait until our Leapcontroller is connected
//and afterwards launch a our window
//It also preloads all data (fonts,sounds, images)
bool Application::Initialize()
{
	m_winWidth = 800;
	m_winHeight = 600;

	//wait till we can connect to the controller
	while(!m_controller.isConnected())
	{}

	m_window.create(sf::VideoMode(m_winWidth,m_winHeight),"LEAP");
	m_window.setFramerateLimit(60);
	if(!m_window.isOpen())
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}

	//Load all data in our data folder during startup
	if (!m_font.loadFromFile("data/sunshine.ttf"))
	{
	    std::cout << "Failed to load game font!" << std::endl;
		return false;
	}

	if(!m_controller.addListener(m_listener))
	{
		std::cout << "Failed to add custom listener to controller!" << std::endl;
		return false;
	}

	if(!m_texPap.loadFromFile("data/pap.png"))
	{
		std::cout << "Failed to load paper image!" << std::endl;
		return false;
	}

	if(!m_texSci.loadFromFile("data/sci.png"))
	{
		std::cout << "Failed to load scissor image!" << std::endl;
		return false;
	}

	if(!m_texRoc.loadFromFile("data/roc.png"))
	{
		std::cout << "Failed to load rock image!" << std::endl;
		return false;
	}

	m_sprPlayer.setPosition(50,200);
	m_sprBot.setPosition(490,200);

	if(!m_blopSb.loadFromFile("data/blop.wav"))
	{
		std::cout << "Failed to load blop sound" << std::endl;
	}

	m_blopSound.setBuffer(m_blopSb);

	if(!m_winSb.loadFromFile("data/win.wav"))
	{
		std::cout << "Failed to load win sound" << std::endl;
	}

	m_winSound.setBuffer(m_winSb);

	if(!m_loseSb.loadFromFile("data/lose.wav"))
	{
		std::cout << "Failed to load lose sound" << std::endl;
	}

	m_loseSound.setBuffer(m_loseSb);
	return true;
}

//This is our main loop
//Depending on our current gamestate there are
//different outputs to the screen
void Application::Run()
{
	//Initialize all our text that is displayed during the game
	//set the used font for each text etc.
	sf::Text text,lblStart,lblQuit,lblOptions;
	text.setFont(m_font);
	text.setCharacterSize(60);
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

	       //used for menu navigation
	       if (event.type == sf::Event::KeyPressed)
	       {
	       		//play a blop sound ón button press
	       		m_blopSound.play();
				if (event.key.code == sf::Keyboard::Return)
				{
					if(Handler::cState==Handler::MENU1)
					{
						Handler::aiBehaviour = Handler::RNG;
						Handler::cState=Handler::WAITING;
					}
					else if(Handler::cState==Handler::MENU2)
					{
						Handler::aiBehaviour = Handler::HARD;
						Handler::cState=Handler::WAITING;
					}
					else if(Handler::cState==Handler::MENU3)
						m_window.close();
					else if(Handler::cState==Handler::PRESENT||Handler::cState==Handler::UNKNOWN)
					{
						if(Handler::playerPts==2||Handler::botPts==2)
						{
							Handler::playerPts = 0;
							Handler::botPts = 0;
							Handler::cState = Handler::MENU1;
						}
						else
							Handler::cState = Handler::WAITING;
					}
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

	   //depending on the current state we must change colors & content
	   //of our labels
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
	   else if(Handler::cState==Handler::PRESENT)
	   {
	   		Present();
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

//Called on shutdown
void Application::Release()
{
	if(m_window.isOpen())
		m_window.close();

	m_controller.removeListener(m_listener);
}

//This function is called upon presenting
//the result of a game.
void Application::Present()
{
	Handler::Result playerR = Handler::playerResult;
	Handler::Result botR = Handler::botResult;
	sf::Text lblPlayerPts,lblBotPts,lblMessage;
	std::string helper = "Player: ";

	for(int i=0;i<Handler::playerPts;++i)
		helper +='o';

	lblPlayerPts.setString(helper);
	lblPlayerPts.setPosition(50,50);
	lblPlayerPts.setFont(m_font);
	lblPlayerPts.setCharacterSize(40);
	helper.clear();
	helper = "Computer: ";
	for(int i=0;i<Handler::botPts;++i)
		helper+='o';

	if(Handler::playerPts==2)
	{
		if(m_winSound.getStatus()!=sf::Sound::Playing)
			m_winSound.play();
		lblMessage.setString("Player won!");
	}
	else if(Handler::botPts==2)
	{
		if(m_loseSound.getStatus()!=sf::Sound::Playing)
			m_loseSound.play();
		lblMessage.setString("Computer won!");
	}
	
	lblBotPts.setString(helper);
	lblBotPts.setPosition(490,50);
	lblBotPts.setFont(m_font);
	lblBotPts.setCharacterSize(40);
	helper.clear();

	//set the correct texture depending on what the 
	//player picked
	if(playerR==Handler::SCISSOR)
		m_sprPlayer.setTexture(m_texSci);
	else if(playerR==Handler::ROCK)
		m_sprPlayer.setTexture(m_texRoc);
	else if(playerR==Handler::PAPER)
		m_sprPlayer.setTexture(m_texPap);

	if(botR==Handler::SCISSOR)
		m_sprBot.setTexture(m_texSci);
	else if(botR==Handler::ROCK)
		m_sprBot.setTexture(m_texRoc);
	else if(botR==Handler::PAPER)
		m_sprBot.setTexture(m_texPap);

	m_window.draw(lblPlayerPts);
	m_window.draw(lblBotPts);
	m_window.draw(m_sprBot);
	m_window.draw(m_sprPlayer);

	if(Handler::playerPts==2||Handler::botPts==2)
	{
		lblMessage.setFont(m_font);
		lblMessage.setColor(sf::Color::Red);
		lblMessage.setCharacterSize(40);
		auto textBounds = lblMessage.getLocalBounds();
		auto textPos = sf::Vector2f(m_winWidth/2-textBounds.width/2,
									   150);

		lblMessage.setPosition(textPos);
		m_window.draw(lblMessage);
	}
}