#include "glSetup.h"

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
#include <vector>
using namespace std;

void	init();
void	render(GLFWwindow* window);
void	reshape(GLFWwindow* window, int w, int h);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void	mouse(GLFWwindow* window, int button, int action, int mods);

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// control variable
bool pAdd	 = false;
bool pRemove = false;
bool pDrag	 = false;
bool pInsert = false;
bool target  = false;

// (x, y, z) of data points
//const int	maxNum = 255;
int   curNum = 0;
//float p[maxNum + 1][3];
vector<vector<float>> p;
int pSelect;

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

			p[pSelect].clear();
			p[pSelect].push_back((float)xpos);
			p[pSelect].push_back((float)ypos);
			p[pSelect].push_back(0);
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

// Linear system: Ac = b
//		from n equations of the form:
//		p_i(t) = c_0^i + (c_1^i * t^1) + (c_2^i * t^2) + (c_3^i * t^3)
//
MatrixXf A;
MatrixXf b;  // 4n * 3 to solve the 3 linear systems at once
MatrixXf c;  // 4n * 3 to solve the 3 linear systems at once

void buildLinearSystem()
{
	// Build A and b for N segments. A is independent of the locations of the points.
	A.resize(4 * curNum, 4 * curNum);
	A.setZero();

	b.resize(4 * curNum, 3);

	// Equation number
	int row = 0;

	// 2n equations for end point interpolation
	for (int i = 0; i < curNum - 1; i++, row += 2)
	{
		// p_i(0) = c_0^i
		A(row, 4 * i + 0) = 1;

		b(row, 0) = p[i][0];
		b(row, 1) = p[i][1];
		b(row, 2) = p[i][2];

		// p_i(1) = c_0^i + c_1^i + c_2^i + c_3^i
		A(row + 1, 4 * i + 0) = 1;
		A(row + 1, 4 * i + 1) = 1;
		A(row + 1, 4 * i + 2) = 1;
		A(row + 1, 4 * i + 3) = 1;

		b(row + 1, 0) = p[i + 1][0];
		b(row + 1, 1) = p[i + 1][1];
		b(row + 1, 2) = p[i + 1][2];
	}

	// (n-1) equations for the tangential continuity
	for (int i = 0; i < curNum - 1; i++, row++)
	{
		// p'_i(1) = 1+c_1^i + 2*c_2^i + 3*c_3^i = c_1^(i+1) = p'_(i+1)(0)
		A(row, 4 * i + 1) = 1;
		A(row, 4 * i + 2) = 2;
		A(row, 4 * i + 3) = 3;
		A(row, 4 * i + 5) = -1;

		b(row, 0) = 0;
		b(row, 1) = 0;
		b(row, 2) = 0;
	}

	// (n-1) equations for the second-derivative continuity
	for (int i = 0; i < curNum - 1; i++, row++)
	{
		// p''_i(1) = 2*c_2^i + 6*c_3^i = 2*c_2^(i+1) = p''_(i+1)(0)
		A(row, 4 * i + 2) = 2;
		A(row, 4 * i + 3) = 6;
		A(row, 4 * i + 6) = -2;

		b(row, 0) = 0;
		b(row, 1) = 0;
		b(row, 2) = 0;
	}

	// 2 equations for the natural boundary condition
	{
		// p''_0(0) = 2*c_2^0 = 0
		A(row, 2) = 2;

		b(row, 0) = 0;
		b(row, 1) = 0;
		b(row, 2) = 0;

		row++;

		// p''_(n-1)(1) = 2*c_2^(n-1) + 6*c_3^(n-1) = 0
		A(row, 4 * (curNum - 1) + 2) = 2;
		A(row, 4 * (curNum - 1) + 3) = 6;

		b(row, 0) = 0;
		b(row, 1) = 0;
		b(row, 2) = 0;

		row++;
	}
}

void solveLinearSystem()
{
	// Solve the 3 linear systems at once
	c = A.colPivHouseholderQr().solve(b);
}

void init()
{
	if (pAdd)	 pAdd = false;
	if (pDrag)	 pDrag = false;
	if (pRemove) pRemove = false;
	if (pInsert) pInsert = false;
}

