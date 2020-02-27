#ifndef SceneMiniGame1_H
#define SceneMiniGame1_H

#include "../Scene.h"
#include <MatrixStack.h>
#include "../Camera2.h"
#include "../Mesh.h"
#include "../Light.h"
#include "MiniGame1Obj.h"
#include <fstream>
#include <iostream>
#include <../../irrklang/include/irrKlang.h>
using namespace irrklang;

using namespace std;

class SceneMiniGame1 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_BACKGROUND,
		GEO_CAR,
		GEO_CLEMENTCAR,
		GEO_VALCAR,
		GEO_WAIMENCAR,
		GEO_GLENDACAR,
		GEO_TEXT,
		GEO_WALL,
		GEO_WALLTEST,
		GEO_POWERUP,
		GEO_SCORE,
		GEO_LOST,
		GEO_MAIN,
		GEO_INDICATOR,
		GEO_CONTROLS,
		GEO_SELECTION,
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

		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
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

	MiniGame1Obj* BackgroundStart;
	MiniGame1Obj* BackgroundMid;
	
	MiniGame1Obj* WallStart;
	MiniGame1Obj* WallMid;

	MiniGame1Obj* Player;

	MiniGame1Obj* Powerup;

	int cursor;

	float gamespeed;

	bool playing;
	bool lost;

	double gameupdate;
	double nextupdate;

	double nextpowerspawn;

	double bouncetime;

	bool scoremenu;
	bool controlmenu;
	bool selectmenu;

	bool walldestroy;

	double animationtime;
	double nextanimation;

	bool wingsgoingup;

	int score;
	float gapsize;

	int Highscores[5];

	//0 - Red
	//1 - Green
	//2 - Blue
	//3 - Gold
	int Highscore1color;

	//Control Screen Size
	float screensizescore;
	float screensizecontrol;
	float screensizeselection;

	//0 - Clement
	//1 - Glenda
	//2 - Waimen
	//3 - Val
	int carselected;
	ISoundEngine* SoundEngine = createIrrKlangDevice();

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y);
	void RenderAnimationOnScreen(Mesh* mesh, int count, float size, float x, float y);
	void CalculateFrameRate();
	void Restart();

public:
	SceneMiniGame1();
	~SceneMiniGame1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif