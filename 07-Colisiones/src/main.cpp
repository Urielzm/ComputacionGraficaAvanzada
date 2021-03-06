#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>


//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;
bool dibujaJarron = true;
int op = 0;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
//Camara para primera persona
std::shared_ptr<FirstPersonCamera> camera2(new FirstPersonCamera());

float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Box boxCollider2;
Sphere sphereCollider(10, 10);

// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
//Modelo del muro
Model modelMuro;
//Modloe del laberinto
Model modeloLaberinto;
//Model jarron
Model modeloJarron;
//Modelo de la puerta
Model modeloArco;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
//Antorcha
Model modelAntorcha;
// Model animate instance
// Mayow
Model mayowModelAnimate;
//Model animate nano souit
Model nanosuitModel;

// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// *******************************************Model matrix definitions************************************************
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixNanosuit = glm::mat4(1.0f);
//Matriz del muro
glm::mat4 modelMatrixMuro = glm::mat4(1.0f);
//Mtriz de la antorcha
glm::mat4 modelMatrixAntorcha = glm::mat4(1.0f);
//Matriz del laberinto
glm::mat4 modelMatrixLaberinto = glm::mat4(1.0f);
//Matriz del jarron
glm::mat4 modelMatrixJarron = glm::mat4(1.0f);
//Matriz del arco
glm::mat4 modelMatrixArco = glm::mat4(1.0f);


int animationIndex = 1;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 2;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Var animate helicopter
float rotHelHelY = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

//variables de escala del Muro
float escalaX = 10;
float escalaY = 12;
float escalaZ = 10;

//Muro posiciones
//Vector de vectores de posicion
//Viendolo de frente X: Positiva -->
//Viendolo de frente X: Negativa <--
std::vector<glm::vec3> muro1Position = {
	//Viendolo de frente

	//Posiciones frontal
	glm::vec3(80.4, 0.0, 50.0),
	glm::vec3(67.0, 0.0, 50.0),
	glm::vec3(53.6, 0.0, 50.0),
	glm::vec3(40.2, 0.0, 50.0),
	glm::vec3(26.8, 0.0, 50.0),
	glm::vec3(13.4, 0.0, 50.0),
	//glm::vec3(0.0, 0.0, 50.0),//Muro central --> Puerta
	glm::vec3(-13.4, 0.0, 50.0),
	glm::vec3(-26.8, 0.0, 50.0),
	glm::vec3(-40.2, 0.0, 50.0),
	glm::vec3(-53.6, 0.0, 50.0),
	glm::vec3(-67.0, 0.0, 50.0),
	glm::vec3(-80.4, 0.0, 50.0),

	//Posiciones IZQUIERDA
	glm::vec3(-84.8, 0.0, 43.0),
	glm::vec3(-84.8, 0.0, 29.6),
	glm::vec3(-84.8, 0.0, 16.2),
	glm::vec3(-84.8, 0.0, 2.8),
	glm::vec3(-84.8, 0.0, -10.6),
	glm::vec3(-84.8, 0.0, -24.0),
	glm::vec3(-84.8, 0.0, -37.4),
	glm::vec3(-84.8, 0.0, -50.8),
	glm::vec3(-84.8, 0.0, -64.2),
	glm::vec3(-84.8, 0.0, -77.6),
	glm::vec3(-84.8, 0.0, -91.0),

	//Posiciones traceras
	glm::vec3(-80.4, 0.0, -100.0),
	glm::vec3(-67.0, 0.0, -100.0),
	glm::vec3(-53.6, 0.0, -100.0),
	glm::vec3(-40.2, 0.0, -100.0),
	glm::vec3(-26.8, 0.0, -100.0),
	glm::vec3(-13.4, 0.0, -100.0),
	glm::vec3(0.0, 0.0, -100.0),//Muro central 
	glm::vec3(13.4, 0.0, -100.0),
	glm::vec3(26.8, 0.0, -100.0),
	glm::vec3(40.2, 0.0, -100.0),
	glm::vec3(53.6, 0.0, -100.0),
	glm::vec3(67.0, 0.0, -100.0),
	glm::vec3(80.4, 0.0, -100.0),
	
	//Posiciones DERECHA
	glm::vec3(84.1, 0.0, -91.0),
	glm::vec3(84.1, 0.0, -77.6),
	glm::vec3(84.1, 0.0, -64.2),
	glm::vec3(84.1, 0.0, -50.8),
	glm::vec3(84.1, 0.0, -37.4),
	glm::vec3(84.1, 0.0, -24.0),
	glm::vec3(84.1, 0.0, -10.6),
	glm::vec3(84.1, 0.0, 2.8),
	glm::vec3(84.1, 0.0, 16.2),
	glm::vec3(84.1, 0.0, 29.6),
	glm::vec3(84.1, 0.0, 43.0)
};

