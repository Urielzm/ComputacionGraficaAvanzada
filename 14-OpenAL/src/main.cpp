#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

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

// OpenAL include
#include <AL/alut.h>
//Shadow box include
#include "Headers/ShadowBox.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;
int op = 0; //Para cambio de camara
int cuentaJarrones = 0;
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
//Camara para primera persona
std::shared_ptr<FirstPersonCamera> camera2(new FirstPersonCamera());


//Posición inical de la camara en tercera persona, que tan cerca del personaje esta
float distanceFromTarget = 30.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Box boxCollider2;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;
ShadowBox *shadowBox;

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
//Modelo del fantasma
Model modelFantasma;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
// Hierba
Model modelGrass;
//Antorcha
Model modelAntorcha;
//Modelo del muro
Model modelMuro;
//Modloe del laberinto
Model modeloLaberinto;
//Model jarron
Model modeloJarron;
//Modelo de la puerta
Model modeloArco;
// Fountain
//Modelo de la fuente
Model modelFountain;
//Modelo de la puerta cerrada
Model modelPuertaCerrada;
//Modelo de la princesa Zelda
Model modelPrincesa;

//Modelos de los letreros
//Modelo de letrero Fin
Model modelLetreroFin;
//Modelo de letrero Inicio
Model modelLetreroInicio;
//Modelo de letrero Conitunar
Model modelLetreroContinuar;
//Modelo de letrero Completo
Model modelLetreroCompleto;

//Modelo de la puerta Final
Model modelMarco;
Model modelPuertaIzquierda;
Model modelPuertaDerecha;

// Model animate instance
// Mayow
Model mayowModelAnimate;


// Terrain model instance
//Terrain terrain(-1, -1, 200, 16, "../Textures/heightmap.png");
Terrain terrain(-1, -1, 200, 16, "../Textures/TerrenoP1.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

/*std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };*/

std::string fileNames[6] = {
	"../Textures/sky3/eclipserojo_ft.tga",
	"../Textures/sky3/eclipserojo_bk.tga",
	"../Textures/sky3/eclipserojo_up.tga",
	"../Textures/sky3/eclipserojo_dn.tga",
	"../Textures/sky3/eclipserojo_rt.tga",
	"../Textures/sky3/eclipserojo_lt.tga"
};

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
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
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
//Matriz de la puerta cerrada
glm::mat4 modelMatrixPuertaCerrda = glm::mat4(1.0f);
//Matriz del fantasma
glm::mat4 modelMatrixFantasma = glm::mat4(1.0f);
//Matriz del fantasma 1
glm::mat4 modelMatrixFantasma1 = glm::mat4(1.0f);
//Matriz para la princesa
glm::mat4 modelMatrixPrincesa = glm::mat4(1.0f);

//Mtrices para los letreros
//Matriz del letrero de fin
glm::mat4 modelMatrixLetreroFin = glm::mat4(1.0f);
glm::mat4 modelMatrixLetreroContinuar = glm::mat4(1.0f);
glm::mat4 modelMatrixLetreroInicio = glm::mat4(1.0f);
glm::mat4 modelMatrixLetreroCompleto = glm::mat4(1.0f);


//Matriz de la puerta
glm::mat4 modelMatrixPuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaIzquierda = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaDerecha = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaMarco = glm::mat4(1.0f);

int animationIndex = 1;
//sourcesPlay[6] = false;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 2;
bool enableCountSelected = true;

float rotBuzzLeftArm = 0.0, rotBuzzRightArm = 0.0;

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
int ganador=0;
//Activar audio de ataque
//bool ataque = false;

// Var animate helicopter
float rotHelHelY = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

//variables de escala del Muro
float escalaX = 10;
float escalaY = 12;
float escalaZ = 5;

//variables de escala del Muro de contorno
float escalaXp = 7;
float escalaYp = 10;
float escalaZp = 1;

//variables de escala del Muro del laberinto
float escalaXA = 10.2;
float escalaYA = 10;
float escalaZA = 5;

//puerta abierta
float abrir = 0.0;

//Vida del personaje
float vida = 10.0;
bool vivo = true;
float vidaFantasma = 2.0;
bool fantasmaVivo = true;
//Variables para reiniciar el juego
bool inicioFin = true;
bool juegoTerminado = false;
bool trasladaPersonaje = false;
bool complet = false;

//Muro posiciones
//Vector de vectores de posicion
//Viendolo de frente X: Positiva -->
//Viendolo de frente X: Negativa <--
std::vector<glm::vec3> muro1Position = {
	//Viendolo de frente

	//Posiciones frontal
	//glm::vec3(80.4, 0.0, 50.0),
	//glm::vec3(67.0, 0.0, 50.0),
	//glm::vec3(53.6, 0.0, 50.0),
	glm::vec3(48.2, 0.0, 50.7),
	//glm::vec3(26.8, 0.0, 50.0),
	//glm::vec3(13.4, 0.0, 50.0),
	//glm::vec3(0.0, 0.0, 50.0),//Muro central --> Puerta
	//glm::vec3(-13.4, 0.0, 50.0),
	//glm::vec3(-26.8, 0.0, 50.0),
	glm::vec3(-44.2, 0.0, 50.7),
	//glm::vec3(-53.6, 0.0, 50.0),
	//glm::vec3(-67.0, 0.0, 50.0),
	//glm::vec3(-80.4, 0.0, 50.0),

	//Posiciones IZQUIERDA
	/*glm::vec3(-84.8, 0.0, 43.0),
	glm::vec3(-84.8, 0.0, 29.6),
	glm::vec3(-84.8, 0.0, 16.2),
	glm::vec3(-84.8, 0.0, 2.8),
	glm::vec3(-84.8, 0.0, -10.6),*/
	glm::vec3(-85.8, 0.0, -20.0),
	/*glm::vec3(-84.8, 0.0, -37.4),
	glm::vec3(-84.8, 0.0, -50.8),
	glm::vec3(-84.8, 0.0, -64.2),
	glm::vec3(-84.8, 0.0, -77.6),
	glm::vec3(-84.8, 0.0, -91.0),*/

	//Posiciones traceras
	/*glm::vec3(-80.4, 0.0, -100.0),
	glm::vec3(-67.0, 0.0, -100.0),
	glm::vec3(-53.6, 0.0, -100.0),
	glm::vec3(-40.2, 0.0, -100.0),
	glm::vec3(-26.8, 0.0, -100.0),
	glm::vec3(-13.4, 0.0, -100.0),*/
	glm::vec3(6.0, 0.0, -100.0),//Muro central 
	/*glm::vec3(13.4, 0.0, -100.0),
	glm::vec3(26.8, 0.0, -100.0),
	glm::vec3(40.2, 0.0, -100.0),
	glm::vec3(53.6, 0.0, -100.0),
	glm::vec3(67.0, 0.0, -100.0),
	glm::vec3(80.4, 0.0, -100.0),*/

	//Posiciones DERECHA
	/*glm::vec3(84.1, 0.0, -91.0),
	glm::vec3(84.1, 0.0, -77.6),
	glm::vec3(84.1, 0.0, -64.2),
	glm::vec3(84.1, 0.0, -50.8),
	glm::vec3(84.1, 0.0, -37.4),*/
	glm::vec3(85.1, 0.0, -29.0),
	/*glm::vec3(84.1, 0.0, -10.6),
	glm::vec3(84.1, 0.0, 2.8),
	glm::vec3(84.1, 0.0, 16.2),
	glm::vec3(84.1, 0.0, 29.6),
	glm::vec3(84.1, 0.0, 43.0)*/
	glm::vec3(6.0, 0.0, 99.0),//Muro frontal
	glm::vec3(-99.0, 0.0, -5.0),
	glm::vec3(99.0, 0.0, -5.0)
};

//Vector de orientación de Muros
std::vector<float> muro1Orientation = {
	//Viendolo de frente
	//Orientaciones frontales:
	0.0, 0.0, //0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado IZQUIERDO
	-90.0, //-90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0, -90.0,
	//Orientaciones traceras
	0.0, //0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado Derecho
	90.0, //90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90.0, 90
	0.0,90.0,90.0
};

//Posiciones de laberinto
std::vector<glm::vec3> laberintoPosition = {
	//glm::vec3(6.8359375,0,34.375)
	glm::vec3(-48,0,40),
	glm::vec3(-32,0,40),
	glm::vec3(-16,0,40),
	glm::vec3(0,0,	40),
	glm::vec3(16,0,	40),
	glm::vec3(32,0,	40),
	glm::vec3(48,0,	40),
	glm::vec3(64,0,	40),
	glm::vec3(0	,0,32),
	glm::vec3(0	,0,16),
	glm::vec3(0	,0,0),
	glm::vec3(0,0, -16),
	glm::vec3(-8,0,	3.125),
	glm::vec3(-24,0,	3.125),
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
	glm::vec3(11.72656250,0, -18.9375),
	glm::vec3(11.72656250,0, -34.9375),
	glm::vec3(11.72656250,0, -50.9375),
	glm::vec3(3.7265625,0,-58.9375),
	glm::vec3(-12.078125,0, -58.9375),
	glm::vec3(-0.3906250,0, -66.9375),
	glm::vec3(-0.3906250,0, -82.9375),
	glm::vec3(-8.3906250,0, -90.9375),
	glm::vec3(-11.1171875,0, -90.9375),
	glm::vec3(-48,0,	17.1875),
	glm::vec3(-32.390625,0,	17.1875),
	glm::vec3(-43.359375, 0, 9.1875),
	glm::vec3(-43.359375,0, -6.8125),
	glm::vec3(-53.7109375,0, -14.8125),
	glm::vec3(-69.7109375,0, -14.8125),
	glm::vec3(-37.7109375,0, -14.8125),
	glm::vec3(-21.7109375,0, -14.8125),
	glm::vec3(-53.7109375,0, -24.578125),
	glm::vec3(-69.7109375,0, -24.578125),
	glm::vec3(-37.7109375,0, -24.578125),
	glm::vec3(-21.7109375,0, -24.578125),
	glm::vec3(-60.15625,0, -32.578125),
	glm::vec3(-68.15625,0, -40.578125),
	glm::vec3(-13.83203125,0, -32.578125),
	glm::vec3(-5.8671875,0, -40.578125),
	glm::vec3(-13.8671875,0, -40.578125),
	glm::vec3(-29.8671875,0, -40.578125),
	glm::vec3(-37.8671875,0, -48.578125),
	glm::vec3(-37.8671875,0, -64.578125),
	glm::vec3(-37.8671875,0, -80.578125),
	glm::vec3(-29.8671875,0, -70.1171875),
	glm::vec3(-45.8671875,0, -55.078125),
	glm::vec3(-61.8671875,0, -55.078125),
	glm::vec3(-61.8671875,0, -63.078125),
	glm::vec3(-61.8671875,0, -79.078125),
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
	glm::vec3(14.0859375,0, -81.78125),
	glm::vec3(-76.9609375,0, 2.34375),
	glm::vec3(-60.5859375,0, 2.34375),
	glm::vec3(-80.8671875,0, -69.140625),
	glm::vec3(75.0078125,0, -70.703125),
	glm::vec3(14.15625,0, -90.9375),
	glm::vec3(-64,0, 17.1875),
	glm::vec3(-79,0, 17.1875),
	glm::vec3(75.0078125,0, -41.40625)
};

//Vector de orientacion de los muros del laberinto
std::vector<float> laberintoOrientation = {
	//0
	0, 0, 0, 0, 0, 0, 0, 0, 90, 90, 90, 90, 0, 0, 90, 90, 0, 90, 90, 90, 90, 0, 0, 0, 0, 90, 90, 0, 0,
	90, 90, 90, 0, 0, 90, 90, 0, 90, 0, 0, 90, 90, 0, 0, 0, 0, 0, 0, 0, 0, 90, 0, 90, 0, 0, 0, 90, 90,
	90, 0, 0, 0, 90, 90, 0, 0, 90, 90, 90, 90, 90, 90, 90, 0, 0, 0, 0, 0, 90,0,0,0,0, 90,0,0, 0
};

// Lamps positions
std::vector<glm::vec3> lamp1Position = { 
	glm::vec3(-7.03, 0, -19.14), 
	glm::vec3(24.41, 0, -34.57), 
	glm::vec3(-10.15, 0, -54.10),

	//Antorchas frontales
	//glm::vec3(80.4, 0.0, 52.0),
	glm::vec3(-48.8671875, 0, -54.5),
	//glm::vec3(53.6, 0.0, 52.0),
	glm::vec3(48.4375, 0.0, -25.0),
	//glm::vec3(26.8, 0.0, 52.0),
	glm::vec3(13.4, 0.0, 52.8),//Frontal Derecha
	//glm::vec3(0.0, 0.0, 50.0),//Muro central --> Puerta
	glm::vec3(-13.4, 0.0, 52.8),//Frontal izquierda
	//glm::vec3(-26.8, 0.0, 52.0),
	glm::vec3(-57.03125, 0.0, 17.8),
	//glm::vec3(-53.6, 0.0, 52.0),
	glm::vec3(67.96875, 0.0, 40.7)
	//glm::vec3(-80.4, 0.0, 52.0),

	//Antorchas IZQUIERDA
	/*glm::vec3(-86.8, 0.0, 43.0),
	//glm::vec3(-86.8, 0.0, 29.6),
	glm::vec3(-86.8, 0.0, 16.2),
	//glm::vec3(-86.8, 0.0, 2.8),
	glm::vec3(-86.8, 0.0, -10.6),
	//glm::vec3(-86.8, 0.0, -24.0),
	glm::vec3(-86.8, 0.0, -37.4),
	//glm::vec3(-86.8, 0.0, -50.8),
	glm::vec3(-86.8, 0.0, -64.2),
	//glm::vec3(-86.8, 0.0, -77.6),
	glm::vec3(-86.8, 0.0, -91.0),

	//Posiciones traceras
	glm::vec3(-80.4, 0.0, -100.0),
	//glm::vec3(-67.0, 0.0, -100.0),
	glm::vec3(-53.6, 0.0, -100.0),
	//glm::vec3(-40.2, 0.0, -100.0),
	glm::vec3(-26.8, 0.0, -100.0),
	//glm::vec3(-13.4, 0.0, -100.0),
	glm::vec3(0.0, 0.0, -100.0),//Muro central
	//glm::vec3(13.4, 0.0, -100.0),
	glm::vec3(26.8, 0.0, -100.0),
	//glm::vec3(40.2, 0.0, -100.0),
	glm::vec3(53.6, 0.0, -100.0),
	//glm::vec3(67.0, 0.0, -100.0),
	glm::vec3(80.4, 0.0, -100.0),

	//Antorchas DERECHA
	//glm::vec3(86.1, 0.0, -91.0),
	glm::vec3(86.1, 0.0, -77.6),
	//glm::vec3(86.1, 0.0, -64.2),
	glm::vec3(86.1, 0.0, -50.8),
	//glm::vec3(86.1, 0.0, -37.4),
	glm::vec3(86.1, 0.0, -24.0),
	//glm::vec3(86.1, 0.0, -10.6),
	glm::vec3(86.1, 0.0, 2.8),
	//glm::vec3(86.1, 0.0, 16.2),
	glm::vec3(86.1, 0.0, 29.6),
	//glm::vec3(86.1, 0.0, 43.0)*/

};
std::vector<float> lamp1Orientation = { 
	-17.0, -82.67, 23.70, 
	//Antorchas Frontales
	-180.0, -180.0, 0.0, 0.0, -180.0, -180.0, //0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado IZQUIERDO
	//-90.0, -90.0, -90.0, -90.0, -90.0, -90.0, //-90.0, -90.0, -90.0, -90.0, -90.0,
	//Orientacionestraceras
	//0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	//Orientaciones lado Derecho
	//90.0, 90.0, 90.0, 90.0, 90.0, 90.0 //90.0, 90.0, 90.0, 90.0, 90

};

std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = {21.37 + 90, -65.0 + 90};

//Vectores de posicion y orientacion de jarrones 
std::vector<glm::vec3> jarronPosition = {
		glm::vec3(-48.4375, 0.0, -69.53125),
		glm::vec3(48.4375, 0.0, -36.71875),
		glm::vec3(67.578125, 0.0, 20.3125),
		glm::vec3(-42.1875, 0.0, 23.046875),
		glm::vec3(-26.953125, 0.0, -33.59375),
		glm::vec3(0.0, 0.0, 56.0)

};
//Vectores de orientacion de jarrones 
std::vector<float> jarronOrientation = { 0,0,0,0,0,0 };

//Vector posicion Fantasmas
std::vector<glm::vec3> fantasmaPosition = {
		glm::vec3(0.0, 0.0, 45.703125),
		glm::vec3(-39.453125, 0.0, 22.0),
		glm::vec3(67.578125, 0.0, -7.03125),
		glm::vec3(48.046875, 0.0, -79.296875),
		glm::vec3(-52, 0.0, -94.921875)

};
//Vectores de orientacion de fantasmas
std::vector<float> fantasmaOrientation = { -90,-90,-90, -90, -90 };
//Vector para ver si el tantasma esta vivo
bool fantasmasVivos[] = { true, true, true, true, true};
//Vector de vidas de los fantasmas
std::vector<int> fantasmasVidas = { 2,2,2,2,2 };

//Vector de etiquetas de jarrones

//Vector de indicadores de jarrones
//std::vector<int> jarronId = { 1, 1 };
bool jarronE[] = { true, true, true, true , true , true };

bool dibujaJarron = true;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(-10.0, 0.0, 60.0)},
		{"lambo", glm::vec3(10.0, 0.0, 60.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)},
		{"fountain", glm::vec3(0.0, 0.0, 52.0)},
		{"fire1", glm::vec3(15.4, 0.0, 55.0)},
		{"fire2", glm::vec3(-11.4, 0.0, 55.0)},
		{"fire3", glm::vec3(-46.8671875, 0, -56.5)},
		{"fire4", glm::vec3(50.4375, 0.0, -27.0)},
		{"fire5", glm::vec3(-55.03125, 0.0, 15.8)},
		{"fire6", glm::vec3(69.9, 0.0, 38.7)},
};

