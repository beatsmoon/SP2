#ifndef SCENETEXT_H
#define SCENETEXT_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "PlayerInfo/PlayerInfo.h"
#include "Controllers/Keyboard.h"
#include "Controllers/Mouse.h"

class SceneText : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT, 
		GEO_RIGHT, 
		GEO_TOP, 
		GEO_BOTTOM, 
		GEO_FRONT, 
		GEO_BACK,
		GEO_WM_CAR,
		GEO_WM_CAR_WHEEL,
		GEO_VAL_CAR,
		GEO_VAL_CAR_WHEEL,
		GEO_G_CAR,
		GEO_G_CAR_WHEEL,
		GEO_C_CAR,
		GEO_C_CAR_WHEEL,
		GEO_SPACESTATION_FLOOR,
		GEO_SPACESTATION_WALL,
		GEO_SPACESTATION_CEILING,
		GEO_INTERFACE_BASE,
		GEO_STATS_WM,
		GEO_STATS_V,
		GEO_STATS_G,
		GEO_STATS_C,
		GEO_PAUSE,
		GEO_HIGHSCOREBOARD,
		GEO_INDICATOR,
		GEO_RACETRACK,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
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

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,

		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,

		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,

		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_LIGHT4_TYPE,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,

		U_LIGHT5_TYPE,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,

		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum STATE
	{
		STATE_START_MENU,
		STATE_SELECTION_SCREEN,
		STATE_TEST_DRIVE,
		STATE_MINI_GAME1,
		STATE_MINI_GAME2,
		STATE_TOTAL
	};

	enum MENU_CHOICE
	{
		CHOICE_NONE,
		CHOICE_RESUME,
		CHOICE_HIGHSCORE,
		CHOICE_EXIT,
		CHOICE_BACK,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[6];

	Camera2 camera;
	Camera2 cameraCar;
	CPlayerInfo* thePlayer;
	CMouse* theMouse;
	CKeyboard* theKeyboard;

	Car* CarWM; //TODO change this into an array
	bool CarUI[4] = { false, };
	float CarUIHeight[4] = {0, };
	float pauseHeight = 0;

	int renderingState = STATE_SELECTION_SCREEN;

	bool highScoreBoard = false;
	int pauseMenuSelection = CHOICE_NONE;
	float moveIndicator = 0;
	int moveDirection = 1;

	
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderPause();
	void RenderSpaceStation();
	void RenderWMCar();
	void RenderStatsUI();
	void RenderValCar();
	void RenderGCar();
	void RenderCCar();
	void RenderIndicator();
	void RenderHighscore();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void CalculateFrameRate();
	

public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif