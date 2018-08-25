/*
*		dgGLSLManager.h
*
*
*		Copyright 2013 Jeong SeokCheol in DongGuk Univ. All rights reserved.
*	
*/


#ifndef __DG_GLSLMANAGER_H__
#define __DG_GLSLMANAGER_H__

#include "dgCommon.h"

class GLSLManager
{
public:
	// Construction & Destruction
	GLSLManager();
	~GLSLManager();

	// Member Functions
	void LoadFromString(GLenum type, const string& source);
	void LoadFromFile(GLenum whichShader, const string& filename);
	void CreateAndLinkProgram();
	void Use();
	void UnUse();
	void AddAttribute(const string& attribute);
	void AddUniform(const string& uniform);
	GLuint GetProgram() const;

	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string& attribute);
	GLuint operator()(const string& uniform);

	void DeleteProgram();

private:
	enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
	GLuint	m_Program;
	int m_totalShaders;
	//0->vertexshader, 1->fragmentshader, 2->geometryshader
	GLuint m_shaders[3];	
	map<string,GLuint> m_attributeList;
	map<string,GLuint> m_uniformList;
};

#endif //__DG_GLSLMANAGER_H__