double deltaTime;
double currTime, lastTime;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;//colliders de cajas
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;//colliders de esferas
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB2;//colliders de cajas2

// Framesbuffers
GLuint depthMap, depthMapFBO;



/**********************
 * OpenAL config
 * OpenAL funciona con un escuchante, un listener y un generador de sonido(gernera la fuen)
 */

// OpenAL Defines
//Definimos el numero de buffers que vamos a crear.
//Para agregar una  septima, modificamos a 10.
#define NUM_BUFFERS 9 //agregamos una 9 funete de audio, ponemos 10 en los dos
#define NUM_SOURCES 9
#define NUM_ENVIRONMENTS 1
// Listener
//Para las fuente de sonido de nuestros mismos datos
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
//Source 3
//Almacenamos la posicion y la velocidad del audio
ALfloat source3Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
//Almacenamos la posicion y la velocidad del audio
ALfloat source4Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
//Almacenamos la posicion y la velocidad del audio
ALfloat source5Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };
//Almacenamos la posicion y la velocidad del audio
ALfloat source6Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };
//Almacenamos la posicion y la velocidad del audio
ALfloat source7Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source7Vel[] = { 0.0, 0.0, 0.0 };
//Almacenamos la posicion y la velocidad del audio
ALfloat source8Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source8Vel[] = { 0.0, 0.0, 0.0 };

// Buffers
//Arreglo de buffers, con el numero definido anteriormente
ALuint buffer[NUM_BUFFERS];//Arreglo de buffers, con el numero definido anteriormente
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;

