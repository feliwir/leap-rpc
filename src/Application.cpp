#include "Application.hpp"
#include <iostream>

Application::Application()
{
	glfwInit();
}


Application::~Application()
{
	glfwTerminate();
}

bool Application::Initialize()
{
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

	m_window = glfwCreateWindow(800,600,"LEAP",nullptr,nullptr);
	if(!m_window)
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}

	return true;
}

void Application::Run()
{
	glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(m_window,KeyCallback);
    while (!glfwWindowShouldClose(m_window))
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    Release();
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "KEY CALLBACK" << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::Release()
{
	if(m_window)
		glfwDestroyWindow(m_window);

	m_controller.removeListener(m_listener);
}