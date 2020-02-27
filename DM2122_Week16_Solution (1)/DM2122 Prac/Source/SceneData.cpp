#include "SceneData.h"

Data::Data()
{
}

Data::~Data()
{
}

Data::SceneType Data::getCurrScene()
{
	return currScene;
}

void Data::setCurrScene(SceneType _scene)
{
	currScene = _scene;
}

Data::CarType Data::getCurrCar()
{
	return currCar;
}

void Data::setCurrCar(CarType _car)
{
	currCar = _car;
}

bool Data::getIsEnd()
{
	return isEnd;
}

void Data::EndApp()
{
	isEnd = true;
}
