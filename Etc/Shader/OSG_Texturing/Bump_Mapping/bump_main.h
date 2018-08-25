
// Include useful library
#include "dgCommon.h"
#include "dgObjLoader.h"
#include "dgTrackball.h"
#include "dgGLSLManager.h"

// proto-types
void display();
void reshape(int _w, int _h);
void keyboard(unsigned char _key, int _x, int _y);
void mouse(int _button, int _state, int _x, int _y);
void motion(int _x, int _y);
void idle();
void DeallocateMemory();
void AllocateMemory();
void InitializeGL();
void SystemVariableSetting();
void specialKey(int key, int x, int y);

// Global variables
Trackball		 g_Trackball;
ObjLoader		g_ObjLoader;
GLint			g_WindowSize[2];
GLfloat			g_TranslateFactor[3];
GLfloat			g_ScaleFactor[3];
GLboolean		g_RotateState;
GLboolean		g_Description;
GLboolean		g_TranslateState;
GLboolean		g_ScaleState;
GLint			g_TransPrev[2];
GLint			g_ScalePrev[2];
GLint			g_LoadedObj;
GLSLManager		g_GLSLManage;

GLuint			g_BaseTex;
GLuint			g_NormalTex;
IplImage		*g_BaseImage;
IplImage		*g_NormalImage;