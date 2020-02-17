#include "Controller.h"
#include <iostream>
using namespace std;

CController::CController()
	: thePlayerInfo(NULL)
{
}


CController::~CController()
{
	// We just set thePlayerInfo to NULL without deleting. SceneText will delete this.
	if (thePlayerInfo)
		thePlayerInfo = NULL;
}


// Create this controller
bool CController::Create(CPlayerInfo* thePlayerInfo)
{
	cout << "CController::Create()" << endl;
	this->thePlayerInfo = thePlayerInfo;
	return false;
}

// Read from the controller
int CController::Read(const const float deltaTime)
{
	//cout << "CController::Read()" << endl;
	return 0;
}

// Detect and process front / back movement on the controller
bool CController::Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier)
{
	//cout << "CController::Move_FrontBack()" << endl;
	if (thePlayerInfo)
	{
		thePlayerInfo->Move_FrontBack(deltaTime, direction, speedMultiplier);
	}
	return false;
}

// Detect and process left / right movement on the controller
bool CController::Move_LeftRight(const float deltaTime, const bool direction)
{
	//cout << "CController::Move_LeftRight()" << endl;
	if (thePlayerInfo)
	{
		thePlayerInfo->Move_LeftRight(deltaTime, direction);
	}
	return false;
}

// Detect and process look up / down on the controller
bool CController::Look_UpDown(const float deltaTime, const bool direction)
{
	//cout << "CController::Look_UpDown()" << endl;
	if (thePlayerInfo)
	{
		thePlayerInfo->Look_UpDown(deltaTime, direction);
	}
	return false;
}

// Detect and process look left / right on the controller
bool CController::Look_LeftRight(const float deltaTime, const bool direction)
{
	//cout << "CController::Look_LeftRight()" << endl;
	if (thePlayerInfo)
	{
		thePlayerInfo->Look_LeftRight(deltaTime, direction);
	}
	return false;
}

bool CController::Look_UpDown(const float deltaTime, const bool direction, double mouse_diff_y)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->Look_UpDown(deltaTime, direction, mouse_diff_y);
	}
	return false;
}

bool CController::Look_LeftRight(const float deltaTime, const bool direction, double mouse_diff_x)
{
	if (thePlayerInfo)
	{
		thePlayerInfo->Look_LeftRight(deltaTime, direction, mouse_diff_x);
	}
	return false;
}

bool CController::Move_Jump()
{
	if (thePlayerInfo)
	{
		thePlayerInfo->Move_Jump();
	}
	return false;
}

bool CController::Move_ToggleCrouch()
{
	if (thePlayerInfo)
	{
		thePlayerInfo->Move_ToggleCrouch();
	}
	return false;
}

bool CController::Move_SetProne()
{
	if (thePlayerInfo)
	{
		thePlayerInfo->Move_SetProne();
	}
	return false;
}

// Reload current weapon
bool CController::Reload(const float deltaTime)
{
	cout << "CController::Reload()" << endl;
	return false;
}

// Change current weapon (for primary only)
bool CController::Change(const float deltaTime)
{
	cout << "CController::Change()" << endl;
	return false;
}

// Fire primary weapon
bool CController::FirePrimary(const float deltaTime)
{
	cout << "CController::FirePrimary()" << endl;
	return false;
}

// Fire secondary weapon
bool CController::FireSecondary(const float deltaTime)
{
	cout << "CController::FireSecondary()" << endl;
	return false;
}