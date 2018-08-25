/* -------------------------------------
	Mode selection
 -------------------------------------*/

// #define _USE_MATH_DEFINES
#define _DG_USING_CV
// #define _DG_USING_CG
// #define _DG_USING_NEWMAT

/* -------------------------------------
	Associated linking library
 -------------------------------------*/
#pragma once
#pragma comment(lib, "glew32.lib")

#ifdef _DG_USING_CV // For Intel OpenCV Language

#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")

#endif

/* -------------------------------------
	Include System header files
 -------------------------------------*/
#include <windows.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <cmath>
#include <map>
//#include <winsock2.h>

/* -------------------------------------
	Include Graphics & Vision Library
-------------------------------------*/
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>

#ifdef	_DG_USING_CV // For Intel OpenCV Language
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#endif

/* -------------------------------------
	Using Name space
-------------------------------------*/
using namespace std;
using std::vector;

// User define & macro for math
#define TRACKBALLSIZE	(0.8f)						// Size of Trackball 
#define DG_SQRT(x)		sqrt((x))					// sqrt X^2
#define DG_SQR(x)		((x) * (x))					// X^2
#define DG_DEG2RAD(x)	(x * M_PI / 180.0)			// Degree to Radian
#define DG_RAD2DEG(x)	(x * 180.0 / M_PI)			// Radian to Degree
#define DG_MIN(x, y)	( x > y ? y : x )			// Minimum value
#define DG_MAX(x, y)	( x > y ? x : y )			// Maximum value
#define DG_ABS(x)		( x > 0.0 ? x : (-1.0 * x)) // Absolute

// Key & Mouse state
#define DG_LEFT_BUTTON		1
#define DG_RIGHT_BUTTON		2
#define DG_WHEEL_BUTTON		3
