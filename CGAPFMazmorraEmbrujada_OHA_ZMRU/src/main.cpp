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
//std::shared_ptr<FirstPersonCamera> camera2(new FirstPersonCamera());


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
//Modelo del fantasma
Model modelFantasma;
//Antorcha
Model modelAntorcha;
//Modelo del muro
Model modelMuro;
//Modloe del laberinto
Model modeloLaberinto;
//Model jarron
Model modeloJarron;
//Modelo del arco
Model modeloArco;
//Modelo puerta cerrada
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
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
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
int ganador = 0;
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
	glm::vec3(48.2, 0.0, 50.7),

	glm::vec3(-44.2, 0.0, 50.7),

	glm::vec3(-85.8, 0.0, -20.0),

	glm::vec3(6.0, 0.0, -100.0),//Muro central 

	glm::vec3(85.1, 0.0, -29.0),
	glm::vec3(6.0, 0.0, 99.0),//Muro frontal
	glm::vec3(-99.0, 0.0, -5.0),
	glm::vec3(99.0, 0.0, -5.0)
};

//Vector de orientación de Muros
std::vector<float> muro1Orientation = {
	//Viendolo de frente
	//Orientaciones frontales:
	0.0, 0.0,
	//Orientaciones lado IZQUIERDO
	-90.0,
	//Orientaciones traceras
	0.0,
	//Orientaciones lado Derecho
	90.0,
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

};
std::vector<float> lamp1Orientation = {
	//-17.0, -82.67, 23.70,
	//Antorchas Frontales
	-180.0, -180.0, 0.0, 0.0, -180.0, -180.0, //0.0, 0.0, 0.0, 0.0, 0.0, 0.0,

};

std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90 };

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
bool fantasmasVivos[] = { true, true, true, true, true };
//Vector de vidas de los fantasmas
std::vector<int> fantasmasVidas = { 2,2,2,2,2 };

//Vector de etiquetas de jarrones

//Vector de indicadores de jarrones
//std::vector<int> jarronId = { 1, 1 };
bool jarronE[] = { true, true, true, true , true , true };

