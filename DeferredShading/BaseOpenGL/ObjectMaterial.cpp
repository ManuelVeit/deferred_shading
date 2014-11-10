#include "StdAfx.h"
#include "ObjectMaterial.h"



ObjectMaterial::ObjectMaterial(void)
{
	m_AmbientTextureId = m_DiffuseTextureId = m_BumpTextureId = 0;

	m_Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Specular = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Shininess = 0.0f;
}


ObjectMaterial::~ObjectMaterial(void)
{
}

void ObjectMaterial::CreateAmbientTexture(const char* filename)
{
	m_AmbientTextureId = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS
	);
}

void ObjectMaterial::CreateAmbientTexture(GLuint renderedTexture)
{
	m_AmbientTextureId = renderedTexture;
}

void ObjectMaterial::CreateDiffuseTexture(const char* filename)
{
	m_DiffuseTextureId = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS
	);
}

void ObjectMaterial::CreateBumpTexture(const char* filename)
{
	m_BumpTextureId = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS
	);

	glBindTexture(GL_TEXTURE_2D, 0);
}

