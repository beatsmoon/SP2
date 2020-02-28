//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Application.h"

#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneText.h"
#include "MiniGame1//SceneMiniGame1.h"
#include "MiniGame2.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	//update opengl new window size
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Minigame1 Add
	CurrentScene = Data::GetInstance()->getCurrScene();
	bouncetime = GetTickCount64();
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	glfwSetWindowSizeCallback(m_window, resize_callback);
	
}

void Application::Run()
{

	//Main Loop
	
	Scene* scene = new SceneText();
	Scene* mainscene= scene;
	//Minigame1 Add
	Scene* minigame1 = new SceneMiniGame1();
	Scene* minigame2 = new MiniGame2();
	minigame1->Init();
	minigame2->Init();

	scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !Data::GetInstance()->getIsEnd())
	{
		glfwPollEvents();
		UpdateInput();

		scene->Update(m_timer.getElapsedTime());
		scene->Render();

		if (Data::GetInstance()->getCurrScene()!= CurrentScene)
		{
			CurrentScene = Data::GetInstance()->getCurrScene();

			switch (CurrentScene)
			{
			case Data::SceneType::MAIN:
				scene = mainscene;
				break;
			case Data::SceneType::MINIGAME1:
				scene = minigame1;
				break;
			case Data::SceneType::MINIGAME2:
				scene = minigame2;
				break;
			default:
				break;
			}
		}

	

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms. 
		PostInputUpdate();
	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();


	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = 800 >> 1;
		mouse_currY = 600 >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::ToggleCursor()
{
	double mouse_currX, mouse_currY;
	mouse_currX = 800 >> 1;
	mouse_currY = 600 >> 1;
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
	glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		// Hide the cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
		glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);
	}
	else
	{
		// Hide the cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
		glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);
	}
}


void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}