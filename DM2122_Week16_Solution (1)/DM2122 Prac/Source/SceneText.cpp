#include "SceneText.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include <cmath>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "../../Common/Source/KeyboardController.h"
#include "../../Common/Source/MouseController.h"
#include "MiniGame2.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

SceneText::SceneText()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneText::~SceneText()
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

void SceneText::Init()
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
	camera.Init(thePlayer->GetPos(), thePlayer->GetTarget(), thePlayer->GetUp());
	thePlayer->AttachCamera(&camera);
	thePlayer->SelectCar(Cars[CHOICE_WM]);

	// Init Car Stats
	Cars[CHOICE_WM] = new Car("WM", 60, 30, 40, 10);
	Cars[CHOICE_WM]->SetDirection(Vector3(0, 0, 1));
	cameraCar.Init((Cars[CHOICE_WM]->GetPos() - Cars[CHOICE_WM]->GetDirection()) * 10, Cars[CHOICE_WM]->GetPos(), Vector3(0, 1, 0));

	Cars[CHOICE_VL] = new Car("VL", 55, 35, 36, 10);
	Cars[CHOICE_VL]->SetDirection(Vector3(0, 0, 1));
	cameraCar.Init((Cars[CHOICE_VL]->GetPos() - Cars[CHOICE_VL]->GetDirection()) * 10, Cars[CHOICE_VL]->GetPos(), Vector3(0, 1, 0));

	Cars[CHOICE_GL] = new Car("GL", 50, 30, 15, 10);
	Cars[CHOICE_GL]->SetDirection(Vector3(0, 0, 1));
	cameraCar.Init((Cars[CHOICE_GL]->GetPos() - Cars[CHOICE_GL]->GetDirection()) * 10, Cars[CHOICE_GL]->GetPos(), Vector3(0, 1, 0));

	Cars[CHOICE_CM] = new Car("CM", 50, 40, 40, 10);
	Cars[CHOICE_CM]->SetDirection(Vector3(0, 0, 1));
	cameraCar.Init((Cars[CHOICE_CM]->GetPos() - Cars[CHOICE_CM]->GetDirection()) * 10, Cars[CHOICE_CM]->GetPos(), Vector3(0, 1, 0));
	


	{
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
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");

	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");
	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");

	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");
	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");

	m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
	m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
	m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
	m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
	m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");
	m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
	m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
	m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
	m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
	m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
	m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");

	m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");
	m_parameters[U_LIGHT7_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[7].spotDirection");
	m_parameters[U_LIGHT7_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[7].cosCutoff");
	m_parameters[U_LIGHT7_COSINNER] = glGetUniformLocation(m_programID, "lights[7].cosInner");
	m_parameters[U_LIGHT7_EXPONENT] = glGetUniformLocation(m_programID, "lights[7].exponent");
	m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
	m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
	m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
	m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
	m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
	m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");

	m_parameters[U_IS_CAR] = glGetUniformLocation(m_programID, "isCar");
	m_parameters[U_IS_CAR_WHITE] = glGetUniformLocation(m_programID, "isCarWhite");

	m_parameters[U_IS_SKYBOXSIDE] = glGetUniformLocation(m_programID, "isSkybox");
	m_parameters[U_SKYBOX_OFFSET] = glGetUniformLocation(m_programID, "skyboxOffset");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(-84.f, 20.f, -38.f);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 0;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(-84, 20.f, 35);;
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 0;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(105, 20.f, 30);
	light[2].color.Set(0.5f, 0.5f, 0.5f);
	light[2].power = 0;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	light[3].type = Light::LIGHT_SPOT;
	light[3].position.Set(95, 20.f, -39);
	light[3].color.Set(0.5f, 0.5f, 0.5f);
	light[3].power = 0;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);

	light[4].type = Light::LIGHT_POINT;
	light[4].position.Set(60, 10, -50);
	light[4].color.Set(0.5f, 0.5f, 0.5f);
	light[4].power = 10;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);

	light[5].type = Light::LIGHT_POINT;
	light[5].position.Set(-60, 10, -50);
	light[5].color.Set(0.5f, 0.5f, 0.5f);
	light[5].power = 10;
	light[5].kC = 1.f;
	light[5].kL = 0.01f;
	light[5].kQ = 0.001f;
	light[5].cosCutoff = cos(Math::DegreeToRadian(45));
	light[5].cosInner = cos(Math::DegreeToRadian(30));
	light[5].exponent = 3.f;
	light[5].spotDirection.Set(0.f, 1.f, 0.f);

	light[6].type = Light::LIGHT_POINT;
	light[6].position.Set(-60, 10, 50);
	light[6].color.Set(0.5f, 0.5f, 0.5f);
	light[6].power = 10;
	light[6].kC = 1.f;
	light[6].kL = 0.01f;
	light[6].kQ = 0.001f;
	light[6].cosCutoff = cos(Math::DegreeToRadian(45));
	light[6].cosInner = cos(Math::DegreeToRadian(30));
	light[6].exponent = 3.f;
	light[6].spotDirection.Set(0.f, 1.f, 0.f);

	light[7].type = Light::LIGHT_POINT;
	light[7].position.Set(60, 10, 50);
	light[7].color.Set(0.5f, 0.5f, 0.5f);
	light[7].power = 10;
	light[7].kC = 1.f;
	light[7].kL = 0.01f;
	light[7].kQ = 0.001f;
	light[7].cosCutoff = cos(Math::DegreeToRadian(45));
	light[7].cosInner = cos(Math::DegreeToRadian(30));
	light[7].exponent = 3.f;
	light[7].spotDirection.Set(0.f, 1.f, 0.f);

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

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &light[2].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &light[3].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &light[4].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &light[5].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

	glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
	glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
	glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
	glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
	glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
	glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &light[6].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], light[6].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT6_COSINNER], light[6].cosInner);
	glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);

	glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
	glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
	glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
	glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
	glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
	glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
	glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &light[7].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT7_COSCUTOFF], light[7].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT7_COSINNER], light[7].cosInner);
	glUniform1f(m_parameters[U_LIGHT7_EXPONENT], light[7].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 8); 
	}

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	//Interfaces
	meshList[GEO_INTERFACE_BASE] = MeshBuilder::GenerateQuad("UIBase", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_INTERFACE_BASE]->textureID = LoadTGA("Image//new_UIBase4.tga");

	meshList[GEO_STATS_WM] = MeshBuilder::GenerateQuad("UIBase1", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STATS_WM]->textureID = LoadTGA("Image//new_UIBase1.tga");
	meshList[GEO_STATS_V] = MeshBuilder::GenerateQuad("UIBase3", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STATS_V]->textureID = LoadTGA("Image//new_UIBase2.tga");
	meshList[GEO_STATS_G] = MeshBuilder::GenerateQuad("UIBase2", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STATS_G]->textureID = LoadTGA("Image//new_UIBase3.tga");
	meshList[GEO_STATS_C] = MeshBuilder::GenerateQuad("UIBase4", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STATS_C]->textureID = LoadTGA("Image//new_UIBase4.tga");

	meshList[GEO_INTERFACE_FLAPPY] = MeshBuilder::GenerateQuad("UIBase5", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_INTERFACE_FLAPPY]->textureID = LoadTGA("Image//new_UIBase5.tga");
	meshList[GEO_INTERFACE_SURFERS] = MeshBuilder::GenerateQuad("UIBase6", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_INTERFACE_SURFERS]->textureID = LoadTGA("Image//new_UIBase6.tga");

	meshList[GEO_WM_CAR] = MeshBuilder::GenerateOBJ("left", "OBJ//Car_WaiMen.obj");
	meshList[GEO_WM_CAR]->textureID = LoadTGA("Image//Car_WaiMen.tga");

	meshList[GEO_WM_CAR_WHEEL] = MeshBuilder::GenerateOBJ("val_car_wheel", "OBJ//Car_Wheel_WaiMen.obj");
	meshList[GEO_WM_CAR_WHEEL]->textureID = LoadTGA("Image//Car_WaiMen.tga");

	meshList[GEO_VAL_CAR] = MeshBuilder::GenerateOBJ("val_car", "OBJ//Car_Val.obj");
	meshList[GEO_VAL_CAR]->textureID = LoadTGA("Image//Car_Val.tga");

	meshList[GEO_VAL_CAR_WHEEL] = MeshBuilder::GenerateOBJ("val_car_wheel", "OBJ//Car_Wheel_Val.obj");
	meshList[GEO_VAL_CAR_WHEEL]->textureID = LoadTGA("Image//Car_Val.tga");

	meshList[GEO_G_CAR] = MeshBuilder::GenerateOBJ("glenda_car", "OBJ//Car_Glenda.obj");
	meshList[GEO_G_CAR]->textureID = LoadTGA("Image//Car_Glenda.tga");

	meshList[GEO_G_CAR_WHEEL] = MeshBuilder::GenerateOBJ("glenda_car_wheel", "OBJ//Car_Wheel_Glenda.obj");
	meshList[GEO_G_CAR_WHEEL]->textureID = LoadTGA("Image//Car_Glenda.tga");

	meshList[GEO_C_CAR] = MeshBuilder::GenerateOBJ("clement_car", "OBJ//Car_Clement.obj");
	meshList[GEO_C_CAR]->textureID = LoadTGA("Image//Car_Clement.tga");

	meshList[GEO_C_CAR_WHEEL] = MeshBuilder::GenerateOBJ("clement_car_wheel", "OBJ//Car_Wheel_Clement.obj");
	meshList[GEO_C_CAR_WHEEL]->textureID = LoadTGA("Image//Car_Clement.tga");

	//meshList[GEO_TESTDRIVE_ENVIRONMENT_OBJ] = MeshBuilder::GenerateOBJ("meteorite", "OBJ//met.obj");
	//meshList[GEO_TESTDRIVE_ENVIRONMENT_OBJ]->textureID = LoadTGA("Image//met.tga");

	meshList[GEO_SPACESTATION_FLOOR] = MeshBuilder::GenerateOBJ("floor", "OBJ//floor.obj");
	meshList[GEO_SPACESTATION_FLOOR]->textureID = LoadTGA("Image//floorUV.tga");

	meshList[GEO_SPACESTATION_WALL] = MeshBuilder::GenerateOBJ("wall", "OBJ//walls.obj");
	meshList[GEO_SPACESTATION_WALL]->textureID = LoadTGA("Image//wallUV.tga");

	meshList[GEO_RACETRACK] = MeshBuilder::GenerateOBJ("track", "OBJ//racetrack.obj");
	meshList[GEO_RACETRACK]->textureID = LoadTGA("Image//racetrackUV.tga");

	meshList[GEO_PAUSE] = MeshBuilder::GenerateQuad("pause",Color(1,1,1),4,4);
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//pause.tga");

	meshList[GEO_INDICATOR] = MeshBuilder::GenerateQuad("pause", Color(1, 1, 1), 4, 4);
	meshList[GEO_INDICATOR]->textureID = LoadTGA("Image//indicator.tga");

	meshList[GEO_HIGHSCOREBOARD] = MeshBuilder::GenerateQuad("highscore", Color(1, 1, 1), 4, 4);
	meshList[GEO_HIGHSCOREBOARD]->textureID = LoadTGA("Image//highscoreboard.tga");

	meshList[GEO_MINIGAME_BOOTH] = MeshBuilder::GenerateOBJ("booth1", "OBJ//minigamebase.obj");
	meshList[GEO_MINIGAME_BOOTH]->textureID = LoadTGA("Image//minigamebaseUV.tga");

	meshList[GEO_MINI_CAR_BODY] = MeshBuilder::GenerateOBJ("minicar", "OBJ//minibody.obj");
	meshList[GEO_MINI_CAR_BODY]->textureID = LoadTGA("Image//minibodyUV.tga");

	meshList[GEO_MINI_CAR_WHEELS] = MeshBuilder::GenerateOBJ("minicar", "OBJ//miniwheels.obj");
	meshList[GEO_MINI_CAR_WHEELS]->textureID = LoadTGA("Image//miniwheelsUV.tga");

	meshList[GEO_CARSURFER_ROCK] = MeshBuilder::GenerateOBJ("rock", "OBJ//rock.obj");
	meshList[GEO_CARSURFER_ROCK]->textureID = LoadTGA("Image//rockUV.tga");

	meshList[GEO_HOLOGRAM] = MeshBuilder::GenerateOBJ("booth_hologram", "OBJ//hologram.obj");
	meshList[GEO_HOLOGRAM]->textureID = LoadTGA("Image//hologramUV.tga");

	meshList[GEO_CARSURFER_ROAD] = MeshBuilder::GenerateOBJ("miniroad", "OBJ//miniroad.obj");
	meshList[GEO_CARSURFER_ROAD]->textureID = LoadTGA("Image//miniroadUV.tga");

	meshList[GEO_FLAPPYCAR_ENVIRONMENT] = MeshBuilder::GenerateOBJ("wall", "OBJ//miniwalls.obj");
	meshList[GEO_FLAPPYCAR_ENVIRONMENT]->textureID = LoadTGA("Image//miniwallUV.tga");

	meshList[GEO_SPEEDOMETER] = MeshBuilder::GenerateQuad("speedometer", Color(1, 1, 1), 4, 4);
	meshList[GEO_SPEEDOMETER]->textureID = LoadTGA("Image//speedometer.tga");

	meshList[GEO_SPEEDOMETER_NEEDLE] = MeshBuilder::GenerateQuad("speedometer_needle", Color(1, 1, 1), 4, 4);
	meshList[GEO_SPEEDOMETER_NEEDLE]->textureID = LoadTGA("Image//speedometer_needle.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");


	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	theKeyboard->Create(thePlayer);

	theMouse = new CMouse();
	theMouse->Create(thePlayer);
	SoundEngine->play2D("audio/dejavu.mp3", GL_TRUE); //Play sound for wall breaking
}