//Posiciones de laberinto
std::vector<glm::vec3> laberintoPosition = {

	glm::vec3(-48,0,40 ),
	glm::vec3(- 32,0,40),
	glm::vec3(- 16,0,40),
	glm::vec3(0,0,	40),
	glm::vec3(16,0,	40),
	glm::vec3(32,0,	40),
	glm::vec3(48,0,	40),
	glm::vec3(64,0,	40),
	glm::vec3(0	,0,32),
	glm::vec3(0	,0,16),
	glm::vec3(0	,0,0),
	glm::vec3(0,0, - 16),
	glm::vec3(- 8,0,	3.125),
	glm::vec3(- 24,0,	3.125),
	glm::vec3(58.984375,0,32),
	glm::vec3(58.984375,0,16),
	glm::vec3(50.984375,0,7.8125),
	glm::vec3(75.781250,0,	31.84570313),
	glm::vec3(75.781250,0,	15.84375),
	glm::vec3(75.781250,0, -0.15625),
	glm::vec3(75.781250,0, -16.1562),
	glm::vec3(67.781250,0, -10.9375),
	glm::vec3(51.781250,0, -10.9375),
	glm::vec3(35.781250,0, -10.9375),
	glm::vec3(19.781250,0, -10.9375),
	glm::vec3(11.72656250,0, -2.937),
	glm::vec3(11.72656250,0, 13.0625),
	glm::vec3(19.72656250,0, 21.0625),
	glm::vec3(35.72656250,0, 21.0625),
	glm::vec3(11.72656250,0, -18.9375 ),
	glm::vec3(11.72656250,0, -34.9375 ),
	glm::vec3(11.72656250,0, -50.9375 ),
	glm::vec3(3.7265625,0,- 58.9375 ),
	glm::vec3(- 12.078125,0, -58.9375),
	glm::vec3(- 0.3906250,0, -66.9375),
	glm::vec3(- 0.3906250,0, -82.9375),
	glm::vec3(- 8.3906250,0, -90.9375),
	glm::vec3(- 11.1171875,0, -90.9375	),
	glm::vec3(- 480,0,	17.1875	),
	glm::vec3(- 32,0,	17.1875	),
	glm::vec3(- 43.359375, 0, 9.1875 ),
	glm::vec3(- 43.359375,0, -6.8125 ),
	glm::vec3(- 53.7109375,0, -14.8125),
	glm::vec3(- 69.7109375,0, -14.8125),
	glm::vec3(- 37.7109375,0, -14.8125),
	glm::vec3(- 21.7109375,0, -14.8125),
	glm::vec3(- 53.7109375,0, -24.578125),
	glm::vec3(- 69.7109375,0, -24.578125),
	glm::vec3(- 37.7109375,0, -24.578125),
	glm::vec3(- 21.7109375,0, -24.578125),
	glm::vec3(- 60.15625,0, -32.578125	),
	glm::vec3(- 68.15625,0, -40.578125	),
	glm::vec3(- 13.83203125,0, -32.578125),
	glm::vec3(- 5.8671875,0, - 40.578125),
	glm::vec3(- 13.8671875,0, -40.578125),
	glm::vec3(- 29.8671875,0, -40.578125),
	glm::vec3(- 37.8671875,0, -48.578125),
	glm::vec3(- 37.8671875,0, -64.578125),
	glm::vec3(- 37.8671875,0, -80.578125),
	glm::vec3(- 29.8671875,0, -70.1171875),
	glm::vec3(- 45.8671875,0, -55.078125),
	glm::vec3(- 61.8671875,0, -55.078125),
	glm::vec3(- 61.8671875,0, -63.078125),
	glm::vec3(- 61.8671875,0, -79.078125),
	glm::vec3(38.09570313 ,0,-25.78125),
	glm::vec3(54.19140625 ,0,-25.78125),
	glm::vec3(30.09570313 ,0,-33.78125),
	glm::vec3(30.09570313 ,0,-49.78125),
	glm::vec3(30.09570313 ,0,-65.78125),
	glm::vec3(62.1015625,0, -33.78125),
	glm::vec3(62.1015625,0, -49.78125),
	glm::vec3(62.1015625,0, -65.78125),
	glm::vec3(62.1015625,0, -81.78125),
	glm::vec3(70.1015625,0, -89.78125),
	glm::vec3(54.1015625,0, -89.78125),
	glm::vec3(38.1015625,0, -89.78125),
	glm::vec3(70.1015625,0, -56.640625),
	glm::vec3(22.0859375,0, -73.8203125),
	glm::vec3(14.0859375,0, -81.78125)

};

//Vector de orientacion de los muros del laberinto
std::vector<float> laberintoOrientation = {
	0, 0, 0, 0, 0, 0, 0, 0, 90, 90, 90, 90, 0, 0, 90, 90, 0, 90, 90, 90, 90, 0, 0, 0, 0, 90, 90, 0, 0,
	90, 90, 90, 0, 0, 90, 90, 0, 90, 0, 0, 90, 90, 0, 0, 0, 0, 0, 0, 0, 0, 90, 0, 90, 0, 0, 0, 90, 90,
	90, 0, 0, 0, 90, 90, 0, 0, 90, 90, 90, 90, 90, 90, 90, 0, 0, 0, 0, 0, 90
};


