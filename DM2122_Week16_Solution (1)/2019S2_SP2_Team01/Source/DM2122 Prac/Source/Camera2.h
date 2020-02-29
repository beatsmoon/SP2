#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"

class Camera2 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera2();
	~Camera2();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	Vector3 GetCameraPos() const;
	void SetCameraPos(Vector3 pos);
	Vector3 GetCameraTarget() const;
	void SetCameraTarget(Vector3 pos);
	Vector3 GetCameraUp() const;
	void SetCameraUp(Vector3 pos);
};

#endif