bool dibujaJarron = true;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
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
std::vector<bool> sourcesPlay = { true, true, true, true, true, true, false, true, true };

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
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
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

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

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
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

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
	}
	else
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
	}
	else
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
	}
	else
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
	}
	else
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
	}
	else
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
	for (int i = 0; i < randData.size(); i++) {
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
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
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
	buffer[2] = alutCreateBufferFromFile("../sounds/AmbienteGjost.wav");
	//Creamos el buffer numero 3
	buffer[3] = alutCreateBufferFromFile("../sounds/fire.wav");
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
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
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
	/*modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	modelGrass.destroy();*/
	//modelFountain.destroy();
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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
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

	offsetX = 0;//el angulo que puedes desplazar
	offsetY = 0;//el angulo que puedes desplazar

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && vivo == true && complet == false) {
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(5.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && vivo == true && complet == false) {
		modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-5.0f), glm::vec3(0, 1, 0));
		animationIndex = 0;
	}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && vivo == true && complet == false) {
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 0.7));
		animationIndex = 2;
	}
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && vivo == true && complet == false) {
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -0.7));
		animationIndex = 2;
	}
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && vivo == true) {
		animationIndex = 3;
		sourcesPlay[6] = true;
	}
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		//modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(3.0f), glm::vec3(0, 1, 0));

		//animationIndex = 3;
		if (vivo == false || complet == true) {
			vivo = true;
			//modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, 60));
			modelMatrixMayow = glm::translate(modelMatrixArco, glm::vec3(0, 0, 10));
			modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));
			vida = 10;
			animationIndex = 1;
			for (int i = 0; i < fantasmaPosition.size(); i++) {
				fantasmasVivos[i] = true;
				fantasmasVidas[i] = 2;
			}
			//trasladaPersonaje = true;
			//jarronE[] = { true, true, true, true , true , true };
			for (int i = 0; i < jarronOrientation.size(); i++) {
				jarronE[i] = true;
			}

			complet = false;
			cuentaJarrones = 0;
			//abrir = 0;
			rotBuzzLeftArm = (0.0); abrir = 0.0;
			rotBuzzRightArm = (0.0);

		}

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


	//Modelo del Fantasma
	modelMatrixFantasma = glm::translate(modelMatrixFantasma, glm::vec3(0.0, 0.0, 45.703125));
	modelMatrixFantasma = glm::rotate(modelMatrixFantasma, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	//Variabls para las maquinas de estados
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
	//Modelo del laberinto
	//Modelo de la antorcha
	//Modelo del jarron
	//Aquí Y no importa porque más adelante se definira con respecto al mapa de alturas

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

	//modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	//Matriz de la princesa
	modelMatrixPrincesa = glm::translate(modelMatrixPrincesa, glm::vec3(-4.6875, 0.0, -94.140625));
	modelMatrixPrincesa = glm::rotate(modelMatrixPrincesa, glm::radians(90.0f), glm::vec3(0, 1, 0));

	//modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));
	//Modelo de Mayow
		//rotate  viendolo de frente -90:  <--
		//rotate  viendolo de frente -180:  ^
		//rotate  viendolo de frente 90:  -->
		//rotate  viendolo de frente 0:  v
	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 60.0));
	//modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(20.0, 0.0, 60.0));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-180.0f), glm::vec3(0, 1, 0));

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	//Libreria estandar que se creo  para poder crear apuntaores a programas.
	//Posición de luz, apuntador a la camra, distancia maxima de alcance, plano cercano y el  FOV(filed of view)
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		animationIndex = 1;
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
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		axis = glm::axis(glm::quat_cast(modelMatrixMayow));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));//Angulo en x
		target = glm::vec3(modelMatrixMayow[3].x, modelMatrixMayow[3].y + 10, modelMatrixMayow[3].z);//Aqui se dice que tan distante del personaje


		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		if (modelSelected == 1)
			angleTarget -= glm::radians(45.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		shadowBox->getCenter();

		glm::mat4 lightProjection = glm::mat4(1.0), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;

		lightView = glm::lookAt(shadowBox->getCenter(), shadowBox->getCenter() + glm::normalize(-lightPos), glm::vec3(0, 1.0f, 0));

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


		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Orientation.size());

		for (int i = 0; i < lamp1Position.size(); i++) {
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
		for (int i = 0; i < lamp2Position.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
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
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/



		 // collinder de los fantasmas
		for (int i = 0; i < fantasmaPosition.size(); i++) {
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
			}
			else if (i == 1) {
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
			}
			else if (i == 1) {
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
		for (int i = 0; i < lamp1Position.size(); i++) {

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
			//}
		}

		// Collider de mayow
		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);

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
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					int op;
					std::istringstream(it->first) >> op;
					jarronE[op] = false;

					cuentaJarrones = 0;
					for (int i = 0; i <= jarronPosition.size(); i++) {
						if (jarronE[i] == false) {
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
					if (it->first == "Princesa" && jt->first == "mayow") {
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
					if (it->first == "fantasma0" && fantasmasVivos[0] == true) {
						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2) {
							vida = vida - 1;
							std::cout << "Colision " << it->first << " with "
								<< jt->first << vida << std::endl;
							if (vida <= 0) {
								vivo = false;
							}
						}
						else if (animationIndex == 3) {
							fantasmasVidas[0] = fantasmasVidas[0] - 1;
							std::cout << "Vida fantasma" << fantasmasVidas[0] << std::endl;
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

						if (animationIndex == 1 || animationIndex == 0 || animationIndex == 2) {
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
					//if (jt->first.compare("dart") == 0)
						//modelMatrixDart = std::get<1>(jt->second);
				}
			}
		}

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/

		 /*******************************************
		  * State machines
		  *******************************************/

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
			if (numberAdvance == 0) {
				fantasmaPosition[0].x -= 0.5;
			}
			else if (numberAdvance == 1) {
				fantasmaPosition[0].z += 0.5;
			}
			else if (numberAdvance == 2) {
				fantasmaPosition[0].x += 0.5;
			}
			else if (numberAdvance == 3) {
				fantasmaPosition[0].z -= 0.5;
			}
			else if (numberAdvance == 4) {
				fantasmaPosition[0].x -= 0.5;
			}

			advanceCount += 0.1;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
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
			if (advanceCount3 > maxAdvance3) {
				advanceCount3 = 0;
				numberAdvance3++;
				state3 = 2;
			}
			break;
		case 2:
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
			if (advanceCount4 > maxAdvance4) {
				advanceCount4 = 0;
				numberAdvance4++;
				state4 = 2;
			}
			break;
		case 2:
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

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixArco[3].x;
		source0Pos[1] = modelMatrixArco[3].y;
		source0Pos[2] = modelMatrixArco[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = 25;
		source2Pos[1] = 5;
		source2Pos[2] = 0;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini. 13.4, 0.0, 52.8
		source3Pos[0] = 13.4;
		source3Pos[1] = 10.0;
		source3Pos[2] = 52.8;
		//Indicamos cual es la fuente de sonido que queremos enviarle
		//Posicion y valor
		alSourcefv(source[3], AL_POSITION, source3Pos);

		//indicarle cual es la posición del sonido, para este caso se la ponemos en el lamborghini.
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
		source6Pos[0] = modelMatrixMayow[3].x + 1;
		source6Pos[1] = modelMatrixMayow[3].y + 1;
		source6Pos[2] = modelMatrixMayow[3].z + 1;
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


		//Se le tiene que configurar la camara con allistener
		alListenerfv(AL_ORIENTATION, listenerOri);
		//Vamos iterando sobre el arrreglo que creamos donde colocamos los true
		//si es true, lo remplazas pro false y unicamente lo reproduce una vez
		//Jugar con la logica por la colisión de un Objeto
		for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
			if (sourcesPlay[i]) {
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);//el indice que queremos que reproduzca, 0,1,2,3, etc.
			}
		}

	}
}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);


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

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

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

	//Modelo del fantasma
	modelFantasma.setShader(&shaderDepth);
	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {
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


	 //Render de la puerta cerrada
	modelMatrixPuertaCerrda[3][1] = terrain.getHeightTerrain(modelMatrixPuertaCerrda[3][0], modelMatrixPuertaCerrda[3][2]);
	modelPuertaCerrada.setScale(glm::vec3(escalaXp, escalaYp, escalaZp));
	modelPuertaCerrada.render(modelMatrixPuertaCerrda);

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
	modelMatrixPuerta[3][1] = terrain.getHeightTerrain(modelMatrixPuerta[3][0], modelMatrixPuerta[3][2]) + abrir;



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

	// Render de las lamparas y antorchas
	for (int i = 0; i < lamp1Position.size(); i++) {

		lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z) + 4.0;
		modelAntorcha.setPosition(lamp1Position[i]);
		modelAntorcha.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelAntorcha.setOrientation(glm::vec3(15, lamp1Orientation[i], 0));
		modelAntorcha.render();

	}

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	//Render de los fantasmas
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < fantasmaPosition.size(); i++) {
		if (fantasmasVivos[i] == true) {
			fantasmaPosition[i].y = terrain.getHeightTerrain(fantasmaPosition[i].x, fantasmaPosition[i].z) + 2;
			modelFantasma.setPosition(fantasmaPosition[i]);
			modelFantasma.setScale(glm::vec3(1.0, 1.0, 1.0));
			modelFantasma.setOrientation(glm::vec3(0, fantasmaOrientation[i], 0));
			modelFantasma.render();
		}
	}
	glEnable(GL_CULL_FACE);

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
				modelMatrixMayow[3][1] + 2,
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
		modelMatrixLetreroCompleto[3][1] = terrain.getHeightTerrain(modelMatrixLetreroCompleto[3][0], modelMatrixLetreroCompleto[3][2]);
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
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {

		if (renderParticles && it->second.first.compare("fountain") == 0) {
			/**********
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 7;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if (currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
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
		else if (renderParticles && it->second.first.compare("fire1") == 0) {
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