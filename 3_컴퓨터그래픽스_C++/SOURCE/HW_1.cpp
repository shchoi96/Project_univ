#include "glSetup.h"

#include <glm/glm/glm.hpp>        // OpenGL Mathematics
#include <glm/glm/gtc/type_ptr.hpp>    // glm::value_ptr()
using namespace glm;

#include <iostream>
using namespace std;

void init();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Camera configuation
vec3 eye(2, 2, 2);
vec3 up(0, 1, 0);
vec3 center(0, 0, 0);

// Light configuration
vec4 light(0.0, 0.0, 0.8, 0);	// Light position

// Global coordinate frame
float AXIS_LENGTH = 5.25;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// control variable
int	selection = 1;			// Selected example
bool toggle = false;
bool frame = false;

// point
vec4 p[36][18];
vec3 normal[36][18];
vec3 p_center[36][18];

//angle control
int control_y = 36;
int control_z = 18;

int main(int argc, char* argv[])
{
	fovy = 37.8;
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;
	glfwSetKeyCallback(window, keyboard);
	glEnable(GL_DEPTH_TEST);
	// Normal vectors are normalized after transformation.
	glEnable(GL_NORMALIZE);
	reshape(window, windowW, windowH);

	init();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();			// Events
		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void init()
{
	// Circle Point
	glm::mat4 trans = mat4(1.0);
	p[0][0] = vec4(0.2f, 0.0f, 0.0f, 1.0f);
	trans = rotate(trans, radians(20.0f), vec3(0.0, 0.0, 1.0));
	for (int i = 1; i < 18; i++)
	{
		p[0][i] = trans * p[0][i - 1];
	}
	trans = mat4(1.0);
	trans = translate(trans, vec3(0.5, 0, 0));
	for (int i = 0; i < 18; i++) // Translate
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
	int k, l;
	// Unit Normal Vector
	for (int i = 1; i <= 36; i++) {
		k = i - 1;
		for (int j = 1; j <= 18; j++) {
			l = j - 1;
			p_center[i - 1][j - 1] = vec3(p[k][l] + p[k][j % 18] + p[i % 36][j % 18] + p[i % 36][l]);
			p_center[i - 1][j - 1] = vec3(p_center[i - 1][j - 1].x/4.0, p_center[i - 1][j - 1].y/4.0, p_center[i - 1][j - 1].z/4.0);
			normal[k][l] = normalize(cross(vec3(p[i % 36][l] - p[k][l]), vec3(p[k][j % 18] - p[k][l])));
		}
	}
}

void drawTorus()
{
	glBegin(GL_QUADS);
	for (int i = 1; i <= control_y; i++)
	{
		int k = i - 1;
		for (int j = 1; j <= control_z; j++)
		{
			int l = j - 1;
			// 벡터 내적으로 시야에 보이는 면의 색 결정
			if (selection == 6) {
				if (dot(normal[k][l], vec3(eye - p_center[k][l])) >= 0)
					glColor3f(0, 0, 1);
				else
					glColor3f(1, 0, 0);
			}
			if (frame)
				glColor3f(0, 0, 0);
			glVertex3f(p[k][j % 18].x, p[k][j % 18].y, p[k][j % 18].z);
			glVertex3f(p[k][l].x, p[k][l].y, p[k][l].z);
			glVertex3f(p[i % 36][l].x, p[i % 36][l].y, p[i % 36][l].z);
			glVertex3f(p[i%36][j % 18].x, p[i%36][j % 18].y, p[i%36][j % 18].z);
		}
	}
	glEnd();
}
// select 1
void drawPoint()
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < control_y; i++)
	{
		for (int j = 0; j < control_z; j++)
		{
			glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
		}
	}
	glEnd();
}
// select 2
void drawWireframe()
{
	glColor3f(0, 0, 0);
	if (control_y == 0) {
		glBegin(GL_LINES);
		for (int i = 1; i <= control_z; i++) {
			glVertex3f(p[0][i%18].x, p[0][i%18].y, p[0][i%18].z);
			glVertex3f(p[0][i-1].x, p[0][i-1].y, p[0][i-1].z);
		}
		glEnd();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		frame = true;
		drawTorus();
		frame = false;
	}
	
}
// select 3
void drawQuads()
{
	int k, l;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0, 0, 1);
	drawTorus();
}
// select 4
void drawQuadsAndWireframe()
{
	//Quads
	glPolygonOffset(0.0f, 0.0f);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	drawQuads();

	//wireframe
	glPolygonOffset(1.0f, 1.0f);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	drawWireframe();
}
// select 5
void toggleNormal()
{
	if (toggle) {
		//법선 벡터
		glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		int k, l;
		for (int i = 1; i <= control_y; i++)
		{
			k = i - 1;
			for (int j = 1; j <= control_z; j++)
			{
				l = j - 1;
				glVertex3f(p_center[i - 1][j - 1].x, p_center[i - 1][j - 1].y, p_center[i - 1][j - 1].z);
				glVertex3f(p_center[i - 1][j - 1].x + normal[k][l].x / 10, p_center[i - 1][j - 1].y + normal[k][l].y / 10, p_center[i - 1][j - 1].z + normal[k][l].z / 10);
			}
		}
		glEnd();
	}
}
// select 6
void drawTwoSidedQuadsAndWireframe()
{	
	//quads
	glPolygonOffset(1.0f, 1.0f);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	drawTorus();

	//wireframe
	glPolygonOffset(0.0f, 0.0f);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	drawWireframe();
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

	// Smooth shading
	glShadeModel(GL_SMOOTH);

	glPushMatrix();

	switch (selection)
	{
	case 1: drawPoint();						break;
	case 2: drawWireframe();					break;
	case 3: drawQuads();						break;
	case 4: drawQuadsAndWireframe();			break;
	case 6: drawTwoSidedQuadsAndWireframe();	break;
	}
	toggleNormal();
	glPopMatrix();
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

		case GLFW_KEY_A: if (control_y < 36) control_y++;	break;
		case GLFW_KEY_S: if (control_y >  0) control_y--;	break;
		case GLFW_KEY_J: if (control_z < 18) control_z++;	break;
		case GLFW_KEY_K: if (control_z >  0) control_z--;	break;

			// Example selection
		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: toggle = !toggle; break;
		case GLFW_KEY_6: selection = 6; break;
		}
	}
}