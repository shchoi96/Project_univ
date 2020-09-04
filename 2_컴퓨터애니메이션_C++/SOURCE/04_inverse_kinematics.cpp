#define M_PI 3.141592
#include "glSetup.h"

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;

void	init();
void	quit();
bool	solveIK(const Vector3f& ee_goal);
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void	mouse(GLFWwindow* window, int button, int action, int mods);

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

// Joints and links
const int		nLinks = 3;
float	linkLength[3] = { 0.4, 0.4, 0.4 };
float	jointAngle[3] = { -0.1, -0.1, -0.1 }; // Degree

// Global transformation of the articulated figure
Vector3f basePosition(0.0, -0.7, 0.0);

// Forward kinematics control joint
int selectedJoint = 0;

// Inverse kinematics
bool	target = false;
int		numIterations = 0;
Vector3f ee_goal;

// Constants for IK
const float THRESHOLD_RATIO = 1.0E-4;		// Accuracy threshold ratio
const float MAX_ANGLE_CHANGE = M_PI / 4;	// Max dtheta per IK iteration
const float MAX_DX_RATIO = 0.5;				// Max dx ratio wrt a link for clamping
const float lambda = 0.2;					// Lambda for the damped least squares method
const float alpha = 0.5;					// Alpha for the Jacobian transpose method

enum SolutionMethod {
	JACOBIAN_TRANSPOSE = 0,			// Most simple
	PSEUDOINVERSE,					// Most efficient in the reachable workspace
	DAMPED_LEAST_SQUARES			// Most robust
};
SolutionMethod method = DAMPED_LEAST_SQUARES;

int main(int argc, char* argv[])
{
	// Orthographics viewing
	perspectiveView = false;

	// Initialize the OpenGL system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	// Callbacks
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouse);

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
		if (target)
		{
			// Solve the inverse kinematics problem
			bool solved = solveIK(ee_goal);
			numIterations++;

			// If solved, then turn off the target pointer in the screen
			if (solved) target = false;
		}
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
	cout << "Keyboard Input : j for the Jacobian transpose method" << endl;
	cout << "Keyboard Input : p for the pseudoinverse method" << endl;
	cout << "Keyboard Input : d for the damped least squares method" << endl;
	cout << "Mouse Input : desired position of the end-effector" << endl;
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

