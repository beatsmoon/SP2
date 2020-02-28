#include "SceneMiniGame1.h"
#include "GL\glew.h"
#include "../Application.h"
#include <Mtx44.h>
#include "../shader.hpp"
#include "../MeshBuilder.h"
#include "../Utility.h"
#include "../LoadTGA.h"
#include "../../../Common/Source//KeyboardController.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f;

SceneMiniGame1::SceneMiniGame1()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneMiniGame1::~SceneMiniGame1()
{

}

void SceneMiniGame1::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0,10,1), Vector3(0, 0, 0), Vector3(0, 1, 0));


	

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	
	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("FlappyCarBackground", Color(1, 0.1, 0.1), 1.f, 1.f);
	meshList[GEO_WALLTEST] = MeshBuilder::GenerateQuad("FlappyCarBackgroundTest", Color(1, 1, 1), 1.f, 1.f);
	
	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("FlappyCarBackground", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image/Background_FlappyCar1_Clement.tga");

	//Car meshes
	meshList[GEO_CAR] = MeshBuilder::GenerateText("Car", 2, 2);
	meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");

	meshList[GEO_CLEMENTCAR] = MeshBuilder::GenerateText("ClementCar", 2, 2);
	meshList[GEO_CLEMENTCAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");
	
	meshList[GEO_GLENDACAR] = MeshBuilder::GenerateText("GlendaCar", 2, 2);
	meshList[GEO_GLENDACAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");	
	
	meshList[GEO_VALCAR] = MeshBuilder::GenerateText("ValCar", 2, 2);
	meshList[GEO_VALCAR]->textureID = LoadTGA("Image//ValCar_FlappyCar1_Clement.tga");		
	
	meshList[GEO_WAIMENCAR] = MeshBuilder::GenerateText("WaimenCar", 2, 2);
	meshList[GEO_WAIMENCAR]->textureID = LoadTGA("Image//WaimenCar_FlappyCar1_Clement.tga");	

	
	meshList[GEO_POWERUP] = MeshBuilder::GenerateText("Powerup", 2, 2);
	meshList[GEO_POWERUP]->textureID = LoadTGA("Image//PowerUp_FlappyCar1_Clement.tga");	
	
	//Menu meshes
	meshList[GEO_SCORE] = MeshBuilder::GenerateQuad("Score", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_SCORE]->textureID = LoadTGA("Image//Highscore_FlappyCar1_Clement.tga");
	
	meshList[GEO_MAIN] = MeshBuilder::GenerateQuad("Main", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_MAIN]->textureID = LoadTGA("Image//MainMenu_FlappyCar1_Clement.tga");	
	
	meshList[GEO_INDICATOR] = MeshBuilder::GenerateQuad("Indicator", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_INDICATOR]->textureID = LoadTGA("Image//Indicator_FlappyCar1_Clement.tga");	
	
	meshList[GEO_LOST] = MeshBuilder::GenerateQuad("Lost_Menu", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LOST]->textureID = LoadTGA("Image//Lost_FlappyCar1_Clement.tga");	

	meshList[GEO_CONTROLS] = MeshBuilder::GenerateQuad("Control_Menu", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_CONTROLS]->textureID = LoadTGA("Image//Controls_FlappyCar1_Clement.tga");	
	
	meshList[GEO_SELECTION] = MeshBuilder::GenerateQuad("Selection_Menu", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_SELECTION]->textureID = LoadTGA("Image//Selection_FlappyCar1_Clement.tga");
	
	//Genral Text Mesh
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga"); 


	//Inital values for startup
	BackgroundStart = new MiniGame1Obj(400,300,0,0);
	playing = false;
	lost = false;
	gameupdate = GetTickCount64();
	nextupdate = 5000;

	Player = new MiniGame1Obj(400, 300, 0, 0);
	Player->settype(0);
	wingsgoingup = false;


	gamespeed = -0.4;
	score = 0;
	nextanimation = 150;
	gapsize = 175;

	walldestroy = false;

	cursor = 0;

	bouncetime = GetTickCount64();
	scoremenu = false;
	controlmenu = false;

	Highscore1color = 0;

	screensizescore = 0;
	screensizecontrol = 0;gameupdate = GetTickCount64();
	nextupdate = 5000;

	Player = new MiniGame1Obj(400, 300, 0, 0);
	Player->settype(0);
	wingsgoingup = false;


	gamespeed = -0.4;
	score = 0;
	nextanimation = 150;
	gapsize = 175;

	walldestroy = false;

	cursor = 2;

	bouncetime = GetTickCount64();
	scoremenu = false;
	controlmenu = false;
	selectmenu = false;

	Highscore1color = 0;

	screensizescore = 0;
	screensizecontrol = 0;
	screensizeselection = 0;


	carselected = 0;

	soundbuffer = GetTickCount64();

}

void SceneMiniGame1::Update(double dt)
{
	//Check and set current car
	switch (Data::GetInstance()->getCurrCar())
	{
	case Data::CarType::CAR_C:
		carselected = 0;
		break;
	case Data::CarType::CAR_G:
		carselected = 1;
		break;
	case Data::CarType::CAR_WM:
		carselected = 2;
		break;
	case Data::CarType::CAR_V:
		carselected = 3;
		break;
	default:
		carselected = 0;
		break;
	}
	CalculateFrameRate();
	

	//States for game
	//User is playing now
	if (playing == true && lost == false)
	{
		//Checking for game update
		//Spawn New Wall + Speed Up Game
		if (gameupdate <= GetTickCount64())
		{
			gameupdate = GetTickCount64() + nextupdate; //Set time to next update
			float speed;
			//Max speed according to which car selected
			switch (carselected)
			{
			case 0:
				speed = -5.5;
				break;
			case 1:
				speed = -4.5;
				break;
			case 2:
				speed = -4.5;
				break;
			case 3:
				speed = -4;
			}
			//Game speed up
			if (gamespeed > -speed) //SPEED
			{
				gamespeed -= 0.5;
				if (gamespeed <= -speed) //Reached max speed
				{
					gapsize = 150;
					nextupdate = 3500;
				}
			}

			//Form new wall
			WallMid = WallStart; //Set wallmid to start
			if (WallStart != nullptr) //Check if starting has a wall
			{
				while (WallMid->getnextadress() != nullptr) //Check until last wall
				{
					WallMid = WallMid->getnextadress(); //if not last wall get next wall
				}
			}
			//Set random gap location
			float ylocation = (rand() % 590) + 10; //Random num from 10 - 599 
			//Form new wall
			MiniGame1Obj* newwall = new MiniGame1Obj(810, ylocation, gamespeed - 1, 0);
			if (WallStart == nullptr) //If there was no first wall
			{
				WallStart = newwall; //Set start to new wall
			}
			else
			{
				WallMid->setnextaddress(newwall); //Set next wall to be new wall

			}
		}

		//Spawn Powerup
		if (nextpowerspawn <= GetTickCount64())
		{
			//Set random time until next powerup spawn
			nextpowerspawn = GetTickCount() + (((rand() % 10) + 10) * 1000); //Randomsise Next Spawn Time (10-20s)
			Powerup = new MiniGame1Obj(810, 300, gamespeed - 1, 0); //Spawn new powerup;
			int type = rand() % 4; //nunmber from 0 - 3
			//0 - Slowdown
			//1 - Destroy Wall
			//2 - Gain Score
			//3 - Random
			Powerup->settype(type);
		}

		//Update Animations
		if (animationtime <= GetTickCount64())
		{
			//Set time until next animation
			animationtime = GetTickCount64() + nextanimation;
			//For Player
			//0 - Sprite 1
			//1 - Sprite 2
			//2 - Sprite 3
			//3 - Sprite 4
			int type = Player->returntype(); //Check current animation state
			if (wingsgoingup == true) //Check if animation going foward
			{
				type++;
				if (type >= 4) //Max animation state is 3
				{
					wingsgoingup = false;
					type = 2;
				}
			}
			else //Animation going backwards
			{
				type--;
				if (type <= -1) //minimum animation state is 0
				{
					wingsgoingup = true;
					type = 0;
				}
			}
			Player->settype(type); //Set animation state
		}

		//Update Powerups
		if (Powerup != nullptr)
		{
			Powerup->movexybyvelocity(); //Move powerup location based on velocity
			//Check if too far left
			if (Powerup->returnlocationx() < -30)
			{
				//Delete powerup
				delete Powerup;
				Powerup = nullptr;
			}

			//Check if player has touched powerup box
			//Check x location
			else if ((Player->returnlocationx() + 25 >= Powerup->returnlocationx() - 25 && Player->returnlocationx() + 25 <= Powerup->returnlocationx() + 25) || /*Front half*/
				(Player->returnlocationx() - 25 >= Powerup->returnlocationx() - 25 && Player->returnlocationx() - 25 <= Powerup->returnlocationx() + 25) /*Back half*/)
			{
				//Check y - location
				if (((Player->returnlocationy() + 25 >= Powerup->returnlocationy() - 25 && Player->returnlocationy() + 25 <= Powerup->returnlocationy() + 25 ||
					Player->returnlocationy() - 25 >= Powerup->returnlocationy() - 25 && Player->returnlocationy() - 25 <= Powerup->returnlocationy() + 25)))
				{
					//0 - Slowdown
					//1 - Destroy Wall
					//2 - Gain Score
					//3 - Random
					// Check what kind of powerup
					switch (Powerup->returntype())
					{
					case 0:
						gamespeed += 1;
						gapsize = 175;
						nextupdate = 5000;
						break;

					case 1:
						walldestroy = true;
						break;

					case 2:
						score += 100;
						break;

					case 3:
						//Generate a random number
						int type = rand() % 3; //Random number from 0 - 2
						switch (type)
						{
						case 0:
							gamespeed += 1;
							gapsize = 175;
							nextupdate = 5000;
							break;

						case 1:
							walldestroy = true;
							break;

						case 2:
							score += 100;
							break;
						}
						break;
					}
					//Destory powerup box after collection
					delete Powerup;
					Powerup = nullptr;
				}
			}

		}

		//Destroy Wall 
		//Check for Key press and powerup collected
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && walldestroy == true)
		{
			//Set mid to starting wall
			WallMid = WallStart;
			//Store prev wall
			MiniGame1Obj* Prev;
			Prev = WallStart; //Sety prev to startingh wall
			while (WallMid != nullptr) //Check until no more walls
			{
				//Wall directly infront of player
				if (WallMid->returnlocationx() > Player->returnlocationx())
				{
					if (WallMid == WallStart) //First Wall
					{
						WallStart = WallMid->getnextadress();
						delete WallMid;
						WallMid = nullptr;
					}
					else //Not first wall
					{
						Prev->setnextaddress(WallMid->getnextadress());
						delete WallMid;
						WallMid = nullptr;
					}
					SoundEngine->play2D("audio/sound_wallbreak.mp3", GL_FALSE); //Play sound for wall breaking
					walldestroy = false;
					break;
				}
				else //Wall is not directly infront of player
				{
					Prev = WallMid;
					WallMid = WallMid->getnextadress(); //Get next wall
				}
				
			}
		}

		//Update Wall Position
		WallMid = WallStart;
		while (WallMid != nullptr)
		{
			//Move wall by veclocity
			WallMid->movexybyvelocity();

			//AABB collision check
			//Player x position check (front half or back half hit)
			if ((Player->returnlocationx() + 25 >= WallMid->returnlocationx() - 25 && Player->returnlocationx() + 25 <= WallMid->returnlocationx() + 25) || /*Front half*/
				(Player->returnlocationx() - 25 >= WallMid->returnlocationx() - 25 && Player->returnlocationx() - 25 <= WallMid->returnlocationx() + 25) /*Back half*/)
			{
				bool Topactive = true;
				bool Bottomactive = true;
				//Check if at top
				if (WallMid->returnlocationy() > 590) //At top
				{
					Topactive = false;
					//Check player y location
					if (Player->returnlocationy()-25 <= (WallMid->returnlocationy()) - gapsize)
					{
						lost = true;
					}

				}
				//Check if at bottom
				if (WallMid->returnlocationy() < 10) //At Bottom
				{
					Bottomactive = false;
					//Check Player y location
					if (Player->returnlocationy() + 25 >= WallMid->returnlocationy() + gapsize)
					{
						lost = true;
					}
				}
				//Wall gap in middle
				if (Topactive == true && Bottomactive == true)
				{
					//Check Player y location
					if (((Player->returnlocationy() + 25 <= WallMid->returnlocationy() - (gapsize/2) || Player->returnlocationy() + 25  >= WallMid->returnlocationy() + (gapsize / 2)) ||
						Player->returnlocationy() - 25 <= WallMid->returnlocationy() - (gapsize / 2) || Player->returnlocationy() - 25 >= WallMid->returnlocationy() + (gapsize / 2)))
					{
						lost = true;
					}
				}
				
			}
			if (WallMid->getnextadress() == nullptr) //No more walls
			{
				break;
			}
			WallMid = WallMid->getnextadress(); //Check next wall
		}

		//Delete Wall if wall if too far left
		if (WallStart != nullptr && WallStart->returnlocationx() <= -20)
		{
			MiniGame1Obj* store = WallStart;
			WallStart = WallStart->getnextadress();
			delete store; //Delete wall
			score += 100; //Increase score
		}

		//Update Background
		BackgroundMid = BackgroundStart;
		while (true) //Check until no more backgrounds
		{
			//Set background vecloity to gamespeed
			BackgroundMid->setvelx(gamespeed);
			//Move background by velocity
			BackgroundMid->movexybyvelocity();
			//Check if there is another background pointer
			if (BackgroundMid->getnextadress() == nullptr)
			{
				break;
			}
			//Set next background
			BackgroundMid = BackgroundMid->getnextadress();
		}
		
		//Check if last background is far enough to spawn another background
		if (BackgroundMid->returnlocationx() <= 400)
		{
			MiniGame1Obj* NewBackGround = new MiniGame1Obj(BackgroundMid->returnlocationx() + 798.5 ,300,BackgroundMid->returnvelocityx(),0);
			BackgroundMid->setnextaddress(NewBackGround);
			
		}
		if (BackgroundStart->returnlocationx() <= -400) //Background cannot be seen anymore
		{
			MiniGame1Obj* Store = BackgroundStart->getnextadress();
			delete BackgroundStart; //Delet background
			BackgroundStart = Store; //New starting background
		}
		
		//Player Movement
		float negvelocity = -0.15; //Default Weight
		//Set weight based on stats of car (Weight) (Name: Stat no.)
		switch (carselected)
		{
		case 0: //Clement 3
			negvelocity = -0.25;
			break;
		case 1: //Glenda 2
			negvelocity = -0.15;
			break;
		case 2: //Waimen 2
			negvelocity = -0.15;
			break;
		case 3: //Val 1
			negvelocity = -0.10;
			break;
		}
		Player->setvely(Player->returnvelocityy() + negvelocity); //Set velcoity based on weight
		//Player Jumped
		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE)) //floatiness
		{
			//Set y upwards velocity based on car stats (Floatiness) (Name : Stat no.)
			switch (carselected)
			{
			case 0: //Clement 1
				Player->setvely(4.95);
				break;
			case 1: //Glenda 3
				Player->setvely(5.55);
				break;
			case 2: //Waimen 2
				Player->setvely(5);
				break;
			case 3: //Val 2
				Player->setvely(5);
				break;
			}
			//Waits to play sound again
			if (soundbuffer <= GetTickCount64())
			{
				soundbuffer = GetTickCount64() + 250;
				SoundEngine->play2D("audio/sound_jump.mp3", GL_FALSE); //Play jump sound
			}

		}
		Player->movexybyvelocity(); //Move player by velocity
		if (Player->returnlocationy() < 25) //Check if player at bottom
		{
			Player->sety(25); //Prevent playe from falling below the road
		}
		else if (Player->returnlocationy() > 575) //Check if player is on the top
		{
			Player->sety(575); //Prevent player from flying off the top of the map
		}

		//Player just lost
		if (lost == true)
		{
			int x = 0;
			bool newhighscore = false;
			std::string line;
			std::ifstream scorefile("Highscore//MiniGame1Highscore.txt"); //Read from file
			//Get Highscores from filem
			if (scorefile.is_open())
			{
				while (getline(scorefile, line))
				{
					Highscores[x] = stoi(line); //Convert string to int
					x++;
				}
			scorefile.close();
			}

			//Compare highscores
			for (int x = 0; x < 5; x++)
			{
				if (score >= Highscores[x])
				{
					int store = score;
					if (x == 0) //If new top score
					{
						newhighscore = true;
					}
					//Shift rest of the scores down
					for (; x < 5; x++)
					{
						int store2 = Highscores[x]; //Save old current score
						Highscores[x] = store; //Set new score
						store = store2; //Store old current score
					}
				}
			}
			//Play sound based on if new highscore or not
			if (newhighscore == true)
			{
				SoundEngine->play2D("audio/sound_highscore.mp3", GL_FALSE); //Play sound for highscore
			}
			else
			{
				SoundEngine->play2D("audio/sound_crash.wav", GL_FALSE); //Play sound for crashing
			}

			//Write into txt file
			ofstream scorefiles("Highscore//MiniGame1Highscore.txt");
			if (scorefiles.is_open())
			{
				//Write all scores
				for (int x = 0; x < 5; x++)
				{
					scorefiles << (Highscores[x]) << "\n";
				}
				scorefiles.close();
			}
		}
	}
	//User has lost
	else if (lost == true)
	{
		if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN)) //Wait for return keypress
		{
			Restart();
		}
	}
	//User hasnt started game
	else
	{
		//Main Menu
		if (scoremenu == false && controlmenu == false && selectmenu == false)
		{
			//Reduce Score Screen size
			if (screensizescore > 0)
			{
				screensizescore -= 20;
				if (screensizescore < 0)
				{
					screensizescore = 0;
				}
			}
			//Reduce Control Screen size
			if (screensizecontrol > 0)
			{
				screensizecontrol -= 20;
				if (screensizecontrol < 0)
				{
					screensizecontrol = 0;
				}
			}
			//Reduce Car Selection Selection screen size
			if (screensizeselection > 0)
			{
				screensizeselection -= 20;
				if (screensizeselection < 0)
				{
					screensizeselection = 0;
				}
			}
			//Moving Cursor
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) && bouncetime <= GetTickCount64())
			{
				SoundEngine->play2D("audio/sound_minigame1cursor.mp3", GL_FALSE); //Play sound for cursor
				cursor--;
				if (cursor <= -1) //Minimun cursor number is 0
				{
					cursor = 4;
				}
				bouncetime = GetTickCount64() + 100; //Set time until next keypress
			}
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) && bouncetime <= GetTickCount64())
			{
				SoundEngine->play2D("audio/sound_minigame1cursor.mp3", GL_FALSE);//Play sound for cursor
				cursor++;
				if (cursor >= 5) //MAx cursor num is 4
				{
					cursor = 0;
				}
				bouncetime = GetTickCount64() + 100; //Set time until next keypress
			}
			//Selection Menu Button
			if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN) && bouncetime <= GetTickCount64())
			{
				switch (cursor)
				{
				//Start Game
				case 0:
				{
					SoundEngine->play2D("audio/sound_start.wav", GL_FALSE); //Play sound for starting game
					bouncetime = GetTickCount64() + 250; //Set time until next key press allowed
					playing = true;
					BackgroundStart->setvelx(gamespeed);
					gameupdate = GetTickCount64() + nextupdate;
					srand(time(nullptr)); //Get Time seed
					float size = (rand() % 580) + 10;
					WallStart = new MiniGame1Obj(800, size, gamespeed - 1, 0);
					Player->setvely(0);
					nextpowerspawn = GetTickCount() + (((rand() % 10) + 10) * 1000); //Randomsise Next Spawn Time (10-20s)
					Player->settype(0);
					animationtime = GetTickCount64() + nextanimation;
					break;
				}
				//Highscores
				case 1:
				{
					if (controlmenu == false && selectmenu == false)
					{
						bouncetime = GetTickCount64() + 1000;//Set time until next key press allowed
						scoremenu = true;
						std::string line;
						std::ifstream scorefile("Highscore//MiniGame1Highscore.txt");
						int x = 0;
						if (scorefile.is_open())
						{
							while (getline(scorefile, line))
							{
								Highscores[x] = stoi(line); //Convert string to int
								x++;
							}
							scorefile.close();
						}

						SoundEngine->play2D("audio/sound_highscoremenu.wav", GL_FALSE); //Play Sound for highscore menu
					}
					break;
				}

				//Controls
				case 2:
				{
					bouncetime = GetTickCount64() + 1000;//Set time until next key press allowed
					if (scoremenu == false && selectmenu == false)
					{
						controlmenu = true;
						SoundEngine->play2D("audio/sound_minigame1menu.mp3", GL_FALSE); //Play sound for menu screen

					}

					break;
				}
				//Car Selection
				case 3:
				{
					bouncetime = GetTickCount64() + 1000; //Set time until next key press allowed
					if (scoremenu == false && controlmenu == false)
					{
						selectmenu = true;
					}
					SoundEngine->play2D("audio/sound_minigame1menu.mp3", GL_FALSE); //Play sound for menu screen
					break;
				}
				//Exit to main
				case 4:
				{
					Data::GetInstance()->setCurrScene(Data::SceneType::MAIN);
					cursor = 2; //Set cursor to controls
				}
				}
			}
		}
		else if (controlmenu == true) // control menu
		{
			//Increase contol screen size
			if (screensizecontrol < 500)
			{
				screensizecontrol += 10;
				if (screensizecontrol > 500) //Max screen  size is 500
				{
					screensizecontrol = 500;
				}
			}

			if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN) && bouncetime <= GetTickCount64())
			{
				bouncetime = GetTickCount64() + 500;
				controlmenu = false;
			}
		}
		else if (selectmenu == true) //Car Selection Menu
		{
			if (screensizeselection < 550)
			{
				screensizeselection += 10;
				if (screensizeselection > 550) //Max scren size is 550
				{
					screensizeselection = 550;
				}
			}


			//Update Animations
			if (animationtime <= GetTickCount64())
			{
				animationtime = GetTickCount64() + nextanimation;
				//For Player
				//0 - Sprite 1
				//1 - Sprite 2
				//2 - Sprite 3
				//3 - Sprite 4
				int type = Player->returntype();
				if (wingsgoingup == true)
				{
					type++;
					if (type >= 4)
					{
						wingsgoingup = false;
						type = 2;
					}
				}
				else
				{
					type--;
					if (type <= -1)
					{
						wingsgoingup = true;
						type = 0;
					}
				}
				Player->settype(type);

				
			}

			if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN) && bouncetime <= GetTickCount64())
			{
				bouncetime = GetTickCount64() + 500;
				selectmenu = false;
			}
			if (KeyboardController::GetInstance()->IsKeyReleased('1') && bouncetime <= GetTickCount64()) //Clement
			{
				bouncetime = GetTickCount64() + 500;
				carselected = 0;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");	

				Data::GetInstance()->setCurrCar(Data::CarType::CAR_C); //Set Car Type

				SoundEngine->play2D("audio/sound_menu.wav", GL_FALSE); //Play Sound

			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('2') && bouncetime <= GetTickCount64()) //Glenda
			{
				bouncetime = GetTickCount64() + 500;
				carselected = 1;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");
				Data::GetInstance()->setCurrCar(Data::CarType::CAR_G); //Set Car Type


				SoundEngine->play2D("audio/sound_menu.wav", GL_FALSE); //Play Sound


			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('3') && bouncetime <= GetTickCount64()) //Waimen
			{
				bouncetime = GetTickCount64() + 500;
				carselected = 2;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//WaimenCar_FlappyCar1_Clement.tga");\
				Data::GetInstance()->setCurrCar(Data::CarType::CAR_WM); //Set Car Type

				SoundEngine->play2D("audio/sound_menu.wav", GL_FALSE); //Play Sound


			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('4') && bouncetime <= GetTickCount64()) //Val
			{
				bouncetime = GetTickCount64() + 500;
				carselected = 3;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ValCar_FlappyCar1_Clement.tga");
				Data::GetInstance()->setCurrCar(Data::CarType::CAR_V); //Set Car Type

				SoundEngine->play2D("audio/sound_menu.wav", GL_FALSE); //Play sound

			}
		}
		else //scoremenu
		{
			
			//Increase score screen size
			if (screensizescore < 500)
			{
				screensizescore += 10;
				if (screensizescore > 500) //Max screen size is 500
				{
					screensizescore = 500;
				}
			}
			//Update Colors for highscore
			if (gameupdate <= GetTickCount64())
			{
				gameupdate = GetTickCount64() + 250;
				Highscore1color++;
				if (Highscore1color >= 4)
				{
					Highscore1color = 0;
				}
			}
			//Player wants to return to main menu
			if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN) && bouncetime <= GetTickCount64())
			{
				bouncetime = GetTickCount64() + 500;
				scoremenu = false;
			}
		}
	}
}

