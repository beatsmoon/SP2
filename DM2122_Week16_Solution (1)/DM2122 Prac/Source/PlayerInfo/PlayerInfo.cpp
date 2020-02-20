#include "PlayerInfo.h"
#include <iostream>

//#include "../Controllers/Mouse.h"
//#include "../Controllers/Keyboard.h"
//#include "../../../Common/Source/KeyboardController.h"
#include "Mtx44.h"
#include "MatrixStack.h"
#include "../../../Common/Source/MouseController.h"
//#include "RenderHelper.h"
//#include "GraphicsManager.h"
//#include "MeshBuilder.h"
#include "../Application.h"
#include "../../Source/SceneText.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: m_dSpeed(80.0)
	, m_dAcceleration(10.0)
	, m_dJumpSpeed(20.0)
	, m_dJumpAcceleration(-10.0)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_bPause(false)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
	//m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	int spaceStationWidth = 130;

	// Set Boundary
	maxBoundary.Set(spaceStationWidth,1, spaceStationWidth);
	minBoundary.Set(-spaceStationWidth, -1, -spaceStationWidth);


	//this->SetAABB(Vector3(1,1,1),Vector3(-1,-1,-1));

	
}


// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
//void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
//{
//	if (m_pTerrain != NULL)
//	{
//		this->m_pTerrain = m_pTerrain;
//
//		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
//	}
//}

bool CPlayerInfo::Move_FrontBack(const float dt, const bool direction, const float speedMultiplier)
{
	//cout << position << endl;
	Vector3 viewVector = target - position;
	Vector3 rightUV;
	Vector3 temp;
	temp = viewVector;
	temp.y = 0;
	temp.Normalized();
	if (direction)
	{
		position += temp * (float)m_dSpeed * (float)dt;
		target = position + viewVector;
		//cout << position << endl;
		return true;
	}
	else
	{
		position -= temp * (float)m_dSpeed * (float)dt;
		target = position + viewVector;
		return true;
	}
	return false;
}

bool CPlayerInfo::Move_LeftRight(const float dt, const bool direction)
{
	Vector3 viewVector = target - position;
	Vector3 rightUV;
	if (direction)
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position -= rightUV * (float)m_dSpeed * (float)dt;
		target = position + viewVector;
		return true;
	}
	else
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position += rightUV * (float)m_dSpeed * (float)dt;
		target = position + viewVector;
		return true;
	}
	return false;
}

bool CPlayerInfo::Move_Jump()
{
	//if (position.y == m_pTerrain->GetTerrainHeight(position))
	//{
	//	SetToJumpUpwards(true);
	//	return true;
	//}
	if (position.y <= 0)
	{
		Velocity.y = m_dJumpSpeed;
	}
	return false;
}

bool CPlayerInfo::Look_UpDown(const float dt, const bool direction)
{
	if (m_bPause)
		return false;
	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;
	if (direction)
	{
		float pitch = (float)(m_dSpeed * (float)dt);
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		return true;
	}
	else
	{
		float pitch = (float)(-m_dSpeed * (float)dt);
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		return true;
	}
}

bool CPlayerInfo::Look_LeftRight(const float dt, const bool direction)
{
	if (m_bPause)
		return false;
	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;
	if (direction)
	{
		float yaw = (float)m_dSpeed * (float)dt;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
		return true;
	}
	else
	{
		float yaw = (float)(-m_dSpeed * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		viewUV = rotation * viewUV;
		target = position + viewUV;
		rightUV = viewUV.Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		up = rightUV.Cross(viewUV).Normalized();
		return true;
	}
}

bool CPlayerInfo::Look_UpDown(const float dt, const bool direction, double mouse_diff_y)
{
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0
	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float pitch = (float)(-120 * camera_pitch * (float)dt);
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	viewUV = rotation * viewUV;
	target = position + viewUV;
	return true;
}

bool CPlayerInfo::Look_LeftRight(const float dt, const bool direction, double mouse_diff_x)
{
	if (m_bPause)
		return false;
	double camera_yaw = mouse_diff_x * 0.0174555555555556 * 2;		// 3.142 / 180.0 HARDCODED
	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float yaw = (float)(-m_dSpeed * camera_yaw * (float)dt); // 60 is m_dSpeed in magic number form
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	viewUV = rotation * viewUV;
	target = position + viewUV;
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();
	return true;
}

bool CPlayerInfo::Toggle_Pause()
{
	m_bPause = !m_bPause;
	if (m_bPause)
	{
		MouseController::GetInstance()->SetKeepMouseCentered(false);
	}
	else
	{
		MouseController::GetInstance()->SetKeepMouseCentered(true);
	}

	Application::ToggleCursor();

	return true;
}


// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

bool CPlayerInfo::GetPause()
{
	return m_bPause;
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	Vector3 viewVector = target - position;
	viewVector.y = Math::Clamp(viewVector.y, -0.8f, 0.8f);
	Vector3 viewUV = (viewVector).Normalized();

	position += Velocity * dt;
	if (position.y > 0)
		Velocity.y -= 10 * dt;
	else if (Velocity.y < 0)
	{
		position.y = 0;
		Velocity.y = 0;
	}

	Constrain();
	target = position + viewVector;
	// if Mouse Buttons were activated, then act on them


	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	//if (position.y > maxBoundary.y - 1.0f)
	//	position.y = maxBoundary.y - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	//if (position.y < minBoundary.y + 1.0f)
	//	position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

}

void CPlayerInfo::AttachCamera(Camera2* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}
