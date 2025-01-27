#pragma once
#include "Vector3.h"
//#include "../FPSCamera.h"

#include "../Camera2.h"
#include "../Car.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CPlayerInfo
{
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

public:
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

	// Initialise this class instance
	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set m_dJumpSpeed of the player
	void SetJumpSpeed(const double m_dJumpSpeed);
	// Set m_dJumpAcceleration of the player
	void SetJumpAcceleration(const double m_dJumpAcceleration);
	// Set Fall Speed of the player
	void SetFallSpeed(const double m_dFallSpeed);
	// Set Fall Acceleration of the player
	void SetFallAcceleration(const double m_dFallAcceleration);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	//void SetTerrain(GroundEntity* m_pTerrain);

	// Detect and process front / back movement on the controller
	bool Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	bool Move_LeftRight(const float deltaTime, const bool direction);
	bool Move_Jump();
	bool Look_UpDown(const float deltaTime, const bool direction);
	bool Look_LeftRight(const float deltaTime, const bool direction);
	bool Look_UpDown(const float deltaTime, const bool direction, double mouse_diff_y);
	bool Look_LeftRight(const float deltaTime, const bool direction, double mouse_diff_x);
	void Toggle_Pause();
	void Toggle_TestDrive();

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get Jump Speed of the player
	double GetJumpSpeed(void) const;
	// Get Jump Acceleration of the player
	double GetJumpAcceleration(void) const;
	// Get Fall Speed of the player
	double GetFallSpeed(void) const;
	// Get Fall Acceleration of the player
	double GetFallAcceleration(void) const;

	bool GetPause();

	// Update
	void Update(double dt = 0.0333f);
	void Render();
	void RenderUI();
	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	void AttachCamera(Camera2* _cameraPtr);
	void DetachCamera();

	// Handling Camera
	void SelectCar(Car* _carPTR);
	Car* GetSelectedCar();
	
	

private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
	Vector3 maxBoundary, minBoundary;
	//GroundEntity* m_pTerrain;
	Vector3 velocity;
	double m_dSpeed;
	double m_dAcceleration;

	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dFallSpeed;
	double m_dFallAcceleration;

	double m_dElapsedTime;

	bool m_bPause;
	bool m_bCar;

	Camera2* attachedCamera;
	Car* selectedCar;
};
