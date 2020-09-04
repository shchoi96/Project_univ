#define M_PI 3.141592
#include "glSetup.h"

#include <glm/glm/glm.hpp>        // OpenGL Mathematics
#include <glm/glm/gtc/type_ptr.hpp>    // glm::value_ptr()
using namespace glm;

#include <iostream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Camera configuation
vec3 eye(10,10,10);
vec3 up(0, 1, 0);
vec3 center(0, 0, 0);

// Light configuration
vec4 lightInitialP(2, 0.0, 1.0, 1);	// Initial light position

// Global coordinate frame
float AXIS_LENGTH = 5.25;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// control variable
bool n_toggle = false;

float thetaLight[3];

bool lightOn[3];        // Point = 0, distant = 1, spot = 2 lights

bool exponent = false;
float exponentInitial = 0.0;        // [0, 128]
float exponentValue = exponentInitial;
float exponentNorm = exponentValue / 128.0; // [0, 1]

bool cutoff = true;
float cutoffMax = 60;            // [0, 90] degree
float cutoffInitial = 30.0;          // [0, cutoffMax] degree
float cutoffValue = cutoffInitial;
float cutoffNorm = cutoffValue / cutoffMax; // [0, 1]

bool shininess = false;
float shininessMax = 10;
float shininessInitial = 10;				// [0, 128]
float shininessValue = shininessInitial;
float shininessNorm = shininessValue / 10; // [0, 1]

int material = 0;    // Predefined material parameters

// Play configuration
bool pause = true;

float timeStep = 1.0 / 120; // 120fps. 60fps using vsync = 1
float period = 4.0;

// Cureent frame
int frame = 0;

// point
vec4 p[36][18];
vec3 normal[36][18];
vec3 p_center[36][18];
vec3 vertex_normal[36][18];

// Sphere, cylinder
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* cone = NULL;

void reinitialize()
{
	frame = 0;

	lightOn[0] = true;	// Point Light
	lightOn[1] = false; // Distant Light
	lightOn[2] = false; // Spot Light
	for (int i = 0; i < 3; i++)
		thetaLight[i] = 0;

	exponentValue = exponentInitial;
	exponentNorm = exponentValue / 128.0;

	cutoffValue = cutoffInitial;
	cutoffNorm = cutoffValue / cutoffMax;

	shininessValue = shininessInitial;
	shininessNorm = shininessValue / shininessMax;
}

void animate()
{
	frame += 1;
	for (int i = 0; i < 3; i++)
		if (lightOn[i]) thetaLight[i] += 4 / period;
}

int main(int argc, char* argv[])
{
	fovy = 16.1;
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;
	glfwSetKeyCallback(window, keyboard);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	reshape(window, windowW, windowH);
	init();

	float previous = glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();      // Events

		float now = glfwGetTime();
		float delta = now - previous;
		previous = now;
		elapsed += delta;
		if (elapsed > timeStep)
		{
			// Animate 1 frame
			if (!pause) animate();
			if (shininess)
			{
				shininessNorm += radians(6 / period) / M_PI;
				shininessValue = shininessMax * (acos(cos(shininessNorm * M_PI)) / M_PI);
			}
			elapsed = 0;	// reset the elapsed time
		}

		render(window);        // Draw one frame
		glfwSwapBuffers(window);  // Swap buffers
	}
	quit();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void init()
{
	// Animation system
	reinitialize();

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

	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_FILL);
	gluQuadricNormals(cone, GLU_SMOOTH);
	gluQuadricOrientation(cone, GLU_OUTSIDE);
	gluQuadricTexture(cone, GL_FALSE);

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
				p_center[k][l] = vec3(p[k][l] + p[k][j18] + p[i36][j18] + p[i36][l]);
				p_center[k][l] = vec3(p_center[k][l].x / 4.0, p_center[k][l].y / 4.0, p_center[k][l].z / 4.0);
				normal[k][l] = normalize(cross(vec3(p[i36][l] - p[k][l]), vec3(p[k][j18] - p[k][l])));
				vertex_normal[k][l] += normal[k][l] * 0.25f;
				vertex_normal[i36][l] += normal[k][l] * 0.25f;
				vertex_normal[k][j18] += normal[k][l] * 0.25f;
				vertex_normal[i36][j18] += normal[k][l] * 0.25f;
			}
		}
}

void quit()
{
	// Delete quadric shapes
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(cone);
}


