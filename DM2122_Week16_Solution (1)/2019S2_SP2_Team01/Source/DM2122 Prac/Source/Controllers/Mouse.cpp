#include "Mouse.h"
#include <iostream>
using namespace std;

#include "../../../Common/Source/MouseController.h"
#include "../PlayerInfo/PlayerInfo.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

// Create this controller
bool CMouse::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	//cout << "CMouse::Create()" << endl;

	return false;
}


// Read from the controller
int CMouse::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	//cout << "CMouse::Read()" << endl;

	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	// Process the keys for customisation
	if (mouse_diff_x != 0.0)
		Look_LeftRight(deltaTime, true, mouse_diff_x);
	if (mouse_diff_y != 0.0)
		Look_UpDown(deltaTime, true, Math::Clamp(mouse_diff_y, -60.0, 60.0));
	static int scroll = 0;
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != scroll)
	{
		scroll = MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET);
	}

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{

	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{

	}
	return 0;
}
