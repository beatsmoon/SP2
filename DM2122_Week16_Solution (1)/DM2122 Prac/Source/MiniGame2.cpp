#include "MiniGame2.h"
#include "SceneText.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

MiniGame2::MiniGame2()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

MiniGame2::~MiniGame2()
{
	if (theMouse)
	{
		delete theMouse;
		theMouse = NULL;
	}
	if (theKeyboard)
	{
		delete theKeyboard;
		theKeyboard = NULL;
	}
}

void MiniGame2::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	thePlayer = CPlayerInfo::GetInstance();
	thePlayer->Init();
	camera.Init(Vector3(0,0,0), Vector3(0,-90,0),Vector3(0,0.60f,-0.80f));
	thePlayer->AttachCamera(&camera);
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
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_IS_ROAD] = glGetUniformLocation(m_programID, "isRoad");
	m_parameters[U_ROAD_OFFSET] = glGetUniformLocation(m_programID, "roadOffset");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 5, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1); 



	meshList[GEO_TRACK] = MeshBuilder::GenerateQuad("track", Color(1.f, 1.f, 1.f), 1.f, 1.f);
	meshList[GEO_TRACK]->textureID = LoadTGA("image//MiniGame2Track.tga");

	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("rock","Obj//rock.obj");
	meshList[GEO_ROCK]->textureID = LoadTGA("image//rock.tga");

	meshList[GEO_WM_CAR] = MeshBuilder::GenerateOBJ("WMCar", "Obj//Car_WaiMen.obj");
	meshList[GEO_WM_CAR]->textureID = LoadTGA("image//Car_WaiMen.tga");

	meshList[GEO_VAL_CAR] = MeshBuilder::GenerateOBJ("ValCar", "Obj//Car_Val.obj");
	meshList[GEO_VAL_CAR]->textureID = LoadTGA("image//Car_Val.tga");

	meshList[GEO_G_CAR] = MeshBuilder::GenerateOBJ("GCar", "Obj//car_glenda.obj");
	meshList[GEO_G_CAR]->textureID = LoadTGA("image//Car_glenda.tga");

	meshList[GEO_C_CAR] = MeshBuilder::GenerateOBJ("CCar", "Obj//Car_Clement.obj");
	meshList[GEO_C_CAR]->textureID = LoadTGA("image//Car_Clement.tga");

	meshList[GEO_DEATHMENU] = MeshBuilder::GenerateQuad("DeathMenu", Color(1, 1, 1), 1, 1);
	meshList[GEO_DEATHMENU]->textureID = LoadTGA("image//deathmenu.tga");

	meshList[GEO_INSTRUCTIONS] = MeshBuilder::GenerateQuad("instructions", Color(1, 1, 1), 1, 1);
	meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("image//MiniGame2_instructions.tga");
	

	meshList[GEO_INDICATOR] = MeshBuilder::GenerateQuad("arrow", Color(1, 1, 1), 4, 4);
	meshList[GEO_INDICATOR]->textureID = LoadTGA("image//indicator.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("image//calibri.tga");


	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	theKeyboard->Create(thePlayer);

	theMouse = new CMouse();
	theMouse->Create(thePlayer);
	
	changeScore = false;

	SoundEngine->play2D("audio/MiniGame2bgm.mp3", GL_TRUE);

	//Car
	Position temp;
	temp.Set(0,0,0);
	//string name, float top_speed, float handling, float acceleration, float nitro, Position pos
	theCar = new Car("MiniGame2Car",100.f,10.f,10.f,0.f);
}

