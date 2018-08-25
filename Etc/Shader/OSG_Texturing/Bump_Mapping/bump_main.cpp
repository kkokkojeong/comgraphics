
#include "bump_main.h"

/* -------------------------------------------------

		@ Display Callback Functions

-------------------------------------------------*/

GLuint TextureMapping(IplImage *_tex)
{
	//glEnable(GL_TEXTURE_CUBE_MAP)

	GLuint inputTex;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &inputTex);
	glBindTexture(GL_TEXTURE_2D, inputTex);

	// Set the texture magnification & minifying 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Create the best possible precision of the depth map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _tex->width, _tex->height,
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE, _tex->imageData);

	return inputTex;
}

void display()
{
//	int *face, *normIDX;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set Camera
	gluLookAt(0.0, 0.0, 5.0,			// camera position
		0.0, 0.0, 0.0,					// lookat point
		0.0, 1.0, 0.0					// upvector
		);

	// To-do Drawing
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDisable(GL_TEXTURE_2D);

	float mat[4][4];
	glPushMatrix();
	{
		glTranslatef(g_TranslateFactor[0], g_TranslateFactor[1], g_TranslateFactor[2]);
		glScalef(g_ScaleFactor[0], g_ScaleFactor[1], g_ScaleFactor[2]);
		g_Trackball.GetMatrix(mat);
		glMultMatrixf(&mat[0][0]);	
		glEnable(GL_NORMALIZE);
		
		
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);
			/* Counter clockwise (GL_CCW) winding */
			glTexCoord2f(0,0);				glVertex2f(-3,-3);
			glTexCoord2f(1,0);				glVertex2f( 3,-3);
			glTexCoord2f(1,1);				glVertex2f( 3, 3);
			glTexCoord2f(0,1);				glVertex2f(-3, 3);
		glEnd();
	}
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

/* -------------------------------------------------

		@ Reshape Callback Functions

-------------------------------------------------*/
void reshape(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	// use perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f * _w / _h, 1.0f, 100.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// update to Resize
	g_Trackball.SetTrackballCanvasSize(_w, _h);
	g_WindowSize[0] = _w;
	g_WindowSize[1] = _h;
}
/* -------------------------------------------------

		@ Mouse Callback Functions

-------------------------------------------------*/
void mouse(int _button, int _state, int _x, int _y)
{

	switch(_button) {
		case GLUT_LEFT_BUTTON:
			if(_state == GLUT_UP) {

			} else if(_state == GLUT_DOWN) {
				if(g_RotateState) g_Trackball.SetPreviousPosition(_x, _y);				
				if(g_TranslateState) {
					g_TransPrev[0] = _x;
					g_TransPrev[1] = _y;
				}
				if(g_ScaleState) {
					g_ScalePrev[0] = _x;
					g_ScalePrev[1] = _y;
				}
			}
			break;	
		case GLUT_RIGHT_BUTTON:
			if(_state == GLUT_UP) {

			} else if(_state == GLUT_DOWN) {
				if(g_TranslateState) {
					g_TransPrev[0] = _x;
					g_TransPrev[1] = _y;
				}
				if(g_ScaleState) {
					g_ScalePrev[0] = _x;
					g_ScalePrev[1] = _y;
				}
			}
			break;

	}
}
void motion(int _x, int _y)
{
	if(g_RotateState) g_Trackball.SetTrackballUpdate(_x, _y);
	if(g_TranslateState)  {
		g_TranslateFactor[0] += (_x - g_TransPrev[0]) / 100.0;
		g_TranslateFactor[1] += (g_TransPrev[1] - _y) / 100.0;

		g_TransPrev[0] = _x;
		g_TransPrev[1] = _y;
	}
	if(g_ScaleState) {
		float dx = (_x - g_ScalePrev[0]) / 400.0;
		float dy = (g_ScalePrev[1] - _y) / 400.0;
		float scaleV = 0.0;
		scaleV =  dx + dy;

		g_ScaleFactor[0] += scaleV;
		g_ScaleFactor[1] += scaleV;
		g_ScaleFactor[2] += scaleV;
		g_ScalePrev[0] = _x;
		g_ScalePrev[1] = _y;
	}
}

/* -------------------------------------------------

		@ Idle Callback Functions

-------------------------------------------------*/
void idle()
{
	glutPostRedisplay();
}

