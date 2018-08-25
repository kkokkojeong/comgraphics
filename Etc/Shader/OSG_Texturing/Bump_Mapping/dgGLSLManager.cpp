/*
*		dgGLSLManager.cpp
*
*
*		Copyright 2013 Jeong SeokCheol in DongGuk Univ. All rights reserved.
*	
*/

#include "dgGLSLManager.h"

/*----------------------------------------------------

	@ Construction & Destruction

-----------------------------------------------------*/
GLSLManager::GLSLManager()
{
	m_totalShaders = 0;
	m_shaders[VERTEX_SHADER] = 0;
	m_shaders[FRAGMENT_SHADER] = 0;
	m_shaders[GEOMETRY_SHADER] = 0;
	m_attributeList.clear();
	m_uniformList.clear();
}

GLSLManager::~GLSLManager()
{
	m_attributeList.clear();
	m_uniformList.clear();
}

/*----------------------------------------------------

	@ Member Functions

-----------------------------------------------------*/
void GLSLManager::LoadFromString(GLenum type, const string& source)
{
	// Create shader
	GLuint shader = glCreateShader(type);

	const char *ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	// Compile Shader
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog= new GLchar[infoLogLength];
		glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
		cerr << "Compile log: "<< infoLog << endl;
		delete [] infoLog;
	}

	m_shaders[m_totalShaders++] = shader;

}

void GLSLManager::CreateAndLinkProgram()
{
	// Create the program
	m_Program = glCreateProgram();

	// attach shader to program..
	if(m_shaders[VERTEX_SHADER] != 0){
		glAttachShader(m_Program, m_shaders[VERTEX_SHADER]);
	}

	if(m_shaders[GEOMETRY_SHADER] != 0){
		glAttachShader(m_Program, m_shaders[GEOMETRY_SHADER]);
	}

	if(m_shaders[FRAGMENT_SHADER] != 0){
		glAttachShader(m_Program, m_shaders[FRAGMENT_SHADER]);
	}

	// link and check ..
	GLint status;
	glLinkProgram(m_Program);
	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	
	if(status == GL_FALSE){
		GLint infoLogLength;

		glGetProgramiv (m_Program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetProgramInfoLog (m_Program, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		delete [] infoLog;
	}

	// delete shader files
	glDeleteShader(m_shaders[VERTEX_SHADER]);
	glDeleteShader(m_shaders[FRAGMENT_SHADER]);
	glDeleteShader(m_shaders[GEOMETRY_SHADER]);
}

void GLSLManager::Use()
{
	glUseProgram(m_Program);
}

void GLSLManager::UnUse()
{
	glUseProgram(0);
}

void GLSLManager::AddAttribute(const string& attribute)
{
	m_attributeList[attribute] = glGetAttribLocation(m_Program, attribute.c_str());
}

void GLSLManager::AddUniform(const string& uniform) 
{
	m_uniformList[uniform] = glGetUniformLocation(m_Program, uniform.c_str());
}

GLuint GLSLManager::operator[](const string& attribute)
{
	return m_attributeList[attribute];
}

GLuint GLSLManager::operator()(const string& uniform)
{
	return m_uniformList[uniform];
}

GLuint GLSLManager::GetProgram() const 
{
	return m_Program;
}

void GLSLManager::LoadFromFile(GLenum whichShader, const string& filename)
{
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if(fp) {		 
		/*string line, buffer;
		while(getline(fp, line)) {
			buffer.append(line);
			buffer.append("\r\n");
		}		*/
		string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));

		//copy to source
		LoadFromString(whichShader, buffer);		
	} 
	else {
		cerr << "Error loading shader: "<< filename << endl;
	}
}

void GLSLManager::DeleteProgram()
{
	glDeleteProgram(m_Program);

	m_Program = -1;
}