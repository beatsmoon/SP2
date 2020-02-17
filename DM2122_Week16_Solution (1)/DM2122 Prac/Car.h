#pragma once
#include"Source/Vertex.h"
#include <string.h>

using namespace std;

class Car
{
private:
	Position Pos;
	string Name;
	float Top_speed;
	float Handling;
	float Acceleration;
	float Nitro;

public:
	Car(string name, float top_speed, float handling, float acceleration, float nitro, Position pos);
	~Car();

	// Getters and Setters
	void Set_pos(Position);
	void Set_name(string);
	void Set_top_speed(float);
	void Set_handling(float);
	void Set_acceleration(float);
	void Set_nitro(float);

	Position Get_pos();
	string Get_name();
	float Get_top_speed();
	float Get_handling();
	float Get_acceleration();
	float Get_nitro();

	// Functions

};

