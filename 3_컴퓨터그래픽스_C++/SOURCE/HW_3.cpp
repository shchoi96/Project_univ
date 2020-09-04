#define _CRT_SECURE_NO_WARNINGS
#include "glSetup.h"

#include <glm/glm.hpp>				// OpenGL Mathematics
#include <glm/gtc/type_ptr.hpp>		// glm::value_ptr()
using namespace glm;

#include <iostream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Camera configuation
vec3 eye(3.5, 3, 3.5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

vec4 light(1.0, 1.5, 0.5, 0);	// Light position

// Global coordinate frame
float AXIS_LENGTH = 2;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };

// Play configuration
bool pause = true;

float timeStep = 1.0 / 120; // 120fps. 60fps using vsync = 1
float period = 4.0;

// Cureent frame
int frame = 0;

void animate() {
	frame += 1;
}

// point
vec4 p[36][18];
vec3 normal[36][18];
vec3 vertex_normal[36][18];

int selection = 0;

int main(int argc, char* argv[])
{
	// Initialize the OpenGL system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	// Callbacks
	glfwSetKeyCallback(window, keyboard);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	// Normal vectors are normalized after transformation.
	glEnable(GL_NORMALIZE);

	// Viewport and perspective setting
	reshape(window, windowW, windowH);
	// Initialization - Main loop - Finalization
	init();

	// Main loop
	float previous = glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();			// Events

		// Time passed during a single loop
		float now = glfwGetTime();
		float delta = now - previous;
		previous = now;

		// Time passed after the previous frame
		elapsed += delta;

		// Deal with the current frame
		if (elapsed > timeStep)
		{
			// Animate 1 frame
			if (!pause) animate();

			elapsed = 0;	// reset the elapsed time
		}
		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
	}
	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLuint texID[3];

void LoadTexture(const char* filename, int width, int height)
{

	GLubyte* data;
	FILE* file;
	file = fopen(filename, "rb"); 
	if (file == NULL) return; 

	data = (GLubyte*)malloc(width * height * 3);

	fread(data, width * height * 3, 1, file);
	fclose(file);

	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	free(data);
}

void init()
{
	// Texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(3, texID);
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	LoadTexture("Material\\marble.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	LoadTexture("Material\\wood.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, texID[2]);
	LoadTexture("Material\\check.raw", 512, 512);

	// Keyboard


	// Circle Point
	glm::mat4 trans = mat4(1.0);
	p[0][0] = vec4(0.5f, 0.0f, 0.0f, 1.0f);
	trans = rotate(trans, radians(20.0f), vec3(0.0, 0.0, 1.0));
	for (int i = 1; i < 18; i++)
	{
		p[0][i] = trans * p[0][i - 1];
	}
	trans = mat4(1.0);
	trans = translate(trans, vec3(1, 0, 0));
	for (int i = 0; i < 18; i++)
	{
		p[0][i] = trans * p[0][i];
	}
	// Torus Point
	trans = rotate(mat4(1.0), radians(10.0f), vec3(0.0, 1.0, 0.0));
	for (int i = 1; i < 36; i++)
		for (int j = 0; j < 18; j++)
		{
			p[i][j] = trans * p[i - 1][j];
		}
	int k, l, i36, j18;
	// Unit Normal Vector
	for (int i = 0; i < 36; i++)
		for (int j = 0; j < 18; j++)
			vertex_normal[i][j] = vec3(0, 0, 0);

	for (int i = 1; i <= 36; i++) {
		k = i - 1;
		i36 = i % 36;
		for (int j = 1; j <= 18; j++) {
			l = j - 1;
			j18 = j % 18;
			normal[k][l] = normalize(cross(vec3(p[i36][l] - p[k][l]), vec3(p[k][j18] - p[k][l])));
			vertex_normal[k][l] += normal[k][l] * 0.25f;
			vertex_normal[i36][l] += normal[k][l] * 0.25f;
			vertex_normal[k][j18] += normal[k][l] * 0.25f;
			vertex_normal[i36][j18] += normal[k][l] * 0.25f;
		}
	}
}

// Light
void setupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse[4] = { 1.0, 1.0, 1.0, 1 };
	GLfloat specular[4] = { 1.0, 1.0, 1.0, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));
}

// Material
void setupMaterial()
{
	// Make it possible to change a subset of material parameters
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Material
	GLfloat mat_ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat mat_shininess = 25;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

// Draw a torus model
void drawTorus()
{
	int k, l, i36, j18;
	vec2 texcoord[4];

	glBindTexture(GL_TEXTURE_2D, texID[selection]);
	glBegin(GL_QUADS);
	
	for (int i = 1; i <= 36; i++)
	{
		k = i - 1;
		i36 = i % 36;
		for (int j = 1; j <= 18; j++)
		{
			l = j - 1;
			j18 = j % 18;
			texcoord[0] = vec2(k / 36.0, j / 18.0);
			texcoord[1] = vec2(k / 36.0, l / 18.0);
			texcoord[2] = vec2(i / 36.0, l / 18.0);
			texcoord[3] = vec2(i / 36.0, j / 18.0);

			glTexCoord2fv(value_ptr(texcoord[1]));
			glNormal3fv(value_ptr(vertex_normal[k][l]));
			glVertex3fv(value_ptr(p[k][l]));
			glTexCoord2fv(value_ptr(texcoord[2]));
			glNormal3fv(value_ptr(vertex_normal[i36][l]));
			glVertex3fv(value_ptr(p[i36][l]));
			glTexCoord2fv(value_ptr(texcoord[3]));
			glNormal3fv(value_ptr(vertex_normal[i36][j18]));
			glVertex3fv(value_ptr(p[i36][j18]));
			glTexCoord2fv(value_ptr(texcoord[0]));
			glNormal3fv(value_ptr(vertex_normal[k][j18]));
			glVertex3fv(value_ptr(p[k][j18]));
		}
	}
	glEnd();
}

void render(GLFWwindow* window)
{
	// Background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// Axes
	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	// Lighting
	setupLight();

	// Material
	setupMaterial();

	// Draw
	float theta1 = frame * 4 / period;
	glm::vec3 axis(0, 1, 0);

	glRotatef(theta1, axis.x, axis.y, axis.z);
	glColor3f(1.0, 1.0, 1.0);
	drawTorus();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			// Quit
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;

			// Play on/off
		case GLFW_KEY_SPACE:	pause = !pause; break;

			// Period control
		case GLFW_KEY_UP:		period += 0.1; break;
		case GLFW_KEY_DOWN:		period = max(period - 0.1, 0.1);	break;

		case GLFW_KEY_1:	selection = 0; break;
		case GLFW_KEY_2:	selection = 1; break;
		case GLFW_KEY_3:	selection = 2; break;
		}
	}
}