bool solveIK(const Vector3f& ee_goal)
{
	// The joint axes are all the same in this example.
	Vector3f jointAxis(0, 0, 1);

	// Joint positions
	Vector3f jointPosition[nLinks];

	// Transform
	Matrix4f T;

	// Base transform
	T.setIdentity();
	T.block<3, 1>(0, 3) = basePosition; // 3x1 translation vector

	// Joints and links
	Matrix4f T_joint, T_link;
	for (int i = 0; i < nLinks; i++)
	{
		// The position of the joint
		jointPosition[i] = T.block<3, 3>(0, 0) * Vector3f(0, 0, 0) + T.block<3, 1>(0, 3);

		// Joint transform using an Eigen 4x4 matrix. 3x3 rotation matrix
		T_joint.setIdentity();
		T_joint.block<3, 3>(0, 0) = Matrix3f(AngleAxisf(jointAngle[i], Vector3f::UnitZ()));
		T = T * T_joint;

		// Link transform using an Eigen 4x4 matrix: 3x1 translation vector
		T_link.setIdentity();
		T_link.block<3, 1>(0, 3) = Vector3f(0.0, linkLength[i], 0.0);
		T = T * T_link;
	}
	
	// The current position of the end-effector
	Vector3f ee_current = T.block<3, 3>(0, 0) * Vector3f(0, 0, 0) + T.block<3, 1>(0, 3);

	// Jacobian
	MatrixXf J(3, nLinks);
	for (int i = 0; i < nLinks; i++)	// Iterate all the joints
	{
		// The i-th column of the Jacobian matrix
		J.block<3, 1>(0, i) = jointAxis.cross(ee_current - jointPosition[i]);
	}

	// Delta x
	Vector3f dx = ee_goal - ee_current;

	// Clamping dx so that it is less than MAX_DX_RATIO * linkLength[0]
	float l = dx.norm();
	if (l > linkLength[0] * MAX_DX_RATIO)
		dx *= (linkLength[0] * MAX_DX_RATIO) / l;

	// Check to see if the solution is already found.
	if (dx.norm() < THRESHOLD_RATIO * linkLength[0])	// Relative error wrt a link
	{
		cout << "IK iterations = " << numIterations << endl;
		return true;
	}

	// Solve IK
	VectorXf dtheta;

	switch (method)
	{
	case JACOBIAN_TRANSPOSE:	// The Jacobian transpose metehod
		dtheta = alpha * (J.transpose() * dx);
		break;
	case PSEUDOINVERSE:			// The pseudoinverse method
		dtheta = J.bdcSvd(ComputeThinU | ComputeThinV).solve(dx);
		break;
	case DAMPED_LEAST_SQUARES:	// The damped least squares method
		int n = J.rows();
		MatrixXf A = (J * J.transpose() + lambda * lambda * MatrixXf::Identity(n, n));
		VectorXf f = A.colPivHouseholderQr().solve(dx);
		dtheta = J.transpose() * f;
		break;
	}

	// Update the joint angles
	for (int j = 0; j < nLinks; j++)
	{
		// Clamping dtheta[j] so that it becomes less than MAX_ANGLE_CHANGE
		float l = fabs(dtheta[j]);
		if (l > MAX_ANGLE_CHANGE)
			dtheta[j] *= MAX_ANGLE_CHANGE / l;

		// Joint angle within [-M_PI, M_PI]j
		jointAngle[j] += dtheta[j];

		if (jointAngle[j] > M_PI)	jointAngle[j] -= 2 * M_PI;
		if (jointAngle[j] < -M_PI)	jointAngle[j] += 2 * M_PI;
	}
	return false;
}

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

	// Desired position of the end-effector
	if (target)
	{
		glTranslatef(ee_goal[0], ee_goal[1], ee_goal[2]);
		drawJoint(radiusJoint, Vector3f(0.95, 0, 0));
	}

	// Transform
	Matrix4f T;

	// Base Transform
	T.setIdentity();
	T.block<3, 1>(0, 3) = basePosition;		// 3x1 translation vector

	// Geometry of the base
	glLoadMatrixf(T.data());
	drawBase(radiusLink, Vector3f(0.0, 0.5, 0.5));

	// Joints and links
	Matrix4f	T_joint, T_link;
	for (int i = 0; i < nLinks; i++)
	{
		// Joint transform using an Eigen 4x4 matrix. 3x3 rotation matrix
		T_joint.setIdentity();
		T_joint.block<3, 3>(0, 0) = Matrix3f(AngleAxisf(jointAngle[i], Vector3f::UnitZ()));
		T = T * T_joint;

		// Geometry of the joint and link
		glLoadMatrixf(T.data());
		drawJoint(radiusJoint, Vector3f(0.95, 0, 0));
		drawLink(radiusLink, linkLength[i], Vector3f(0, 0.95, 0.95));

		// Link transform using an Eigen 4x4 matrix: 3x1 translation vector
		T_link.setIdentity();
		T_link.block<3, 1>(0, 3) = Vector3f(0.0, linkLength[i], 0.0);
		T = T * T_link;
	}

	// End-effector
	glLoadMatrixf(T.data());
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

		case GLFW_KEY_LEFT: jointAngle[selectedJoint] += 0.1; break;
		case GLFW_KEY_RIGHT: jointAngle[selectedJoint] -= 0.1; break;

			// Inverse kinematics method
		case GLFW_KEY_J: method = JACOBIAN_TRANSPOSE;	 break;
		case GLFW_KEY_P: method = PSEUDOINVERSE;		 break;
		case GLFW_KEY_D: method = DAMPED_LEAST_SQUARES;	 break;
		}

		// Target off
		target = false;
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// Target on
		target = true;
		numIterations = 0;

		// In the screen coordinate
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		ee_goal = Vector3f(xpos, ypos, 0);

		// In the workspace. See reshape() in glSetup.cpp
		float aspect = (float)screenW / screenH;
		ee_goal[0] = 2.0 * (ee_goal[0] / screenW - 0.5) * aspect;
		ee_goal[1] = -2.0 * (ee_goal[1] / screenH - 0.5);
	}
}