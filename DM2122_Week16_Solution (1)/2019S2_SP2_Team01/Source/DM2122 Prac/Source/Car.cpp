#include "Car.h"

Car::Car(string _name, float _topSpeed, float _handling, float _acceleration, float _nitro)
	:Name(_name),
	Top_speed(_topSpeed),
	Handling(_handling),
	Acceleration(_acceleration),
	Nitro(_nitro)
{
	Pos = Vector3(0, 0, 0);
	Velocity = Vector3(0, 0, 0);
	Direction = Vector3(1, 0, 0);
}

Car::~Car()
{
}

void Car::SetPos(Vector3 pos)
{
	this->Pos = pos;

}

void Car::SetVelocity(Vector3 _vel)
{
	Velocity = _vel;
}

void Car::SetDirection(Vector3 _dir)
{
	Direction = _dir;
}

void Car::Set_name(string name)
{
	this->Name = name;

}

void Car::Set_top_speed(float top_speed)
{
	this->Top_speed = top_speed;
}

void Car::Set_handling(float handling)
{
	this->Handling = handling;
}

void Car::Set_acceleration(float acceleration)
{
	this->Acceleration = acceleration;

}

void Car::Set_nitro(float nitro)
{
	this->Nitro = nitro;

}

void Car::Set_Selected_car(Car* CarSelect)
{
	selectedCar = CarSelect;
}

Vector3 Car::GetPos()
{
	return Pos;
}

Vector3 Car::GetVelocity()
{
	return Velocity;
}

Vector3 Car::GetDirection()
{
	return Direction;
}

float Car::GetSpeed()
{
	return Velocity.Length();
}

string Car::Get_name()
{
	return Name;
}

float Car::Get_top_speed()
{
	return Top_speed;
}

float Car::Get_handling()
{
	return Handling;
}

float Car::Get_acceleration()
{
	return Acceleration;
}

float Car::Get_nitro()
{
	return Nitro;
}

Car* Car::Get_Selected_Car()
{
	return selectedCar;
}



void Car::Accelerate(bool _direction, double dt)
{
	// Find the direction car is facing
	Vector3 dirVec = Direction;

	// Car Velocity += acceleration towards the faced-direction
	if (_direction)
		Velocity += dirVec * Acceleration * dt;
	else
		Velocity -= dirVec * Acceleration * dt;

	// TODO: Change vel to single axis and towards car target in upate
}

void Car::Turn(bool _direction, double dt)
{
	//TODO: Inertia towards target instead of direction or multiply by length of velocity
	float handling = -Handling;
	handling *= dt * 2;
	handling = Math::DegreeToRadian(handling);
	if (!_direction)
		handling = -handling;
	// Find the direction car is facing
	Vector3 dirVec = Direction;

	// rotate direction in xz plane
	dirVec.x = (dirVec.x * cos(handling)) - (dirVec.z * sin(handling));
	dirVec.z = (dirVec.z * cos(handling)) + (dirVec.x * sin(handling));
	// Set Car Target in that direction
	Direction = dirVec;

	float p = Velocity.Length();
	if (!Velocity.IsZero())
	{
		Vector3 t = Velocity.Normalized();
		t *= 1 - (Handling * 0.001f);
		dirVec *= (Handling * 0.001f);
		Velocity = (t + dirVec) * p;
	}

	//selectedCar->Target = dirVec * distVec.Length() + selectedCar->GetPos();
}

void Car::update(double dt)
{
	Pos += Velocity * dt;
	//TODO Dissapate Velocity
	Vector3 Friction;
	if (!Velocity.IsZero())
		Friction = Velocity.Normalized();
	Velocity -= Friction * 1.f * dt;
}

void Car::Reset()
{
	Velocity.SetZero();
	Pos.SetZero();
	//Direction = Vector3(0, 0, 1);
}
