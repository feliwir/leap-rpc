#pragma once
#include <GLFW/glfw3.h>
#include "Listener.hpp"

class Application
{
public:
	Application();
	~Application();

	bool Initialize();
	void Run();
	void Release();
private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	GLFWwindow* m_window;
	LeapListener m_listener;
	Leap::Controller m_controller;
};