//Se agrega un true, por cada fuente.
std::vector<bool> sourcesPlay = {true, true, true, true, true, true, false, true, true};

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	//float rate = 0.00075f;
	float rate = 0.001f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}



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
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	//Iniciando las colisiones de las cajas 
	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//Iniciando las colisiones de las cajas 2
	boxCollider2.init();
	boxCollider2.setShader(&shader);
	boxCollider2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	//Iniciando las colisiones de las esferas
	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

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

	//Modelo del fantasma
	modelFantasma.loadModel("../models/Fantasma/Fantasma3/f2.obj");
	modelFantasma.setShader(&shaderMulLighting);

	//Modelo del muro
	modelMuro.loadModel("../models/Personaje_proyecto/Laberinto1/MuroYTorre/Muro/Muro_de_castillo2.obj");
	modelMuro.setShader(&shaderMulLighting);
	//Modelo del laberinto
	modeloLaberinto.loadModel("../models/Personaje_proyecto/Laberinto1/ParedConMusgo/Pared_con_musgo3.obj");
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

	//Modelo de la puerta cerrada
	modelPuertaCerrada.loadModel("../models/PuertaFinal/PuertaCerrada2.obj");
	modelPuertaCerrada.setShader(&shaderMulLighting);

	//Puerta
	modelMarco.loadModel("../models/PuertaFinal/Marco.obj");
	modelMarco.setShader(&shaderMulLighting);
	modelPuertaDerecha.loadModel("../models/PuertaFinal/PuertaDerecha.obj");
	modelPuertaDerecha.setShader(&shaderMulLighting);
	modelPuertaIzquierda.loadModel("../models/PuertaFinal/PuertaIzquierda.obj");
	modelPuertaIzquierda.setShader(&shaderMulLighting);

	

	//Lamp models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	//Modelos de los letreros
	//Letrero de fin
	modelLetreroFin.loadModel("../models/Texto/LetreroDeFin.obj");
	modelLetreroFin.setShader(&shaderMulLighting);
	//Letrero de continuar
	modelLetreroContinuar.loadModel("../models/Texto/letrasParaReinicio.obj");
	modelLetreroContinuar.setShader(&shaderMulLighting);
	//Letrero de inicio
	modelLetreroInicio.loadModel("../models/Texto/LetrasMissionInicio.obj");
	modelLetreroInicio.setShader(&shaderMulLighting);
	//Letrero de completo
	modelLetreroCompleto.loadModel("../models/Texto/LetrasMissionPassed.obj");
	modelLetreroCompleto.setShader(&shaderMulLighting);

	//Mayow
	//mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.loadModel("../models/Zelda/source/ZELDA2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);

	//Modelo de la pricesa
	//modelPrincesa.loadModel("../models/PrincesaZelda/PrincesaZelda.obj");
	modelPrincesa.loadModel("../models/Bender/Bender.fbx");
	modelPrincesa.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));//
	camera->setDistanceFromTarget(distanceFromTarget);//que tan lejos esta del personaje, este valor se establece previeamente. --> distanceFromTarget
	camera->setSensitivity(1.0);
	//Establecemos la camara con un angulo de inclinacion de 45 grados, los valores que se le pasan a la función estan en radianes 20°
	//camera->mouseMoveCamera(0.0, 0.1745, 0.1745);

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
	Texture textureTerrainR("../Textures/Acero.png");
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

	//Texture textureParticlesFountain("../Textures/bluewater.png");
	Texture textureParticlesFountain("../Textures/smokeTransparencia.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 0.01f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for( int i = 0; i < randData.size(); i++ ) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,0.1f,0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/humo.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	//Creamos el buffer numero 3
	buffer[3] = alutCreateBufferFromFile("../sounds/car_acel.wav");
	//buffer[3] = alutCreateBufferFromFile("../sounds/Help_me.wav");
	//Creamos el buffer numero 4
	buffer[4] = alutCreateBufferFromFile("../sounds/Help_me.wav");
	//Creamos el buffer numero 5
	buffer[5] = alutCreateBufferFromFile("../sounds/AmbienteGjost.wav");
	//Creamos el buffer numero 6
	buffer[6] = alutCreateBufferFromFile("../sounds/Espada.wav");
	//Creamos el buffer numero 7
	buffer[7] = alutCreateBufferFromFile("../sounds/FondoDemoniaco.wav");
	//Creamos el buffer numero 8
	buffer[8] = alutCreateBufferFromFile("../sounds/NinaFantasma.wav");

	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	//Aceleracion
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Así como la ganancia(de una señal, que tan fuerte se escucha)
	//Cambiamos el indice del arreglo
	alSourcef(source[3], AL_PITCH, 1.0f);//que tan rapido se reproduce el sonido
	alSourcef(source[3], AL_GAIN, 1.0f);//ganancia, rango de 0 a 1, es decir 1, el volumen normal del audio
	alSourcefv(source[3], AL_POSITION, source3Pos);//posicion de la fuente, cambia constantemente, con respeco al modelo
	alSourcefv(source[3], AL_VELOCITY, source3Vel);//Velocidad, no se ocupa, pero hay que configurarla
	alSourcei(source[3], AL_BUFFER, buffer[3]);//Buffer asosiado a esa fuente
	alSourcei(source[3], AL_LOOPING, AL_TRUE);//Una vez que ya empezo a reproducir, si se va a repetir el sonido, para que no se repita es al_false
	alSourcef(source[3], AL_MAX_DISTANCE, 500);//Umbral, para saber que tanto nos alejamos, para que se escuche menos

	//Help me
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Cambiamos el indice del arreglo
	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 1.0f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 500);

	//ambiente
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Cambiamos el indice del arreglo
	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 1.0f);
	alSourcefv(source[5], AL_POSITION, source5Pos);
	alSourcefv(source[5], AL_VELOCITY, source5Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	alSourcei(source[5], AL_LOOPING, AL_TRUE);
	alSourcef(source[5], AL_MAX_DISTANCE, 3500);

	//Espada
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Cambiamos el indice del arreglo
	alSourcef(source[6], AL_PITCH, 1.0f);
	alSourcef(source[6], AL_GAIN, 1.0f);
	alSourcefv(source[6], AL_POSITION, source6Pos);
	alSourcefv(source[6], AL_VELOCITY, source6Vel);
	alSourcei(source[6], AL_BUFFER, buffer[6]);
	alSourcei(source[6], AL_LOOPING, AL_FALSE);//para que se repita una vez
	alSourcef(source[6], AL_MAX_DISTANCE, 500);

	//Fondo con Demonio
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Cambiamos el indice del arreglo
	alSourcef(source[7], AL_PITCH, 1.0f);
	alSourcef(source[7], AL_GAIN, 1.0f);
	alSourcefv(source[7], AL_POSITION, source7Pos);
	alSourcefv(source[7], AL_VELOCITY, source7Vel);
	alSourcei(source[7], AL_BUFFER, buffer[7]);
	alSourcei(source[7], AL_LOOPING, AL_TRUE);//para que se este repitiendo continuamente
	alSourcef(source[7], AL_MAX_DISTANCE, 700);

	//Nina fantasma
	//Condfiguración de la fuente de sonido, el comportamiento de que tan fuerte se escucha
	//Cambiamos el indice del arreglo
	alSourcef(source[8], AL_PITCH, 1.0f);
	alSourcef(source[8], AL_GAIN, 1.0f);
	alSourcefv(source[8], AL_POSITION, source8Pos);
	alSourcefv(source[8], AL_VELOCITY, source8Vel);
	alSourcei(source[8], AL_BUFFER, buffer[8]);
	alSourcei(source[8], AL_LOOPING, AL_TRUE);//para que se este repitiendo continuamente
	alSourcef(source[8], AL_MAX_DISTANCE, 700);

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	boxCollider2.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();

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
	modelGrass.destroy();
	modelFountain.destroy();
	//Destruyendo el modelo de la antorcha
	modelAntorcha.destroy();
	//destruyendo objeto del muro
	modelMuro.destroy();
	//Jarron
	modeloJarron.destroy();
	//Arco
	modeloArco.destroy();
	//Puerta cerrada
	modelPuertaCerrada.destroy();
	//Famntasma
	modelFantasma.destroy();

	//Puerta
	modelMarco.destroy();
	modelPuertaDerecha.destroy();
	modelPuertaIzquierda.destroy();

	//Princesa
	modelPrincesa.destroy();

	//MOdelos de los letreros
	//Letrero de fin
	modelLetreroFin.destroy();
	modelLetreroInicio.destroy();
	modelLetreroCompleto.destroy();
	modelLetreroContinuar.destroy();

	// Custom objects animate
	mayowModelAnimate.destroy();

	//Princesa
	modelPrincesa.destroy();

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
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
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

	offsetX = 0;//el angulo que puedes desplazar
	offsetY = 0;//el angulo que puedes desplazar

	/*if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		rotBuzzLeftArm = (-1.57); abrir = 10;
		rotBuzzRightArm = (-1.57);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		rotBuzzLeftArm = 0.0;
		rotBuzzRightArm = 0.0;
		abrir = 0;
	}
		
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotBuzzRightArm -= 0.02;*/

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 2)
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

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && vivo==true && complet == false){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(5.0f), glm::vec3(0, 1, 0));
		//modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(5.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && vivo == true && complet == false){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-5.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && vivo == true && complet == false){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 1.1));
		animationIndex = 2;
	}else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && vivo == true && complet == false){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -1.1));
		animationIndex = 2;
	}
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && vivo == true) {
		//modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(3.0f), glm::vec3(0, 1, 0));
		animationIndex = 3;
		sourcesPlay[6] = true;
	}
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ) {
		//modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(3.0f), glm::vec3(0, 1, 0));
		
		//animationIndex = 3;
		if (vivo==false || complet==true) {
			vivo = true;
			//modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 60));
			modelMatrixMayow = glm::translate(modelMatrixArco, glm::vec3(0, 0, 10));
			modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));
			vida = 10;

			for (int i = 0; i < fantasmaPosition.size();i++) {
				fantasmasVivos[i] = true;
				fantasmasVidas[i] = 2;
			}
			//trasladaPersonaje = true;
			//jarronE[] = { true, true, true, true , true , true };
			for (int i = 0; i < jarronOrientation.size(); i++) {
				jarronE[i] = true;
			}
			
			complet = false;
			//abrir = 0;
			rotBuzzLeftArm = (0.0); abrir = 0.0;
			rotBuzzRightArm = (0.0);

		}
		
		/*if (complet == true) {
			modelMatrixMayow = glm::translate(modelMatrixArco, glm::vec3(0, 0, 10));
			modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));
			complet = false;
			vida = 10;
			vidaFantasma = 2;
			for (int i = 0; i < jarronOrientation.size(); i++) {
				jarronE[i] = true;
			}
			cuentaJarrones = 0;
			fantasmaVivo = true;

		}*/
		
		//std::cout << ganador << std::endl;
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

/*const GLubyte letraA[20] = {
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff,  0xc0, 0xff,  0xc0, 0xc0, 0xc0,
	0x61, 0x80, 0x3f, 0x00, 0x1e, 0x00
};


void mostrarTexto() {

	glRasterPos3d(2, 0, 0);
	glBitmap(10, 10, 0, 0, 0, 0, letraA);
}*/