void MiniGame2::Update(double dt)
{
	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('Q'))
	{
		//to do: switch light type to POINT and pass the information to
		light[0].type = Light::LIGHT_POINT;
	}
	else if (Application::IsKeyPressed('W'))
	{
		//to do: switch light type to DIRECTIONAL and pass the
		light[0].type = Light::LIGHT_DIRECTIONAL;
	}
	else if (Application::IsKeyPressed('E'))
	{
		//to do: switch light type to SPOT and pass the information to
		light[0].type = Light::LIGHT_SPOT;
	}


	// Hardware Abstraction
	theKeyboard->Read(dt);
	theMouse->Read(dt);

	camera.Update(dt);
	thePlayer->Update();

	Rotate += 50 * (float)(dt);
	if (gameEnd == false && gameStart == true)
	{
		if (KeyboardController::GetInstance()->IsKeyDown('A'))
		{

			if (carX < 13)
			{
				carX += ((theCar->Get_acceleration()) / 5) * 8 * (float)(dt);
				if (RotateCar < 25)
				{
					RotateCar += ((theCar->Get_acceleration()) / 5) * 50 * (float)(dt);
				}
			}

		}
		if (KeyboardController::GetInstance()->IsKeyUp('A'))
		{
			if (RotateCar > 0)
			{
				RotateCar -= (theCar->Get_acceleration() / 5) * 80 * (float)(dt);
			}
		}

		if (KeyboardController::GetInstance()->IsKeyDown('D'))
		{

			if (carX > -13)
			{
				carX -= ((theCar->Get_acceleration()) / 5) * 8 * (float)(dt);
				if (RotateCar > -25)
				{
					RotateCar -= ((theCar->Get_acceleration()) / 5) * 50 * (float)(dt);
				}
			}

		}
		if (KeyboardController::GetInstance()->IsKeyUp('D'))
		{
			if (RotateCar < 0)
			{
				RotateCar += (theCar->Get_acceleration() / 5) * 80 * (float)(dt);
			}
		}
		LanesRandom();
		RockGoingDown(dt);
		NitroBoostCoolDown(dt);
		DistanceTravelled(dt);
	}
	if (gameStart == false)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			gameStart = true;
		}
	}

	if (gameEnd == false && gameStart == true)
	{
		if (NitroUsed)
		{
			offset_Y += 1.f * dt;
		}
		else
		{
			offset_Y += 0.5f * dt;
		}
		//Math::Wrap(offset_Y, 0.f, 1.f);
		if (offset_Y >= 1)
			offset_Y -= 1;



		CollisionUpdate(dt);
	}

	


	CalculateFrameRate();

}

void MiniGame2::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	//viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	viewStack.LookAt(0, 0, 0, 0, -90, 0, 0, 0.60f, 0.80f);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}


	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//RenderText(meshList[GEO_TEXT], "HELLO WORLD", Color(0, 1, 0));
	//modelStack.PopMatrix();
	//string pos = "[" + to_string(thePlayer->GetPos().x) + ", " + to_string(thePlayer->GetPos().y) + ", " + to_string(thePlayer->GetPos().z) + "]";
//	string pos = "[" + to_string(thePlayer->GetUp().x) + ", " + to_string(thePlayer->GetUp().y) + ", " + to_string(thePlayer->GetUp().z) + "]";


//No transform needed
	//RenderTextOnScreen(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0), 2, 0, 0);

	RenderTrack();
	//RenderRock(0,20);
	RenderCar();
	if (rock1End == false)
	{
		if (lanes == 0)
		{
			RenderRock(-17, StartZ);
		}

		if (lanes == 1)
		{
			RenderRock(0, StartZ);
		}

		if (lanes == 2)
		{
			RenderRock(17, StartZ);
		}
	}
	
	string nitro = to_string(theCar->Get_nitro());
	RenderTextOnScreen(meshList[GEO_TEXT], "Nitro : " + nitro, Color(0, 1, 0), 3, 0, 2);

	string dis = to_string(distance);
	RenderTextOnScreen(meshList[GEO_TEXT], "SCORE : " + dis, Color(0, 1, 0), 3, 15, 19);

	RenderInstructions();

	RenderDeathMenu();
}

void MiniGame2::Exit()
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

}

void MiniGame2::RenderMesh(Mesh* mesh, bool enableLight)
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
		glUniform1i(m_parameters[U_IS_ROAD], 0);

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

