#pragma once
class MiniGame1Obj
{

private:
	float locationx;
	float locationy;
	float velocityx;
	float velocityy;

	//For Powerups
	//0 - Slowdown
	//1 - Destroy Wall
	//2 - Gain Score
	//3 - Random
	//For Player
	//0 - Sprite 1
	//1 - Sprite 2
	//2 - Sprite 3
	//3 - Sprite 4
	int type;

	MiniGame1Obj* Next;
public:
	MiniGame1Obj();
	MiniGame1Obj(float initalx, float initaly, float velx, float vely);
	
	void setnextaddress(MiniGame1Obj* Address);
	MiniGame1Obj* getnextadress();

	float returnlocationx();
	float returnlocationy();
	float returnvelocityx();
	float returnvelocityy();
	int returntype();


	void setx(float newx);
	void sety(float newy);
	void setvelx(float newx);
	void setvely(float nety);
	void settype(int newtype);

	void movexybyvelocity();
};

