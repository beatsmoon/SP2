
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"


struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();


	void UpdateInput();
	void PostInputUpdate();

	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);

	static bool IsKeyPressed(unsigned short key);

private:

	//Declare a window object
	static int m_window_width;
	static int m_window_height;
	StopWatch m_timer;
};

#endif