void MiniGame2::RenderRoadMesh(Mesh* mesh)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1f(m_parameters[U_ROAD_OFFSET], offset_Y);
	glUniform1i(m_parameters[U_IS_ROAD], 1);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void MiniGame2::RenderTrack()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(70, 70, 70);
	RenderRoadMesh(meshList[GEO_TRACK]);
	modelStack.PopMatrix();

}
void MiniGame2::RenderCar()
{  // WMCAR
	/*modelStack.PushMatrix();
	modelStack.Translate(carX, -40, -10);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(RotateCar, 0, 1, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_WM_CAR], false);
	modelStack.PopMatrix();*/

	//VAL CAR

	/*modelStack.PushMatrix();
	modelStack.Translate(carX, -40, -10);
	modelStack.Rotate(RotateCar, 0, 1, 0);
	modelStack.Scale(7.f, 7.f, 7.f);
	RenderMesh(meshList[GEO_VAL_CAR], false);
	modelStack.PopMatrix();*/
	
	//G CAR
	//modelStack.PushMatrix();
	//modelStack.Translate(-1 + carX, -40, -10);
	//modelStack.Rotate(-90, 0, 1, 0);
	//modelStack.Rotate(RotateCar, 0, 1, 0);
	//modelStack.Scale(2.f, 2.f, 2.f);
	//RenderMesh(meshList[GEO_G_CAR], false);
	//modelStack.PopMatrix();

	//C CAR
	modelStack.PushMatrix();
	modelStack.Translate(carX, -40, -10);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(RotateCar, 0, 1, 0);
	modelStack.Scale(10.f, 10.f, 10.f);
	RenderMesh(meshList[GEO_C_CAR], false);
	modelStack.PopMatrix();
}
void MiniGame2::RenderDeathMenu()
{
	if (gameEnd == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -30, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_DEATHMENU], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		float x,y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		cout << y << endl;
		modelStack.Translate(3.6f, -10.f, -3.2f);
		if (y > 240)
		{
			modelStack.Translate(0,0,0);
			
			if (y > 320)
			{
				modelStack.Translate(0.f, 0.f, -0.8f);
				if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
				{
					//reset();
				}
			}
			else if(MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
			{
				reset();
			}
		}
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(0.4f,0.4f, 0.4f);
		RenderMesh(meshList[GEO_INDICATOR], false);
		

		modelStack.PopMatrix();
		ReadHighScore_minigame2();
	}
}

void MiniGame2::RenderInstructions()
{
	if (gameStart == false)
	{

		modelStack.PushMatrix();
		modelStack.Translate(0, -30, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_INSTRUCTIONS], false);
		modelStack.PopMatrix();
	}
}


void MiniGame2::reset()
{
	NitroUsed = false;
	gameEnd = false;
	rock1End = true;
	distance = 0;
	carX = 0;
	StartZ = 30;
	Rotate = 0;
	RotateCar = 0;
	collision = false;
	hit = false;
	HighScore = false;
	theCar->Set_nitro(0);
	MouseController::GetInstance()->SetKeepMouseCentered(true);
}

void MiniGame2::RenderRock(float posX, float posZ)
{
	modelStack.PushMatrix();
	modelStack.Translate(posX, -50, posZ);
	modelStack.Rotate(-Rotate, 1, 0, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROCK], false);
	modelStack.PopMatrix();

}



void MiniGame2::NitroBoostCoolDown(double dt) //FOR NITROBOOST TO FILL UP AND FOR TIME USING IT
{
	if (NitroUsed == false)
	{
		if (theCar->Get_nitro() < 100)
		{
			theCar->Set_nitro(theCar->Get_nitro() + (5.f) * (float)(dt));
		}
		else
		{
			if (Application::IsKeyPressed(VK_SPACE))
			{
				NitroUsed = true;
			}
		}
	}

	if (NitroUsed == true)
	{
		if (theCar->Get_nitro() > 0)
		{
			theCar->Set_nitro(theCar->Get_nitro() - (10.f) * (float)(dt));
		}
		else
		{
			NitroUsed = false;
		}
	}

}

