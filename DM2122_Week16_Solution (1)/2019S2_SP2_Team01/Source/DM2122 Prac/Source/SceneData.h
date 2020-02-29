#pragma once

#include "SingletonTemplate.h"

class Data :public Singleton<Data>
{
	friend Singleton<Data>;

public:
	enum class SceneType
	{
		MAIN,
		MINIGAME1,
		MINIGAME2,
	};

	enum class CarType
	{
		CAR_WM,
		CAR_V,
		CAR_C,
		CAR_G,
		CAR_TOTAL,
		CAR_NONE
	};

	Data();
	~Data();

	SceneType getCurrScene();
	void setCurrScene(SceneType _scene);
	CarType getCurrCar();
	void setCurrCar(CarType _car);
	bool getIsEnd();
	void EndApp();

private:
	bool isEnd = false;
	SceneType currScene = SceneType::MAIN;
	CarType currCar = CarType::CAR_WM;
};

