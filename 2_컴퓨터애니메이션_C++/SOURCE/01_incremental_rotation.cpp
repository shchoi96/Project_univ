#include "glSetup.h"
#include "mesh.h"

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;

void	init(const char* filename);
void	setupLight();

void	update();
void	render(GLFWwindow* window);
void	reshape(GLFWwindow* window, int w, int h);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// Play configuration
bool	pause = false;

// Camera configuation
Vector3f eye(3, 3, 3);
Vector3f center(0, 0, 0);
Vector3f up(0, 1, 0);

// Light configuration
Vector4f light(5.0, 5.0, 0.0, 1);	// Light position

// Global coordinate frame
float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

// Colors
GLfloat bgColor[4] = { 1,1,1,1 };

// Default mesh file name
const char* defaultMeshFileName = "Models/bunny.off";

// control parameter
bool	useQuaternion = false;

void printConfiguration()
{
	if (useQuaternion)	cout << "Quaternion with normalization" << endl;
	else				cout << "Rotation matrix without normalization" << endl;
}

int main(int argc, char* argv[])
{
	// Immediate mode to verify the artifacts ASAP
	vsync = 0;

	// Filename for deformable body configuration
	const char* filename;
	if (argc >= 2)	filename = argv[1];
	else			filename = defaultMeshFileName;

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

	// Usage
	cout << endl;
	cout << "Keyboard Input: q for quaternion with normalization" << endl;
	cout << "Keyboard Input: r for rotation matrix without normalization" << endl;
	cout << endl;

	// Print the current configuration
	printConfiguration();

	// Initialization - Main loop - Finalization
	init(filename);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		if (!pause) update();

		render(window);				// Draw one frame
		glfwSwapBuffers(window);	// Swap buffers
		glfwPollEvents();			// Events
	}

	// Terminate the glfw system
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// Mesh
MatrixXf	vertex;
MatrixXf	normal;
ArrayXXi	face;

// Time
int frame = 0;

// Orientation
Matrix4f	T;
Quaternionf q;
Vector3f	axis;

void init(const char* filename)
{
	// Mesh from the file
	cout << "Reading " << filename << endl;
	readMesh(filename, vertex, normal, face);

	// Initial orientation of the mesh
	T.setIdentity();
	q.setIdentity();
}

void update()
{
	// Generate a random axis at every 2,000 frames
	if (frame % 2000 == 0) axis = Vector3f::Random().normalized();

	// Accelerate the accumulation at every 5-th 2,000 frames
	int N = 1;
	if ((frame / 2000) % 5 == 4)
	{
		// 10,000x fast forward for severe artifacts
		N = 10000;

		// In this case, generate a random axis at every 100 frames
		if (frame % 100 == 0)	axis = Vector3f::Random().normalized();
	}

	// Accumulate the incremental rotation N times
	for (int j = 0; j < N; j++)
	{
		// Incremental rotation
		float angleInc = 0.001;
		AngleAxisf aa(angleInc, axis);

		if (useQuaternion)
		{
			// Rotate the orientation of the mesh using quaternion
			q = Quaternionf(aa) * q;

			// Normalize the quaternion
			q.normalize();

			// Convert the quaternion into the rotation matrix
			T.block<3, 3>(0, 0) = Matrix3f(q);
		}
		else
		{
			// Rotate the orientation of the mesh using rotation matrix
			T.block<3, 3>(0, 0) = Matrix3f(aa) * T.block<3, 3>(0, 0);
		}
	}

	// Check the integrity at every 2,000 frames
	if (!useQuaternion && frame % 2000 == 0)
	{
		Vector3f	x = T.block<3, 3>(0, 0).col(0);
		Vector3f	y = T.block<3, 3>(0, 0).col(1);
		Vector3f	z = T.block<3, 3>(0, 0).col(2);

		cout << "Unitary:\t" << x.norm() << ", " << y.norm() << ", " << z.norm() << endl;
		cout << "Orthogonal:\t" << x.dot(y) << ", " << y.dot(z) << ", " << z.dot(x) << endl;
		cout << "Determinant:\t" << T.block<3, 3>(0, 0).determinant() << endl << endl;
	}
	frame++;
}

// Draw a sphere after setting up its material
void drawMesh()
{
	// Material
	GLfloat mat_ambient[4] =  { 0.10, 0.10 ,0.10 ,1 };
	GLfloat mat_diffuse[4] =  { 0.95, 0.95, 0.95, 1 };
	GLfloat mat_specular[4] = { 0.50, 0.50, 0.50, 1 };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	// Mesh
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < face.cols(); ++i)
	{
		glNormal3fv(normal.col(face(0, i)).data());
		glVertex3fv(vertex.col(face(0, i)).data());

		glNormal3fv(normal.col(face(1, i)).data());
		glVertex3fv(vertex.col(face(1, i)).data());

		glNormal3fv(normal.col(face(2, i)).data());
		glVertex3fv(vertex.col(face(2, i)).data());
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
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH);

	// Lighting
	setupLight();

	// Apply the rotation. Eigen and OpenGL employ the same colum-major representation.
	glMultMatrixf(T.data());

	// Draw the mesh after setting up the material
	drawMesh();
}

// Light
void setupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[4] =  { 0.1, 0.1, 0.1, 1 };
	GLfloat diffuse[4] =  { 1.0, 1.0, 1.0, 1 };
	GLfloat specular[4] = { 1.0, 1.0, 1.0, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light.data());
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			// Quaternion
		case GLFW_KEY_Q:	useQuaternion = true;	printConfiguration(); break;
		case GLFW_KEY_R:	useQuaternion = false;	printConfiguration(); break;

			// Quit
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;

			// Play/pause toggle
		case GLFW_KEY_SPACE:	pause = !pause; break;
		}
	}
}