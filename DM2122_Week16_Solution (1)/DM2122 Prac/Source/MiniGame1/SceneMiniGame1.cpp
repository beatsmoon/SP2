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

	meshList[GEO_CAR] = MeshBuilder::GenerateText("FlappyCar", 2, 2);
	//meshList[GEO_CAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");	
	meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");	
	
	meshList[GEO_POWERUP] = MeshBuilder::GenerateText("Powerup", 2, 2);
	meshList[GEO_POWERUP]->textureID = LoadTGA("Image//PowerUp_FlappyCar1_Clement.tga");	
	
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
	

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga"); 

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

	cursor = 0;

	bouncetime = GetTickCount64();
	scoremenu = false;
	controlmenu = false;
	selectmenu = false;

	Highscore1color = 0;

	screensizescore = 0;
	screensizecontrol = 0;
	screensizeselection = 0;


	carselected = 0;

}

void SceneMiniGame1::Update(double dt)
{
	//if (Application::IsKeyPressed(0x31))
	//{
	//	glDisable(GL_CULL_FACE);
	//}
	////else if (Application::IsKeyPressed(0x32))
	//{
	//	glEnable(GL_CULL_FACE);
	//}
	//else if (Application::IsKeyPressed(0x33))
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//}
	//else if (Application::IsKeyPressed(0x34))
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//}
	
	CalculateFrameRate();
	
	//Active
	if (playing == true && lost == false)
	{
		//Spawn New Wall + Speed Up Game
		if (gameupdate <= GetTickCount64())
		{
			gameupdate = GetTickCount64() + nextupdate;
			if (gamespeed > -4.5)
			{
				gamespeed -= 0.5;
				if (gamespeed == -4.5)
				{
					gapsize = 150;
					nextupdate = 3500;
				}
			}

			WallMid = WallStart;
			if (WallStart != nullptr)
			{
				while (WallMid->getnextadress() != nullptr)
				{
					WallMid = WallMid->getnextadress();
				}
			}
			float ylocation = (rand() % 590) + 10; //Random num from 10 - 599
			MiniGame1Obj* newwall = new MiniGame1Obj(810, ylocation, gamespeed - 1, 0);
			if (WallStart == nullptr)
			{
				WallStart = newwall;
			}
			else
			{
				WallMid->setnextaddress(newwall);

			}
		}

		//Spawn Powerup
		if (nextpowerspawn <= GetTickCount64())
		{
			nextpowerspawn = GetTickCount() + (((rand() % 10) + 10) * 1000); //Randomsise Next Spawn Time (10-20s)
			Powerup = new MiniGame1Obj(810, 300, gamespeed - 1, 0);
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

		//Update Powerups
		if (Powerup != nullptr)
		{
			Powerup->movexybyvelocity();
			if (Powerup->returnlocationx() < -30)
			{
				delete Powerup;
				Powerup = nullptr;
			}
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
					delete Powerup;
					Powerup = nullptr;
				}
			}

		}

		//Destroy Wall button
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) && walldestroy == true)
		{
			WallMid = WallStart;
			MiniGame1Obj* Prev;
			Prev = WallStart;
			while (WallMid != nullptr)
			{
				if (WallMid->returnlocationx() > Player->returnlocationx())
				{
					if (WallMid == WallStart) //First Wall
					{
						WallStart = WallMid->getnextadress();
						delete WallMid;
						WallMid = nullptr;
					}
					else
					{
						Prev->setnextaddress(WallMid->getnextadress());
						delete WallMid;
						WallMid = nullptr;
					}
					walldestroy = false;
					break;
				}
				else
				{
					Prev = WallMid;
					WallMid = WallMid->getnextadress();
				}
				
			}
		}

		//Update Wall Position
		WallMid = WallStart;
		while (WallMid != nullptr)
		{
			WallMid->movexybyvelocity();

			//AABB collision check
			//Player x position check (front half or back half hit
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
			if (WallMid->getnextadress() == nullptr)
			{
				break;
			}
			WallMid = WallMid->getnextadress();
		}

		//Delete Wall
		if (WallStart != nullptr && WallStart->returnlocationx() <= -20)
		{
			MiniGame1Obj* store = WallStart;
			WallStart = WallStart->getnextadress();
			delete store;
			score += 100;
		}

		//Update Background
		BackgroundMid = BackgroundStart;
		while (true)
		{
			BackgroundMid->setvelx(gamespeed);
			BackgroundMid->movexybyvelocity();
			if (BackgroundMid->getnextadress() == nullptr)
			{
				break;
			}
			BackgroundMid = BackgroundMid->getnextadress();
		}
		
		if (BackgroundMid->returnlocationx() <= 400)
		{
			MiniGame1Obj* NewBackGround = new MiniGame1Obj(BackgroundMid->returnlocationx() + 798.5 ,300,BackgroundMid->returnvelocityx(),0);
			BackgroundMid->setnextaddress(NewBackGround);
			
		}
		if (BackgroundStart->returnlocationx() <= -400)
		{
			MiniGame1Obj* Store = BackgroundStart->getnextadress();
			delete BackgroundStart;
			BackgroundStart = Store;
		}
		
		//Player Movement
		Player->setvely(Player->returnvelocityy() + -0.15);
		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
		{
			Player->setvely(5.55);
		}
		Player->movexybyvelocity();
		if (Player->returnlocationy() < 25)
		{
			Player->sety(25);
		}
		else if (Player->returnlocationy() > 575)
		{
			Player->sety(575);
		}

		//Player just lost
		if (lost == true)
		{
			int x = 0;
			std::string line;
			std::ifstream scorefile("Highscore//MiniGame1Highscore.txt");
			if (scorefile.is_open())
			{
				while (getline(scorefile, line))
				{
					Highscores[x] = stoi(line); //Convert string to int
					x++;
				}
			scorefile.close();
			}

			for (int x = 0; x < 5; x++)
			{
				if (score >= Highscores[x])
				{
					int store = score;
					for (; x < 5; x++)
					{
						int store2 = Highscores[x];
						Highscores[x] = store;
						store = store2;
					}
				}
			}

			ofstream scorefiles("Highscore//MiniGame1Highscore.txt");
			if (scorefiles.is_open())
			{
				for (int x = 0; x < 5; x++)
				{
					scorefiles << (Highscores[x]) << "\n";
				}
				scorefiles.close();
			}
		}
	}
	//After losing
	else if (lost == true)
	{
		if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
		{
			Restart();
		}
	}
	else //Havnt started
	{
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
			if (screensizeselection > 0)
			{
				screensizeselection -= 20;
				if (screensizeselection < 0)
				{
					screensizeselection = 0;
				}
			}
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) && bouncetime <= GetTickCount64())
			{
				cursor--;
				if (cursor <= -1)
				{
					cursor = 4;
				}
				bouncetime = GetTickCount64() + 100;
			}
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) && bouncetime <= GetTickCount64())
			{
				cursor++;
				if (cursor >= 5)
				{
					cursor = 0;
				}
				bouncetime = GetTickCount64() + 100;
			}
			//Start Game
			if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN) && bouncetime <= GetTickCount64())
			{
				switch (cursor)
				{
					//Start Game
				case 0:
				{
					bouncetime = GetTickCount64() + 250;
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
						bouncetime = GetTickCount64() + 1000;
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
					}
					break;
				}

				//Controls
				case 2:
				{
					bouncetime = GetTickCount64() + 1000;
					if (scoremenu == false && selectmenu == false)
					{
						controlmenu = true;
					}

					break;
				}
				//Car Selection
				case 3:
				{
					bouncetime = GetTickCount64() + 1000;
					if (scoremenu == false && controlmenu == false)
					{
						selectmenu = true;
					}
					break;
				}
				case 4:
				{

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
				if (screensizecontrol > 500)
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
			if (screensizeselection < 500)
			{
				screensizeselection += 10;
				if (screensizeselection > 500)
				{
					screensizeselection = 500;
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
			if (KeyboardController::GetInstance()->IsKeyReleased('1')) //Clement
			{
				carselected = 0;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");	
			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('2')) //Glenda
			{
				carselected = 1;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");

			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('3')) //Waimen
			{
				carselected = 2;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//WaimenCar_FlappyCar1_Clement.tga");

			}			
			if (KeyboardController::GetInstance()->IsKeyReleased('4')) //Val
			{
				carselected = 3;
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ValCar_FlappyCar1_Clement.tga");
			}
		}
		else //scoremenu
		{
			
			//Increase score screen size
			if (screensizescore < 500)
			{
				screensizescore += 10;
				if (screensizescore > 500)
				{
					screensizescore = 500;
				}
			}

			if (gameupdate <= GetTickCount64())
			{
				gameupdate = GetTickCount64() + 250;
				Highscore1color++;
				if (Highscore1color >= 4)
				{
					Highscore1color = 0;
				}
			}
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

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	

	//Render Background
	BackgroundMid = BackgroundStart;
	while (BackgroundMid != nullptr)
	{
		RenderImageOnScreen(meshList[GEO_BACKGROUND], 800, 600, BackgroundMid->returnlocationx(), BackgroundMid->returnlocationy());
		BackgroundMid = BackgroundMid->getnextadress();
	}
	//Render Car
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
	//RenderTextOnScreen(meshList[GEO_CAR], "j", Color(1, 1, 1), 50, Player->returnlocationx() - 25, Player->returnlocationy() - 25);
	//RenderImageOnScreen(meshList[GEO_CAR], 50, 50, Player->returnlocationx()-25, Player->returnlocationy()-25);

	//Render Powerups
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
		//RenderImageOnScreen(meshList[GEO_POWERUP], 20, 20, Powerup->returnlocationx(), Powerup->returnlocationy());
	}
	//Render Wallks
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
	if (lost == true)
	{
		RenderImageOnScreen(meshList[GEO_LOST], 350, 350, 400, 300);

		text = std::to_string(score);
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 1, 0), 55, 425, 245);

	}
	else if (playing == false)
	{
		if (scoremenu == false && controlmenu == false && selectmenu == false)
		{
			RenderImageOnScreen(meshList[GEO_MAIN], 600, 600, 400, 300);
			//0 - Start
			//1 - Highscores
			//2 - Controls
			//3 - Back to main
			int y = 330;
			switch (cursor)
			{
			case 3: //120
				y -= 70;

			case 2: //190
				y -= 70;

			case 1: //260
				y -= 70;

			case 0: //330
				break;
			}
			RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 50, 220, y);

		}
		else if (scoremenu == true)
		{

			//Render Scoreboard
			RenderImageOnScreen(meshList[GEO_SCORE], screensizescore, screensizescore, 400, 300);
			//Render Scores
			if (screensizescore >= 500)
			{
				//Render 1st Score
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
		else if (selectmenu == true)
		{
			RenderImageOnScreen(meshList[GEO_SELECTION], screensizeselection, screensizeselection, 400, 300);
			if (screensizeselection >= 500)
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
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");
				RenderAnimationOnScreen(meshList[GEO_CAR], count, 150, 225, 315);

				//Glenda Car 2)
				meshList[GEO_CAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");
				RenderAnimationOnScreen(meshList[GEO_CAR], count, 150, 475, 315);
				
				//Waimen Car 3)
				meshList[GEO_CAR]->textureID = LoadTGA("Image//WaimenCar_FlappyCar1_Clement.tga");
				RenderAnimationOnScreen(meshList[GEO_CAR], count, 150, 225, 140);
				
				//Val Car 4)
				meshList[GEO_CAR]->textureID = LoadTGA("Image//ValCar_FlappyCar1_Clement.tga");
				RenderAnimationOnScreen(meshList[GEO_CAR], count, 150, 475, 140);

				switch (carselected)
				{
				case 0:
					meshList[GEO_CAR]->textureID = LoadTGA("Image//ClementCar_FlappyCar1_Clement.tga");
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 190, 410);
					break;
				case 1:
					meshList[GEO_CAR]->textureID = LoadTGA("Image//GlendaCar_FlappyCar1_Clement.tga");
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 440, 410);
					break;
				case 2:
					meshList[GEO_CAR]->textureID = LoadTGA("Image//WaimenCar_FlappyCar1_Clement.tga");
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 190, 236);
					break;
				case 3:
					meshList[GEO_CAR]->textureID = LoadTGA("Image//ValCar_FlappyCar1_Clement.tga");
					RenderImageOnScreen(meshList[GEO_INDICATOR], 50, 25, 440, 236);
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
	else
	{
		text = "Score: " + std::to_string(score);
		RenderTextOnScreen(meshList[GEO_TEXT], text, Color(0, 0, 1), 35, 0, 0);

		if (walldestroy == true)
		{
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

	//Deleting Pointers
	BackgroundMid = BackgroundStart;
	while (BackgroundMid != nullptr)
	{
		MiniGame1Obj* Store = BackgroundMid;
		BackgroundMid = BackgroundMid->getnextadress();
		delete Store;
	}	
	

	WallMid = WallStart;
	while (WallMid != nullptr)
	{
		MiniGame1Obj* Store = WallMid;
		WallMid = WallMid->getnextadress();
		delete Store;
	}

	if (Powerup != nullptr)
	{
		delete Powerup;
	}
	delete Player;
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


}