void MiniGame2::DistanceTravelled(double dt)
{
	if (gameEnd == false && gameStart == true)
	{
		if (NitroUsed == false)
		{
			distance += (theCar->Get_acceleration() * dt);
		}
		if (NitroUsed == true)
		{
			distance += ((3)*theCar->Get_acceleration() * dt);

		}
	}
}

void MiniGame2::LanesRandom()
{
	srand(time(NULL));
	if (rock1End == true)
	{
		lanes = (rand() % 3);
		rock1End = false;
	}
}

void MiniGame2::RockGoingDown(double dt)
{
	if (rock1End == false && hit == false)
	{
		if (StartZ > -30)
		{
			if (NitroUsed == false)
			{
				StartZ -= (theCar->Get_acceleration() / 100 * (getDistance() / 100)) + (10) * (float)(dt) * 2;
			}
			else if (NitroUsed == true)
			{
				StartZ -= ((theCar->Get_acceleration() / 100 * (getDistance() / 100)) + (10) * (float)(dt)) * 3;
			}
		}
		else
		{
			rock1End = true;
			StartZ = 30;
		}
	}
}



bool MiniGame2::collisionWithRock()
{
	//By Lanes 1st
	bool CollisionLane = false;
	if (lanes == 0) // CarX < 8 
	{
		if ((carX >= -15.5f) && carX <= -6.5f)
		{
			CollisionLane = true;
			//printf("XXX");
		}
	}
	if (lanes == 1) // X = 0
	{
		if ((carX <= 6.5f) && carX >= -5.5f)
		{
			CollisionLane = true;
			//printf("XXX");
		}
	}
	if (lanes == 2) 
	{
		if ((carX >= 6.5f ) && (carX <= 15.5f))
		{
			CollisionLane = true;
			//printf("XXX");
		}
	}
	//By Distance Axis Z
	bool CollisionDistance = false;
	if (StartZ >= -15 && StartZ <= -5)
	{
		CollisionDistance = true;
		//printf("YYY");
	}
	return CollisionLane && CollisionDistance;
}

void MiniGame2::CollisionUpdate(double dt)
{
	if ((collisionWithRock() == true && true))
	{
		if (NitroUsed == true)
		{
			hit = true;
			
		}

		else if (NitroUsed == false)
		{
			gameEnd = true;
			changeScore = true;
			MouseController::GetInstance()->SetKeepMouseCentered(false);
		}
	}

	if (hit == true)
	{
		if (StartZ < 30)
		{
			StartZ += (50) * (float)(dt) * 3;
		}
		else
		{
			hit = false;
			rock1End = true;

		}
	}
	
	if (gameEnd == true && gameStart == false)
	{
		StartZ = StartZ;
	}

}
void MiniGame2::ReadHighScore_minigame2()
{
	if (gameEnd == true)
	{
		
			int x = 0;
			int store = distance;
			std::string line;
			std::ifstream scorefile("Highscore//minigame2.txt");
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
				if (changeScore == true)
				{
					if (distance >= Highscores[x])
					{
						for (; x < 5; x++)
						{
							int store2 = Highscores[x];
							Highscores[x] = store;
							store = store2;
						}
						changeScore = false;
					}
				}
			}
		

		ofstream scorefiles("Highscore//minigame2.txt");
		if (scorefiles.is_open())
		{
			for (int x = 0; x < 5; x++)
			{
				scorefiles << (Highscores[x]) << "\n";
			}
			scorefiles.close();
		}

		//Print Score
		for (int i = 0; i < 5; i++)
		{
				RenderTextOnScreen(meshList[GEO_TEXT],to_string( Highscores[i]), Color(0.98f, 0.41f, 1.f), 5, 5, 8.8f + (-1.4 * i));
		}

	}
}




float MiniGame2::getDistance()
{
	return distance;
}

void MiniGame2::RenderText(Mesh* mesh, std::string text, Color color)
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

void MiniGame2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

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
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void MiniGame2::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	//printf("Current Frames Per Second: %d\n\n", fps);
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}

