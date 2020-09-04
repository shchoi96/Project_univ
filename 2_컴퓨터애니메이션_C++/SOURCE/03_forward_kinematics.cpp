#include "glSetup.h"

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;

void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Light configuration
Vector4f	light(0.0, 0.0, 5.0, 1);	// Light position

// Global coordinate frame
float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// Sphere, cylinder
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;

int main(int argc, char* argv[])
{
	// Orthographics viewing
	perspectiveView = false;

	// Initialize the OpenGL system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	// Callbacks
	glfwSetKeyCallback(window, keyboard);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	// Normal vectors are normalized after transformation.
	glEnable(GL_NORMALIZE);

	// Back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Viewport and perspective setting
	reshape(window, windowW, windowH);

	// Initialization - Main loop - Finalization
	init();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
		glfwPollEvents();			// Events
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

	// Keyboard
	cout << "Keyboard Input : [1:3] for joint selection" << endl;
	cout << "Keyboard Input : 1/r arrows for adjusting angles" << endl;
}

void quit()
{
	// Delete quadric shapes
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
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
	glLightfv(GL_LIGHT0, GL_POSITION, light.data());
}

// Material
void setupMaterial()
{
	// Material
	GLfloat mat_ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void setDiffuseColor(const Vector3f& color)
{
	GLfloat mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

// Draw a sphere after setting up its material
void drawSphere(float radius, const Vector3f& color)
{
	// Material
	setDiffuseColor(color);

	// Sphere using GLU quadrics
	gluSphere(sphere, radius, 72, 72);
}

// Draw a cylinder after setting up its material
void drawCylinder(float radius, float height, const Vector3f& color)
{
	// Material
	setDiffuseColor(color);

	// Sphere using GLU quadrics
	gluCylinder(cylinder, radius, radius, height, 72, 5);
}

void drawJoint(float radius, const Vector3f& color)
{
	glPushMatrix();

	// Front sphere
	glTranslatef(0, 0, radius);		// Translatioin using OpenGL glTranslatef()
	drawSphere(radius, color);

	// Rear sphere
	glTranslatef(0, 0, -2.0 * radius);	// -2.0 to avoid calling push/pop matrix
	drawSphere(radius, color);

	glPopMatrix();

	// Cylinder
	drawCylinder(radius, radius, color);
}

void drawLink(float radius, float height, const Vector3f& color)
{
	glPushMatrix();

	// Rotation using OpenGL glRotatef(angle_in_degree, axis_x, axis_y, axis_z)
	glRotatef(-90, 1, 0, 0);

	// Draw sphere using gluCylinder() after setting up the material
	drawCylinder(radius, height, color);

	glPopMatrix();
}

void drawCube(float width, float height, float depth, const Vector3f& color)
{
	// Material
	setDiffuseColor(color);

	// Cube
	glPushMatrix();
	glScalef(width, height, depth);

	glBegin(GL_QUADS);

	// front
	glNormal3f(0, 0, 1);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	// back
	glNormal3f(0, 0, -1);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	// right
	glNormal3f(1, 0, 0);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);

	// left
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	// top
	glNormal3f(0, 1, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	// bottom
	glNormal3f(0, -1, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glEnd();

	glPopMatrix();
}

void drawBase(float radiusLink, const Vector3f& color)
{
	float baseLinkHeight = 0.15;
	float baseWidth = 1.0;
	float baseHeight = 0.1;
	float baseDepth = 1.0;

	glPushMatrix();
	glTranslatef(0, -(baseLinkHeight + baseHeight / 2), 0);
	drawLink(radiusLink, (baseLinkHeight + baseHeight / 2), color);
	drawCube(baseWidth, baseHeight, baseDepth, color);
	glPopMatrix();
}

// Joints and links
int		nLinks = 3;
float	linkLength[3] = { 0.4, 0.4, 0.4 };
float	jointAngle[3] = { -5, -5, -5 }; // Degree

int selectedJoint = 0;

void render(GLFWwindow* window)
{
	// Background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Lighting
	setupLight();

	// Material
	setupMaterial();

	float radiusJoint = 0.05;
	float radiusLink = 0.04;

	// Base transform using OpenGL glTranslatef()
	glTranslatef(0.0, -0.7, 0.0);

	// Geometry of the base
	drawBase(radiusLink, Vector3f(0.0, 0.5, 0.5));

	// Joints and links
	for (int i = 0; i < nLinks; i++)
	{
		// Joint transform
		glRotatef(jointAngle[i], 0, 0, 1);

		// Geometry of the joint and link
		drawJoint(radiusJoint, Vector3f(0.95, 0, 0));
		drawLink(radiusLink, linkLength[i], Vector3f(0, 0.95, 0.95));

		// Link transform
		glTranslatef(0, linkLength[i], 0);
	}

	// End-effector
	drawJoint(radiusJoint, Vector3f(0.95, 0.95, 0.0)); // Draw the EE as a joint
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

			// Example selection
		case GLFW_KEY_1: selectedJoint = 0; break;
		case GLFW_KEY_2: selectedJoint = 1; break;
		case GLFW_KEY_3: selectedJoint = 2; break;

		case GLFW_KEY_LEFT: jointAngle[selectedJoint] += 5; break;
		case GLFW_KEY_RIGHT: jointAngle[selectedJoint] -= 5; break;
		}
	}
}