void SceneText::Update(double dt)
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

	//Checking Cars
	for (int i = 0; i < 4; i++)
	{
		if ((((thePlayer->GetPos().x - light[i].position.x) >= -80) && ((thePlayer->GetPos().x - light[i].position.x) <= 80)) && (((thePlayer->GetPos().z - light[i].position.z) >= -50) && ((thePlayer->GetPos().z - light[i].position.z) <= 50)))
		{
			light[i].power = 10;

			CarUI[i] = true;
			if (CarUIHeight[i] < 8)
				CarUIHeight[i] += 24 * dt;

			if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
			{
				//renderingState = STATE_TEST_DRIVE;
				thePlayer->SelectCar(Cars[i]);
				//thePlayer->Toggle_TestDrive();
				whichCar = i;

				switch (i)
				{
				case CHOICE_WM:
					Data::GetInstance()->setCurrCar(Data::CarType::CAR_WM);
					break;
				case CHOICE_VL:
					Data::GetInstance()->setCurrCar(Data::CarType::CAR_V);
					break;
				case CHOICE_GL:
					Data::GetInstance()->setCurrCar(Data::CarType::CAR_G);
					break;
				case CHOICE_CM:
					Data::GetInstance()->setCurrCar(Data::CarType::CAR_C);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			light[i].power = 0;
			if (CarUIHeight[i] > 0)
				CarUIHeight[i] -= 24 * dt;
			else
			{
				CarUI[i] = false;
				CarUIHeight[i] = 0;
			}
		}
	}

	//Check for flappy car booth UI and key input to sceene change
	if ((thePlayer->GetPos().x >= 100 - 80 && thePlayer->GetPos().x <= 100 + 80)
		&& (thePlayer->GetPos().z >= 115 - 80 && thePlayer->GetPos().x <= 115 + 80))
	{
		CarUI[4] = true;
		if (CarUIHeight[4] < 8)
			CarUIHeight[4] += 24 * dt;
		if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
			Data::GetInstance()->setCurrScene(Data::SceneType::MINIGAME1);

	}
	else
	{
		if (CarUIHeight[4] > 0)
			CarUIHeight[4] -= 24 * dt;

		else
		{
			CarUI[4] = false;
			CarUIHeight[4] = 0;
		}
	}

	//Car Surfers UI Check
	if ((thePlayer->GetPos().x <= -100 + 80 && thePlayer->GetPos().x >= -100 - 80)
		&& (thePlayer->GetPos().z >= 115 - 80 && thePlayer->GetPos().x <= 115 + 80))
	{
		CarUI[5] = true;
		if (CarUIHeight[5] < 8)
			CarUIHeight[5] += 24 * dt;
		if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
			Data::GetInstance()->setCurrScene(Data::SceneType::MINIGAME2);
	}
	else
	{
		if (CarUIHeight[5] > 0)
			CarUIHeight[5] -= 24 * dt;

		else
			CarUI[5] = false;
			CarUIHeight[5] = 0;
		}
	}

	// If the game is paused / in options screen
	if (thePlayer->GetPause())
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('E') || MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
		{
			if (pauseMenuSelection == CHOICE_RESUME)
			{
				thePlayer->Toggle_Pause();
			}
			else if (pauseMenuSelection == CHOICE_HIGHSCORE)
			{
				highScoreBoard = true;
			}
			else if (pauseMenuSelection == CHOICE_BACK)
			{
				highScoreBoard = false;
			}
			else if (pauseMenuSelection == CHOICE_RETURN_STATION)
			{
				renderingState = STATE_SELECTION_SCREEN;
				thePlayer->Toggle_TestDrive();
			}
			else if (pauseMenuSelection == CHOICE_EXIT)
			{
				// exit the program here
				Data::GetInstance()->EndApp();
			}
		}

		if (pauseMenuSelection == CHOICE_BACK)
		{
			moveIndicator += moveDirection * 0.3 * dt;
			if (moveIndicator > 0.13)
			{
				moveDirection = -1;
			}
			else if (moveIndicator < -0.05)
			{
				moveDirection = 1;
			}
		}

		if (pauseHeight < 1)
			pauseHeight += 5 * dt;
	}
	else
	{
		if (pauseHeight > 0)
			pauseHeight -= 5 * dt;
	}

	if (renderingState == STATE_TEST_DRIVE && isOnGround == true)
	{


		if (pow((pow((thePlayer->GetSelectedCar()->GetPos().x - 110), 2) + pow((thePlayer->GetSelectedCar()->GetPos().z - 38), 2)), 0.5) <= 109)
		{
			//cout << "mother" << endl;
			isOnGround = false;
		}

		if (pow((pow((thePlayer->GetSelectedCar()->GetPos().x - 110), 2) + pow((thePlayer->GetSelectedCar()->GetPos().z - 38), 2)), 0.5) >= 159)
		{
			isOnGround = false;
		}

	}
	else if (isOnGround == false)
	{
		thePlayer->GetSelectedCar()->Reset();

		if (hasCarFell == false)
		{
			counter++;
			turnCarWhite = true;
			hasCarFell = true;
		}



		isOnGround = true;
	}

	if (counter > 0 && counter < 20)
	{
		if (hasCarFell == true)
		{
			if (turnCarWhite == true)
			{
				turnCarWhite = false;
			}
			else
			{
				turnCarWhite = true;
			}
			counter++;
			//cout << counter << endl;
			if (counter == 20)
			{
				counter = 0;
				hasCarFell = false;
			}
		}

	}

	// Skybox Movement
	skyboxOffset += 0.00f * dt;
	if (skyboxOffset >= 1)
	{
		skyboxOffset - 1;
	}
	////detection for mini game booth 1
	// alr done by clement on other branch so can ignore this while merging thx
	//if (((((thePlayer->GetPos().x - (95)) >= -50) && ((thePlayer->GetPos().x - (95)) <= 50)) && (((thePlayer->GetPos().z - 110) >= -50) && ((thePlayer->GetPos().z - 110) <= 50))))
	//{
	//	if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
	//	{
	//		//cout << "hello" << endl;
	//		Data::GetInstance()->setCurrScene(Data::SceneType::MINIGAME1);
	//	}
	//}
	//detection for mini game booth 2
	//if (((((thePlayer->GetPos().x - (-99)) >= -50) && ((thePlayer->GetPos().x - (-99)) <= 50)) && (((thePlayer->GetPos().z - 115) >= -50) && ((thePlayer->GetPos().z - 115) <= 50))))
	//{
	//	if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
	//	{
	//		//cout << "hello" << endl;
	//		Data::GetInstance()->setCurrScene(Data::SceneType::MINIGAME2);
	//	}
	//	
	//}
	//detection for mini game booth 2

	if (((((thePlayer->GetPos().x - (42)) >= -80) && ((thePlayer->GetPos().x - (42)) <= 80)) && (((thePlayer->GetPos().z - (-120)) >= -50) && ((thePlayer->GetPos().z - (-120)) <= 50))))
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
		{
			//cout << "hello" << endl;
			Data::GetInstance()->setCurrScene(Data::SceneType::MAIN);
			renderingState = STATE_TEST_DRIVE;
			thePlayer->Toggle_TestDrive();
		}

	}

	//cout << Cars[whichCar]->Get << endl;
	/*while (Cars[whichCar]->GetVelocity().Length() > 0)
	{
		needleRotation = -Cars[whichCar]->GetVelocity().Length();

		if (needleRotation > 180)
		{
			needleRotation = 180;
		}
	}*/

	// Hardware Abstraction
	theKeyboard->Read(dt);
	theMouse->Read(dt);

	camera.Update(dt);
	thePlayer->Update();
	CalculateFrameRate();
}


