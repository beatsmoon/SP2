#pragma once
#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "Car.h"
#include "PlayerInfo/PlayerInfo.h"
#include "Controllers/Keyboard.h"
#include "Controllers/Mouse.h"
#include "SceneData.h"
#include <../../irrklang/include/irrKlang.h>
#pragma comment(lib, "../../irrklang/irrKlang.lib")

class MiniGame2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_TRACK,
		GEO_ROCK,
		GEO_WM_CAR,
		GEO_VAL_CAR,
		GEO_G_CAR,
		GEO_C_CAR,
		GEO_DEATHMENU,
		GEO_INSTRUCTIONS,
		GEO_INDICATOR,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum MINIGAME2_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_IS_ROAD,
		U_ROAD_OFFSET,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[1];

	Camera2 camera;
	CPlayerInfo* thePlayer;
	CMouse* theMouse;
	CKeyboard* theKeyboard;
	Car* theCar;
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	bool NitroUsed = false;
	bool gameEnd = false;
	bool gameStart = false;
	bool crashsound = false;
	bool rock1End = true;
	float distance = 0;
	int lanes;
	float carX = 0;
	float StartZ = 30; //Starting Position of Z
	float Rotate = 0;
	float RotateCar = 0;
	int Highscores[5];
	int carselected = 0;

	float offset_Y = 0;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderRoadMesh(Mesh* mesh);
	void RenderTrack();
	void RenderRock(float posX, float posZ);
	void NitroBoostCoolDown(double dt);
	void DistanceTravelled(double dt);
	void LanesRandom(); // Rand() for which lane rock should be at
	void RockGoingDown(double dt); //posZ going down
	void RenderCar();
	void RenderDeathMenu();
	void RenderInstructions();
	void reset();

	bool collisionWithRock();
	bool collision = false;
	void CollisionUpdate(double dt);
	bool hit = false;
	bool HighScore = false;
	void ReadHighScore_minigame2();
	float getDistance();
	bool changeScore = false;
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void CalculateFrameRate();


public:
	MiniGame2();
	~MiniGame2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