std::vector<glm::vec3> lamp1Position = {

	//Posiciones de la lampara 1
	glm::vec3(-7.03, 0, -19.14), 
	glm::vec3(24.41, 0, -34.57), 
	glm::vec3(-10.15, 0, -54.10),

	//Posicions de las antorchas
	//total 34 antorchas
	//Viendolo de frente

	//Antorchas frontales
	glm::vec3(80.4, 0.0, 52.0),
	glm::vec3(67.0, 0.0, 52.0),
	glm::vec3(53.6, 0.0, 52.0),
	glm::vec3(40.2, 0.0, 52.0),
	glm::vec3(26.8, 0.0, 52.0),
	glm::vec3(13.4, 0.0, 52.0),
	//glm::vec3(0.0, 0.0, 50.0),//Muro central --> Puerta
	glm::vec3(-13.4, 0.0, 52.0),
	glm::vec3(-26.8, 0.0, 52.0),
	glm::vec3(-40.2, 0.0, 52.0),
	glm::vec3(-53.6, 0.0, 52.0),
	glm::vec3(-67.0, 0.0, 52.0),
	glm::vec3(-80.4, 0.0, 52.0),

	//Antorchas IZQUIERDA
	glm::vec3(-86.8, 0.0, 43.0),
	glm::vec3(-86.8, 0.0, 29.6),
	glm::vec3(-86.8, 0.0, 16.2),
	glm::vec3(-86.8, 0.0, 2.8),
	glm::vec3(-86.8, 0.0, -10.6),
	glm::vec3(-86.8, 0.0, -24.0),
	glm::vec3(-86.8, 0.0, -37.4),
	glm::vec3(-86.8, 0.0, -50.8),
	glm::vec3(-86.8, 0.0, -64.2),
	glm::vec3(-86.8, 0.0, -77.6),
	glm::vec3(-86.8, 0.0, -91.0),

	/*Posiciones traceras
	glm::vec3(-80.4, 0.0, -100.0),
	glm::vec3(-67.0, 0.0, -100.0),
	glm::vec3(-53.6, 0.0, -100.0),
	glm::vec3(-40.2, 0.0, -100.0),
	glm::vec3(-26.8, 0.0, -100.0),
	glm::vec3(-13.4, 0.0, -100.0),
	glm::vec3(0.0, 0.0, -100.0),//Muro central 
	glm::vec3(13.4, 0.0, -100.0),
	glm::vec3(26.8, 0.0, -100.0),
	glm::vec3(40.2, 0.0, -100.0),
	glm::vec3(53.6, 0.0, -100.0),
	glm::vec3(67.0, 0.0, -100.0),
	glm::vec3(80.4, 0.0, -100.0),*/

	//Antorchas DERECHA
	glm::vec3(86.1, 0.0, -91.0),
	glm::vec3(86.1, 0.0, -77.6),
	glm::vec3(86.1, 0.0, -64.2),
	glm::vec3(86.1, 0.0, -50.8),
	glm::vec3(86.1, 0.0, -37.4),
	glm::vec3(86.1, 0.0, -24.0),
	glm::vec3(86.1, 0.0, -10.6),
	glm::vec3(86.1, 0.0, 2.8),
	glm::vec3(86.1, 0.0, 16.2),
	glm::vec3(86.1, 0.0, 29.6),
	glm::vec3(86.1, 0.0, 43.0)
};


//Vector de orientación de Muros
std::vector<float> muro1Orientation = {
	//Viendolo de frente
	//Orientaciones frontales:
	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado IZQUIERDO
	-90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0,
	//Orientacionestraceras
	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado Derecho
	90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90
};

//Vector de orientacion de las lamparas
std::vector<float> lamp1Orientation = {
	//Orientación de las lamparas tipo1
	-17.0, -82.67, 23.70,
	//Vector de orientación de antorchas
	//Viendolo de frente
	//Orientaciones frontales:
	0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado IZQUIERDO
	-90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0,
	//Orientacionestraceras
	//0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado Derecho
	90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90
}; 

// Lamps positions
// 5 lamparas
/*std::vector<glm::vec3> antorchasPosition = { glm::vec3(-7.03, 0, -19.14), glm::vec3(
		24.41, 0, -34.57), glm::vec3(-10.15, 0, -54.10), glm::vec3(13.4, 0.0, 52.0) };
std::vector<float> antorchasOrientation = { -17.0, -82.67, 23.70, 0.0 };*/

std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = {21.37 + 90, -65.0 + 90};

//Vectores de posicion y orientacion de jarrones 
std::vector<glm::vec3> jarronPosition = { 
		glm::vec3(0.0, 0.0, 56.0),
		glm::vec3(4.0, 0.0, 56.0),
		glm::vec3(8.0, 0.0, 56.0),
		glm::vec3(12.0, 0.0, 56.0),
		glm::vec3(16.0, 0.0, 56.0),
		glm::vec3(20.0, 0.0, 56.0)

};
//Vectores de orientacion de jarrones 
std::vector<float> jarronOrientation = { 0,0,0,0,0,0 };

//Vector de etiquetas de jarrones

//Vector de indicadores de jarrones
std::vector<int> jarronId = { 1, 1 };
bool jarronE[] = { true, true, true, true , true , true };

