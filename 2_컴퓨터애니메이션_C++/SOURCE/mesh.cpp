#include "mesh.h"

#include <fstream>

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;

bool readMesh(const char* fname, MatrixXf& vertex, MatrixXf& normal, ArrayXXi& face)
{
	ifstream is(fname);
	if (is.fail()) return false;

	char magicNumber[256];
	is >> magicNumber;

	// # vertices, # faces, # edges
	int nVertices = 0, nFaces = 0, nEdges = 0;
	is >> nVertices >> nFaces >> nEdges;
	cout << "# vertices = " << nVertices << endl;
	cout << "# faces = " << nFaces << endl;

	// Vertices
	vertex.resize(3, nVertices);
	for (int i = 0; i < nVertices; ++i)
		is >> vertex(0, i) >> vertex(1, i) >> vertex(2, i);

	// Normals
	normal.resize(3, nVertices);
	normal.setZero();

	// Faces
	face.resize(3, nFaces); // Only support triangles
	int n;
	for (int i = 0; i < nFaces; ++i)
	{
		is >> n >> face(0, i) >> face(1, i) >> face(2, i);
		if (n != 3) cout << "# vertices of the " << i << "-th faces = " << n << endl;

		// Normal vector of the face
		Vector3f v1 = vertex.col(face(1, i)) - vertex.col(face(0, i));
		Vector3f v2 = vertex.col(face(2, i)) - vertex.col(face(0, i));
		Vector3f v = v1.cross(v2).normalized();

		// Add it to the normal vector of each vertex
		normal.col(face(0, i)) += v;
		normal.col(face(1, i)) += v;
		normal.col(face(2, i)) += v;
	}

	// Normalization of the normal vectors
	for (int i = 0; i < nVertices; ++i)
		normal.col(i).normalize();

	return true;
}