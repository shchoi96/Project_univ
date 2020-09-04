// ������, ����, ��FILL
#include "glSetup.h"

#include <iostream>

void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// Control variable
GLenum	mode = GL_POINTS;
bool	stipple = false;
bool	fill = true;

int main(int argc, char* argv[])
{
	// Orthographics viewing
	perspectiveView = false;

	// Initialize the OpenGL system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	// Callbacks
	glfwSetKeyCallback(window, keyboard);

	// Viewport and perspective setting
	reshape(window, windowW, windowH);

	// Usage
	std::cout << std::endl;
	std::cout << "Keyboard Input: [1:7] for primitive selection" << std::endl;
	std::cout << "Keyboard Input: s for stippling on/off" << std::endl;
	std::cout << "Keyboard Input: f for polygon filling on/off" << std::endl;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
		glfwPollEvents();			// Events
	}

	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void render(GLFWwindow* window)
{
	// Background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Stipple
	if (stipple)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3 * dpiScaling, 0xcccc);
	}
	else
		glDisable(GL_LINE_STIPPLE);

	// Polygon mode
	if (fill)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPointSize(5 * dpiScaling);
	glLineWidth(5 * dpiScaling);

	// Drawing with various primitive types
	glBegin(mode);
	{
		glColor3f(1, 0, 0);
		glVertex2f(-0.5, -0.5);

		glColor3f(0, 1, 0);
		glVertex2f(0.5, -0.5);

		glColor3f(0, 0, 1);
		glVertex2f(0.5, 0.5);

		glColor3f(0, 0, 0);
		glVertex2f(-0.5, 0.5);
	}
	glEnd();
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

		// Control for stippling and polygon fillling
		case GLFW_KEY_S: stipple = !stipple;	break;
		case GLFW_KEY_F: fill = !fill;			break;

		// Numeric
		case GLFW_KEY_1:	mode = GL_POINTS;			break;
		case GLFW_KEY_2:	mode = GL_LINES;			break;
		case GLFW_KEY_3:	mode = GL_LINE_STRIP;		break;
		case GLFW_KEY_4:	mode = GL_LINE_LOOP;		break;
		case GLFW_KEY_5:	mode = GL_TRIANGLES;		break;
		case GLFW_KEY_6:	mode = GL_TRIANGLE_STRIP;	break;
		case GLFW_KEY_7:	mode = GL_TRIANGLE_FAN;		break;
		
		// Special keys
		case GLFW_KEY_SPACE:			break;
		case GLFW_KEY_PERIOD:			break;
		case GLFW_KEY_LEFT:				break;
		case GLFW_KEY_RIGHT:			break;
		case GLFW_KEY_LEFT_BRACKET:		break;
		case GLFW_KEY_RIGHT_BRACKET:	break;
		case GLFW_KEY_UP:				break;
		case GLFW_KEY_DOWN:				break;
		}
	}
}