// Draw the natural cubic spline
void drawNaturalCubicSpline()
{
	int N_SUB_SEGMENTS = 40;

	// Curve
	glLineWidth(1.5 * dpiScaling);
	glColor3f(0, 0, 0);
	for (int i = 0; i < curNum - 1; i++)
	{
		// N_SUB_SEGMENTS for each curve segment
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < N_SUB_SEGMENTS; j++)
		{
			float t = (float)j / (N_SUB_SEGMENTS - 1);

			float x = c(4 * i + 0, 0) + (c(4 * i + 1, 0) + (c(4 * i + 2, 0) + c(4 * i + 3, 0) * t) * t) * t;
			float y = c(4 * i + 0, 1) + (c(4 * i + 1, 1) + (c(4 * i + 2, 1) + c(4 * i + 3, 1) * t) * t) * t;
			float z = c(4 * i + 0, 2) + (c(4 * i + 1, 2) + (c(4 * i + 2, 2) + c(4 * i + 3, 2) * t) * t) * t;
			glVertex3f(x, y, z);
		}
		glEnd();
	}

	// Data points
	glPointSize(10);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < curNum; i++)
		glVertex3f(p[i][0], p[i][1], p[i][2]);
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

	if (curNum > 1)
	{
		buildLinearSystem();
		solveLinearSystem();

		// Draw the natural cubic spline curve
		drawNaturalCubicSpline();
	}
	else if (curNum > 0)
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

		case GLFW_KEY_A: init(); pAdd	 = !pAdd;		break;
		case GLFW_KEY_R: init(); pRemove = !pRemove;	break;
		case GLFW_KEY_D: init(); pDrag	 = !pDrag;		break;
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
		xpos = 2.0 * (xpos/screenW - 0.5) * aspect;
		ypos = -2.0 * (ypos/screenH - 0.5);

		// Find the closest point
		if (curNum > 0)
		{
			float x = p[0][0] - xpos;
			float y = p[0][1] - ypos;
			float dist = x * x + y * y;
			pSelect = 0;
			for (int i = 1; i < curNum; i++)
			{
				x = p[i][0] - xpos;
				y = p[i][1] - ypos;
				if (dist > x * x + y * y)
				{
					dist = x * x + y * y;
					pSelect = i;
				}
			}
		}

		// control
		if (pAdd)
		{
			vector<float> temp;
			temp.push_back((float)xpos);
			temp.push_back((float)ypos);
			temp.push_back(0);
			p.push_back(temp);
			curNum++;
		}
		if (pRemove)
		{
			vector<vector<float>>::iterator iter = p.begin();
			iter += pSelect;
			p.erase(iter);
			curNum--;
		}
		if (pInsert)
		{
			// Find the closest segment two points
			if (curNum > 1)
			{
				int N_SUB_SEGMENTS = 40;
				float x1 = c(4 * 0 + 0, 0);
				float y1 = c(4 * 0 + 0, 1);
				float t = (float)1 / (N_SUB_SEGMENTS - 1);
				float x2 = c(4 * 0 + 0, 0) + (c(4 * 0 + 1, 0) + (c(4 * 0 + 2, 0) + c(4 * 0 + 3, 0) * t) * t) * t;
				float y2 = c(4 * 0 + 0, 1) + (c(4 * 0 + 1, 1) + (c(4 * 0 + 2, 1) + c(4 * 0 + 3, 1) * t) * t) * t;
				float vx1 = x2 - x1;
				float vy1 = y2 - y1;
				float vx2 = xpos - x1;
				float vy2 = ypos - y1;
				float dist1 = sqrt(vx1 * vx1 + vy1 * vy1);
				float dist2 = sqrt(vx2 * vx2 + vy2 * vy2);
				float dot = vx1 * vx2 + vy1 * vy2;
				float dist, px, py;
				pSelect = 0;
				if (dot > 0 && dist1 > dot / dist1)
				{
					dist = sqrt(dist2 * dist2 - dot * dot / dist1 / dist1);
					float portion = dot / dist1 / dist1;
					px = (1 - portion) * x1 + portion * x2;
					py = (1 - portion) * y1 + portion * y2;
				}
				else
				{
					float tempDist = sqrt((xpos - x2) * (xpos - x2) + (ypos - y2) * (ypos - y2));
					if (dist2 > tempDist)
					{
						dist = tempDist;
						px = x2;
						py = y2;
					}
					else
					{
						dist = dist2;
						px = x1;
						py = y1;
					}
				}
				// Curve
				for (int i = 0; i < curNum - 1; i++)
				{
					// N_SUB_SEGMENTS for each curve segment
					for (int j = 2; j < N_SUB_SEGMENTS; j++)
					{
						x1 = x2;
						y1 = y2;
						t = (float)j / (N_SUB_SEGMENTS - 1);
						x2 = c(4 * i + 0, 0) + (c(4 * i + 1, 0) + (c(4 * i + 2, 0) + c(4 * i + 3, 0) * t) * t) * t;
						y2 = c(4 * i + 0, 1) + (c(4 * i + 1, 1) + (c(4 * i + 2, 1) + c(4 * i + 3, 1) * t) * t) * t;
						vx1 = x2 - x1;		vy1 = y2 - y1;
						vx2 = xpos - x1;	vy2 = ypos - y1;
						dist1 = sqrt(vx1 * vx1 + vy1 * vy1);
						dist2 = sqrt(vx2 * vx2 + vy2 * vy2);
						dot = vx1 * vx2 + vy1 * vy2;
						if (dot > 0 && dist1 > dot / dist1)
						{
							float compare = sqrt(dist2 * dist2 - dot * dot / dist1 / dist1);
							if (dist > compare)
							{
								dist = compare;
								float portion = dot / dist1 / dist1;
								px = (1 - portion) * x1 + portion * x2;
								py = (1 - portion) * y1 + portion * y2;
								pSelect = i;
							}
						}
						else
						{
							float tempDist = sqrt((xpos - x2) * (xpos - x2) + (ypos - y2) * (ypos - y2));
							if (dist2 > tempDist)
							{
								if (dist > tempDist)
								{
									dist = tempDist;
									px = x2;
									py = y2;
									pSelect = i;
								}
							}
							else
							{
								if (dist > dist2)
								{
									dist = dist2;
									px = x1;
									py = y1;
									pSelect = i;
								}
							}
						}
					}
				}

				// Insert new point
				vector<float> temp;
				temp.push_back(px);
				temp.push_back(py);
				temp.push_back(0);
				vector<vector<float>>::iterator iter = p.begin();
				iter += pSelect + 1;
				p.insert(iter, temp);
				curNum++;
			}



		}
	}
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// Target off
		target = false;
	}
}