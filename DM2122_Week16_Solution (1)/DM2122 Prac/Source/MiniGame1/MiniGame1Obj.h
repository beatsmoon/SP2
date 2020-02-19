#pragma once
class MiniGame1Obj
{

private:
	float locationx;
	float locationy;
	float velocityx;
	float velocityy;
	float sizex;
	float sizey;
	MiniGame1Obj* Next;
public:
	MiniGame1Obj(float initalx, float initaly, float velx, float vely);
	void setnextaddress(MiniGame1Obj* Address);
	MiniGame1Obj* getnextadress();

	float returnlocationx();
	float returnlocationy();
	float returnvelocityx();
	float returnvelocityy();
	float returnsizex();
	float returnsizey();


	void setx(float newx);
	void sety(float newy);
	void setvelx(float newx);
	void setvely(float nety);
	void setsizex(float newx);
	void setsizey(float newy);

	void movexybyvelocity();
};

