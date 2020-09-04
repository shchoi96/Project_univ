//회전, 스케일링,  행성운동  
#include "glSetup.h"

#include <glm/glm/glm.hpp>				// OpenGL Mathematics
#include <glm/glm/gtc/type_ptr.hpp>		// glm::value_ptr()
using namespace glm;

#include <iostream>
using namespace std;

void init();
void animate();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Camera configuation
vec3 eye(3.5, 3, 3.5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

// Light configuration
vec4 light(5.0, 5.0, 5.0, 1);	// Light position

// Global coordinate frame
float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// Selected example
int selection = 1;

// Sphere, cylinder
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

// Play configuration
bool pause = false;
float timeStep = 1.0 / 120; // 120fps. 60fps using vsync = 1
float period = 4.0;

// Current frame
int frame = 0;

void animate()
{
	frame += 1;
}

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
	//
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

	// Finalization
	quit();

	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void init()
{
	// Prepare quadric shapes
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	gluQuadricTexture(disk, GL_FALSE);

	// Keyboard
	cout << "Keyboard input: space for play/pause" << endl;
	cout << "Keyboard input: up for increasing period" << endl;
	cout << "Keyboard input: down for decreasing period" << endl;
	cout << "Keyboard input: 1 for rotation example" << endl;
	cout << "Keyboard input: 2 for rotation wrt a pivot example" << endl;
	cout << "Keyboard input: 3 for scaling example" << endl;
	cout << "Keyboard input: 4 for scaling wrt a pivot example" << endl;
	cout << "Keyboard input: 5 for scaling wrt a direction example" << endl;
	cout << "Keyboard input: 6 for a solar system example" << endl;
}

void quit()
{
	// Delete quadric shapes
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);
}

// Light
void setupLIght()
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
	// Material
	GLfloat mat_ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void setDiffuseColor(const vec3& color)
{
	GLfloat mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

// Draw a sphere after setting up its material
void drawSphere(float radius, const vec3& color)
{
	// Material
	setDiffuseColor(color);

	// Sphere using GLU quadrics
	gluSphere(sphere, radius, 72, 72);
}

// Draw a cylinder after setting up its material
void drawCylinder(float radius, float height, const vec3& color)
{
	// Material
	setDiffuseColor(color);

	// Sphere using GLU quadrics
	gluCylinder(cylinder, radius, radius, height, 72, 5);
}

// Draw a disk after setting up its material
void drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	// Material
	setDiffuseColor(color);

	// Sphere using GLU quadrics
	gluDisk(disk, inner_radius, outer_radius, 72, 5);
}

void drawCube()
{
	// Cube
	glBegin(GL_QUADS);

	// front
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);
	
	// back
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	// right
	glNormal3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	// left
	glNormal3f(-1, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 1);

	// top
	glNormal3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	// bottom
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);

	glEnd();
}

void rotationExample()
{
	float thetal = frame * 4 / period;
	glm::vec3 axis(0, 0, 1);

	glRotatef(thetal, axis.x, axis.y, axis.z);

	setDiffuseColor(glm::vec3(1, 1, 1));	// Material
	drawCube();
}

void rotationPivotExample()
{
	float thetal = frame * 4 / period;
	glm::vec3 axis(0, 0, 1);
	glm::vec3 pivot(0.5, 0.5, 0);

	glTranslatef(pivot.x, pivot.y, pivot.z);
	glRotatef(thetal, axis.x, axis.y, axis.z);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(1, 1, 1));	// Material
	drawCube();
}

void scalingExample()
{
	float s = 1.0 + 0.95*sin(frame * 0.1 / period);

	glScalef(s, 1, 1);

	setDiffuseColor(glm::vec3(1, 1, 1));	// Material
	drawCube();
}

void scalingPivotExample()
{
	float s = 1.0 + 0.95*sin(frame*0.1 / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);

	glTranslatef(pivot.x, pivot.y, pivot.z);
	glScalef(s, 1, 1);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(1, 1, 1));	// Material
	drawCube();
}

