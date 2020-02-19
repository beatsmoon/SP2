#include "MiniGame1Obj.h"

MiniGame1Obj::MiniGame1Obj(float initalx, float initaly, float velx, float vely)
{
	locationx = initalx;
	locationy = initaly;
	Next = nullptr;
	velocityx = velx;
	velocityy = vely;
}

void MiniGame1Obj::setnextaddress(MiniGame1Obj* Address)
{
	Next = Address;
}


MiniGame1Obj* MiniGame1Obj::getnextadress()
{
	return Next;
}


float MiniGame1Obj::returnlocationx()
{
	return locationx;
}

float MiniGame1Obj::returnlocationy()
{
	return locationy;
}

float MiniGame1Obj::returnvelocityx()
{
	return velocityx;
}

float MiniGame1Obj::returnvelocityy()
{
	return velocityy;
}


void MiniGame1Obj::setx(float newx)
{
	locationx = newx;
}

void MiniGame1Obj::sety(float newy)
{
	locationy = newy;
}

void MiniGame1Obj::setvelx(float newx)
{
	velocityx = newx;
}

void MiniGame1Obj::setvely(float newy)
{
	velocityx = newy;
}

void MiniGame1Obj::movexybyvelocity()
{

	locationx += velocityx;
	locationy += velocityy;
}