// Draw a torus model
void drawTorus()
{
	int k, l, i36, j18;
	glBegin(GL_QUADS);
	for (int i = 1; i <= 36; i++)
	{
		k = i - 1;
		i36 = i % 36;
		for (int j = 1; j <= 18; j++)
		{
			l = j - 1;
			j18 = j % 18;
			glNormal3f(normal[k][l].x, normal[k][l].y, normal[k][l].z);
			glVertex3f(p[k][j18].x, p[k][j18].y, p[k][j18].z);
			glVertex3f(p[k][l].x, p[k][l].y, p[k][l].z);
			glVertex3f(p[i36][l].x, p[i36][l].y, p[i36][l].z);
			glVertex3f(p[i36][j18].x, p[i36][j18].y, p[i36][j18].z);
		}
	}
	glEnd();
}
void drawWireframe()
{
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawTorus();
}
void drawQuads()
{
	glColor3f(0, 0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	drawTorus();
}
void drawQuadsAndWireframe()
{
	//Quads
	glPolygonOffset(1.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	drawQuads();
	
	//wireframe
	glDisable(GL_LIGHTING);
	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);
	drawWireframe();
	glEnable(GL_LIGHTING);
}
// Draw the vertex normal vectors
void toggleN() {
	if (n_toggle) {
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		for (int i = 0; i < 36; i++)
			for (int j = 0; j < 18; j++)
			{
				glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
				glVertex3f(p[i][j].x + vertex_normal[i][j].x / 5, p[i][j].y + vertex_normal[i][j].y / 5, p[i][j].z + vertex_normal[i][j].z / 5);
			}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void computeRotation(const vec3& a, const vec3& b, float& theta, vec3& axis)
{
	axis = cross(a, b);
	float sinTheta = length(axis);
	float cosTheta = dot(a, b);
	theta = atan2(sinTheta, cosTheta) * 180 / M_PI;
}

// Light
void setupLight(const vec4& p, int i)
{
	GLfloat ambient[4] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse[4] = { 1.0, 1.0, 1.0, 1 };
	GLfloat specular[4] = { 1.0, 1.0, 1.0, 1 };
	
	glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0 + i, GL_POSITION, value_ptr(p));


	glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.0);

	if (i == 2) // Spot light
	{
		vec3 spotDirection = -vec3(p);
		glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, value_ptr(spotDirection));
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, cutoffValue);    // [0, 90]
		glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, exponentValue); // [0, 128]
	}
	else
	{ // Point and distant light.
	  // 180 to turn off cutoff when it was used as a spot light.
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, 180); // uniform light distribution
	}
}

//Draw a sphere using a GLU quadric
void drawSphere(float radius, int slices, int stacks)
{
	gluSphere(sphere, radius, slices, stacks);
}
// Draw a cylinder using a GLU quadric
void drawCylinder(float radius, float height, int slices, int stacks)
{
	gluCylinder(cylinder, radius, radius, height, slices, stacks);
}
// Draw a cone using a GLU quadric
void drawCone(float radius, float height, int slices, int stacks)
{
	gluCylinder(cone, 0, radius, height, slices, stacks);
}
void drawArrow(const vec3& p, bool tailOnly)
{
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[4] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininessValue);

	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	if (!tailOnly)
	{
		float theta;
		vec3 axis;
		computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
		glRotatef(theta, axis.x, axis.y, axis.z);
	}
	// Tail sphere
	float arrowTailRadius = 0.05;
	glColor3f(1, 0, 0); // ambient and diffuse
	drawSphere(arrowTailRadius, 16, 16);

	if (!tailOnly)
	{
		// Shaft cylinder
		float arrowShaftRadius = 0.02;
		float arrowShaftLength = 0.2;
		glColor3f(0, 1, 0);
		drawCylinder(arrowShaftRadius, arrowShaftLength, 16, 5);

		// Head cone
		float arrowheadHeight = 0.09;
		float arrowheadRadius = 0.06;
		glTranslatef(0, 0, arrowShaftLength + arrowheadHeight);
		glRotatef(180, 1, 0, 0);
		glColor3f(0, 0, 1); // ambient and diffuse
		drawCone(arrowheadRadius, arrowheadHeight, 16, 5);
	}
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}

// Material
void setupColoredMaterial(const vec3& color)
{
	// Material
	GLfloat mat_ambient[4] = { 0.1,0.1,0.1,1 };
	GLfloat mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	GLfloat mat_specular[4] = { 0.5, 0.5, 0.5, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininessValue);
}

void drawSpotLight(const vec3& p, float cutoff)
{
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);

	float theta;
	vec3 axis;
	computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
	glRotatef(theta, axis.x, axis.y, axis.z);

	// Color
	setupColoredMaterial(vec3(0, 0, 1));

	// tan(cutoff) = r/h
	float h = 0.15;
	float r = h * tan(radians(cutoff));
	drawCone(r, h, 16, 5);
	// color
	setupColoredMaterial(vec3(1, 0, 0));
	// Apex
	float apexRadius = 0.06 * (0.5 + exponentValue / 128.0);
	drawSphere(apexRadius, 16, 16);
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
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);
	// Smooth shading
	glShadeModel(GL_SMOOTH);
	vec3 axis(0, 1, 0); // Axis of rotation 
	glEnable(GL_LIGHTING);
	vec4 lightP[3];
	for (int i = 0; i < 3; i++)
	{
		if (!lightOn[i]) { glDisable(GL_LIGHT0 + i); continue; }
		glEnable(GL_LIGHT0 + i);
		lightP[i] = lightInitialP;
		if (i == 1) lightP[i].w = 0;
		mat4 R = rotate(mat4(1.0), radians(thetaLight[i]), axis);		
		lightP[i] = R * lightP[i];
		setupLight(lightP[i], i);
	}
	// Draw the geometries of the lights
	for (int i = 0; i < 3; i++)
	{
		if (!lightOn[i]) continue;

		if (i == 2) drawSpotLight(lightP[i], cutoffValue);
		else		drawArrow(lightP[i], i == 0);	// Tail only for a point light
	}
	// Material
	setupColoredMaterial(vec3(0.1,0.1,0.1));
	glPushMatrix();
	drawQuadsAndWireframe();
	toggleN();
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

		case GLFW_KEY_SPACE: pause = !pause; break;

		case GLFW_KEY_N: n_toggle = !n_toggle; break;
		case GLFW_KEY_P: lightOn[0] = !lightOn[0]; break;
		case GLFW_KEY_D: lightOn[1] = !lightOn[1]; break;
		case GLFW_KEY_S: lightOn[2] = !lightOn[2]; break;
		case GLFW_KEY_T: shininess = !shininess; break;
		}
	}
}