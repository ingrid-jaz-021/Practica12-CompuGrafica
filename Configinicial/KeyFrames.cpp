// Práctica #12
// Serrano Cuevas Ingrid Jazmín
// Fecha de entrega: 7 de noviembre de 2026.
// 319213197

#include <iostream>
#include <cmath>
#include <fstream> // Generar archivo txt
#include <iomanip> // Ocupa numeros decimales

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void cargarAnimacionTruco(void); // Animación Hacer el muerto

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
//float FLegs = 0.0f;
//float RLegs = 0.0f;
float rotFL = 0.0f; // Pierna Delantera Izquierda
float rotFR = 0.0f; // Pierna Delantera Derecha
float rotBL = 0.0f; // Pierna Trasera Izquierda
float rotBR = 0.0f; // Pierna Trasera Derecha
float head = 0.0f;
float tail = 0.0f;
float rotDogX = 0.0f; // Inclinación vertical del torso
float rotDogZ = 0.0f; // Rotación lateral



//KeyFrames
float dogPosX , dogPosY , dogPosZ  ;

#define MAX_FRAMES 9 // Número de KeyFrames maximos que se ocupan
int i_max_steps = 1500; // Velocidad de la animación
int i_curr_steps = 0;
typedef struct _frame {
	
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	// 1. Se agregan las variables para cada parte del perro
	float head; // Movimiento de la cabeza
	float headInc;
	float tail; // Movimiento de la cola
	float tailInc;
	float rotFL; // Movimiento de la pierna izq delantera
	float rotFLInc;
	float rotFR; // Movimiento de la pierna der delantera
	float rotFRInc;
	float rotBL; // Movimiento de la pierna izq trasera
	float rotBLInc;
	float rotBR; // Movimiento de la pierna der trasera
	float rotBRInc;
	float rotDogX; // Inclinación del torso
	float rotDogXInc;
	float rotDogZ; // Rotación lateral
	float rotDogZInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;   // En que punto de la línea del tiempo está

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	// 2. Se guarda las variables en el sistema
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].rotFL = rotFL;
	KeyFrame[FrameIndex].rotFR = rotFR;
	KeyFrame[FrameIndex].rotBL = rotBL;
	KeyFrame[FrameIndex].rotBR = rotBR;
	KeyFrame[FrameIndex].rotDogX = rotDogX;
	KeyFrame[FrameIndex].rotDogZ = rotDogZ;

	FrameIndex++;
}

// Inicializar
void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;

	rotDog = KeyFrame[0].rotDog;
	// 3. Se inicializa
	head = KeyFrame[0].head;
	tail = KeyFrame[0].tail;
	rotFL = KeyFrame[0].rotFL;
	rotFR = KeyFrame[0].rotFR;
	rotBL = KeyFrame[0].rotBL;
	rotBR = KeyFrame[0].rotBR;
	rotDogX = KeyFrame[0].rotDogX;
	rotDogZ = KeyFrame[0].rotDogZ;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	// 4. Calcular la interpolación de la variable
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].rotFLInc = (KeyFrame[playIndex + 1].rotFL - KeyFrame[playIndex].rotFL) / i_max_steps;
	KeyFrame[playIndex].rotFRInc = (KeyFrame[playIndex + 1].rotFR - KeyFrame[playIndex].rotFR) / i_max_steps;
	KeyFrame[playIndex].rotBLInc = (KeyFrame[playIndex + 1].rotBL - KeyFrame[playIndex].rotBL) / i_max_steps;
	KeyFrame[playIndex].rotBRInc = (KeyFrame[playIndex + 1].rotBR - KeyFrame[playIndex].rotBR) / i_max_steps;
	KeyFrame[playIndex].rotDogXInc = (KeyFrame[playIndex + 1].rotDogX - KeyFrame[playIndex].rotDogX) / i_max_steps;
	KeyFrame[playIndex].rotDogZInc = (KeyFrame[playIndex + 1].rotDogZ - KeyFrame[playIndex].rotDogZ) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

}

