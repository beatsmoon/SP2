#include "PlayerInfo.h"
#include <iostream>

//#include "../Controllers/Mouse.h"
//#include "../Controllers/Keyboard.h"
//#include "../../../Common/Source/KeyboardController.h"
#include "Mtx44.h"
#include "MatrixStack.h"
//#include "RenderHelper.h"
//#include "GraphicsManager.h"
//#include "MeshBuilder.h"
#include "../Application.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(30.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	//, attachedCamera(NULL)
	, theCurrentPosture(STAND)
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

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	//this->SetAABB(Vector3(1,1,1),Vector3(-1,-1,-1));
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 10.0;
	}
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
	return false;
}

bool CPlayerInfo::Move_ToggleCrouch()
{
	Vector3 viewDirection = target - position;
	if (theCurrentPosture == STAND)
	{
		theCurrentPosture = CROUCH;
		//position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 5.0f;
		target = position + viewDirection;
		m_dSpeed = 30.0f;
	}
	else if (theCurrentPosture != STAND)
	{
		theCurrentPosture = STAND;
		//position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z));
		target = position + viewDirection;
		m_dSpeed = 40.0f;
	}
	return true;
}

bool CPlayerInfo::Move_SetProne()
{
	Vector3 viewDirection = target - position;
	theCurrentPosture = PRONE;
	//position.y = m_pTerrain->GetTerrainHeight(Vector3(position.x, 0.0f, position.z)) - 7.0f;
	target = position + viewDirection;
	m_dSpeed = 10.0f;
	return true;
}

bool CPlayerInfo::Look_UpDown(const float dt, const bool direction)
{
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
	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
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

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
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

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t; v is m_dJumpSpeed AFTER updating using SUVAT where u is 
	// the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * m_dElapsedTime;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values.
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	//m_dJumpSpeed += m_dJumpAcceleration * dt;
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * m_dElapsedTime;
	// Check if the jump speed is below terrain, then it should be reset to terrain height


	//if (position.y < m_pTerrain->GetTerrainHeight(position))
	//{
	//	Vector3 viewDirection = target - position;
	//	position.y = m_pTerrain->GetTerrainHeight(position);
	//	target = position + viewDirection;
	//	m_dFallSpeed = 0.0;
	//	m_bFallDownwards = false;
	//	m_dElapsedTime = 0.0;
	//}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	Vector3 viewVector = target - position;
	viewVector.y = Math::Clamp(viewVector.y, -0.8f, 0.8f);
	Vector3 viewUV = (viewVector).Normalized();
	Constrain();
	target = position + viewVector;
	// if Mouse Buttons were activated, then act on them

	// If the user presses R key, then reset the view to default values
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);

	// If a camera is attached to this playerInfo class, then update it
	//if (attachedCamera)
	//{
	//	attachedCamera->SetCameraPos(position);
	//	attachedCamera->SetCameraTarget(target);
	//	attachedCamera->SetCameraUp(up);
	//}
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.y > maxBoundary.y - 1.0f)
	{
		position.y = maxBoundary.y - 1.0f;
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.y < minBoundary.y + 1.0f)
		position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		//if (position.y != m_pTerrain->GetTerrainHeight(position))
		//	position.y = m_pTerrain->GetTerrainHeight(position);

		switch (theCurrentPosture)
		{
		case CROUCH:
			position.y -= 5.0f;
			break;
		case PRONE:
			position.y -= 7.0f;
			break;
		default:
			break;
		}

	}
}

//void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
//{
//	attachedCamera = _cameraPtr;
//}

//void CPlayerInfo::DetachCamera()
//{
//	attachedCamera = nullptr;
//}