void SceneText::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// Lights
	{
		if (light[0].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[0].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
		else
		{
			// default is point light (only position since point light is 360 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}

		if (light[1].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[1].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		}
		else
		{
			// default is point light (only position since point light is 361 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		}

		if (light[2].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[2].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		}
		else
		{
			// default is point light (only position since point light is 362 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		}

		if (light[3].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[3].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[3].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		}
		else
		{
			// default is point light (only position since point light is 363 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
			glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		}

		if (light[4].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[4].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[4].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
		}
		else
		{
			// default is point light (only position since point light is 364 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
			glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
		}

		if (light[5].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[5].position.x, light[5].position.y, light[5].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[5].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[5].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
		}
		else
		{
			// default is point light (only position since point light is 365 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
			glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
		}

		if (light[6].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[6].position.x, light[6].position.y, light[6].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[6].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[6].position;
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[6].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
		}
		else
		{
			// default is point light (only position since point light is 366 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[6].position;
			glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
		}

		if (light[7].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[7].position.x, light[7].position.y, light[7].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
		}
		// if it is spot light, pass in position and direction 
		else if (light[7].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[7].position;
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[7].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
		}
		else
		{
			// default is point light (only position since point light is 367 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[7].position;
			glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
			glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
		}

	}


	switch (renderingState)
	{
	case STATE_START_MENU:
		break;
	case STATE_SELECTION_SCREEN:
	{
		// Render Environment
		RenderSkybox();
		RenderSpaceStation();

		// Render Car Stat Interfaces
		RenderStatsUI();

		// Render Cars
		modelStack.PushMatrix();
		modelStack.Translate(-84, -15.6f, -38);
		modelStack.Scale(3, 3, 3);
		RenderWMCar();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-84, -27.6f, 35);
		modelStack.Rotate(90.f, 0, 1, 0);
		modelStack.Scale(35, 35, 35);
		RenderValCar();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-84, -27.6f, 35);
		modelStack.Rotate(90.f, 0, 1, 0);
		modelStack.Scale(35, 35, 35);
		RenderValCar();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(105, -27.6f, 30);
		modelStack.Rotate(180.f, 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		RenderGCar();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(95, -20.6f, -39);
		modelStack.Scale(37, 37, 37);
		RenderCCar();
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "[" + to_string(thePlayer->GetPos().x) + ", " + to_string(thePlayer->GetPos().y) + ", " + to_string(thePlayer->GetPos().z) + "]", Color(0, 1, 0), 2, 0, 2);
		RenderCarSurfersBooth();
		RenderFlappyCarBooth();
		break;
	}
	case STATE_TEST_DRIVE:

		cout << "hello" << endl;

		modelStack.PushMatrix();
		//modelStack.Scale(10, 10, 10);
		RenderSkybox();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(110, -7, 38);
		modelStack.Scale(18, 18, 18);
		RenderMesh(meshList[GEO_RACETRACK], false);
		modelStack.PopMatrix();

		//modelStack.PushMatrix();
		//modelStack.Translate(110, -7, 38);
		////modelStack.Scale(18, 18, 18);
		//RenderMesh(meshList[GEO_TESTDRIVE_ENVIRONMENT_OBJ], false);
		//modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Translate(thePlayer->GetSelectedCar()->GetPos().x, thePlayer->GetSelectedCar()->GetPos().y, thePlayer->GetSelectedCar()->GetPos().z); //TODO swap to selected car ptr
		//modelStack.Scale(Scaling, Scaling, Scaling);
		float angle = atan2f(thePlayer->GetSelectedCar()->GetDirection().x, thePlayer->GetSelectedCar()->GetDirection().z);
		angle = Math::RadianToDegree(angle);
		modelStack.Rotate(angle, 0, 1, 0);
		modelStack.Rotate(-90, 0, 1, 0);
		//modelStack.Scale(3, 3, 3);

		switch (Data::GetInstance()->getCurrCar())
		{
		case Data::CarType::CAR_WM:
			RenderWMCar();
			break;
		case Data::CarType::CAR_V:
			modelStack.PushMatrix();
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(12, 12, 12);
			RenderValCar();
			modelStack.PopMatrix();
			break;
		case Data::CarType::CAR_G:
			modelStack.PushMatrix();
			modelStack.Scale(3.5, 3.5, 3.5);
			RenderGCar();
			modelStack.PopMatrix();
			break;
		case Data::CarType::CAR_C:
			modelStack.PushMatrix();
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(12, 12, 12);
			RenderCCar();
			modelStack.PopMatrix();
			break;
		case Data::CarType::CAR_NONE:
			cout << "Error: No Car Selected" << endl;
			break;
		default:
			cout << "Error: Default Case ST::Render()" << endl;
			break;
		}

		modelStack.PopMatrix();
		RenderSpeedometer();
		RenderTextOnScreen(meshList[GEO_TEXT], "[" + to_string(thePlayer->GetSelectedCar()->GetPos().x) + ", " + to_string(thePlayer->GetSelectedCar()->GetPos().y) + ", " + to_string(thePlayer->GetSelectedCar()->GetPos().z) + "]", Color(0, 1, 0), 2, 0, 2);

		break;
	}

	if (thePlayer->GetPause() || pauseHeight > 0)
		if (highScoreBoard)
		{
			RenderHighscore();	//the board
			RenderHighscores();	//the text
		}
		else if (renderingState == STATE_SELECTION_SCREEN)
			RenderPause();
		else if (renderingState == STATE_TEST_DRIVE)
			RenderTestDrivePause();
	else
		pauseHeight = 0;


	
	//No transform needed
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Re:Pink", Color(1, 0.7f, 0.8f), 3, 0, 0);

}

void SceneText::Exit()
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

void SceneText::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_IS_CAR], 0);
	glUniform1i(m_parameters[U_IS_SKYBOXSIDE], 0);
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