void applicationLoop() {
	bool psi = true;
	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	//Modelo del Fantasma
	modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 45.703125));
	modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	int state1 = 0;
	float advanceCount1 = 0.0;
	float rotCount1 = 0.0;
	int numberAdvance1 = 0;
	int maxAdvance1 = 0.0;

	int state2 = 0;
	float advanceCount2 = 0.0;
	float rotCount2 = 0.0;
	int numberAdvance2 = 0;
	int maxAdvance2 = 0.0;

	int state3 = 0;
	float advanceCount3 = 0.0;
	float rotCount3 = 0.0;
	int numberAdvance3 = 0;
	int maxAdvance3 = 0.0;

	int state4 = 0;
	float advanceCount4 = 0.0;
	float rotCount4 = 0.0;
	int numberAdvance4 = 0;
	int maxAdvance4 = 0.0;

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
	
	//Puerta Abierta
	modelMatrixPuerta = glm::translate(modelMatrixPuerta, glm::vec3(-1.5625, 0.0, -94.5));
	modelMatrixPuerta = glm::rotate(modelMatrixPuerta, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//modelMatrixPuerta[3][1] = terrain.getHeightTerrain(modelMatrixPuerta[3][0], modelMatrixPuerta[3][2]) + 0.2;
	//modelMatrixPuerta = glm::scale(modelMatrixPuerta, glm::vec3(10.0f, 10.0f, 10.0f));


	//Modelo de la puerta cerrada
	modelMatrixPuertaCerrda = glm::translate(modelMatrixPuertaCerrda, glm::vec3(74.1953125, 0.0, 40.0));
	//modelMatrixPuertaCerrda = glm::translate(modelMatrixPuertaCerrda, glm::vec3(0.0, 0.0, 52.0));
	//modelMatrixPuertaCerrda = glm::scale(modelMatrixPuertaCerrda, glm::vec3(escalaXp, escalaYp, escalaZp));
	
	//Modelo de la roca
	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	//Modelo del aircraft
	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(-10.0, 0.0, 60.0));

	//matriz del lambo
	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(10.0, 0.0, 60.0));

	//Matriz de la princesa
	modelMatrixPrincesa = glm::translate(modelMatrixPrincesa, glm::vec3(-4.6875, 0.0, -94.140625));
	modelMatrixPrincesa = glm::rotate(modelMatrixPrincesa, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));
	//Modelo de Mayow
		//rotate  viendolo de frente -90:  <--
		//rotate  viendolo de frente -180:  ^
		//rotate  viendolo de frente 90:  -->
		//rotate  viendolo de frente 0:  v
	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 60.0));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));
	//modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(48.4375, 0.0, -25.0));
	//modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));
	

	//Modelo de la fuente
	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(-14.0625, 0.0, -93.75));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0] , modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	//Libreria estandar que se creo  para poder crear apuntaores a programas.
	//Posición de luz, apuntador a la camra, distancia maxima de alcance, plano cercano y el  FOV(filed of view)
	shadowBox = new ShadowBox(-lightPos, camera.get(),15.0f,0.1,45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		//Para el texto
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//glRasterPos2d(2, 0);
		//glBitmap(10, 10, 0, 0, 0, 0, letraA);


		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;
		std::map<std::string, bool> collisionDetection2;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),//que tanto abarca la camara
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}
		else{
			axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));//Angulo en x
			target = glm::vec3(modelMatrixMayow[3].x, modelMatrixMayow[3].y+10, modelMatrixMayow[3].z);//Aqui se dice que tan distante del personaje
			//target = modelMatrixMayow[3];
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(45.0f);
		camera->setCameraTarget(target);
		//camera->mouseMoveCamera(0.0,90,0.0);
		//camera->setAngleArundTarteg(95);
		//camera->mouseMoveCamera(0.0, 10, 0.5);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();

		//view = camera->getViewMatrix();
		if (op == 0) {
			view = camera2->getViewMatrix();
		}
		else {
			view = camera->getViewMatrix();

		}

		shadowBox->update(screenWidth, screenHeight);
		shadowBox->getCenter();

		// Projection light shadow mapping
		/*glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 20.0f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));*/

		glm::mat4 lightProjection = glm::mat4(1.0), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;

		lightView = glm::lookAt(shadowBox->getCenter(), shadowBox->getCenter()+glm::normalize(-lightPos), glm::vec3(0,1.0f,0));

		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

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
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.0, 0.0)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));
		/*shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));//Comportamiento del objeto en ausencia de luz
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));//Comportamiento del objeto al impactar la luz
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.1, 0.0, 0.0)));//Brillo acumulado, depende de la posición de la luz y del espectador
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));//Direccion*/

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));
		/*shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));//Comportamiento del objeto en ausencia de luz
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));//Comportamiento del objeto al impactar la luz
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.1, 0.0, 0.0)));//Brillo acumulado, depende de la posición de la luz y del espectador
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));//Direccion*/

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
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());

		for (int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			//matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
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

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
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
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		/*glm::vec3 front = glm::normalize(-lightPos);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		glm::vec3 up = glm::normalize(glm::cross(front, right));
		glDisable(GL_CULL_FACE);
		glm::mat4 boxViewTransform = glm::mat4(1.0f);
		boxViewTransform = glm::translate(boxViewTransform, centerBox);
		boxViewTransform[0] = glm::vec4(right, 0.0);
		boxViewTransform[1] = glm::vec4(up, 0.0);
		boxViewTransform[2] = glm::vec4(front, 0.0);
		boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		boxLightViewBox.enableWireMode();
		boxLightViewBox.render(boxViewTransform);
		glEnable(GL_CULL_FACE);*/

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

		//Collider del Fantasma
		/*glm::mat4 modelmatrixColliderFantasma = glm::mat4(modelMatrixFantasma);
		AbstractModel::OBB fantasmaoBodyCollider;
		// Set the orientation of collider before doing the scale
		fantasmaoBodyCollider.u = glm::quat_cast(modelMatrixFantasma);
		modelmatrixColliderFantasma = glm::scale(modelmatrixColliderFantasma, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderFantasma = glm::translate(modelmatrixColliderFantasma,
			glm::vec3(modelFantasma.getObb().c.x,
				modelFantasma.getObb().c.y,
				modelFantasma.getObb().c.z));
		fantasmaoBodyCollider.c = glm::vec3(modelmatrixColliderFantasma[3]);
		fantasmaoBodyCollider.e = modelFantasma.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		addOrUpdateColliders(collidersOBB, "Fantasma", fantasmaoBodyCollider, modelMatrixFantasma);*/

		

		//Collider del fantasma
		/*if (fantasmaVivo) {
			glm::mat4 modelMatrixColliderFantasma = glm::mat4(modelMatrixFantasma);
			AbstractModel::SBB fantasmaCollider;
			modelMatrixColliderFantasma = glm::scale(modelMatrixColliderFantasma, glm::vec3(1.0, 1.0, 1.0));
			//modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, modelFantasma.getSbb().c);
			modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma,
				glm::vec3(modelFantasma.getSbb().c.x,
					modelFantasma.getSbb().c.y,
					modelFantasma.getSbb().c.z));
			fantasmaCollider.c = glm::vec3(modelMatrixColliderFantasma[3]);
			fantasmaCollider.ratio = modelFantasma.getSbb().ratio * 1.0;
			addOrUpdateColliders(collidersSBB, "fantasma", fantasmaCollider, modelMatrixFantasma);
		}*/
		// collinder de los fantasmas
		for (int i = 0; i < fantasmaPosition.size(); i++) {
			//if (jarronE[i]) {
				/*AbstractModel::SBB fantasmaCollider;
				glm::mat4 modelMatrixColliderFantasma = glm::mat4(1.0);
				modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, fantasmaPosition[i]);
				modelMatrixColliderFantasma = glm::rotate(modelMatrixColliderFantasma, glm::radians(fantasmaOrientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersSBB, "" + std::to_string(i), fantasmaCollider, modelMatrixColliderFantasma);
				// Set the orientation of collider before doing the scale
				fantasmaCollider.u = glm::quat_cast(modelMatrixColliderFantasma);
				modelMatrixColliderFantasma = glm::scale(modelMatrixColliderFantasma, glm::vec3(2.0, 2.0, 2.0));
				modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, modeloJarron.getObb().c);
				fantasmaCollider.c = glm::vec3(modelMatrixColliderFantasma[3]);
				fantasmaCollider.e = modeloJarron.getObb().e * glm::vec3(2.0, 2.0, 2.0);
				std::get<0>(collidersSBB.find("" + std::to_string(i))->second) = fantasmaCollider;*/
				//}
			if (fantasmasVivos[i] == true) {
				glm::mat4 modelMatrixColliderFantasma = glm::mat4(1.0);
				AbstractModel::SBB fantasmaCollider;
				modelMatrixColliderFantasma = glm::scale(modelMatrixColliderFantasma, glm::vec3(1.0, 1.0, 1.0));
				//modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, modelFantasma.getSbb().c);
				modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, fantasmaPosition[i]);
				modelMatrixColliderFantasma = glm::rotate(modelMatrixColliderFantasma, glm::radians(fantasmaOrientation[i]), glm::vec3(0, 1, 0));
				//glm::vec3(modelFantasma.getSbb().c.x, modelFantasma.getSbb().c.y, modelFantasma.getSbb().c.z));

				fantasmaCollider.c = glm::vec3(modelMatrixColliderFantasma[3]);
				fantasmaCollider.ratio = modelFantasma.getSbb().ratio * 1.0;
				addOrUpdateColliders(collidersSBB, "fantasma" + std::to_string(i), fantasmaCollider, modelMatrixColliderFantasma);
			}
		}

		/*glm::mat4 modelMatrixColliderFantasma = glm::mat4(modelMatrixFantasma1);
		AbstractModel::SBB fantasmaCollider;
		modelMatrixColliderFantasma = glm::scale(modelMatrixColliderFantasma, glm::vec3(1.0, 1.0, 1.0));
		//modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, modelFantasma.getSbb().c);
		modelMatrixColliderFantasma = glm::translate(modelMatrixColliderFantasma, fantasmaPosition[0]);
			/*glm::vec3(modelFantasma.getSbb().c.x,
				modelFantasma.getSbb().c.y,
				modelFantasma.getSbb().c.z));
		fantasmaCollider.c = glm::vec3(modelMatrixColliderFantasma[3]);
		fantasmaCollider.ratio = modelFantasma.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "fantasma1", fantasmaCollider, modelMatrixFantasma1);*/


		// Collider del aricraft
		glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		AbstractModel::OBB aircraftCollider;
		// Set the orientation of collider before doing the scale
		modelMatrixColliderAircraft[3][1] = terrain.getHeightTerrain(modelMatrixColliderAircraft[3][0], modelMatrixColliderAircraft[3][2])+2;
		aircraftCollider.u = glm::quat_cast(modelMatrixColliderAircraft);
		modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderAircraft = glm::translate(modelMatrixColliderAircraft, 
			glm::vec3(modelAircraft.getObb().c.x,
				modelAircraft.getObb().c.y,
				modelAircraft.getObb().c.z));
		aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);

		// Collider de la puerta cerrada
		glm::mat4 modelMatrixColliderPuertaCerrada = glm::mat4(modelMatrixPuertaCerrda);
		AbstractModel::OBB puertaCerradaCollider;
		// Set the orientation of collider before doing the scale
		puertaCerradaCollider.u = glm::quat_cast(modelMatrixPuertaCerrda);
		modelMatrixColliderPuertaCerrada = glm::scale(modelMatrixColliderPuertaCerrada, glm::vec3(escalaXp, escalaYp, escalaZp));
		modelMatrixColliderPuertaCerrada = glm::translate(modelMatrixColliderPuertaCerrada, modelPuertaCerrada.getObb().c);
		puertaCerradaCollider.c = glm::vec3(modelMatrixColliderPuertaCerrada[3]);
		puertaCerradaCollider.e = modelPuertaCerrada.getObb().e * glm::vec3(escalaXp, escalaYp, escalaZp);
		addOrUpdateColliders(collidersOBB, "puertaCerrada", puertaCerradaCollider, modelMatrixPuertaCerrda);

		// Collider de la puerta abierta derecha
		glm::mat4 modelMatrixColliderPuertaAbierta = glm::mat4(modelMatrixPuerta);
		AbstractModel::OBB puertaAbiertaaCollider;
		// Set the orientation of collider before doing the scale
		puertaAbiertaaCollider.u = glm::quat_cast(modelMatrixPuerta);
		modelMatrixColliderPuertaAbierta = glm::scale(modelMatrixColliderPuertaAbierta, glm::vec3(escalaXA, escalaYA, escalaZA));
		modelMatrixColliderPuertaAbierta = glm::translate(modelMatrixColliderPuertaAbierta, modelMarco.getObb().c);
		//modelMatrixColliderPuertaAbierta = glm::rotate(modelMatrixColliderPuertaAbierta, rotBuzzRightArm, glm::vec3(0, 1, 0));
		puertaAbiertaaCollider.c = glm::vec3(modelMatrixColliderPuertaAbierta[3]);
		puertaAbiertaaCollider.e = modelMarco.getObb().e * glm::vec3(escalaXA, escalaYA, escalaZA);
		addOrUpdateColliders(collidersOBB, "puertaAbierta", puertaAbiertaaCollider, modelMatrixPuerta);

		/*AbstractModel::OBB muroCollider;
		glm::mat4 modelMatrixCollidermuro = glm::mat4(1.0);
		modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro,glm::vec3( -1.5625, 0.0, -93.796875));
		modelMatrixCollidermuro = glm::rotate(modelMatrixCollidermuro, glm::radians(90),glm::vec3(0, 1, 0));
		addOrUpdateColliders(collidersOBB, "PuertaAbierta", muroCollider, modelMatrixCollidermuro);
		// Set the orientation of collider before doing the scale
		muroCollider.u = glm::quat_cast(modelMatrixCollidermuro);
		modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(escalaX, escalaY, escalaZ));
		modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro, modelMuro.getObb().c);
		muroCollider.c = glm::vec3(modelMatrixCollidermuro[3]);
		muroCollider.e = modelMuro.getObb().e * glm::vec3(escalaX, escalaY, escalaZ);
		std::get<0>(collidersOBB.find("muro1-" + std::to_string(i))->second) = muroCollider;*/

		

		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);

		

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
				modelMatrixColliderJarron = glm::scale(modelMatrixColliderJarron, glm::vec3(2.0, 2.0, 2.0));
				modelMatrixColliderJarron = glm::translate(modelMatrixColliderJarron, modeloJarron.getObb().c);
				jarronCollider.c = glm::vec3(modelMatrixColliderJarron[3]);
				jarronCollider.e = modeloJarron.getObb().e * glm::vec3(2.0, 2.0, 2.0);
				std::get<0>(collidersOBB2.find("" + std::to_string(i))->second) = jarronCollider;
			}
		}

		// Muros1 colliders
		for (int i = 0; i < muro1Position.size(); i++) {
			muro1Position[i].y = terrain.getHeightTerrain(muro1Position[i].x, muro1Position[i].z);
			//modelMuro.setPosition(muro1Position[i]);
			AbstractModel::OBB muroCollider;
			glm::mat4 modelMatrixCollidermuro = glm::mat4(1.0);
			modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro, muro1Position[i]);
			modelMatrixCollidermuro = glm::rotate(modelMatrixCollidermuro, glm::radians(muro1Orientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "muro1-" + std::to_string(i), muroCollider, modelMatrixCollidermuro);
			// Set the orientation of collider before doing the scale
			muroCollider.u = glm::quat_cast(modelMatrixCollidermuro);
			if (i == 0) {
				//modelMuro.setScale(glm::vec3(62, escalaY, escalaZ));
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(62, escalaY, escalaZ));
			}else if (i == 1) {
				//modelMuro.setScale(glm::vec3(62, escalaY, escalaZ));
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(62, escalaY, escalaZ));
			}
			else if (i == 2) {
				//modelMuro.setScale(glm::vec3(110, escalaY, escalaZ));
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(110, escalaY, escalaZ));
			}
			else if (i == 3) {
				//Muro Trasero
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(150, escalaY, escalaZ));
			}
			else if (i == 4) {
				//modelMuro.setScale(glm::vec3(110, escalaY, escalaZ));
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(110, escalaY, escalaZ));
			}
			else if (i == 5) {
				//Muro Trasero
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(150, escalaY, escalaZ));
			}
			else if (i == 6) {
				//Muro Trasero
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(147, escalaY, escalaZ));
			}
			else if (i == 7) {
				//Muro Trasero
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(147, escalaY, escalaZ));
			}
			else {
				modelMatrixCollidermuro = glm::scale(modelMatrixCollidermuro, glm::vec3(escalaX, escalaY, escalaZ));
			}
			modelMatrixCollidermuro = glm::translate(modelMatrixCollidermuro, modelMuro.getObb().c);
			muroCollider.c = glm::vec3(modelMatrixCollidermuro[3]);
			if (i == 0) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(62, escalaY, escalaZ);
			}else if (i == 1) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(62, escalaY, escalaZ);
			}
			else if (i == 2) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(110, escalaY, escalaZ);
			}
			else if (i == 3) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(150, escalaY, escalaZ);
			}
			else if (i == 4) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(110, escalaY, escalaZ);
			}
			else if (i == 5) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(150, escalaY, escalaZ);
			}
			else if (i == 6) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(147, escalaY, escalaZ);
			}
			else if (i == 7) {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(147, escalaY, escalaZ);
			}
			else {
				muroCollider.e = modelMuro.getObb().e * glm::vec3(escalaX, escalaY, escalaZ);
			}
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
			//modelMatrixColliderLaberinto = glm::scale(modelMatrixColliderLaberinto, glm::vec3(8.6, 1.5, 6.5));
			modelMatrixColliderLaberinto = glm::scale(modelMatrixColliderLaberinto, glm::vec3(1.0, 1.5, 1.0));
			modelMatrixColliderLaberinto = glm::translate(modelMatrixColliderLaberinto, modeloLaberinto.getObb().c);
			laberintoCollider.c = glm::vec3(modelMatrixColliderLaberinto[3]);
			//laberintoCollider.e = modeloLaberinto.getObb().e * glm::vec3(8.6, 1.5, 6.5);
			laberintoCollider.e = modeloLaberinto.getObb().e * glm::vec3(1.0, 1.5, 1.0);
			std::get<0>(collidersOBB.find("laberinto1-" + std::to_string(i))->second) = laberintoCollider;
		}

		// Lamps1 colliders
		/*for (int i = 0; i < lamp1Position.size(); i++){
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
			lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
		}*/

		// Lamps1 colliders
		for (int i = 0; i < lamp1Position.size(); i++) {
			if (i < 3) {
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

		// Collider de mayow
		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
		//modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		//if (trasladaPersonaje == true) {
			//modelmatrixColliderMayow = glm::translate(modelMatrixArco, glm::vec3(0,0,2));
			//mayowModelAnimate.setPosition(modelMatrixMayow,glm::vec3());
			//trasladaPersonaje = false;
		//}
		// Set the orientation of collider before doing the scale
		mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
		modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
				glm::vec3(mayowModelAnimate.getObb().c.x,
						mayowModelAnimate.getObb().c.y,
						mayowModelAnimate.getObb().c.z));
		mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021);//* glm::vec3(0.787401574, 0.787401574, 0.787401574);
		mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
		addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow);

		//Collider para Bender
		glm::mat4 modelmatrixColliderPrincesa = glm::mat4(modelMatrixPrincesa);
		AbstractModel::OBB princesaBodyCollider;
		// Set the orientation of collider before doing the scale
		princesaBodyCollider.u = glm::quat_cast(modelmatrixColliderPrincesa);
		modelmatrixColliderPrincesa = glm::scale(modelmatrixColliderPrincesa, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderPrincesa = glm::translate(modelmatrixColliderPrincesa,
			glm::vec3(modelPrincesa.getObb().c.x,
				modelPrincesa.getObb().c.y,
				modelPrincesa.getObb().c.z));
		princesaBodyCollider.e = modelPrincesa.getObb().e * glm::vec3(0.021, 0.021, 0.021);
		princesaBodyCollider.c = glm::vec3(modelmatrixColliderPrincesa[3]);
		addOrUpdateColliders(collidersOBB, "Princesa", princesaBodyCollider, modelMatrixPrincesa);

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

		//Aqui se dibuja la caja 1
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

		//Aqui se dibuja la esfera
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
					//if (it->first == "5") {
						//complet = true;
					//}
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					int op;
					std::istringstream(it->first) >> op;
					jarronE[op] = false;

					cuentaJarrones = 0;
					for (int i = 0; i <= jarronPosition.size();i++) {
						if (jarronE[i]==false) {
							cuentaJarrones++;
						}
					}
					if (cuentaJarrones == 6) {
						rotBuzzLeftArm = (-1.57); abrir = 10;
						rotBuzzRightArm = (-1.57);
					}
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
					/*if (it->first=="" && it->first == "mayow") {
						complet = true;
						std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					}*/
					/*std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;*/
					if (it->first == "Princesa" && jt->first=="mayow") {
						complet = true;
					}
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
					if (it->first=="fantasma0" && fantasmasVivos[0] == true) {
						if (animationIndex == 1 || animationIndex == 0|| animationIndex == 2) {
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if(animationIndex==3){
							fantasmasVidas[0] = fantasmasVidas[0] - 1;
							std::cout << "Vida fantasma"<< fantasmasVidas[0] << std::endl;
							if (fantasmasVidas[0] <= 0) {
								//fantasmaVivo = false;
								fantasmasVivos[0] = false;
							}
						}
					}
					if (it->first == "fantasma1" && fantasmasVivos[1] == true) {
						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2) {
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if (animationIndex == 3) {
							fantasmasVidas[1] = fantasmasVidas[1] - 1;
							std::cout << "Vida fantasma" << fantasmasVidas[1] << std::endl;
							if (fantasmasVidas[1] <= 0) {
								//fantasmaVivo = false;
								fantasmasVivos[1] = false;
							}
						}
					}
					if (it->first == "fantasma2" && fantasmasVivos[2] == true) {

						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2){
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if (animationIndex == 3) {
							fantasmasVidas[2] = fantasmasVidas[2] - 1;
							std::cout << "Vida fantasma" << fantasmasVidas[2] << std::endl;
							if (fantasmasVidas[2] <= 0) {
								//fantasmaVivo = false;
								fantasmasVivos[2] = false;
							}
						}
					}
					if (it->first == "fantasma3" && fantasmasVivos[3] == true) {

						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2) {
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if (animationIndex == 3) {
							fantasmasVidas[3] = fantasmasVidas[3] - 1;
							std::cout << "Vida fantasma" << fantasmasVidas[3] << std::endl;
							if (fantasmasVidas[3] <= 0) {
								//fantasmaVivo = false;
								fantasmasVivos[3] = false;
							}
						}
					}
					if (it->first == "fantasma4" && fantasmasVivos[4] == true) {

						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2) {
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if (animationIndex == 3) {
							fantasmasVidas[4] = fantasmasVidas[4] - 1;
							std::cout << "Vida fantasma" << fantasmasVidas[4] << std::endl;
							if (fantasmasVidas[4] <= 0) {
								//fantasmaVivo = false;
								fantasmasVivos[4] = false;
							}
						}
					}
					
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
				}
			}
		}

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/
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
		}

		// Constantes de animaciones
		rotHelHelY += 0.5;
		animationIndex = 1;
		//sourcesPlay[6] = false;

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

		// State machine para el fantasma0
		//if (fantasmaVivo) {
		switch (state) {
		case 0:
			if (numberAdvance == 0)
				maxAdvance = 15.0;
			else if (numberAdvance == 1)
				maxAdvance = 0.5;
			else if (numberAdvance == 2)
				maxAdvance = 30.0;
			else if (numberAdvance == 3)
				maxAdvance = 0.5;
			else if (numberAdvance == 4)
				maxAdvance = 30.0;
			state = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			if (numberAdvance==0) {
				fantasmaPosition[0].x -= 0.5;
			}
			else if (numberAdvance==1) {
				fantasmaPosition[0].z += 0.5;
			}
			else if (numberAdvance==2) {
				fantasmaPosition[0].x += 0.5;
			}
			else if (numberAdvance==3) {
				fantasmaPosition[0].z -= 0.5;
			}
			else if (numberAdvance==4) {
				fantasmaPosition[0].x -= 0.5;
			}
			
			advanceCount += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[0] += 1;
			rotCount += 0.5f;
			if (rotCount >= 45) {
				rotCount = 0;
				state = 0;
				if (numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		}

		// State machine para el fantasma1
		//if (fantasmaVivo) {
		switch (state1) {
		case 0:
			if (numberAdvance1 == 0)
				maxAdvance1 = 7.0;
			else if (numberAdvance1 == 1)
				maxAdvance1 = 3.0;
			else if (numberAdvance1 == 2)
				maxAdvance1 = 14.0;
			else if (numberAdvance1 == 3)
				maxAdvance1 = 3.0;
			else if (numberAdvance1 == 4)
				maxAdvance1 = 14.0;
			state1 = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			if (numberAdvance1 == 0) {
				fantasmaPosition[1].x -= 0.5;
			}
			else if (numberAdvance1 == 1) {
				fantasmaPosition[1].z += 0.5;
			}
			else if (numberAdvance1 == 2) {
				fantasmaPosition[1].x += 0.5;
			}
			else if (numberAdvance1 == 3) {
				fantasmaPosition[1].z -= 0.5;
			}
			else if (numberAdvance1 == 4) {
				fantasmaPosition[1].x -= 0.5;
			}

			advanceCount1 += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount1 > maxAdvance1) {
				advanceCount1 = 0;
				numberAdvance1++;
				state1 = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[1] += 1;
			rotCount1 += 0.5f;
			if (rotCount1 >= 45) {
				rotCount1 = 0;
				state1 = 0;
				if (numberAdvance1 > 4)
					numberAdvance1 = 1;
			}
			break;
		}
		// State machine para el fantasma2
		//if (fantasmaVivo) {
		switch (state2) {
		case 0:
			if (numberAdvance2 == 0)
				maxAdvance2 = 1.0;
			else if (numberAdvance2 == 1)
				maxAdvance2 = 8.0;
			else if (numberAdvance2 == 2)
				maxAdvance2 = 2.0;
			else if (numberAdvance2 == 3)
				maxAdvance2 = 8.0;
			else if (numberAdvance2 == 4)
				maxAdvance2 = 2.0;
			state2 = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			if (numberAdvance2 == 0) {
				fantasmaPosition[2].x -= 0.5;
			}
			else if (numberAdvance2 == 1) {
				fantasmaPosition[2].z += 0.5;
			}
			else if (numberAdvance2 == 2) {
				fantasmaPosition[2].x += 0.5;
			}
			else if (numberAdvance2 == 3) {
				fantasmaPosition[2].z -= 0.5;
			}
			else if (numberAdvance2 == 4) {
				fantasmaPosition[2].x -= 0.5;
			}

			advanceCount2 += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount2 > maxAdvance2) {
				advanceCount2 = 0;
				numberAdvance2++;
				state2 = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[2] += 1;
			rotCount2 += 0.5f;
			if (rotCount2 >= 45) {
				rotCount2 = 0;
				state2 = 0;
				if (numberAdvance2 > 4)
					numberAdvance2 = 1;
			}
			break;
		}
		// State machine para el fantasma3
		//if (fantasmaVivo) {
		switch (state3) {
		case 0:
			if (numberAdvance3 == 0)
				maxAdvance3 = 2.0;
			else if (numberAdvance3 == 1)
				maxAdvance3 = 9.0;
			else if (numberAdvance3 == 2)
				maxAdvance3 = 4.0;
			else if (numberAdvance3 == 3)
				maxAdvance3 = 9.0;
			else if (numberAdvance3 == 4)
				maxAdvance3 = 4.0;
			state3 = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			if (numberAdvance3 == 0) {
				fantasmaPosition[3].x -= 0.5;
			}
			else if (numberAdvance3 == 1) {
				fantasmaPosition[3].z += 0.5;
			}
			else if (numberAdvance3 == 2) {
				fantasmaPosition[3].x += 0.5;
			}
			else if (numberAdvance3 == 3) {
				fantasmaPosition[3].z -= 0.5;
			}
			else if (numberAdvance3 == 4) {
				fantasmaPosition[3].x -= 0.5;
			}

			advanceCount3 += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount3 > maxAdvance3) {
				advanceCount3 = 0;
				numberAdvance3++;
				state3 = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[3] += 1;
			rotCount3 += 0.5f;
			if (rotCount3 >= 45) {
				rotCount3 = 0;
				state3 = 0;
				if (numberAdvance3 > 4)
					numberAdvance3 = 1;
			}
			break;
		}

		// State machine para el fantasma4
		//if (fantasmaVivo) {
		switch (state4) {
		case 0:
			if (numberAdvance4 == 0)
				maxAdvance4 = 1.7;
			else if (numberAdvance4 == 1)
				maxAdvance4 = 7.0;
			else if (numberAdvance4 == 2)
				maxAdvance4 = 3.4;
			else if (numberAdvance4 == 3)
				maxAdvance4 = 7.0;
			else if (numberAdvance4 == 4)
				maxAdvance4 = 3.4;
			state4 = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			if (numberAdvance4 == 0) {
				fantasmaPosition[4].x -= 0.5;
			}
			else if (numberAdvance4 == 1) {
				fantasmaPosition[4].z += 0.5;
			}
			else if (numberAdvance4 == 2) {
				fantasmaPosition[4].x += 0.5;
			}
			else if (numberAdvance4 == 3) {
				fantasmaPosition[4].z -= 0.5;
			}
			else if (numberAdvance4 == 4) {
				fantasmaPosition[4].x -= 0.5;
			}

			advanceCount4 += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount4 > maxAdvance4) {
				advanceCount4 = 0;
				numberAdvance4++;
				state4 = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[4] += 1;
			rotCount4 += 0.5f;
			if (rotCount4 >= 45) {
				rotCount4 = 0;
				state4 = 0;
				if (numberAdvance4 > 4)
					numberAdvance4 = 1;
			}
			break;
		}
		//}
		/*// State machine para el fantasma1
		//if (fantasmaVivo) {
		switch (state) {
		case 0:
			if (numberAdvance == 0)
				maxAdvance = 15.0;
			else if (numberAdvance == 1)
				maxAdvance = 0.5;
			else if (numberAdvance == 2)
				maxAdvance = 30.0;
			else if (numberAdvance == 3)
				maxAdvance = 0.5;
			else if (numberAdvance == 4)
				maxAdvance = 30.0;
			state = 1;
			break;
		case 1:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.5));
			fantasmaPosition[0].x += 0.05;
			advanceCount += 0.1;
			//rotWheelsX += 0.05;
			//rotWheelsY -= 0.02;
			//if (rotWheelsY < 0)
			//	rotWheelsY = 0;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			//modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 0.025));
			//modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(1.0f), glm::vec3(0, 1, 0));
			fantasmaOrientation[0] += 1;
			rotCount += 0.5f;
			if (rotCount >= 45) {
				rotCount = 0;
				state = 0;
				if (numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		}
		//}*/
		

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixArco[3].x;
		source0Pos[1] = modelMatrixArco[3].y;
		source0Pos[2] = modelMatrixArco[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		source3Pos[0] = modelMatrixLambo[3].x;
		source3Pos[1] = modelMatrixLambo[3].y;
		source3Pos[2] = modelMatrixLambo[3].z;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[3], AL_POSITION, source3Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		/*source4Pos[0] = modelMatrixFantasma[3].x;
		source4Pos[1] = modelMatrixFantasma[3].y;
		source4Pos[2] = modelMatrixFantasma[3].z;*/
		source4Pos[0] = fantasmaPosition[0].x;
		source4Pos[1] = fantasmaPosition[0].y;
		source4Pos[2] = fantasmaPosition[0].z;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[4], AL_POSITION, source4Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		source5Pos[0] = 0.0;
		source5Pos[1] = 0.0;
		source5Pos[2] = 0.0;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[5], AL_POSITION, source5Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		source6Pos[0] = modelMatrixMayow[3].x+1;
		source6Pos[1] = modelMatrixMayow[3].y+1;
		source6Pos[2] = modelMatrixMayow[3].z+1;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[6], AL_POSITION, source6Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		source7Pos[0] = fantasmaPosition[3].x;
		source7Pos[1] = fantasmaPosition[3].y;
		source7Pos[2] = fantasmaPosition[3].z;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[7], AL_POSITION, source7Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
		source8Pos[0] = fantasmaPosition[1].x;
		source8Pos[1] = fantasmaPosition[1].y;
		source8Pos[2] = fantasmaPosition[1].z;
		//Indicamos cual es la fuente de 1sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[8], AL_POSITION, source8Pos);

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixMayow[3].x;
		listenerPos[1] = modelMatrixMayow[3].y;
		listenerPos[2] = modelMatrixMayow[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		//En el caso de la canara en tercera persona tiene que ver hacia donde esta apuntando la camara.
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		/*listenerPos[0] = camera->getPosition().x;
		listenerPos[1] = camera->getPosition().y;
		listenerPos[2] = camera->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);
		listenerOri[0] = camera->getFront().x;
		listenerOri[1] = camera->getFront().y;
		listenerOri[2] = camera->getFront().z;
		listenerOri[3] = camera->getUp().x;
		listenerOri[4] = camera->getUp().y;
		listenerOri[5] = camera->getUp().z;*/
		
		//Se le tiene que configurar la camara con allistener
		alListenerfv(AL_ORIENTATION, listenerOri);
		//Vamos iterando sobre el arrreglo que creamos donde colocamos los true
		//si es true, lo remplazas pro false y unicamente lo reproduce una vez
		//Jugar con la logica por la colisión de un Objeto
		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);//el indice que queremos que reproduzca, 0,1,2,3, etc.
			}
		}
		
	}
}

