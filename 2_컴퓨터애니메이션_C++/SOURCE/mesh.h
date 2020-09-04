#ifndef _MESH_H_
#define _MESH_H_

#include <Eigen/Dense>
using namespace Eigen;

bool readMesh(const char* fname, MatrixXf& vertex, MatrixXf& normal, ArrayXXi& face);

#endif // _MESH_H