// Animación - Hacer el muerto
void cargarAnimacionTruco(void)
{
	// Frame 0
	KeyFrame[0].dogPosX = 0.000000f;
	KeyFrame[0].dogPosY = 0.000000f;
	KeyFrame[0].dogPosZ = 0.000000f;
	KeyFrame[0].rotDog = 0.000000f;
	KeyFrame[0].rotDogX = 0.000000f;
	KeyFrame[0].rotDogZ = 0.000000f;
	KeyFrame[0].head = 0.000000f;
	KeyFrame[0].tail = 0.000000f;
	KeyFrame[0].rotFL = 0.000000f;
	KeyFrame[0].rotFR = 0.000000f;
	KeyFrame[0].rotBL = 0.000000f;
	KeyFrame[0].rotBR = 0.000000f;

	// Frame 1
	KeyFrame[1].dogPosX = 0.000000f;
	KeyFrame[1].dogPosY = 0.000000f;
	KeyFrame[1].dogPosZ = -0.151777f;
	KeyFrame[1].rotDog = 0.000000f;
	KeyFrame[1].rotDogX = 0.000000f;
	KeyFrame[1].rotDogZ = 0.000000f;
	KeyFrame[1].head = 0.000000f;
	KeyFrame[1].tail = 0.000000f;
	KeyFrame[1].rotFL = 0.000000f;
	KeyFrame[1].rotFR = 0.000000f;
	KeyFrame[1].rotBL = 0.000000f;
	KeyFrame[1].rotBR = 0.000000f;

	// Frame 2
	KeyFrame[2].dogPosX = 0.000000f;
	KeyFrame[2].dogPosY = -0.082291f;
	KeyFrame[2].dogPosZ = -0.151777f;
	KeyFrame[2].rotDog = 0.000000f;
	KeyFrame[2].rotDogX = -32.653236f;
	KeyFrame[2].rotDogZ = 0.000000f;
	KeyFrame[2].head = 0.000000f;
	KeyFrame[2].tail = 0.000000f;
	KeyFrame[2].rotFL = 0.000000f;
	KeyFrame[2].rotFR = 0.000000f;
	KeyFrame[2].rotBL = -47.835159f;
	KeyFrame[2].rotBR = 42.728424f;

	// Frame 3
	KeyFrame[3].dogPosX = 0.000000f;
	KeyFrame[3].dogPosY = -0.082291f;
	KeyFrame[3].dogPosZ = -0.151777f;
	KeyFrame[3].rotDog = 0.000000f;
	KeyFrame[3].rotDogX = -32.653236f;
	KeyFrame[3].rotDogZ = 0.000000f;
	KeyFrame[3].head = 35.908127f;
	KeyFrame[3].tail = 30.193710f;
	KeyFrame[3].rotFL = 0.713158f;
	KeyFrame[3].rotFR = -64.004135f;
	KeyFrame[3].rotBL = -47.835159f;
	KeyFrame[3].rotBR = 42.728424f;

	// Frame 4
	KeyFrame[4].dogPosX = 0.000000f;
	KeyFrame[4].dogPosY = -0.082291f;
	KeyFrame[4].dogPosZ = -0.151777f;
	KeyFrame[4].rotDog = 0.000000f;
	KeyFrame[4].rotDogX = -32.653236f;
	KeyFrame[4].rotDogZ = 0.000000f;
	KeyFrame[4].head = -28.035355f;
	KeyFrame[4].tail = -39.006042f;
	KeyFrame[4].rotFL = 0.713158f;
	KeyFrame[4].rotFR = 3.117371f;
	KeyFrame[4].rotBL = -47.835159f;
	KeyFrame[4].rotBR = 42.728424f;

	// Frame 5
	KeyFrame[5].dogPosX = 0.000000f;
	KeyFrame[5].dogPosY = -0.082291f;
	KeyFrame[5].dogPosZ = -0.151777f;
	KeyFrame[5].rotDog = 0.000000f;
	KeyFrame[5].rotDogX = -32.653236f;
	KeyFrame[5].rotDogZ = 0.000000f;
	KeyFrame[5].head = -11.402512f;
	KeyFrame[5].tail = -14.337540f;
	KeyFrame[5].rotFL = 0.713158f;
	KeyFrame[5].rotFR = 3.117371f;
	KeyFrame[5].rotBL = -47.835159f;
	KeyFrame[5].rotBR = 42.728424f;

	// Frame 6
	KeyFrame[6].dogPosX = 0.000000f;
	KeyFrame[6].dogPosY = -0.178207f;
	KeyFrame[6].dogPosZ = -0.151777f;
	KeyFrame[6].rotDog = 0.000000f;
	KeyFrame[6].rotDogX = 6.039238f;
	KeyFrame[6].rotDogZ = 79.691696f;
	KeyFrame[6].head = -11.402512f;
	KeyFrame[6].tail = -14.337540f;
	KeyFrame[6].rotFL = 0.713158f;
	KeyFrame[6].rotFR = 3.117371f;
	KeyFrame[6].rotBL = -47.835159f;
	KeyFrame[6].rotBR = 42.728424f;

	// Frame 7
	KeyFrame[7].dogPosX = -0.215057f;
	KeyFrame[7].dogPosY = -0.124588f;
	KeyFrame[7].dogPosZ = -0.151777f;
	KeyFrame[7].rotDog = 0.000000f;
	KeyFrame[7].rotDogX = 6.039238f;
	KeyFrame[7].rotDogZ = 172.971725f;
	KeyFrame[7].head = 6.288528f;
	KeyFrame[7].tail = 1.578903f;
	KeyFrame[7].rotFL = 4.612541f;
	KeyFrame[7].rotFR = 3.117371f;
	KeyFrame[7].rotBL = -0.272751f;
	KeyFrame[7].rotBR = 12.158203f;

	// Frame 8
	KeyFrame[8].dogPosX = -0.215057f;
	KeyFrame[8].dogPosY = -0.124588f;
	KeyFrame[8].dogPosZ = -0.151777f;
	KeyFrame[8].rotDog = 0.000000f;
	KeyFrame[8].rotDogX = 6.039238f;
	KeyFrame[8].rotDogZ = 172.971725f;
	KeyFrame[8].head = -32.505417f;
	KeyFrame[8].tail = 1.578903f;
	KeyFrame[8].rotFL = 36.440849f;
	KeyFrame[8].rotFR = -29.385376f;
	KeyFrame[8].rotBL = -0.272751f;
	KeyFrame[8].rotBR = 12.158203f;

	FrameIndex = 9;
}


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Exporta los frames en un archivo txt
void exportarAnimacion()
{
	std::ofstream archivo("AnimacionGuardada.txt");
	if (archivo.is_open())
	{
		// Obliga al archivo a imprimir el punto decimal y 6 digitos
		archivo << std::fixed << std::setprecision(6);

		archivo << "void cargarAnimacionTruco(void)\n{\n";
		for (int i = 0; i < FrameIndex; i++)
		{
			archivo << "    // Frame " << i << "\n";
			archivo << "    KeyFrame[" << i << "].dogPosX = " << KeyFrame[i].dogPosX << "f;\n";
			archivo << "    KeyFrame[" << i << "].dogPosY = " << KeyFrame[i].dogPosY << "f;\n";
			archivo << "    KeyFrame[" << i << "].dogPosZ = " << KeyFrame[i].dogPosZ << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotDog = " << KeyFrame[i].rotDog << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotDogX = " << KeyFrame[i].rotDogX << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotDogZ = " << KeyFrame[i].rotDogZ << "f;\n"; 
			archivo << "    KeyFrame[" << i << "].head = " << KeyFrame[i].head << "f;\n";
			archivo << "    KeyFrame[" << i << "].tail = " << KeyFrame[i].tail << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotFL = " << KeyFrame[i].rotFL << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotFR = " << KeyFrame[i].rotFR << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotBL = " << KeyFrame[i].rotBL << "f;\n";
			archivo << "    KeyFrame[" << i << "].rotBR = " << KeyFrame[i].rotBR << "f;\n\n";
		}
		archivo << "    FrameIndex = " << FrameIndex << ";\n";
		archivo << "}\n";
		archivo.close();
		printf("¡Codigo de animacion exportado a AnimacionGuardada.txt!\n");
	}
	else
	{
		printf("Error al crear el archivo.\n");
	}
}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 12 - Ingrid Serrano", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	
	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		// 5. Inicializar en la estructura
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].rotFL = 0;
		KeyFrame[i].rotFLInc = 0;
		KeyFrame[i].rotFR = 0;
		KeyFrame[i].rotFRInc = 0;
		KeyFrame[i].rotBL = 0;
		KeyFrame[i].rotBLInc = 0;
		KeyFrame[i].rotBR = 0;
		KeyFrame[i].rotBRInc = 0;
		KeyFrame[i].rotDogX = 0;
		KeyFrame[i].rotDogXInc = 0;
		KeyFrame[i].rotDogZ = 0;
		KeyFrame[i].rotDogZInc = 0;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	
		
		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp= model = glm::translate(model, glm::vec3(dogPosX,dogPosY,dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		// Rotación vertical (X) para inclinar todo el perro hacia atrás
		modelTemp = model = glm::rotate(model, glm::radians(rotDogX), glm::vec3(1.0f, 0.0f, 0.0f));
		// Rotación sobre el eje Z para rodar lateralmente
		modelTemp = model = glm::rotate(model, glm::radians(rotDogZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(rotFL), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(rotFR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(rotBL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(rotBR), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	    Ball.Draw(lightingShader); 
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	
	

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Define la velocidad de movimiento y rotación
	float moveSpeed = 2.0f * deltaTime;
	float rotSpeed = 50.0f * deltaTime;
	float lightSpeed = 2.0f * deltaTime;

	// Dog Controls
	
	// 6. Agregar tecla para las partes del perro
	// Cabeza
	if (keys[GLFW_KEY_4])
	{
		head += rotSpeed;
	}
	if (keys[GLFW_KEY_5])
	{
		head -= rotSpeed;
	}
	// Cola
	if (keys[GLFW_KEY_6])
	{
		tail += rotSpeed;
	}
	if (keys[GLFW_KEY_7])
	{
		tail -= rotSpeed;
	}
	// Pierna Izq Delantera
	if (keys[GLFW_KEY_Z])
	{
		rotFL += rotSpeed;
	}
	if (keys[GLFW_KEY_X])
	{
		rotFL -= rotSpeed;
	}
	// Pierna Der Delantera
	if (keys[GLFW_KEY_C])
	{
		rotFR += rotSpeed;
	}
	if (keys[GLFW_KEY_V])
	{
		rotFR -= rotSpeed;
	}
	// Pierna Izq Trasera
	if (keys[GLFW_KEY_B])
	{
		rotBL += rotSpeed;
	}
	if (keys[GLFW_KEY_N])
	{
		rotBL -= rotSpeed;
	}
	// Pierna Der Trasera
	if (keys[GLFW_KEY_8])
	{
		rotBR += rotSpeed;
	}
	if (keys[GLFW_KEY_9])
	{
		rotBR -= rotSpeed;
	}
	// Inclinacion del torso
	if (keys[GLFW_KEY_1])
	{
		rotDogX += rotSpeed;
	}
	if (keys[GLFW_KEY_0])
	{
		rotDogX -= rotSpeed;
	}
	// Mover al perro en Y (Arriba/Abajo)
	if (keys[GLFW_KEY_I])
	{
		dogPosY += moveSpeed;
	}
	if (keys[GLFW_KEY_U])
	{
		dogPosY -= moveSpeed;
	}
	// Mover al perro en Z (rota)
	if (keys[GLFW_KEY_O]) {
		rotDogZ += rotSpeed;
	}
	if (keys[GLFW_KEY_P]) {
		rotDogZ -= rotSpeed;
	}
	// Exportar archivo
	if (keys[GLFW_KEY_E])
	{
		if (FrameIndex > 1)
		{
			exportarAnimacion();
		}
		else
		{
			printf("No hay suficientes frames para exportar.\n");
		}
	}

	if (keys[GLFW_KEY_2])
	{
		rotDog += rotSpeed;
	}
	if (keys[GLFW_KEY_3])
	{
		rotDog -= rotSpeed;
	}
	if (keys[GLFW_KEY_H])
	{
		dogPosZ += moveSpeed;
	}
	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= moveSpeed;
	}
	if (keys[GLFW_KEY_G])
	{
		dogPosX -= moveSpeed;
	}
	if (keys[GLFW_KEY_J])
	{
		dogPosX += moveSpeed;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// Light controls
	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += lightSpeed;
	}
	if (keys[GLFW_KEY_G]) 
	{
		pointLightPositions[0].x -= lightSpeed;
	}
	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += lightSpeed;
	}
	if (keys[GLFW_KEY_H]) 
	{
		pointLightPositions[0].y -= lightSpeed;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= lightSpeed;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += lightSpeed;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_M])
	{
		// 1. Carga los valores hardcodeados en el arreglo KeyFrame
		cargarAnimacionTruco();

		// 2. Inicia la animación de la misma forma que lo hace la tecla L
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	
	
}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			// 7. Agregar la animación de cada parte del perro
			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;
			rotFL += KeyFrame[playIndex].rotFLInc;
			rotFR += KeyFrame[playIndex].rotFRInc;
			rotBL += KeyFrame[playIndex].rotBLInc;
			rotBR += KeyFrame[playIndex].rotBRInc;
			rotDogX += KeyFrame[playIndex].rotDogXInc;
			rotDogZ += KeyFrame[playIndex].rotDogZInc;

			rotDog += KeyFrame[playIndex].rotDogInc;

			i_curr_steps++;
		}

	}
	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}