void scalingDirectionExample()
{
	float s = 1.0 + 0.95*sin(frame*0.1 / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);
	glm::vec3 direction(1, 1, 0);

	// Compute the rotation axis and angle between the x-axis and the given direction
	glm::vec3 axis = cross(glm::vec3(1, 0, 0), direction);
	float sinTheta = length(axis);
	float cosTheta = dot(glm::vec3(1, 0, 0), direction);
	float theta = atan2(sinTheta, cosTheta) * 180 / 3.14; //M_PI=3.14

	glRotatef(theta, axis.x, axis.y, axis.z);
	glScalef(s, 1, 1);
	glRotatef(-theta, axis.x, axis.y, axis.z);

	setDiffuseColor(glm::vec3(1, 1, 1));	// Material
	drawCube();
}

void solarSystemExample()
{
	// Planet 1
	float theta1 = frame * 4 / period + 10;
	float orbit1 = 1.2;
	float radius1 = 0.2;

	// Planet 2
	float theta2 = frame * 3 / period + 120;
	float orbit2 = 2.0;
	float radius2 = 0.2;

	// Satellite of Planet 2
	float theta21 = frame * 9 / period + 20;
	float orbit21 = 0.5;
	float radius21 = 0.1;

	// Sun
	drawSphere(0.5, vec3(0.95, 0.95, 0.15));

	// Planet 1
	glPushMatrix();
		glRotatef(theta1, 0, 1, 0);

		// Planet
		glPushMatrix();
			glTranslatef(0, 0, orbit1);
			drawSphere(radius1, vec3(0.15, 0.95, 0.15));
		glPopMatrix();
	
		// Support
		drawCylinder(radius1 / 4, orbit1, vec3(0.95, 0.95, 0.95));
	glPopMatrix();

	// Orbit of Planet 1
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		drawDisk(orbit1 - radius1 / 4, orbit1 + radius1 / 4, vec3(0, 1, 0));
	glPopMatrix();

	// Planet 2
	glPushMatrix();
		glRotatef(theta2, 0, 1, 0);

		// Planet
		glPushMatrix();
			glTranslatef(0, 0, orbit2);
			drawSphere(radius2, vec3(0.15, 0.15, 0.95));

			// Satellite of the planet 2
			glPushMatrix();
				glRotatef(theta21, 0, 1, 0);

				// Satellite
				glPushMatrix();
					glTranslatef(0, 0, orbit21);
					drawSphere(radius21, vec3(0.95, 0.15, 0.95));
				glPopMatrix();

				// Support
				drawCylinder(radius21 / 4, orbit21, vec3(0.95, 0.95, 0.95));
			glPopMatrix();

			// Orbit of the satellite of the Planet 2
			glPushMatrix();
				glRotatef(-90, 1, 0, 0);
				drawDisk(orbit21 - radius21 / 4, orbit21 + radius21 / 4, vec3(1, 0, 1));
			glPopMatrix();
		glPopMatrix();

		// Support
		drawCylinder(radius2 / 4, orbit2, vec3(0.95, 0.95, 0.95));
	glPopMatrix();

	// Orbit of Planet 2
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		drawDisk(orbit2 - radius2 / 4, orbit2 + radius2 / 4, vec3(0, 0, 1));
	glPopMatrix();
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
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH*dpiScaling);

	//  Lighting
	setupLIght();

	// Material
	setupMaterial();

	// Draw
	switch (selection)
	{
	case 1: rotationExample();			break;
	case 2: rotationPivotExample();		break;
	case 3: scalingExample();			break;
	case 4: scalingPivotExample();		break;
	case 5: scalingDirectionExample();	break;
	case 6: solarSystemExample();		break;
	}
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
			
			// Play on / off
		case GLFW_KEY_SPACE:	pause = !pause; break;

			// Period control
		case GLFW_KEY_UP:		period += 0.1;	break;
		case GLFW_KEY_DOWN:		period = max(period - 0.1, 0.1);	break;

			// Example selection
		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: selection = 5; break;
		case GLFW_KEY_6: selection = 6; break;
		}
	}
}