void SceneMiniGame1::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Load Camera
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//Render Backgrounds
	BackgroundMid = BackgroundStart;
	while (BackgroundMid != nullptr)
	{
		RenderImageOnScreen(meshList[GEO_BACKGROUND], 800, 600, BackgroundMid->returnlocationx(), BackgroundMid->returnlocationy());
		BackgroundMid = BackgroundMid->getnextadress();
	}
	
	//Render Car based on which animation frame its on
	switch (Player->returntype())
	{
	case 0: //Sprite 1
	{
		RenderAnimationOnScreen(meshList[GEO_CAR], 6, 50, Player->returnlocationx() - 25, Player->returnlocationy()-25);
		break;
	}
	case 1: //Sprite 2
	{
		RenderAnimationOnScreen(meshList[GEO_CAR], 12, 50, Player->returnlocationx() - 25, Player->returnlocationy() - 25);
		break;
	}	
	case 2: //Sprite 3
	{
		RenderAnimationOnScreen(meshList[GEO_CAR], 20, 50, Player->returnlocationx() - 25, Player->returnlocationy() - 25);
		break;
	}	
	case 3: //Sprite 4
	{
		RenderAnimationOnScreen(meshList[GEO_CAR], 26, 50, Player->returnlocationx() - 25, Player->returnlocationy() - 25);
		break;
	}
	default:
	{
		RenderAnimationOnScreen(meshList[GEO_CAR], 26, 50, Player->returnlocationx() - 25, Player->returnlocationy() - 25);
		break;
	}
	}


	//Render Powerups base on which powerup it is
	if (Powerup != nullptr)
	{
		//0 - Slowdown
		//1 - Destroy Wall
		//2 - Gain Score
		//3 - Random
		switch (Powerup->returntype())
		{
		case 0: //Slowdown
		{
			RenderAnimationOnScreen(meshList[GEO_POWERUP], 6, 50, Powerup->returnlocationx() - 25, Powerup->returnlocationy() - 25);
			break;							 
		}									 
		case 1: //Destory Wall				 
		{									 
			RenderAnimationOnScreen(meshList[GEO_POWERUP], 12, 50, Powerup->returnlocationx() - 25, Powerup->returnlocationy() - 25);
			break;							
		}									
		case 2: //Gain Score				
		{									
			RenderAnimationOnScreen(meshList[GEO_POWERUP], 20, 50, Powerup->returnlocationx() - 25, Powerup->returnlocationy() - 25);
			break;							 
		}									 
		case 3: //Random					 
		{									 
			RenderAnimationOnScreen(meshList[GEO_POWERUP], 26, 50, Powerup->returnlocationx() - 25, Powerup->returnlocationy() - 25);
			break;							 
		}									 
		default:							 
		{									 
			RenderAnimationOnScreen(meshList[GEO_POWERUP], 26, 50, Powerup->returnlocationx() - 25, Powerup->returnlocationy() - 25);
			break;
		}
		}
		
	}
	//Render All Walls
	WallMid = WallStart;
	while (WallMid != nullptr)
	{
		bool Topactive = true;
		bool Bottomactive = true;
		//Check if at top
		if (WallMid->returnlocationy() > 590) //At top
		{
			Topactive = false;
			//Render Bottom Half
			float botheight = ((WallMid->returnlocationy()) / 2) - gapsize;
			RenderImageOnScreen(meshList[GEO_WALL], 50, WallMid->returnlocationy(), WallMid->returnlocationx(), botheight);
		}
		//Check if at bottom
		if (WallMid->returnlocationy() < 10) //At Bottom
		{
			Bottomactive = false;
			//Render Top Half
			float topheight = ((600 - WallMid->returnlocationy()) / 2) + WallMid->returnlocationy() + gapsize;
			RenderImageOnScreen(meshList[GEO_WALL], 50, ((600 - WallMid->returnlocationy())), WallMid->returnlocationx(), topheight);
		}

		if (Topactive == true && Bottomactive == true)
		{
			float topheight = ((600 - WallMid->returnlocationy()) / 2) + WallMid->returnlocationy() + (gapsize / 2)/*Gap Size*/;
			RenderImageOnScreen(meshList[GEO_WALL], 50, ((600 - WallMid->returnlocationy())), WallMid->returnlocationx(), topheight);

			float botheight = ((WallMid->returnlocationy()) / 2) - (gapsize / 2)/*Gap Size*/;
			RenderImageOnScreen(meshList[GEO_WALL], 50, WallMid->returnlocationy(), WallMid->returnlocationx(), botheight);
		}

		//RenderImageOnScreen(meshList[GEO_WALLTEST], 50, 50, WallMid->returnlocationx(), WallMid->returnlocationy());
		WallMid = WallMid->getnextadress();
	}

	std::string text;
	if (lost == true) //If playe has lost
	{
		//Render Lost Screen
		RenderImageOnScreen(meshList[GEO_LOST], 350, 350, 400, 300);

		//Render Score
		text = std::to_string(score);
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), 55, 425, 245);


	}
	else if (playing == false) //Main Menu
	{
		//Main Menu
		if (scoremenu == false && controlmenu == false && selectmenu == false)
		{
			//Render Main Menu Screen
			RenderImageOnScreen(meshList[GEO_MAIN], 600, 600, 400, 300);
			
			//Set cursor position
			//0 - Start
			//1 - Highscores
			//2 - Controls
			//3 - Car Selection
			//4 - Back to main
			int y = 400;
			switch (cursor)
			{
			case 4: //120
				y -= 70;

			case 3: //190
				y -= 70;

			case 2: //260
				y -= 70;

			case 1: //330
				y -= 70;

			case 0: //400
				break;
			}
			RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 50, 220, y); //Render Cursor

		}
		else if (scoremenu == true) //Score menu
		{

			//Render Scoreboard
			RenderImageOnScreen(meshList[GEO_SCORE], screensizescore, screensizescore, 400, 300);
			//Render Scores
			if (screensizescore >= 500)
			{
				//Render 1st Score based on color appearing
				switch (Highscore1color)
				{
				case 0:
					RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[0]), Color(1, 0, 0), 65, 245, 405);
					break;
				case 1:
					RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[0]), Color(0, 1, 0), 65, 245, 405);
					break;
				case 2:
					RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[0]), Color(0, 0, 1), 65, 245, 405);
					break;
				case 3:
					RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[0]), Color(1, 0.843137, 0), 65, 245, 405);
					break;
				}

				//Render 2nd score
				RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[1]), Color(0.75294, 0.75294, 0.75294), 65, 245, 347);

				//Render 3rd score
				RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[2]), Color(0.80392, 0.49803, 0.19607), 65, 245, 289);

				//Render 4th score
				RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[3]), Color(0.31764, 1, 0), 65, 245, 231);

				//Render 5th score
				RenderTextOnScreen(meshList[GEO_TEXT], to_string(Highscores[3]), Color(0.31764, 1, 0), 65, 245, 170);
			}
		}
		else if (selectmenu == true) //Car Selection Menu
		{
			//Render Car Selection Menu
			RenderImageOnScreen(meshList[GEO_SELECTION], screensizeselection, screensizeselection, 400, 300);
			//If screen size is big enough
			if (screensizeselection >= 550)
			{
				int count = 6;
				switch (Player->returntype())
				{
				case 0:
					count = 6;
					break;
				case 1:
					count = 12;
					break;
				case 2:
					count = 20;
					break;
				case 3:
					count = 30;
					break;
				}
				//Clement Car 1)
				
				RenderAnimationOnScreen(meshList[GEO_CLEMENTCAR], count, 150, 225, 315);

				//Glenda Car 2)
				
				RenderAnimationOnScreen(meshList[GEO_GLENDACAR], count, 150, 475, 315);
				
				//Waimen Car 3)
				
				RenderAnimationOnScreen(meshList[GEO_WAIMENCAR], count, 150, 225, 130);
				
				//Val Car 4)
				
				RenderAnimationOnScreen(meshList[GEO_VALCAR], count, 150, 475, 130);

				switch (carselected) //Render indicator based on which car is selected
				{
				case 0:
					
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 180, 430);
					break;
				case 1:
				
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 450, 430);
					break;
				case 2:

					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 180, 238);
					break;
				case 3:

					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 450, 238);
					break;
				}
			}
		}
		else //controlmenu
		{
			//Render controls
			RenderImageOnScreen(meshList[GEO_CONTROLS], screensizecontrol, screensizecontrol, 400, 300);
		}
		//(For transition)
		if (scoremenu == false && controlmenu == false && selectmenu == false)
		{

			//Render Scoreboard
			RenderImageOnScreen(meshList[GEO_SCORE], screensizescore, screensizescore, 400, 300);

			//Render controls
			RenderImageOnScreen(meshList[GEO_CONTROLS], screensizecontrol, screensizecontrol, 400, 300);

			//Render Selection Menu
			RenderImageOnScreen(meshList[GEO_SELECTION], screensizeselection, screensizeselection, 400, 300);
		}
	}
	else //Playing
	{
		//Render current score at bottom left
		text = "Score: " + std::to_string(score);
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 0, 1), 35, 0, 0);

		//If wall destroyer power up collected
		if (walldestroy == true)
		{
			//Render wall destoryer text on bottom left above score
			text = "Wall Destroyer";
			RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 0, 1), 35, 0, 52.5);
		}
	}

}

