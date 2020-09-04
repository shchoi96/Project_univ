#include "glSetup.h"

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
#include <vector>
using namespace std;

void	initialize(int r);
void	finalize();
void	init();
void	render(GLFWwindow* window);
void	reshape(GLFWwindow* window, int w, int h);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void	mouse(GLFWwindow* window, int button, int action, int mods);

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// control variable
bool pAdd = false;
bool pRemove = false;
bool pDrag = false;
bool pInsert = false;
bool target = false;

// (x, y, z) of data points
vector<Vector3f> p;
int pSelect;
int	nControlPoints = 0;
Vector3f* controlPoints = NULL;

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
	glDisable(GL_DEPTH_TEST);

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
		if (pDrag && target)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			float aspect = (float)screenW / screenH;
			xpos = 2.0 * (xpos / screenW - 0.5) * aspect;
			ypos = -2.0 * (ypos / screenH - 0.5);
			
			p[pSelect] = Vector3f((float)xpos, (float)ypos, 0);
		}
		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
		glfwPollEvents();			// Events
	}
	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void initialize(int r)
{
	int	repetition = r; // 2 for endpoint interpolation

	nControlPoints = p.size() + 2 * repetition;
	controlPoints = new Vector3f[nControlPoints];

	for (int i = 0; i < p.size(); i++)
		controlPoints[i + repetition] = Vector3f(p[i][0], p[i][1], p[i][2]);

	for (int i = 0; i < repetition; i++)
	{
		controlPoints[i] = controlPoints[repetition];							// From beginning
		controlPoints[p.size() + repetition + i] = controlPoints[p.size() + repetition - 1];	// Before ending
	}
}

void init()
{
	if (pAdd)	 pAdd = false;
	if (pDrag)	 pDrag = false;
	if (pRemove) pRemove = false;
	if (pInsert) pInsert = false;
}

void finalize()
{
	delete[] controlPoints;
}

Vector3f BsplinePoint(Vector3f b[4], float t1)
{
	float t2 = t1 * t1;
	float t3 = t2 * t1;

	float B0 = 1 - 3 * t1 + 3 * t2 - t3;
	float B1 = 4 - 6 * t2 + 3 * t3;
	float B2 = 1 + 3 * t1 + 3 * t2 - 3 * t3;
	float B3 = t3;

	return (b[0] * B0 + b[1] * B1 + b[2] * B2 + b[3] * B3) / 6;
}

// Draw the natural cubic spline
void drawBSpline()
{
	int N_POINTS_PER_SEGMENTS = 40;

	// Curve
	glLineWidth(1.5 * dpiScaling);

	// Color
	glColor3f(0, 1, 0);
	
	Vector3f b[4];
	for (int i = 0; i < nControlPoints - 3; i++)
	{
		for (int j = 0; j < 4; j++)
			b[j] = controlPoints[i + j];

		// N_POINTS_PER_SEGMENTS points for each curve segment
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < N_POINTS_PER_SEGMENTS; j++)
		{
			float t = (float)j / (N_POINTS_PER_SEGMENTS - 1);

			Vector3f pt = BsplinePoint(b, t);
			glVertex3fv(pt.data());
		}
		glEnd();
	}
	// Data points
	glPointSize(10);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < p.size(); i++)
	{
		glVertex3f(p[i][0], p[i][1], p[i][2]);
	}
	glEnd();
}

void drawControlPolygon()
{
	int N_POINTS_PER_SEGMENTS = 40;

	// Segment Points
	glPointSize(2);
	glColor3f(0, 0, 0.4);
	glBegin(GL_POINTS);
	for (int i = 0; i < p.size() - 1; i++)
	{
		Vector3f v = p[i + 1] - p[i];
		for (int j = 0; j < N_POINTS_PER_SEGMENTS; j++)
		{
			float t = (float)j / N_POINTS_PER_SEGMENTS;
			Vector3f segment = p[i] + v * t;
			glVertex3f(segment.x(), segment.y(), 0);
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

	if (p.size() > 1)
	{
		initialize(0);
		drawBSpline();
		drawControlPolygon();
	}
	else if (p.size() > 0)
	{
		// Data points
		glPointSize(10);
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		glVertex3f(p[0][0], p[0][1], p[0][2]);
		glEnd();
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

		case GLFW_KEY_A: init(); pAdd = !pAdd;		break;
		case GLFW_KEY_R: init(); pRemove = !pRemove;	break;
		case GLFW_KEY_D: init(); pDrag = !pDrag;		break;
		case GLFW_KEY_I: init(); pInsert = !pInsert;	break;
		}
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// Target on
		target = true;

		// In the screen coordinate
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// In the workspace. See reshape() in glSetup.cpp
		float aspect = (float)screenW / screenH;
		xpos = 2.0 * (xpos / screenW - 0.5) * aspect;
		ypos = -2.0 * (ypos / screenH - 0.5);
		Vector3f pos = Vector3f((float)xpos, (float)ypos, 0);

		// Find the closest point
		if (p.size() > 0)
		{
			Vector3f v = p[0] - pos;
			float dist = v.dot(v);
			pSelect = 0;
			for (int i = 1; i < p.size(); i++)
			{
				v = p[i] - pos;
				if (dist > v.dot(v))
				{
					dist = v.dot(v);
					pSelect = i;
				}
			}
		}

		// control
		if (pAdd)
		{
			p.push_back(pos);
		}
		if (pRemove)
		{
			vector<Vector3f>::iterator iter = p.begin();
			iter += pSelect;
			p.erase(iter);
		}
		if (pInsert)
		{
			// Find the closest edge
			if (p.size() > 1)
			{
				float d = 0;
				Vector3f point;
				for (int i = 0; i < p.size() - 1; i++)
				{
					Vector3f v1 = p[i + 1] - p[i];
					Vector3f v2 = pos - p[i];
					float d1 = sqrt(v1.dot(v1));
					float d2 = sqrt(v2.dot(v2));
					float dot = v1.dot(v2);
					if (dot > 0 && d1 > dot / d1)
					{
						float portion = dot / d1 / d1;
						float compare = sqrt(d2 * d2 - dot * dot / d1 / d1);
						if (i == 0 || d > compare)
						{
							d = compare;
							point = (1 - portion) * p[i] + portion * p[i + 1];
							pSelect = i;
						}
					}
					else
					{
						Vector3f v3 = pos - p[i + 1];
						float compare = sqrt((v3).dot(v3));
						if (d2 > compare)
						{
							if (i == 0 || d > compare)
							{
								d = compare;
								point = p[i + 1];
								pSelect = i;
							}
						}
						else
						{
							if (i == 0 || d > d2)
							{
								d = d2;
								point = p[i];
								pSelect = i;
							}
						}
					}
				}

				// Insert new point
				vector<Vector3f>::iterator iter = p.begin();
				iter += pSelect + 1;
				p.insert(iter, point);
			}
		}
	}
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// Target off
		target = false;
	}
}