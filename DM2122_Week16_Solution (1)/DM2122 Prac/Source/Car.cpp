#include "Car.h"

Car::Car(string name, float top_speed, float handling, float acceleration, float nitro, Position pos)
{
	this->Name = name;
	this->Pos = pos;
	this->Top_speed = top_speed;
	this->Acceleration = acceleration;
	this->Handling = handling;
	this->Nitro = nitro;
}

void Car::Set_pos(Position pos)
{
	this->Pos = pos;

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

Position Car::Get_pos()
{
	return Pos;
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