void SceneMiniGame1::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	//Deleting All Pointers

	//Deleting Background pointers
	BackgroundMid = BackgroundStart;
	while (BackgroundMid != nullptr)
	{
		MiniGame1Obj* Store = BackgroundMid;
		BackgroundMid = BackgroundMid->getnextadress();
		delete Store;
	}	
	
	//Deleting Wall Pointers
	WallMid = WallStart;
	while (WallMid != nullptr)
	{
		MiniGame1Obj* Store = WallMid;
		WallMid = WallMid->getnextadress();
		delete Store;
	}

	//Deleteing Powerup pointers
	if (Powerup != nullptr)
	{
		delete Powerup;
	}
	//Deleting Player poointer
	delete Player;

	//Selecting sound pointer
	delete SoundEngine;
}

void SceneMiniGame1::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);


	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID > 0){ 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);} 
	else { 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	} 
	mesh->Render(); //this line should only be called once in the whole function

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}



void SceneMiniGame1::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);

}

void SceneMiniGame1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.7f, 0, 0); //0.7f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SceneMiniGame1::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;
	//printf("Current Frames Per Second: %d\n\n", fps);
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}

void SceneMiniGame1::RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y)
{

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack

	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();


	glEnable(GL_DEPTH_TEST);

}

void SceneMiniGame1::RenderAnimationOnScreen(Mesh* mesh, int count, float size, float x, float y)
{

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	//0,6 1st
	//0,12 2nd
	//0,20 3nd
	//0,26 4th
	mesh->Render(0,count);
	//RenderMesh(mesh, false);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);



}

