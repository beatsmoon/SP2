#include "MiniGame1Obj.h"

MiniGame1Obj::MiniGame1Obj(int initalx, int initaly)
{
	locationx = initalx;
	locationy = initaly;
}

float MiniGame1Obj::returnlocationx()
{
	return locationx;
}

float MiniGame1Obj::returnlocationy()
{
	return locationy;
}

void MiniGame1Obj::setx(int newx)
{
	locationx = newx;
}

void MiniGame1Obj::sety(int newy)
{
	locationy = newy;
}
