#include "Keyboard.h"
#include <iostream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"
#include <GLFW/glfw3.h>


CKeyboard::CKeyboard()
{
}


CKeyboard::~CKeyboard()
{
}

// Create this controller
bool CKeyboard::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	//cout << "CKeyboard::Create()" << endl;
	return false;
}


// Read from the controller
int CKeyboard::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	//cout << "CKeyboard::Read()" << endl;
	
	// Process the keys for customisation
	static double HoldTime_Control = 0.0;
	// Movement
	if (KeyboardController::GetInstance()->IsKeyDown('W'))
		Move_FrontBack(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown('S'))
		Move_FrontBack(deltaTime, false);
	if (KeyboardController::GetInstance()->IsKeyDown('A'))
		Move_LeftRight(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown('D'))
		Move_LeftRight(deltaTime, false);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		Look_UpDown(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		Look_UpDown(deltaTime, false);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		Look_LeftRight(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		Look_LeftRight(deltaTime, false);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
		Move_Jump();
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_CONTROL))
		Toggle_Pause();
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
		Toggle_Pause();


	/*
		Controller
	*/
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present == 1)
	{
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		if (axes[1] > 0.f)
		{
			Move_FrontBack(deltaTime * axes[1], true);
		}
		if (axes[1] < 0.f)
		{
			Move_FrontBack(deltaTime * -axes[1], false);
		}
		if (axes[0] > 0.f)
		{

		}
		if (axes[0] > 0.f)
		{
			Move_LeftRight(deltaTime * -axes[0], true);
		}
		if (axes[0] < 0.f)
		{
			Move_LeftRight(deltaTime * axes[0], false);
		}
	}

	// Weapons

	return 0;
}