double deltaTime;
double currTime, lastTime;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;//colliders de cajas
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;//colliders de esferas
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB2;//colliders de cajas2

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	//Iniciando las colisiones de las cajas
	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//Iniciando las colisiones de las cajas 2
	boxCollider2.init();
	boxCollider2.setShader(&shader);
	boxCollider2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//Iniciando las colision de las esfera
	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//Modelo de la roca
	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	//Modelo del Aircraft
	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	//Shader del terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);

	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Modelo del muro
	modelMuro.loadModel("../models/Personaje_proyecto/Laberinto1/MuroYTorre/Muro/Muro_de_castillo2.obj");
	modelMuro.setShader(&shaderMulLighting);
	//Modelo del laberinto
	modeloLaberinto.loadModel("../models/Personaje_proyecto/Laberinto1/ParedConMusgo/Pared_con_musgo2.obj");
	modeloLaberinto.setShader(&shaderMulLighting);

	//Modelo de antocha
	modelAntorcha.loadModel("../models/Personaje_proyecto/Antorcha/Antorcha3.obj");
	modelAntorcha.setShader(&shaderMulLighting);

	//Modelo de Jarron
	modeloJarron.loadModel("../models/Personaje_proyecto/Jarron/Jarron.obj");
	modeloJarron.setShader(&shaderMulLighting);

	//Modelo del arco
	modeloArco.loadModel("../models/Personaje_proyecto/Arco/Arco3.obj");
	modeloArco.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	//modelLamp1.loadModel("../models/Personaje_proyecto/Antorcha/Antorcha.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);

	//nanosuit
	nanosuitModel.loadModel("../models/nanosuit/nanosuit.obj");
	nanosuitModel.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	camera->setPosition(glm::vec3(0.0, 0.0, 0.0));

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/cesped.jpg");
	//Texture textureCesped("../Textures/Suelo1.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCesped.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCesped.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWall.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWall.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWindow.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWindow.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureHighway.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureHighway.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureLandingPad("../Textures/landingPad.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureLandingPad.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureLandingPad.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureLandingPadID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureLandingPad.freeImage(bitmap);

	// Definiendo la textura a utilizar
	//Texture textureTerrainBackground("../Textures/grassy2.png");
	Texture textureTerrainBackground("../Textures/Suelo1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/mud.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/grassFlowers.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	//Texture textureTerrainB("../Textures/path.png");
	Texture textureTerrainB("../Textures/Sangre1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	//Texture textureTerrainBlendMap("../Textures/blendMap.png");
	Texture textureTerrainBlendMap("../Textures/p2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);
}

void destroy() {//**************************************************Destruye los oabjetos******************************************* 
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();

	// Basic objects Delete
	//*********************Destruyendo los Colliders
	skyboxSphere.destroy();
	boxCollider.destroy();
	boxCollider2.destroy();
	sphereCollider.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	nanosuitModel.destroy();

	//Destruyendo el modelo de la antorcha
	modelAntorcha.destroy();
	
	//destruyendo objeto del muro
	modelMuro.destroy();

	// Custom objects animate
	mayowModelAnimate.destroy();

	//Jarron
	modeloJarron.destroy();

	//Arco
	modeloArco.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	//Movimientos de las camaras
	

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	//offsetX = 0;
	//offsetY = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera2->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera2->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera2->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera2->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera2->mouseMoveCamera(offsetX, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;
	
	

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));	

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(2.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 1.1));
		animationIndex = 0;
	}else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -1.1));
		animationIndex = 0;
	}
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0,0,1.45));
		modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(1.0f), glm::vec3(0, 1, 0));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, -1.45));
	}
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, -1.45));
		modelMatrixLambo = glm::rotate(modelMatrixLambo, -glm::radians(1.0f), glm::vec3(0, 1, 0));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, 1.45));
	}
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, 1.45));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0, 0.0, 0.02));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, -1.45));
	}
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, -1.45));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.02, 0.0, -0.02));
		modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, 1.45));
	}

	
	//Para cambiar de camara
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		op = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		op = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	//Modelo del muro
	//Aquí Y no importa porque más adelante se definira con respecto al mapa de alturas
	//modelMatrixMuro = glm::translate(modelMatrixMuro, glm::vec3(10.0, 0.0, -12.0));

	//Modelo del laberinto
	//Aquí Y no importa porque más adelante se definira con respecto al mapa de alturas
	//modelMatrixLaberinto = glm::translate(modelMatrixLaberinto, glm::vec3(0.0, 0.0, 40.0));

	//Modelo de la antorcha
	//modelMatrixAntorcha = glm::translate(modelMatrixAntorcha, glm::vec3(13.4, 0.0, 52.0));
	//modelMatrixAntorcha = glm::translate(modelMatrixAntorcha, glm::vec3(13.4, 0.0, 55.4));

	//Modelo del jarron
	//modelMatrixJarron = glm::translate(modelMatrixJarron, glm::vec3(0.0, 0.0, 52.0));

	//Modelo del Arco
	modelMatrixArco = glm::translate(modelMatrixArco, glm::vec3(0.0, 0.0, 52.0));

	//Modelo del Dart
	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	//Modelo de Mayow
	//rotate  viendolo de frente -90:  <--
	//rotate  viendolo de frente -180:  ^
	//rotate  viendolo de frente 90:  -->
	//rotate  viendolo de frente 0:  v
	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 60.0));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));

	modelMatrixNanosuit = glm::translate(modelMatrixNanosuit, glm::vec3(13.0f, 0.05f, 10.0f));
	modelMatrixNanosuit = glm::rotate(modelMatrixNanosuit, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;
		std::map<std::string, bool> collisionDetection2;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		if(modelSelected == 1 || modelSelected==2){
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}
		else if(modelSelected==3){
			axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
			target = modelMatrixMayow[3];
		}
		else if (modelSelected == 4) {
			axis = glm::axis(glm::quat_cast(modelMatrixLambo));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixLambo));
			target = modelMatrixLambo[3];
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();

		//view = camera->getViewMatrix();
		if (op == 0) {
			view = camera2->getViewMatrix();
		}
		else {
			view = camera->getViewMatrix();
			
		}

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.32437, 0.226053, 1.79149, 1.0));
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.074);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.074);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size() );
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size() );
		
		for (int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			//matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 5.3585, 0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.084333, 3.51245, 1.27987));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for (int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}

		/*for (int i = 0; i < antorchasPosition.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, antorchasPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(antorchasOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 antorchaPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].position", glm::value_ptr(antorchaPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5  + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5  + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5  + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].position", glm::value_ptr(antorchaPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() +5 + i) + "].quadratic", 0.02);
		}*/

		

		/*******************************************
		 * Terrain Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
		shaderTerrain.setInt("backgroundTexture", 0);
		// Se activa la textura de tierra
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		// Se activa la textura de hierba
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		// Se activa la textura del camino
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		// Se activa la textura del blend map
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
		terrain.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelMatrixAircraft[3][1] = terrain.getHeightTerrain(modelMatrixAircraft[3][0], modelMatrixAircraft[3][2]) + 2.0;
		modelAircraft.render(modelMatrixAircraft);

		//Render del Arco
		glm::mat4 modelMatrixArcoBody = glm::mat4(modelMatrixArco);
		modelMatrixArcoBody[3][1] = terrain.getHeightTerrain(modelMatrixArcoBody[3][0], modelMatrixArcoBody[3][2])+4.0;
		modelMatrixArcoBody = glm::scale(modelMatrixArcoBody, glm::vec3(0.7, 0.7, 0.7));
		modeloArco.render(modelMatrixArcoBody);

		//Render del muro
		/*glm::mat4 modelMatrixMuroBody = glm::mat4(modelMatrixMuro);
		modelMatrixMuroBody[3][1] = terrain.getHeightTerrain(modelMatrixMuroBody[3][0], modelMatrixMuroBody[3][2]);
		modelMatrixMuroBody = glm::scale(modelMatrixMuroBody, glm::vec3(escalaX, escalaY, escalaZ));
		modelMuro.render(modelMatrixMuroBody);*/

		//Render del laberinto
		/*glm::mat4 modelMatrixLaberintoBody = glm::mat4(modelMatrixLaberinto);
		modelMatrixLaberintoBody[3][1] = terrain.getHeightTerrain(modelMatrixLaberintoBody[3][0], modelMatrixLaberintoBody[3][2]);
		modelMatrixLaberintoBody = glm::scale(modelMatrixLaberintoBody, glm::vec3(1.0, 1.5, 1.0));
		modeloLaberinto.render(modelMatrixLaberintoBody);*/

		/*if (dibujaJarron) {
			//Render del Jarron
			glm::mat4 modelMatrixJarronBody = glm::mat4(modelMatrixJarron);
			modelMatrixJarronBody[3][1] = terrain.getHeightTerrain(modelMatrixJarronBody[3][0], modelMatrixJarronBody[3][2]);
			modelMatrixJarronBody = glm::scale(modelMatrixJarronBody, glm::vec3(1.0, 1.0, 1.0));
			modeloJarron.render(modelMatrixJarronBody);
		}*/

		//Render de los jarrones
		for (int i = 0; i < jarronPosition.size(); i++) {
			if (jarronE[i]) {
				jarronPosition[i].y = terrain.getHeightTerrain(jarronPosition[i].x, jarronPosition[i].z);
				modeloJarron.setPosition(jarronPosition[i]);
				modeloJarron.setScale(glm::vec3(1.0, 1.0, 1.0));
				modeloJarron.setOrientation(glm::vec3(0, jarronOrientation[i], 0));
				modeloJarron.render();
			}
		}

		// Render del Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);

		// Render del Lambo
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis[3][1] = terrain.getHeightTerrain(modelMatrixLamboChasis[3][0], modelMatrixLamboChasis[3][2]);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelLambo.render(modelMatrixLamboChasis);
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);
		modelLamboRightDor.render(modelMatrixLamboChasis);
		modelLamboFrontLeftWheel.render(modelMatrixLamboChasis);
		modelLamboFrontRightWheel.render(modelMatrixLamboChasis);
		modelLamboRearLeftWheel.render(modelMatrixLamboChasis);
		modelLamboRearRightWheel.render(modelMatrixLamboChasis);
		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		//Render de las paredes del muro
		for (int i = 0; i < muro1Position.size(); i++) {
			muro1Position[i].y = terrain.getHeightTerrain(muro1Position[i].x, muro1Position[i].z);
			modelMuro.setPosition(muro1Position[i]);
			modelMuro.setScale(glm::vec3(escalaX, escalaY, escalaZ));
			modelMuro.setOrientation(glm::vec3(0, muro1Orientation[i], 0));
			modelMuro.render();
		}

		//Render de los muros del laberinto
		for (int i = 0; i < laberintoPosition.size(); i++) {
			laberintoPosition[i].y = terrain.getHeightTerrain(laberintoPosition[i].x, laberintoPosition[i].z);
			modeloLaberinto.setPosition(laberintoPosition[i]);
			modeloLaberinto.setScale(glm::vec3(1.0, 1.5, 1.0));
			modeloLaberinto.setOrientation(glm::vec3(0, laberintoOrientation[i], 0));
			modeloLaberinto.render();
		}

		//Render de las antorchas del muro
		/*for (int i = 0; i < antorchasPosition.size(); i++) {
			antorchasPosition[i].y = terrain.getHeightTerrain(antorchasPosition[i].x, antorchasPosition[i].z)+4.0;
			modelAntorcha.setPosition(antorchasPosition[i]);
			modelAntorcha.setScale(glm::vec3(1.0, 1.0, 1.0));
			modelAntorcha.setOrientation(glm::vec3(15, antorchasOrientation[i], 0));
			modelAntorcha.render();
		}*/

		// Render de las lamparas y antorchas
		for (int i = 0; i < lamp1Position.size(); i++){
			if (i < 3) {
				lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
				modelLamp1.setPosition(lamp1Position[i]);
				modelLamp1.setScale(glm::vec3(0.5, 0.5, 0.5));
				modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
				modelLamp1.render();
			}
			else {
				lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z) + 4.0;
				modelAntorcha.setPosition(lamp1Position[i]);
				modelAntorcha.setScale(glm::vec3(1.0, 1.0, 1.0));
				modelAntorcha.setOrientation(glm::vec3(15, lamp1Orientation[i], 0));
				modelAntorcha.render();
			}
			
		}

		for (int i = 0; i < lamp2Position.size(); i++){
			lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
			modelLamp2.setPosition(lamp2Position[i]);
			modelLamp2.setScale(glm::vec3(1.0, 1.0, 1.0));
			modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2.render();
			modelLampPost2.setPosition(lamp2Position[i]);
			modelLampPost2.setScale(glm::vec3(1.0, 1.0, 1.0));
			modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLampPost2.render();
		}

		//Render de la antorcha
		/*modelMatrixAntorcha[3][1] = terrain.getHeightTerrain(modelMatrixAntorcha[3][0], modelMatrixAntorcha[3][2])+ 4.0;
		glm::mat4 modelMatrixAtorchaBody = glm::mat4(modelMatrixAntorcha);
		modelMatrixAtorchaBody =glm::scale(modelMatrixAtorchaBody, glm::vec3(1.0,1.0,1.0));
		modelAntorcha.setOrientation(glm::vec3(15, 0, 0));
		modelAntorcha.render(modelMatrixAtorchaBody);*/

		//Render del Nanosuit
		modelMatrixNanosuit[3][1] = terrain.getHeightTerrain(modelMatrixNanosuit[3][0], modelMatrixNanosuit[3][2]);
		glm::mat4 modelMatrixNanosuitBody = glm::mat4(modelMatrixNanosuit);
		modelMatrixNanosuitBody = glm::scale(modelMatrixNanosuitBody, glm::vec3(0.15, 0.15, 0.15));
		nanosuitModel.render(modelMatrixNanosuitBody);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		modelMatrixDart[3][1] = terrain.getHeightTerrain(modelMatrixDart[3][0], modelMatrixDart[3][2]);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		/*******************************************
		 * Custom Anim objects obj
		 *******************************************/
		modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
		modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021, 0.021, 0.021));
		mayowModelAnimate.setAnimationIndex(animationIndex);
		mayowModelAnimate.render(modelMatrixMayowBody);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		// Collider del dart vader lego
		glm::mat4 modelmatrixColliderDart = glm::mat4(modelMatrixDart);
		AbstractModel::OBB dartLegoBodyCollider;
		// Set the orientation of collider before doing the scale
		dartLegoBodyCollider.u = glm::quat_cast(modelMatrixDart);
		modelmatrixColliderDart = glm::scale(modelmatrixColliderDart, glm::vec3(0.5, 0.5, 0.5));
		modelmatrixColliderDart = glm::translate(modelmatrixColliderDart,
				glm::vec3(modelDartLegoBody.getObb().c.x,
						modelDartLegoBody.getObb().c.y,
						modelDartLegoBody.getObb().c.z));
		dartLegoBodyCollider.c = glm::vec3(modelmatrixColliderDart[3]);
		dartLegoBodyCollider.e = modelDartLegoBody.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		addOrUpdateColliders(collidersOBB, "dart", dartLegoBodyCollider, modelMatrixDart);

		// Collider del aricraft
		glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		AbstractModel::OBB aircraftCollider;
		// Set the orientation of collider before doing the scale
		aircraftCollider.u = glm::quat_cast(modelMatrixAircraft);
		modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,
				glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderAircraft = glm::translate(
				modelMatrixColliderAircraft, modelAircraft.getObb().c);
		aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);

		//collider de lambo
		// El objetivo es crear un objeto de tipo collider AbstractModel::OBB, el cual tiene las propiedades siguientes:
		// c: Centro de la caja, e: Medias dimensiones de las aritas, u: OrientaciÃ³n en forma de quaternion.
		glm::mat4 modelMatrixColliderLambo = glm::mat4(modelMatrixLambo);
		AbstractModel::OBB lamboCollider;
		lamboCollider.u = glm::quat_cast(modelMatrixLambo);
		modelMatrixColliderLambo[3][1] = terrain.getHeightTerrain(modelMatrixColliderLambo[3][0], modelMatrixColliderLambo[3][2]);
		modelMatrixColliderLambo = glm::scale(modelMatrixColliderLambo, glm::vec3(1.3, 1.3, 1.3));
		modelMatrixColliderLambo = glm::translate(modelMatrixColliderLambo, modelLambo.getObb().c);
		lamboCollider.c = glm::vec3(modelMatrixColliderLambo[3]);
		lamboCollider.e = modelLambo.getObb().e*glm::vec3(1.3, 1.3, 1.3);
		// 1.- Arreglo de colliders, 2.- Etiqueta, 3.- Collider que creamos, 4.- Matrix TranformaciÃ³n Original
		addOrUpdateColliders(collidersOBB, "lambo", lamboCollider, modelMatrixLambo);

		//Collider del muro
		/*glm::mat4 modelMatrixColliderMuro = glm::mat4(modelMatrixMuro);
		AbstractModel::OBB muroCollider;
		muroCollider.u = glm::quat_cast(modelMatrixMuro);
		modelMatrixColliderMuro[3][1] = terrain.getHeightTerrain(modelMatrixColliderMuro[3][0], modelMatrixColliderMuro[3][2]);
		modelMatrixColliderMuro = glm::scale(modelMatrixColliderMuro, glm::vec3(escalaX, escalaY, escalaZ));
		modelMatrixColliderMuro = glm::translate(modelMatrixColliderMuro, modelMuro.getObb().c);
		muroCollider.c = glm::vec3(modelMatrixColliderMuro[3]);
		muroCollider.e = modelMuro.getObb().e*glm::vec3(escalaX, escalaY, escalaZ);
		// 1.- Arreglo de colliders, 2.- Etiqueta, 3.- Collider que creamos, 4.- Matrix TranformaciÃ³n Original
		addOrUpdateColliders(collidersOBB, "muro", muroCollider, modelMatrixMuro);*/

		//Collider del jarron
		/*if (dibujaJarron) {
			glm::mat4 modelMatrixColliderJarron = glm::mat4(modelMatrixJarron);
			AbstractModel::OBB jarronCollider;
			jarronCollider.u = glm::quat_cast(modelMatrixJarron);
			modelMatrixColliderJarron[3][1] = terrain.getHeightTerrain(modelMatrixColliderJarron[3][0], modelMatrixColliderJarron[3][2]);
			modelMatrixColliderJarron = glm::scale(modelMatrixColliderJarron, glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderJarron = glm::translate(modelMatrixColliderJarron, modeloJarron.getObb().c);
			jarronCollider.c = glm::vec3(modelMatrixColliderJarron[3]);
			jarronCollider.e = modeloJarron.getObb().e*glm::vec3(1.0, 1.0, 1.0);
			// 1.- Arreglo de colliders, 2.- Etiqueta, 3.- Collider que creamos, 4.- Matrix Tranformacion Original
			addOrUpdateColliders(collidersOBB2, "jarron", jarronCollider, modelMatrixJarron);
		}*/
		
		// collinder de los jarrones
		for (int i = 0; i < jarronPosition.size(); i++) {
			if (jarronE[i]) {
				AbstractModel::OBB jarronCollider;
				glm::mat4 modelMatrixColliderJarron = glm::mat4(1.0);
				modelMatrixColliderJarron = glm::translate(modelMatrixColliderJarron, jarronPosition[i]);
				modelMatrixColliderJarron = glm::rotate(modelMatrixColliderJarron, glm::radians(jarronOrientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB2, "" + std::to_string(i), jarronCollider, modelMatrixColliderJarron);
				// Set the orientation of collider before doing the scale
				jarronCollider.u = glm::quat_cast(modelMatrixColliderJarron);
				modelMatrixColliderJarron = glm::scale(modelMatrixColliderJarron, glm::vec3(1.0, 1.0, 1.0));
				modelMatrixColliderJarron = glm::translate(modelMatrixColliderJarron, modeloJarron.getObb().c);
				jarronCollider.c = glm::vec3(modelMatrixColliderJarron[3]);
				jarronCollider.e = modeloJarron.getObb().e * glm::vec3(1.0, 1.0, 1.0);
				std::get<0>(collidersOBB2.find("" + std::to_string(i))->second) = jarronCollider;
			}
		}
		

		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);

		// Muros1 colliders
		for (int i = 0; i < muro1Position.size(); i++) {
			AbstractModel::OBB muroCollider;
			glm::mat4 modelMatrixCollidermuro = glm::mat4(1.0);
			modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro, muro1Position[i]);
			modelMatrixCollidermuro = glm::rotate(modelMatrixCollidermuro, glm::radians(muro1Orientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "muro1-" + std::to_string(i), muroCollider, modelMatrixCollidermuro);
			// Set the orientation of collider before doing the scale
			muroCollider.u = glm::quat_cast(modelMatrixCollidermuro);
			modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(escalaX, escalaY , escalaZ));
			modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro, modelMuro.getObb().c);
			muroCollider.c = glm::vec3(modelMatrixCollidermuro[3]);
			muroCollider.e = modelMuro.getObb().e * glm::vec3(escalaX, escalaY, escalaZ);
			std::get<0>(collidersOBB.find("muro1-" + std::to_string(i))->second) = muroCollider;
		}
		// laberinto collider
		for (int i = 0; i < laberintoPosition.size(); i++) {
			AbstractModel::OBB laberintoCollider;
			glm::mat4 modelMatrixColliderLaberinto = glm::mat4(1.0);
			modelMatrixColliderLaberinto = glm::translate(modelMatrixColliderLaberinto, laberintoPosition[i]);
			modelMatrixColliderLaberinto = glm::rotate(modelMatrixColliderLaberinto, glm::radians(laberintoOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "laberinto1-" + std::to_string(i), laberintoCollider, modelMatrixColliderLaberinto);
			// Set the orientation of collider before doing the scale
			laberintoCollider.u = glm::quat_cast(modelMatrixColliderLaberinto);
			modelMatrixColliderLaberinto = glm::scale(modelMatrixColliderLaberinto, glm::vec3(1.0, 1.5, 1.0));
			modelMatrixColliderLaberinto = glm::translate(modelMatrixColliderLaberinto, modeloLaberinto.getObb().c);
			laberintoCollider.c = glm::vec3(modelMatrixColliderLaberinto[3]);
			laberintoCollider.e = modeloLaberinto.getObb().e * glm::vec3(1.0, 1.5, 1.0);
			std::get<0>(collidersOBB.find("laberinto1-" + std::to_string(i))->second) = laberintoCollider;
		}

		// Lamps1 colliders
		for (int i = 0; i < lamp1Position.size(); i++){
			if (i<3) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5, 0.5, 0.5));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = modelLamp1.getObb().e * glm::vec3(1.0, 1.0, 1.0);
				std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;

			}
			else {
				AbstractModel::OBB antorchaCollider;
				glm::mat4 modelMatrixColliderAntorcha = glm::mat4(1.0);
				modelMatrixColliderAntorcha = glm::translate(modelMatrixColliderAntorcha, lamp1Position[i]);
				modelMatrixColliderAntorcha = glm::rotate(modelMatrixColliderAntorcha, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), antorchaCollider, modelMatrixColliderAntorcha);
				// Set the orientation of collider before doing the scale
				antorchaCollider.u = glm::quat_cast(modelMatrixColliderAntorcha);
				modelMatrixColliderAntorcha = glm::scale(modelMatrixColliderAntorcha, glm::vec3(1.0, 1.0, 1.0));
				modelMatrixColliderAntorcha = glm::translate(modelMatrixColliderAntorcha, modelAntorcha.getObb().c);
				antorchaCollider.c = glm::vec3(modelMatrixColliderAntorcha[3]);
				antorchaCollider.e = modelAntorcha.getObb().e * glm::vec3(1.0, 1.0, 1.0);
				std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = antorchaCollider;
			}
		}

		// Lamps2 colliders
		for (int i = 0; i < lamp2Position.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp2Orientation[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampPost2.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLampPost2.getObb().e * glm::vec3(1.0, 1.0, 1.0);
			std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) = lampCollider;
		}

		//collider del Nanosuit
		AbstractModel::SBB nanosuitCollider;
		//Heredamos de la matriz que tiene escalacion 
		glm::mat4 modelMatrixColliderNanosuit= glm::mat4(modelMatrixNanosuit);
		modelMatrixColliderNanosuit = glm::scale(modelMatrixColliderNanosuit, glm::vec3(0.15, 0.15, 0.15));
		//trasladamos el modelo al centro de la esfera
		modelMatrixColliderNanosuit = glm::translate(modelMatrixColliderNanosuit, nanosuitModel.getSbb().c);
		nanosuitCollider.c = glm::vec3(modelMatrixColliderNanosuit[3]);
		//colocando el radio 
		nanosuitCollider.ratio = nanosuitModel.getSbb().ratio*0.15;
		//agregarlo a un vector para que sea iteractivo a un hashmap 
		//se le pasa modelMatrixNanosuit para que no avence, para que se reinicie 
		addOrUpdateColliders(collidersSBB, "nanosuit", nanosuitCollider, modelMatrixNanosuit);


		// Collider de mayow
		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
		modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
		modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
				glm::vec3(mayowModelAnimate.getObb().c.x,
						mayowModelAnimate.getObb().c.y,
						mayowModelAnimate.getObb().c.z));
		mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
		addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow);

		/*******************************************
		 * Render de colliders
		 *******************************************/

		 //Aqui se dibuja la caja 2
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB2.begin(); it != collidersOBB2.end(); it++) {
			int op;
			std::istringstream(it->first) >> op;
			if (jarronE[op]) {
				glm::mat4 matrixCollider = glm::mat4(1.0);
				matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
				matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
				matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
				boxCollider2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
				boxCollider2.enableWireMode();
				boxCollider2.render(matrixCollider);
			}
		}
		

		//Aqui se dibuja la caja
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}
		//Aqui la esfera
		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
				glm::mat4 matrixCollider = glm::mat4(1.0);
				matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
				matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
				sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
				sphereCollider.enableWireMode();
				sphereCollider.render(matrixCollider);
		}

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(mayowCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(mayowCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(mayowCollider.u) * glm::vec4(mayowCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, mayowCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB2.begin(); it != collidersOBB2.end(); it++) {//Recorre la primer lista de colliders
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
					&& testOBBOBB(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					/*std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;*/
							/*if (jt->first == "jarron") {
								std::cout << "Colision " << it->first << " with "
									<< jt->first << std::endl;
								dibujaJarron = false;
								//isCollision = false;
							}*/
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					int op;
					std::istringstream(it->first) >> op;
					jarronE[op] = false;

					//std::cout << jarronEtiqueta[0] << std::endl;
					dibujaJarron = false;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
						&& testOBBOBB(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					/*std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;*/
					/*if (jt->first == "jarron") {
						std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
						dibujaJarron = false;
						//isCollision = false;
					}*/
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
						&& testSphereSphereIntersection(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					/*std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;*/
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					/*std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;*/
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
				colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(jt->second);
					if (jt->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(jt->second);
					if (jt->first.compare("lambo") == 0)
						modelMatrixLambo = std::get<1>(jt->second);
				}
			}
		}

	

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/
		/*
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		}
		else if(keyFramesDartJoints.size() > 0){
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}

		if (record && modelSelected == 2) {
			matrixDart.push_back(modelMatrixDart);
			if (saveFrame) {
				appendFrame(myfile, matrixDart);
				saveFrame = false;
			}
		}
		else if (keyFramesDart.size() > 0) {
			// Para reproducir el frame
			interpolationDart = numPasosDart / (float)maxNumPasosDart;
			numPasosDart++;
			if (interpolationDart > 1.0) {
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if (indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}*/

		// Constantes de animaciones
		rotHelHelY += 0.5;
		animationIndex = 1;

		

		/*******************************************
		 * State machines
		 *******************************************/

		// State machine for the lambo car
		switch(stateDoor){
		case 0:
			dorRotCount += 0.5;
			if(dorRotCount > 75)
				stateDoor = 1;
			break;
		case 1:
			dorRotCount -= 0.5;
			if(dorRotCount < 0){
				dorRotCount = 0.0;
				stateDoor = 0;
			}
			break;
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
