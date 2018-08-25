

#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glut.h>

using namespace std;
using std::vector;


class ObjLoader
{

/* -------------------------------------

	Constructor & Destructor

   -------------------------------------*/
public:
	ObjLoader();
	virtual ~ObjLoader();


/* -------------------------------------

	Member Functions

   -------------------------------------*/
public:
	bool LoadObj(char* _fileName);
	int  GetNumberOfVertices() const;
	int  GetNumberOfFaces()	   const;
	
	double* GetVertices(const int _idx) const;
	double* GetNormals(const int _idx) const;
	int*    GetFace(const int _idx) const;
	int*    GetNormIDX(const int _idx) const;

	GLint GetDisplayList();
	void CalculateNormal();
private:

	void normalize(float v[3]);
	void cross(float v1[3], float v2[3], float result[3]);
	

/* -------------------------------------

	Member Variables

   -------------------------------------*/
private:

	typedef struct {
		double x;
		double y;
		double z;
	} gPoint3D;

	typedef struct {
		double x;
		double y;
	} gPoint2D;


	vector<gPoint3D> m_Vertices;
	vector<gPoint3D> m_Normals;
	vector<gPoint3D> m_Faces;
	vector<gPoint3D> m_NormalIDX;
	vector<gPoint2D> m_TexCoord;
};

#endif