void prepareScene(){

	skyboxSphere.setShader(&shaderSkybox);
	//Roca
	modelRock.setShader(&shaderMulLighting);
	//Aircraft
	modelAircraft.setShader(&shaderMulLighting);

	//Puerta Cerrad
	modelPuertaCerrada.setShader(&shaderMulLighting);
	//Fantasma
	modelFantasma.setShader(&shaderMulLighting);

	//Modelos de los letreros
	//Letrero de fin
	modelLetreroFin.setShader(&shaderMulLighting);
	//Letrero de inicio
	modelLetreroInicio.setShader(&shaderMulLighting);
	//Letrero de Completo
	modelLetreroCompleto.setShader(&shaderMulLighting);
	//Letrero de continuar
	modelLetreroContinuar.setShader(&shaderMulLighting);

	//Puerta Abierta
	modelPuertaDerecha.setShader(&shaderMulLighting);
	modelPuertaIzquierda.setShader(&shaderMulLighting);
	modelMarco.setShader(&shaderMulLighting);

	//Princesa
	modelPrincesa.setShader(&shaderMulLighting);

	modelAntorcha.setShader(&shaderMulLighting);
	modelMuro.setShader(&shaderMulLighting);
	modeloLaberinto.setShader(&shaderMulLighting);
	modeloArco.setShader(&shaderMulLighting);
	modeloJarron.setShader(&shaderMulLighting);

	terrain.setShader(&shaderTerrain);

	// Helicopter
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.setShader(&shaderMulLighting);

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	skyboxSphere.setShader(&shaderDepth);

	modelRock.setShader(&shaderDepth);

	modelAircraft.setShader(&shaderDepth);

	//ModelosAgregados
	modelAntorcha.setShader(&shaderDepth);
	modelMuro.setShader(&shaderDepth);
	modeloLaberinto.setShader(&shaderDepth);
	modeloArco.setShader(&shaderDepth);
	modeloJarron.setShader(&shaderDepth);
	modelPuertaCerrada.setShader(&shaderDepth);
	//Puerta Abierta
	modelPuertaDerecha.setShader(&shaderDepth);
	modelPuertaIzquierda.setShader(&shaderDepth);
	modelMarco.setShader(&shaderDepth);
	//Princesa
	modelPrincesa.setShader(&shaderDepth);

	//Modelos de los letreros
	//Letrero de fin
	modelLetreroFin.setShader(&shaderDepth);
	//Letrero de inicio
	modelLetreroInicio.setShader(&shaderDepth);
	//Letrero de Completo
	modelLetreroCompleto.setShader(&shaderDepth);
	//Letrero de continuar
	modelLetreroContinuar.setShader(&shaderDepth);


	terrain.setShader(&shaderDepth);

	// Helicopter
	modelHeliChasis.setShader(&shaderDepth);
	modelHeliHeli.setShader(&shaderDepth);
	// Lamborginhi
	modelLambo.setShader(&shaderDepth);
	modelLamboLeftDor.setShader(&shaderDepth);
	modelLamboRightDor.setShader(&shaderDepth);
	modelLamboFrontLeftWheel.setShader(&shaderDepth);
	modelLamboFrontRightWheel.setShader(&shaderDepth);
	modelLamboRearLeftWheel.setShader(&shaderDepth);
	modelLamboRearRightWheel.setShader(&shaderDepth);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderDepth);
	modelDartLegoMask.setShader(&shaderDepth);
	modelDartLegoHead.setShader(&shaderDepth);
	modelDartLegoLeftArm.setShader(&shaderDepth);
	modelDartLegoRightArm.setShader(&shaderDepth);
	modelDartLegoLeftHand.setShader(&shaderDepth);
	modelDartLegoRightHand.setShader(&shaderDepth);
	modelDartLegoLeftLeg.setShader(&shaderDepth);
	modelDartLegoRightLeg.setShader(&shaderDepth);

	//Modelo del fantasma
	modelFantasma.setShader(&shaderDepth);

	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLampPost2.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);
}

