#ifndef SceneMiniGame1_H
#define SceneMiniGame1_H

#include "../Scene.h"
#include <MatrixStack.h>
#include "../Camera2.h"
#include "../Mesh.h"
#include "../Light.h"
#include "MiniGame1Obj.h"
#include "../sceneData.h"
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
		GEO_CAR, //Car used for gameplay
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

	//Pointers for background
	MiniGame1Obj* BackgroundStart; //Starting background
	MiniGame1Obj* BackgroundMid;
	
	//Pointers for walls
	MiniGame1Obj* WallStart; //Starting wall
	MiniGame1Obj* WallMid;

	//Pointer for player
	MiniGame1Obj* Player;

	//Pointer for powerup box
	MiniGame1Obj* Powerup;

	//Cursor Position on main screen
	int cursor;

	//Current Speed of game
	float gamespeed;

	//States for game
	bool playing; //Currently playing
	bool lost; //Was playing then lost

	//Time for next game update
	double gameupdate;
	double nextupdate; //HOw much time to add for next update

	//Time for next power up spawn
	double nextpowerspawn;

	//Bool for tracking ability to destory walls
	bool walldestroy; 

	//Time until next allowed key press
	double bouncetime;

	//Checking for menu states
	bool scoremenu; //Score Menu
	bool controlmenu; //Control Menu
	bool selectmenu; //Car Selection menu

	//Variables for animations
	double animationtime; //Time for next animationn
	double nextanimation; //How much time to add until next animation

	bool wingsgoingup; //Bool for tracking if animation going forward or backwards

	int score; //int for tracking current playing score

	float gapsize; //float for setting gap size of the walls

	int Highscores[5]; //int for tracking top 5 highscores

	//0 - Red
	//1 - Green
	//2 - Blue
	//3 - Gold
	int Highscore1color; //int for setting the top score color on the highscore menu

	//Control Screen Size
	float screensizescore; //Size for score menu
	float screensizecontrol; //Size for controls menu
	float screensizeselection; //Size for car selection menu

	//0 - Clement
	//1 - Glenda
	//2 - Waimen
	//3 - Val
	int carselected; //int for which car selected

	//Play Sound
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	double soundbuffer; //Stops sound from playing until time

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y);
	void RenderAnimationOnScreen(Mesh* mesh, int count, float size, float x, float y);
	void CalculateFrameRate();
	void Restart(); //Function for starting new 

public:
	SceneMiniGame1();
	~SceneMiniGame1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

};

#endif