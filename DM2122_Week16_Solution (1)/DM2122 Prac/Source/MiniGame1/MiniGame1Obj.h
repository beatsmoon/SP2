#pragma once
class MiniGame1Obj
{

private:
	float locationx;
	float locationy;

public:
	MiniGame1Obj(int initalx, int initaly);
	float returnlocationx();
	float returnlocationy();
	void setx(int newx);
	void sety(int newy);

};