void SceneMiniGame1::Restart()
{
	//Deleting Pointers
	BackgroundMid = BackgroundStart;
	BackgroundStart = nullptr;
	while (BackgroundMid != nullptr)
	{
		MiniGame1Obj* Store = BackgroundMid->getnextadress();
		delete BackgroundMid;
		BackgroundMid = Store;
	}


	WallMid = WallStart;
	WallStart = nullptr;
	while (WallMid != nullptr)
	{
		MiniGame1Obj* Store = WallMid->getnextadress();
		delete WallMid;
		WallMid = Store;
	}
	if (Powerup != nullptr)
	{
		delete Powerup;
		Powerup = nullptr;
	}
	delete Player;

	BackgroundStart = new MiniGame1Obj(400, 300, 0, 0);

	playing = false;
	lost = false;
	gameupdate = GetTickCount64();
	nextupdate = 5000;

	Player = new MiniGame1Obj(400, 300, 0, 0);
	Player->settype(0);
	wingsgoingup = false;


	gamespeed = -0.4;
	score = 0;
	nextanimation = 150;
	gapsize = 175;

	walldestroy = false;

	cursor = 0;

	bouncetime = GetTickCount64();
	scoremenu = false;
	controlmenu = false;

	Highscore1color = 0;

	screensizescore = 0;
	screensizecontrol = 0; gameupdate = GetTickCount64();
	nextupdate = 5000;

	Player = new MiniGame1Obj(400, 300, 0, 0);
	Player->settype(0);
	wingsgoingup = false;


	gamespeed = -0.4;
	score = 0;
	nextanimation = 150;
	gapsize = 175;

	walldestroy = false;

	cursor = 0;

	bouncetime = GetTickCount64();
	scoremenu = false;
	controlmenu = false;
	selectmenu = false;

	Highscore1color = 0;

	screensizescore = 0;
	screensizecontrol = 0;
	screensizeselection = 0;

	soundbuffer = GetTickCount64();
}