/* -------------------------------------------------

		@ Keyboard Callback Functions

-------------------------------------------------*/
void keyboard(unsigned char _key , int _x, int _y)
{
	switch(_key){
		case 27:
		case 'Q':
		case 'q':
			exit(0);
			break;

		case 'r':
		case 'R': // rotate mode
			g_RotateState = true;
			g_TranslateState = false;
			g_ScaleState = false;
			break;

		case 't':
		case 'T': // translate mode
			g_RotateState = false;
			g_TranslateState = true;
			g_ScaleState = false;
			break;

		case 's':
		case 'S': // scale mode
			g_RotateState = false;
			g_TranslateState = false;
			g_ScaleState = true;
			break;

		case 'c':
		case 'C': // cancel
			g_RotateState		= false;
			g_Description		= true;
			g_TranslateState	= false;
			g_ScaleState		= false;

			g_TranslateFactor[0] = 0.0;
			g_TranslateFactor[1] = 0.0;
			g_TranslateFactor[2] = 0.0;

			g_ScaleFactor[0] = 3.0;
			g_ScaleFactor[1] = 3.0;
			g_ScaleFactor[2] = 3.0;
			g_Trackball.Clear();
			break;

		case 'U':
		case 'u':	// use shader
			g_GLSLManage.Use();
			break;

		case 'D':
		case 'd':	// unuse shader
			g_GLSLManage.UnUse();
			break;
	}
}

void InitializeShaders()
{

	g_GLSLManage.LoadFromFile(GL_VERTEX_SHADER,   "bumpmapping.vert");
	g_GLSLManage.LoadFromFile(GL_FRAGMENT_SHADER, "bumpmapping.frag");

	g_GLSLManage.CreateAndLinkProgram();
	g_GLSLManage.Use();

	// Get uniform variables
	GLuint texLoc = glGetUniformLocation(g_GLSLManage.GetProgram(), "baseTex");
	glUniform1i(texLoc, 0);

	texLoc = glGetUniformLocation(g_GLSLManage.GetProgram(), "normalTex");
	glUniform1i(texLoc, 1);

 	glActiveTexture(GL_TEXTURE0);
 	glBindTexture(GL_TEXTURE_2D, g_BaseTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_NormalTex);

}

/* -------------------------------------------------

		@ Initialize Functions

-------------------------------------------------*/
void InitializeGL()
{
	// Light Properties
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lightPosition[] = {0.0, 0.0, 10.0, 0.0};
	GLfloat lightModelAmbient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat localView[] = {0.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, localView);

	float diffR, diffG, diffB;
	float specR, specG, specB;

	diffR = 200 / 255.0;
	diffG = 100.0182 / 255.0;
	diffB = 50.11858 / 255.0;

	specR = 220 / 255.0;
	specG = 220 / 255.0;
	specB = 220 / 255.0;

	GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_diffuse[] = { diffR, diffG, diffB, 1.0f };
	GLfloat mat_specular[] = { specR, specG, specB, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearDepth(1.0f);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	// Load the Base & Normal Texture(.bmp)
	
//	g_BaseTex   = LoadTextureFile("../Data/rock.bmp");
//	g_NormalTex = LoadTextureFile("../Data/rockNormal.bmp");

	g_BaseImage = cvLoadImage("../Bump/rock.bmp", 1);
	g_BaseTex = TextureMapping(g_BaseImage);

	g_NormalImage = cvLoadImage("../Bump/rockNormal.bmp", 1);
	g_NormalTex = TextureMapping(g_NormalImage);

	g_Trackball.SetTrackballCanvasSize(g_WindowSize[0], g_WindowSize[1]);
}
void SystemVariableSetting()
{
	g_WindowSize[0] = 800;
	g_WindowSize[1] = 800;
	g_RotateState		= false;
	g_Description		= true;
	g_TranslateState	= false;
	g_ScaleState		= false;

	g_TranslateFactor[0] = 0.0;
	g_TranslateFactor[1] = 0.0;
	g_TranslateFactor[2] = 0.0;

	g_ScaleFactor[0] = 1.0;
	g_ScaleFactor[1] = 1.0;
	g_ScaleFactor[2] = 1.0;
}

int main(int argc, char* argv[])
{
	SystemVariableSetting();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// use double buffer, RGBA and depth buffer
	glutInitWindowPosition(100, 100);							// default window position
	glutInitWindowSize(800, 800);								// default window size

	glutCreateWindow("2. Bump Mapping using GLSL");

	InitializeGL();	// initialize opengl setting

	// set callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Set Glew.h 
	glewInit();													
	if (glewIsSupported("GL_VERSION_2_1")){
		printf("Ready for OpenGL 2.1\n");
	}
	else {
		printf("OpenGL 2.1 not supported\n");
		exit(1);
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4){
		printf("Ready for GLSL - Vertex, Fragment, and Geometry Units\n");
	}
	else {
		printf("Not totally ready :( \n");
		exit(1);
	}
	// Print OpenGL, GLEW Version
	printf ("Using glew : %s\n", glewGetString(GLEW_VERSION));
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	// Initialize Shader GLSL
	InitializeShaders();

	glutMainLoop();

	return 0;
}