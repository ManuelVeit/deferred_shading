#pragma once



#include "stdafx.h"
#include "ShaderProgram.h"
#include "ObjectMaterial.h"




class BasicRenderableObject
{
protected :
	GLuint					m_Vao;

	GLuint					m_VBOPosition;
	GLuint					m_VBONormal;
	GLuint					m_VBOTexCoord;
	std::vector<GLuint>		m_SubObjectsVBOTriangles;
	
	GLuint					m_NumberOfSharedVertices;
	GLuint					m_NumberOfSharedNormals;
	GLuint					m_NumberOfSharedTexCoords;
	std::vector<GLuint>		m_SubObjectsNumberOfTriangles;

	GLfloat*				m_Positions;
	GLfloat*				m_Normals;
	GLfloat*				m_TexCoords;
	std::vector<GLuint*>	m_SubObjectsIndexes;

	ShaderProgram*			m_ShaderProgram;

	int								m_CurrentMaterial;
	std::vector<ObjectMaterial*>	m_Materials;
	
public :
	BasicRenderableObject() { } ;
	~BasicRenderableObject();

	void init();
	void initShader(ShaderProgram* shaderProgram);
	void loadMtl(char *filename);
	void loadObj(char *filename);
	void generateFullScreenQuad();
	void addSubObject(std::vector<GLuint>& faceIndexes);
	void setCurrentMaterial(const char* line);
	void fillInVBO();
	void createVertexArrayObject();
	void setTextureTarget(GLuint texture);

	virtual void draw(glm::mat4	viewMatrix, glm::mat4 projectionMatrix);
} ;

