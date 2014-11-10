#pragma once

#include "StdAfx.h"



class ObjectMaterial
{
public:
	ObjectMaterial();
	~ObjectMaterial();

	void CreateAmbientTexture(const char* filename);
	void CreateAmbientTexture(GLuint renderedTexture);
	void CreateDiffuseTexture(const char* filename);
	void CreateBumpTexture(const char* filename);

	inline GLuint		GetAmbientTextureId()	const { return m_AmbientTextureId; }
	inline GLuint		GetDiffuseTextureId()	const { return m_DiffuseTextureId; }
	inline GLuint		GetBumpTextureId()		const { return m_BumpTextureId; }
	inline std::string	GetName()				const { return m_Name; }
	inline glm::vec3	GetAmbient()			const { return m_Ambient; }
	inline glm::vec3	GetDiffuse()			const { return m_Diffuse; }
	inline glm::vec3	GetSpecular()			const { return m_Specular; }
	inline float		GetShininess()			const { return m_Shininess; }

	inline void SetName(std::string name)		{ m_Name = name; }
	inline void SetAmbient(glm::vec3 ambient)	{ m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse)	{ m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular)	{ m_Specular = specular; }
	inline void SetShininess(float shininess)	{ m_Shininess = shininess; }

protected:
	std::string m_Name;

	GLuint		m_AmbientTextureId;
	GLuint		m_DiffuseTextureId;
	GLuint		m_BumpTextureId;

	glm::vec3	m_Ambient;
	glm::vec3	m_Diffuse;
	glm::vec3	m_Specular;
	float		m_Shininess;
};

