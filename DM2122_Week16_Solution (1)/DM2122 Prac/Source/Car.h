#pragma once

#include <Vector3.h>
#include <string.h>

using namespace std;

class Car
{
private:
	Vector3 Pos;
	Vector3 Velocity;
	Vector3 Direction;
	string Name;
	float Top_speed;
	float Handling;
	float Acceleration;
	float Nitro;

public:
	Car(string name, float top_speed, float handling, float acceleration, float nitro);
	~Car();

	// Getters and Setters
	void SetPos(Vector3);
	void SetVelocity(Vector3);
	void SetDirection(Vector3);
	void Set_name(string);
	void Set_top_speed(float);
	void Set_handling(float);
	void Set_acceleration(float);
	void Set_nitro(float);

	Vector3 GetPos();
	Vector3 GetVelocity();
	Vector3 GetDirection();
	string Get_name();
	float Get_top_speed();
	float Get_handling();
	float Get_acceleration();
	float Get_nitro();

	// Functions
	void Accelerate(bool direction, double dt);
	void Turn(bool direction, double dt);

	void update(double dt);

};