void SceneText::RenderSkybox()
{
	int scale = 1200;
	modelStack.PushMatrix();
	modelStack.Scale(scale, scale, scale);
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(-0.5f, 0.f, 0.f);
		modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
		RenderSkyboxWallMesh(meshList[GEO_LEFT]);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.5f, 0.f, 0.f);
		modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
		RenderSkyboxWallMesh(meshList[GEO_RIGHT]);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.f, 0.5f, 0.f);
		modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
		modelStack.PushMatrix();
			modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
			RenderMesh(meshList[GEO_TOP], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.f, -0.5f, 0.f);
		modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
		modelStack.PushMatrix();
		modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
		RenderMesh(meshList[GEO_BOTTOM], false);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.f, 0.f, -0.5f);
		RenderSkyboxWallMesh(meshList[GEO_FRONT]);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.f, 0.f, 0.5f);
		modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
		RenderSkyboxWallMesh(meshList[GEO_BACK]);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneText::RenderSpaceStation()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(14, 8, 8);
	modelStack.PushMatrix();
	//modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_SPACESTATION_FLOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_SPACESTATION_WALL], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneText::RenderWMCar()
{
	modelStack.PushMatrix();
	
	
	RenderCarMesh(meshList[GEO_WM_CAR]);

	modelStack.PushMatrix();
	modelStack.Translate(6.45f, -2.5f, 4.8f);
	RenderCarMesh(meshList[GEO_WM_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(6.45f, -2.5f, -4.8f);
	RenderCarMesh(meshList[GEO_WM_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Scale(5, 5, 5);
	modelStack.Translate(-9.2f, -2.5f, 4.8f);
	RenderCarMesh(meshList[GEO_WM_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-9.2f, -2.5f, -4.8f);
	RenderCarMesh(meshList[GEO_WM_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderStatsUI()
{
	Vector3 UIPos[6];
	UIPos[0] = Vector3(-35, -1, -35);
	UIPos[1] = Vector3(-35, -1, 35);
	UIPos[2] = Vector3(50, -1, 35);
	UIPos[3] = Vector3(50, -1, -35);
	UIPos[4] = Vector3(75, -1, 115);
	UIPos[5] = Vector3(-70, -1, 115);
	for (int i = 0; i < 6; ++i)
	{
		if (CarUIHeight[i] > 0)
		{
			Vector3 Dir = (thePlayer->GetPos() - UIPos[i]).Normalized();
			float angle = atan2f(Dir.x, Dir.z);
			angle = Math::RadianToDegree(angle);
			glDisable(GL_CULL_FACE);
			modelStack.PushMatrix();
			modelStack.Translate(UIPos[i].x, -5, UIPos[i].z);
			modelStack.Rotate(angle, 0, 1, 0); //side to side
			modelStack.Rotate(-20, 1, 0, 0); //side to side
			modelStack.Scale(8, CarUIHeight[i], 8);

			switch (i)
			{
			case 0:
				RenderMesh(meshList[GEO_STATS_WM], false);
				//carSelection = CHOICE_WM;
				break;
			case 1:
				RenderMesh(meshList[GEO_STATS_V], false);
				//carSelection = CHOICE_VL;
				break;
			case 2:
				RenderMesh(meshList[GEO_STATS_G], false);
				//carSelection = CHOICE_GL;
				break;
			case 3:
				RenderMesh(meshList[GEO_STATS_C], false);
				//carSelection = CHOICE_CM;
				break;
			case 4:
				RenderMesh(meshList[GEO_INTERFACE_FLAPPY], false);
				break;
			case 5:
				RenderMesh(meshList[GEO_INTERFACE_SURFERS], false);
				break;
			default:
				RenderMesh(meshList[GEO_STATS_WM], false);
				cout << "out of scope: StatsUI" << endl;
				break;
			}
			modelStack.PopMatrix();
			glEnable(GL_CULL_FACE);
		}
	}
}

void SceneText::RenderValCar()
{
	modelStack.PushMatrix();
	
	
	RenderCarMesh(meshList[GEO_VAL_CAR]);

	modelStack.PushMatrix();
	//modelStack.Translate(0,0,0);
	RenderCarMesh(meshList[GEO_VAL_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderGCar()
{
	modelStack.PushMatrix();
	RenderCarMesh(meshList[GEO_G_CAR]);

	modelStack.PushMatrix();
	//modelStack.Translate(0,0,0);
	RenderCarMesh(meshList[GEO_G_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderCCar()
{
	modelStack.PushMatrix();
	

	RenderCarMesh(meshList[GEO_C_CAR]);

	modelStack.PushMatrix();
	//modelStack.Translate(0,0,0);
	RenderCarMesh(meshList[GEO_C_CAR_WHEEL]);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderIndicator()
{
	modelStack.PushMatrix();
	modelStack.Scale(0.2f, 0.2f, 0.2f);
	RenderMesh(meshList[GEO_INDICATOR], false);
	modelStack.PopMatrix();
}

void SceneText::RenderHighscore()
{
	// Find direction opposite of Player target 
	Vector3 dir = (thePlayer->GetPos() - thePlayer->GetTarget()).Normalized();

	// Find angle from 0 to dir vector
	float angle = atan2f(dir.x, dir.z);
	angle = Math::RadianToDegree(angle);
	float angle2;
	if (angle > 0)
	{
		if ((angle < 45) || (angle > 135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	else
	{
		if ((angle > -45) || (angle < -135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	angle2 = Math::RadianToDegree(angle2);

	// Get postion units away from player in direction found prevoiusly
	Vector3 pos = thePlayer->GetPos() - 5 * dir;

	// Model Stack
	modelStack.PushMatrix();
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(angle, 0, 1, 0); //side to side
	if (angle > 135 || angle < -45)
	{
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(angle2, 1, 0, 0); //up to down
	}
	else
		modelStack.Rotate(-angle2, 1, 0, 0); //up to down

	modelStack.Rotate(10, 0, 1, 0); //up to down
	modelStack.Translate(-0.5f, 0, 0);
	modelStack.Scale(1, pauseHeight, 1);
	RenderMesh(meshList[GEO_HIGHSCOREBOARD], false);

	////Render the highscore text here
	//for (int i = 0; i < 3; i++)
	//{
	//	RenderText(meshList[GEO_TEXT], to_string(flappyCar[i]), Color(0.98f, 0.41f, 1.f));

	//}
	//for (int i = 0; i < 3; i++)
	//{
	//	RenderText(meshList[GEO_TEXT], to_string(carSurfer[i]), Color(0.98f, 0.41f, 1.f));
	//}

	modelStack.PushMatrix();

	float x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	//cout << y << endl;
	modelStack.Translate(-1.6f, 1.6f, 0.1f);
	if (y < 240)
	{
		pauseMenuSelection = CHOICE_BACK;
		modelStack.Translate(moveIndicator, 0, 0);
	}

	RenderIndicator();

	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderMiniDisplayCar()
{
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_MINI_CAR_BODY], false);
	
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_MINI_CAR_WHEELS], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderFlappyCarBooth()
{
	modelStack.PushMatrix();
	modelStack.Translate(100, -33, 115);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_MINIGAME_BOOTH], false);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOLOGRAM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CARSURFER_ROAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.9, 0.8, 0);
	modelStack.Rotate(20, 0, 0, -1);
	RenderMiniDisplayCar();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_FLAPPYCAR_ENVIRONMENT], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

}

void SceneText::RenderCarSurfersBooth()
{
	modelStack.PushMatrix();
	modelStack.Translate(-100, -33, 115);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_MINIGAME_BOOTH], false);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOLOGRAM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CARSURFER_ROAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.05, 0);
	RenderMiniDisplayCar();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.6, 1.45, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_CARSURFER_ROCK], false);
	modelStack.PopMatrix();


	modelStack.PopMatrix();
}

void SceneText::RenderSpeedometer()
{
	modelStack.PushMatrix();
	RenderImageOnScreen(meshList[GEO_SPEEDOMETER], 100, 100, 600, 90,0);
	
	modelStack.PushMatrix();
	/*modelStack.Rotate(90, 1, 1, 1);*/
	modelStack.Translate(0, 0, 0.1f);
	RenderImageOnScreen(meshList[GEO_SPEEDOMETER_NEEDLE], 100,100,600,90,needleRotation);	//change the 90 at the back to a variable for animation

	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneText::RenderCarMesh(Mesh* mesh)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	glUniform1i(m_parameters[U_IS_SKYBOXSIDE], 0);
	glUniform1i(m_parameters[U_IS_CAR], 1);
	glUniform1i(m_parameters[U_IS_CAR_WHITE], turnCarWhite);

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

void SceneText::RenderSkyboxWallMesh(Mesh* mesh)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_IS_CAR], 0);
	glUniform1i(m_parameters[U_IS_SKYBOXSIDE], 1);
	glUniform1f(m_parameters[U_SKYBOX_OFFSET], skyboxOffset);
	

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

void SceneText::RenderPause()
{
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//pause.tga");

	// Find direction opposite of Player target 
	Vector3 dir = (thePlayer->GetPos() - thePlayer->GetTarget()).Normalized();

	// Find angle from 0 to dir vector
	float angle = atan2f(dir.x, dir.z);
	angle = Math::RadianToDegree(angle);
	float angle2;
	if (angle > 0)
	{
		if ((angle < 45) || (angle > 135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	else
	{
		if ((angle > -45) || (angle < -135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	angle2 = Math::RadianToDegree(angle2);

	// Get postion units away from player in direction found prevoiusly
	Vector3 pos = thePlayer->GetPos() - 5 * dir;

	// Model Stack
	modelStack.PushMatrix();
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(angle, 0, 1, 0); //side to side
	if (angle > 135 || angle < -45)
	{
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(angle2, 1, 0, 0); //up to down
	}
	else
		modelStack.Rotate(-angle2, 1, 0, 0); //up to down

	modelStack.Rotate(10, 0, 1, 0); //up to down
	modelStack.Translate(-0.5f, 0, 0);
	modelStack.Scale(1, pauseHeight, 1);
	RenderMesh(meshList[GEO_PAUSE], false);

	modelStack.PushMatrix();
	float x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	cout << y << endl;
	modelStack.Translate(-1.5f, 0.5f, 0.1f);
	if (y < 240)
	{
		pauseMenuSelection = CHOICE_RESUME;
	}
	else if (y > 240)
	{
		modelStack.Translate(0.f, -0.5f, 0.f);
		pauseMenuSelection = CHOICE_HIGHSCORE;

		if (y > 320)
		{
			modelStack.Translate(0.f, -0.4f, 0.f);
			pauseMenuSelection = CHOICE_EXIT;

		}
	}
	RenderIndicator();
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderTestDrivePause()
{
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//pause_testdrive.tga");

	// Find direction opposite of Player target 
	Vector3 dir = (thePlayer->GetPos() - thePlayer->GetTarget()).Normalized();

	// Find angle from 0 to dir vector
	float angle = atan2f(dir.x, dir.z);
	angle = Math::RadianToDegree(angle);
	float angle2;
	if (angle > 0)
	{
		if ((angle < 45) || (angle > 135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	else
	{
		if ((angle > -45) || (angle < -135))
			angle2 = atan2f(dir.y, dir.z);
		else
			angle2 = atan2f(dir.y, dir.x);
	}
	angle2 = Math::RadianToDegree(angle2);

	// Get postion units away from player in direction found prevoiusly
	Vector3 pos = thePlayer->GetPos() - 5 * dir;

	// Model Stack
	modelStack.PushMatrix();
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(angle, 0, 1, 0); //side to side
	if (angle > 135 || angle < -45)
	{
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(angle2, 1, 0, 0); //up to down
	}
	else
		modelStack.Rotate(-angle2, 1, 0, 0); //up to down

	modelStack.Rotate(10, 0, 1, 0); //up to down
	modelStack.Translate(-0.5f, 0, 0);
	modelStack.Scale(1, pauseHeight, 1);
	RenderMesh(meshList[GEO_PAUSE], false);

	modelStack.PushMatrix();
	float x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	cout << y << endl;
	modelStack.Translate(-1.5f, 0.5f, 0.1f);
	if (y < 240)
	{
		pauseMenuSelection = CHOICE_RESUME;
	}
	else if (y > 240)
	{
		modelStack.Translate(0.f, -0.5f, 0.f);
		pauseMenuSelection = CHOICE_HIGHSCORE;
		if (y > 320)
		{
			modelStack.Translate(0.f, -0.4f, 0.f);
			pauseMenuSelection = CHOICE_RETURN_STATION;
			if (y > 370)
			{
				modelStack.Translate(0.f, -0.4f, 0.f);
				pauseMenuSelection = CHOICE_EXIT;
			}
		}

	}
	RenderIndicator();
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneText::RenderImageOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y, float rotation)
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
	modelStack.Rotate(rotation, 0, 0, 1);
	modelStack.Scale(sizex, sizey, 1);

	RenderMesh(mesh, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();


	glEnable(GL_DEPTH_TEST);


}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneText::CalculateFrameRate()
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

void SceneText::RenderHighscores()
{
	//read flappy car highscores here
	int x = 0;
	int store = 0;
	std::string line;
	std::ifstream Flappyfile("Highscore//MiniGame1Highscore.txt");
	if (Flappyfile.is_open())
	{
		while (getline(Flappyfile, line) && x >! 3)
		{
			flappyCar[x] = stoi(line); //Convert string to int
			x++;
		}
		Flappyfile.close();
	}

	//Print Score
	for (int i = 0; i < 3; i++)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], to_string(flappyCar[i]), Color(0.98f, 0.41f, 1.f), 8, 0.9, 4.3f + (-1.7 * i));
	}

	//read car surfers highscores here
	x = 0;
	store = 0;
	std::string lines;
	std::ifstream CarSurferfile("Highscore//minigame2.txt");
	if (CarSurferfile.is_open())
	{
		while (getline(CarSurferfile, lines) && x > !3)
		{
			carSurfer[x] = stoi(lines); //Convert string to int
			x++;
		}
		CarSurferfile.close();
	}

	//Print Score
	for (int q = 0; q < 3; q++)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], to_string(carSurfer[q]), Color(0.98f, 0.41f, 1.f), 8, 4.7, 4.1f + (-1.7 * q));
	}
}