void renderScene(bool renderParticles){
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

	//Render de la puerta cerrada
	//glm::mat4 modelMatrixPuertaCerradaBody = glm::mat4(modelMatrixPuertaCerrda);
	//modelMatrixPuertaCerrda[3][1] = terrain.getHeightTerrain(modelMatrixPuertaCerrda[3][0], modelMatrixPuertaCerrda[3][2]);
	//modelMatrixPuertaCerrda = glm::scale(modelMatrixPuertaCerrda, glm::vec3(2.0, 2.0, 2.0));//Este casi es incesario
	modelMatrixPuertaCerrda[3][1] = terrain.getHeightTerrain(modelMatrixPuertaCerrda[3][0], modelMatrixPuertaCerrda[3][2]);
	modelPuertaCerrada.setScale(glm::vec3(escalaXp, escalaYp, escalaZp));
	modelPuertaCerrada.render(modelMatrixPuertaCerrda);

	//Render de la princesa



	//Render del Arco
	glm::mat4 modelMatrixArcoBody = glm::mat4(modelMatrixArco);
	modelMatrixArcoBody[3][1] = terrain.getHeightTerrain(modelMatrixArcoBody[3][0], modelMatrixArcoBody[3][2]) + 4.0;
	modelMatrixArcoBody = glm::scale(modelMatrixArcoBody, glm::vec3(0.7, 0.7, 0.7));
	modeloArco.render(modelMatrixArcoBody);

	//Render de la puerta abierta
	modelMatrixPuertaMarco = glm::mat4(modelMatrixPuerta);
	modelMatrixPuertaMarco[3][1] = terrain.getHeightTerrain(modelMatrixPuertaMarco[3][0], modelMatrixPuertaMarco[3][2]);
	modelMatrixPuertaMarco = glm::scale(modelMatrixPuertaMarco, glm::vec3(escalaXA, escalaYA, escalaZA));
	//modelMarco.setScale(glm::vec3(10.0, 10.0, 10.0));
	modelMarco.render(modelMatrixPuertaMarco);

	modelMatrixPuertaIzquierda = glm::mat4(modelMatrixPuertaMarco);
	modelMatrixPuertaIzquierda[3][1] = terrain.getHeightTerrain(modelMatrixPuertaIzquierda[3][0], modelMatrixPuertaIzquierda[3][2]);
	modelMatrixPuertaIzquierda = glm::translate(modelMatrixPuertaIzquierda, glm::vec3(0.169327, 0.324083, -0.040858));
	//modelMatrixPuertaIzquierda = glm::rotate(modelMatrixPuertaIzquierda, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixPuertaIzquierda = glm::rotate(modelMatrixPuertaIzquierda, rotBuzzLeftArm, glm::vec3(0, 1, 0));
	//modelMatrixPuertaIzquierda = glm::rotate(modelMatrixPuertaIzquierda, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixPuertaIzquierda = glm::translate(modelMatrixPuertaIzquierda, glm::vec3(-0.169327, -0.324083, 0.040858));
	modelPuertaIzquierda.render(modelMatrixPuertaIzquierda);

	modelMatrixPuertaDerecha = glm::mat4(modelMatrixPuertaMarco);
	modelMatrixPuertaDerecha[3][1] = terrain.getHeightTerrain(modelMatrixPuertaDerecha[3][0], modelMatrixPuertaDerecha[3][2]);
	modelMatrixPuertaDerecha = glm::translate(modelMatrixPuertaDerecha, glm::vec3(-0.188654, 0.323743, -0.044574));
	//modelMatrixPuertaDerecha = glm::rotate(modelMatrixPuertaDerecha, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixPuertaDerecha = glm::rotate(modelMatrixPuertaDerecha, rotBuzzRightArm, glm::vec3(0, 1, 0));
	//modelMatrixPuertaDerecha = glm::rotate(modelMatrixPuertaDerecha, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixPuertaDerecha = glm::translate(modelMatrixPuertaDerecha, glm::vec3(0.188654, -0.323743, 0.044574));
	modelPuertaDerecha.render(modelMatrixPuertaDerecha);
	//Para desplazar el collider de la puerta abierta
	modelMatrixPuerta[3][1] = terrain.getHeightTerrain(modelMatrixPuerta[3][0], modelMatrixPuerta[3][2])+abrir;
	
	

	//Render de los jarrones
	for (int i = 0; i < jarronPosition.size(); i++) {
		if (jarronE[i]) {
			jarronPosition[i].y = terrain.getHeightTerrain(jarronPosition[i].x, jarronPosition[i].z);
			modeloJarron.setPosition(jarronPosition[i]);
			modeloJarron.setScale(glm::vec3(2.0, 2.0, 2.0));
			modeloJarron.setOrientation(glm::vec3(0, jarronOrientation[i], 0));
			modeloJarron.render();
		}
	}

	//Render de las paredes del muro
	/*for (int i = 0; i < muro1Position.size(); i++) {
		muro1Position[i].y = terrain.getHeightTerrain(muro1Position[i].x, muro1Position[i].z);
		modelMuro.setPosition(muro1Position[i]);
		if (i == 0) {
			modelMuro.setScale(glm::vec3(62, escalaY, escalaZ));
		}else if (i == 1) {
			modelMuro.setScale(glm::vec3(62, escalaY, escalaZ));
		}
		else if (i == 2) {
			modelMuro.setScale(glm::vec3(110, escalaY, escalaZ));
		}
		else if (i == 3) {
			modelMuro.setScale(glm::vec3(132, escalaY, escalaZ));
		}
		else if (i == 4) {
			modelMuro.setScale(glm::vec3(110, escalaY, escalaZ));
		}
		else {
			modelMuro.setScale(glm::vec3(escalaX, escalaY, escalaZ));
		}
		modelMuro.setOrientation(glm::vec3(0, muro1Orientation[i], 0));
		modelMuro.render();
	}*/

	//Render de los muros del laberinto
	for (int i = 0; i < laberintoPosition.size(); i++) {
		//laberintoPosition[i].y = terrain.getHeightTerrain(laberintoPosition[i].x, laberintoPosition[i].z)-10;
		laberintoPosition[i].y = terrain.getHeightTerrain(laberintoPosition[i].x, laberintoPosition[i].z);
		modeloLaberinto.setPosition(laberintoPosition[i]);
		//modeloLaberinto.setScale(glm::vec3(8.6, 1.5, 6.5));
		modeloLaberinto.setScale(glm::vec3(1.0, 1.5, 1.0));
		modeloLaberinto.setOrientation(glm::vec3(0, laberintoOrientation[i], 0));
		modeloLaberinto.render();
	}

	// Render the lamps
	/*for (int i = 0; i < lamp1Position.size(); i++){
		lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
		modelLamp1.setPosition(lamp1Position[i]);
		modelLamp1.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
		modelLamp1.render();
	}*/

	// Render de las lamparas y antorchas
	for (int i = 0; i < lamp1Position.size(); i++) {
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

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
	modelGrass.setPosition(grassPosition);
	modelGrass.render();
	glEnable(GL_CULL_FACE);

	// Fountain
	glDisable(GL_CULL_FACE);
	modelFountain.render(modelMatrixFountain);
	glEnable(GL_CULL_FACE);

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

	//Render de los fantasmas
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < fantasmaPosition.size(); i++) {
		if (fantasmasVivos[i]==true) {
			fantasmaPosition[i].y = terrain.getHeightTerrain(fantasmaPosition[i].x, fantasmaPosition[i].z) + 2;
			modelFantasma.setPosition(fantasmaPosition[i]);
			modelFantasma.setScale(glm::vec3(1.0, 1.0, 1.0));
			modelFantasma.setOrientation(glm::vec3(0, fantasmaOrientation[i], 0));
			modelFantasma.render();
		}
	}
	glEnable(GL_CULL_FACE);

	// Fantasma
	// Se deshabilita el cull faces IMPORTANTE para la capa
	/*glDisable(GL_CULL_FACE);
	modelMatrixFantasma[3][1] = terrain.getHeightTerrain(modelMatrixFantasma[3][0], modelMatrixFantasma[3][2])+2.0;
	glm::mat4 modelMatrixFantasmaBody = glm::mat4(modelMatrixFantasma);
	modelMatrixFantasmaBody = glm::scale(modelMatrixFantasmaBody, glm::vec3(1.0, 1.0, 1.0));
	if (fantasmaVivo) {
		//modelFantasma.setPosition(glm::vec3(modelMatrixFantasma[3]));
		modelFantasma.render(modelMatrixFantasmaBody);
	}
	// Se regresa el cull faces IMPORTANTE para la capa
	glEnable(GL_CULL_FACE);*/

	

	// Princesa
	// Se deshabilita el cull faces IMPORTANTE para la capa
	//glDisable(GL_CULL_FACE);
	modelMatrixPrincesa[3][1] = terrain.getHeightTerrain(modelMatrixPrincesa[3][0], modelMatrixPrincesa[3][2]);
	glm::mat4 modelMatrixPrincesaBody = glm::mat4(modelMatrixPrincesa);
	modelMatrixPrincesaBody = glm::scale(modelMatrixPrincesaBody, glm::vec3(0.021, 0.021, 0.021));
	modelPrincesa.setAnimationIndex(0);
	modelPrincesa.render(modelMatrixPrincesaBody);
	// Se regresa el cull faces IMPORTANTE para la capa
	//glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
	glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
	modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021, 0.021, 0.021));
	
	if (vivo) {
		mayowModelAnimate.setAnimationIndex(animationIndex);
	}
	else {
		mayowModelAnimate.setAnimationIndex(4);
	}
	mayowModelAnimate.render(modelMatrixMayowBody);

	// Letrero
	// Se deshabilita el cull faces IMPORTANTE para la capa
	if (vivo == false) {
		glDisable(GL_CULL_FACE);
		modelMatrixLetreroFin[3][1] = terrain.getHeightTerrain(modelMatrixLetreroFin[3][0], modelMatrixLetreroFin[3][2]);
		glm::mat4 modelMatrixLetreroBody = glm::mat4(modelMatrixLetreroFin);
		modelMatrixLetreroBody = glm::scale(modelMatrixLetreroBody, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixLetreroBody = glm::translate(modelMatrixLetreroBody,
			glm::vec3(modelMatrixMayow[3][0],
				modelMatrixMayow[3][1]+2,
				modelMatrixMayow[3][2]));
		//modelMatrixLetreroBody = glm::rotate(modelMatrixLetreroBody, glm::radians(0.0f), glm::vec3(0, 1, 0));
		modelMatrixLetreroBody = glm::rotate(modelMatrixMayow, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelLetreroFin.render(modelMatrixLetreroBody);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		glDisable(GL_CULL_FACE);
		//modelMatrixLetreroFin[3][1] = terrain.getHeightTerrain(modelMatrixLetreroFin[3][0], modelMatrixLetreroFin[3][2]) + 2.0;
		modelMatrixLetreroInicio[3][1] = terrain.getHeightTerrain(modelMatrixLetreroInicio[3][0], modelMatrixLetreroInicio[3][2]);
		glm::mat4 modelMatrixLetreroBody2 = glm::mat4(modelMatrixLetreroInicio);
		modelMatrixLetreroBody2 = glm::scale(modelMatrixLetreroBody2, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixLetreroBody2 = glm::translate(modelMatrixLetreroBody2,
			glm::vec3(modelMatrixMayow[3][0],
				modelMatrixMayow[3][1],
				modelMatrixMayow[3][2]));
		//modelMatrixLetreroBody = glm::rotate(modelMatrixLetreroBody, glm::radians(0.0f), glm::vec3(0, 1, 0));
		modelMatrixLetreroBody2 = glm::rotate(modelMatrixMayow, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelLetreroContinuar.render(modelMatrixLetreroBody2);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);
	}
	//std::cout << ganador << std::endl;
	if (complet == true) {
		glDisable(GL_CULL_FACE);
		modelMatrixLetreroCompleto [3][1] = terrain.getHeightTerrain(modelMatrixLetreroCompleto[3][0], modelMatrixLetreroCompleto[3][2]);
		glm::mat4 modelMatrixLetreroBody = glm::mat4(modelMatrixLetreroCompleto);
		modelMatrixLetreroBody = glm::scale(modelMatrixLetreroBody, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixLetreroBody = glm::translate(modelMatrixLetreroBody,
			glm::vec3(modelMatrixMayow[3][0],
				modelMatrixMayow[3][1],
				modelMatrixMayow[3][2]));
		modelMatrixLetreroBody = glm::rotate(modelMatrixMayow, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelLetreroCompleto.render(modelMatrixLetreroBody);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		glDisable(GL_CULL_FACE);
		//modelMatrixLetreroFin[3][1] = terrain.getHeightTerrain(modelMatrixLetreroFin[3][0], modelMatrixLetreroFin[3][2]) + 2.0;
		modelMatrixLetreroInicio[3][1] = terrain.getHeightTerrain(modelMatrixLetreroInicio[3][0], modelMatrixLetreroInicio[3][2]);
		glm::mat4 modelMatrixLetreroBody2 = glm::mat4(modelMatrixLetreroInicio);
		modelMatrixLetreroBody2 = glm::scale(modelMatrixLetreroBody2, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixLetreroBody2 = glm::translate(modelMatrixLetreroBody2,
			glm::vec3(modelMatrixMayow[3][0],
				modelMatrixMayow[3][1],
				modelMatrixMayow[3][2]));
		//modelMatrixLetreroBody2 = glm::rotate(modelMatrixMayow, glm::radians(90.0f), glm::vec3(0, 1, 0));
		modelMatrixLetreroBody2 = glm::rotate(modelMatrixMayow, glm::radians(180.0f), glm::vec3(0, 1, 0));
		modelLetreroContinuar.render(modelMatrixLetreroBody2);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);
	}
	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	// Update the lambo
	blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	// Update the helicopter
	blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(it->second.first.compare("aircraft") == 0){
			// Render for the aircraft model
			glm::mat4 modelMatrixAircraftBlend = glm::mat4(modelMatrixAircraft);
			modelMatrixAircraftBlend[3][1] = terrain.getHeightTerrain(modelMatrixAircraftBlend[3][0], modelMatrixAircraftBlend[3][2]) + 2.0;
			modelAircraft.render(modelMatrixAircraftBlend);
		}
		else if(it->second.first.compare("lambo") == 0){
			// Lambo car
			glm::mat4 modelMatrixLamboBlend = glm::mat4(modelMatrixLambo);
			modelMatrixLamboBlend[3][1] = terrain.getHeightTerrain(modelMatrixLamboBlend[3][0], modelMatrixLamboBlend[3][2]);
			modelMatrixLamboBlend = glm::scale(modelMatrixLamboBlend, glm::vec3(1.3, 1.3, 1.3));
			modelLambo.render(modelMatrixLamboBlend);
			glActiveTexture(GL_TEXTURE0);
			glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboBlend);
			modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
			modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
			modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
			modelLamboLeftDor.render(modelMatrixLamboLeftDor);
			modelLamboRightDor.render(modelMatrixLamboBlend);
			modelLamboFrontLeftWheel.render(modelMatrixLamboBlend);
			modelLamboFrontRightWheel.render(modelMatrixLamboBlend);
			modelLamboRearLeftWheel.render(modelMatrixLamboBlend);
			modelLamboRearRightWheel.render(modelMatrixLamboBlend);
			// Se regresa el cull faces IMPORTANTE para las puertas
		}
		else if(it->second.first.compare("heli") == 0){
			// Helicopter
			glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
			modelHeliChasis.render(modelMatrixHeliChasis);

			glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
			modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
			modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
			modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
			modelHeliHeli.render(modelMatrixHeliHeli);
		}
		else if(renderParticles && it->second.first.compare("fountain") == 0){
			/**********
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 7;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if(currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.03f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesFountain.turnOff();
			/**********
			 * End Render particles systems
			 */
		}
		else if(renderParticles && it->second.first.compare("fire1") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1-drawBuf]);
			glVertexAttribDivisor(0,0);
			glVertexAttribDivisor(1,0);
			glVertexAttribDivisor(2,0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 9.0;
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0,1);
			glVertexAttribDivisor(1,1);
			glVertexAttribDivisor(2,1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************+
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}
		else if (renderParticles && it->second.first.compare("fire2") == 0) {
		/**********
		 * Init Render particles systems
		 */
		 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		 shaderParticlesFire.setInt("Pass", 1);
		 shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		 shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_1D, texId);
		 glEnable(GL_RASTERIZER_DISCARD);
		 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		 glBeginTransformFeedback(GL_POINTS);
		 glBindVertexArray(particleArray[1 - drawBuf]);
		 glVertexAttribDivisor(0, 0);
		 glVertexAttribDivisor(1, 0);
		 glVertexAttribDivisor(2, 0);
		 glDrawArrays(GL_POINTS, 0, nParticlesFire);
		 glEndTransformFeedback();
		 glDisable(GL_RASTERIZER_DISCARD);

		 shaderParticlesFire.setInt("Pass", 2);
		 glm::mat4 modelFireParticles = glm::mat4(1.0);
		 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		 //modelFireParticles = glm::translate(modelFireParticles, modelMatrixMayow[3]);
		 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 9.0;
		 shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		 shaderParticlesFire.turnOn();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		 glDepthMask(GL_FALSE);
		 glBindVertexArray(particleArray[drawBuf]);
		 glVertexAttribDivisor(0, 1);
		 glVertexAttribDivisor(1, 1);
		 glVertexAttribDivisor(2, 1);
		 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		 glBindVertexArray(0);
		 glDepthMask(GL_TRUE);
		 drawBuf = 1 - drawBuf;
		 shaderParticlesFire.turnOff();

		 /****************************+
		  * Open AL sound data
		  */
		 source1Pos[0] = modelFireParticles[3].x;
		 source1Pos[1] = modelFireParticles[3].y;
		 source1Pos[2] = modelFireParticles[3].z;
		 alSourcefv(source[1], AL_POSITION, source1Pos);

		 /**********
		  * End Render particles systems
		  */
		}
		else if (renderParticles && it->second.first.compare("fire3") == 0) {
		/**********
		 * Init Render particles systems
		 */
		 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		 shaderParticlesFire.setInt("Pass", 1);
		 shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		 shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_1D, texId);
		 glEnable(GL_RASTERIZER_DISCARD);
		 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		 glBeginTransformFeedback(GL_POINTS);
		 glBindVertexArray(particleArray[1 - drawBuf]);
		 glVertexAttribDivisor(0, 0);
		 glVertexAttribDivisor(1, 0);
		 glVertexAttribDivisor(2, 0);
		 glDrawArrays(GL_POINTS, 0, nParticlesFire);
		 glEndTransformFeedback();
		 glDisable(GL_RASTERIZER_DISCARD);

		 shaderParticlesFire.setInt("Pass", 2);
		 glm::mat4 modelFireParticles = glm::mat4(1.0);
		 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		 //modelFireParticles = glm::translate(modelFireParticles, modelMatrixMayow[3]);
		 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 7.0;
		 shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		 shaderParticlesFire.turnOn();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		 glDepthMask(GL_FALSE);
		 glBindVertexArray(particleArray[drawBuf]);
		 glVertexAttribDivisor(0, 1);
		 glVertexAttribDivisor(1, 1);
		 glVertexAttribDivisor(2, 1);
		 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		 glBindVertexArray(0);
		 glDepthMask(GL_TRUE);
		 drawBuf = 1 - drawBuf;
		 shaderParticlesFire.turnOff();

		 /****************************+
		  * Open AL sound data
		  */
		 source1Pos[0] = modelFireParticles[3].x;
		 source1Pos[1] = modelFireParticles[3].y;
		 source1Pos[2] = modelFireParticles[3].z;
		 alSourcefv(source[1], AL_POSITION, source1Pos);

		 /**********
		  * End Render particles systems
		  */
		}
		else if (renderParticles && it->second.first.compare("fire4") == 0) {
		/**********
		 * Init Render particles systems
		 */
		 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		 shaderParticlesFire.setInt("Pass", 1);
		 shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		 shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_1D, texId);
		 glEnable(GL_RASTERIZER_DISCARD);
		 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		 glBeginTransformFeedback(GL_POINTS);
		 glBindVertexArray(particleArray[1 - drawBuf]);
		 glVertexAttribDivisor(0, 0);
		 glVertexAttribDivisor(1, 0);
		 glVertexAttribDivisor(2, 0);
		 glDrawArrays(GL_POINTS, 0, nParticlesFire);
		 glEndTransformFeedback();
		 glDisable(GL_RASTERIZER_DISCARD);

		 shaderParticlesFire.setInt("Pass", 2);
		 glm::mat4 modelFireParticles = glm::mat4(1.0);
		 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		 //modelFireParticles = glm::translate(modelFireParticles, modelMatrixMayow[3]);
		 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 7.0;
		 shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		 shaderParticlesFire.turnOn();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		 glDepthMask(GL_FALSE);
		 glBindVertexArray(particleArray[drawBuf]);
		 glVertexAttribDivisor(0, 1);
		 glVertexAttribDivisor(1, 1);
		 glVertexAttribDivisor(2, 1);
		 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		 glBindVertexArray(0);
		 glDepthMask(GL_TRUE);
		 drawBuf = 1 - drawBuf;
		 shaderParticlesFire.turnOff();

		 /****************************+
		  * Open AL sound data
		  */
		 source1Pos[0] = modelFireParticles[3].x;
		 source1Pos[1] = modelFireParticles[3].y;
		 source1Pos[2] = modelFireParticles[3].z;
		 alSourcefv(source[1], AL_POSITION, source1Pos);

		 /**********
		  * End Render particles systems
		  */
		}
		else if (renderParticles && it->second.first.compare("fire5") == 0) {
		/**********
		 * Init Render particles systems
		 */
		 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		 shaderParticlesFire.setInt("Pass", 1);
		 shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		 shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_1D, texId);
		 glEnable(GL_RASTERIZER_DISCARD);
		 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		 glBeginTransformFeedback(GL_POINTS);
		 glBindVertexArray(particleArray[1 - drawBuf]);
		 glVertexAttribDivisor(0, 0);
		 glVertexAttribDivisor(1, 0);
		 glVertexAttribDivisor(2, 0);
		 glDrawArrays(GL_POINTS, 0, nParticlesFire);
		 glEndTransformFeedback();
		 glDisable(GL_RASTERIZER_DISCARD);

		 shaderParticlesFire.setInt("Pass", 2);
		 glm::mat4 modelFireParticles = glm::mat4(1.0);
		 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		 //modelFireParticles = glm::translate(modelFireParticles, modelMatrixMayow[3]);
		 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 7.0;
		 shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		 shaderParticlesFire.turnOn();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		 glDepthMask(GL_FALSE);
		 glBindVertexArray(particleArray[drawBuf]);
		 glVertexAttribDivisor(0, 1);
		 glVertexAttribDivisor(1, 1);
		 glVertexAttribDivisor(2, 1);
		 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		 glBindVertexArray(0);
		 glDepthMask(GL_TRUE);
		 drawBuf = 1 - drawBuf;
		 shaderParticlesFire.turnOff();

		 /****************************+
		  * Open AL sound data
		  */
		 source1Pos[0] = modelFireParticles[3].x;
		 source1Pos[1] = modelFireParticles[3].y;
		 source1Pos[2] = modelFireParticles[3].z;
		 alSourcefv(source[1], AL_POSITION, source1Pos);

		 /**********
		  * End Render particles systems
		  */
		}
		else if (renderParticles && it->second.first.compare("fire6") == 0) {
		/**********
		 * Init Render particles systems
		 */
		 lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		 currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		 shaderParticlesFire.setInt("Pass", 1);
		 shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		 shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_1D, texId);
		 glEnable(GL_RASTERIZER_DISCARD);
		 glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		 glBeginTransformFeedback(GL_POINTS);
		 glBindVertexArray(particleArray[1 - drawBuf]);
		 glVertexAttribDivisor(0, 0);
		 glVertexAttribDivisor(1, 0);
		 glVertexAttribDivisor(2, 0);
		 glDrawArrays(GL_POINTS, 0, nParticlesFire);
		 glEndTransformFeedback();
		 glDisable(GL_RASTERIZER_DISCARD);

		 shaderParticlesFire.setInt("Pass", 2);
		 glm::mat4 modelFireParticles = glm::mat4(1.0);
		 modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		 //modelFireParticles = glm::translate(modelFireParticles, modelMatrixMayow[3]);
		 modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]) + 7.0;
		 shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		 shaderParticlesFire.turnOn();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		 glDepthMask(GL_FALSE);
		 glBindVertexArray(particleArray[drawBuf]);
		 glVertexAttribDivisor(0, 1);
		 glVertexAttribDivisor(1, 1);
		 glVertexAttribDivisor(2, 1);
		 glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		 glBindVertexArray(0);
		 glDepthMask(GL_TRUE);
		 drawBuf = 1 - drawBuf;
		 shaderParticlesFire.turnOff();

		 /****************************+
		  * Open AL sound data
		  */
		 source1Pos[0] = modelFireParticles[3].x;
		 source1Pos[1] = modelFireParticles[3].y;
		 source1Pos[2] = modelFireParticles[3].z;
		 alSourcefv(source[1], AL_POSITION, source1Pos);

		 /**********
		  * End Render particles systems
		  */
		}

	}
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
