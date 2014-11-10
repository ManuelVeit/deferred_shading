#include "stdafx.h"
#include "BasicRenderableObject.h"
#include "ShaderProgram.h"
#include "Shader.h"



BasicRenderableObject::~BasicRenderableObject()
{
	glDeleteBuffers(1, &m_VBOPosition);
	glDeleteBuffers(1, &m_VBONormal);
	glDeleteBuffers(1, &m_VBOTexCoord);

	for(unsigned int i = 0; i < m_SubObjectsVBOTriangles.size(); ++i)
	{
		glDeleteBuffers(1, &(m_SubObjectsVBOTriangles[i]));
	}

	glDeleteVertexArrays(1, &m_Vao);
}


void BasicRenderableObject::initShader(ShaderProgram* shaderProgram)
{
	printGLErrors("BasicRenderableObject::initShader()");

	m_ShaderProgram = shaderProgram;
	m_ShaderProgram->initAttribute();
}

void BasicRenderableObject::init()
{
	glGenBuffers(1, &m_VBOPosition);
	glGenBuffers(1, &m_VBONormal);
	glGenBuffers(1, &m_VBOTexCoord);
}

void BasicRenderableObject::loadMtl(char* filename)
{
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "--- Loading " << filename << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;

	std::ifstream mtlStream(filename);

	if(!mtlStream.bad())
	{
		std::string token;

		while(!mtlStream.eof())
		{
			mtlStream >> token;

			if(token == "newmtl")
			{
				std::string materialName;

				mtlStream >> materialName;

				m_Materials.push_back( new ObjectMaterial() );
				m_SubObjectsIndexes.push_back(NULL);

				GLuint trianglesVBO;
				glGenBuffers(1, &trianglesVBO);
				m_SubObjectsVBOTriangles.push_back(trianglesVBO);
				m_SubObjectsNumberOfTriangles.push_back(0);

				m_Materials.back()->SetName(materialName);
			}
			else if(token == "Ns")
			{
				float shininess;

				mtlStream >> shininess;

				m_Materials.back()->SetShininess(shininess);
			}
			else if(token == "Ka")
			{
				glm::vec3 ambient;

				mtlStream >> ambient.r;
				mtlStream >> ambient.g;
				mtlStream >> ambient.b;

				m_Materials.back()->SetAmbient(ambient);
			}
			else if(token == "Kd")
			{
				glm::vec3 diffuse;

				mtlStream >> diffuse.r;
				mtlStream >> diffuse.g;
				mtlStream >> diffuse.b;

				m_Materials.back()->SetDiffuse(diffuse);
			}
			else if(token == "Ks")
			{
				glm::vec3 specular;

				mtlStream >> specular.r;
				mtlStream >> specular.g;
				mtlStream >> specular.b;

				m_Materials.back()->SetSpecular(specular);
			}
			else if(token == "Ni")
			{

			}
			else if(token == "d")
			{

			}
			else if(token == "illum")
			{

			}
			else if(token == "map_Ka")
			{
				std::string ambientMapFilename;

				mtlStream >> ambientMapFilename;

				m_Materials.back()->CreateAmbientTexture(ambientMapFilename.c_str());
			}
			else if(token == "map_Kd")
			{
				std::string diffuseMapFilename;

				mtlStream >> diffuseMapFilename;

				m_Materials.back()->CreateDiffuseTexture(diffuseMapFilename.c_str());
			}
			else if(token == "map_Bump")
			{
				std::string bumpMapFilename;

				mtlStream >> bumpMapFilename;

				m_Materials.back()->CreateBumpTexture(bumpMapFilename.c_str());
			}
		}
	}
}

void BasicRenderableObject::loadObj(char *filename)
{
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "--- Loading " << filename << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;

	std::ifstream objStream(filename);

	if(!objStream.bad())
	{
		std::vector<glm::vec3> vertexList;
		std::vector<glm::vec3> normalList;
		std::vector<glm::vec2> texCoordList;

		std::vector<glm::vec3> duplicatedVertices;
		std::vector<glm::vec3> duplicatedNormals;
		std::vector<glm::vec2> duplicatedTexCoords;

		std::vector<GLuint> faceTrianglesIndexes;

		while(!objStream.eof())
		{
			char line[256];
			memset(line, '\0', 256 * sizeof(char));

			objStream.getline(line, 256);
			
			if(line[0] == 'v')
			{
				if(line[1] == 'n')
				{
					glm::vec3 normal;

					sscanf_s(line, "vn %f %f %f", &(normal.x), &(normal.y), &(normal.z));

					normalList.push_back(normal);
				}
				else if(line[1] == 't')
				{
					glm::vec2 texCoord;

					sscanf_s(line, "vt %f %f", &(texCoord.s), &(texCoord.t));

					texCoordList.push_back(texCoord);
				}
				else if(line[1] == ' ')
				{
					glm::vec3 vertex;

					sscanf_s(line, "v %f %f %f", &(vertex.x), &(vertex.y), &(vertex.z));

					vertexList.push_back(vertex);
				}
			}
			else if(line[0] == 'f')
			{
				int faceVertices[3] = {-1, -1, -1};
				int faceNormals[3] = {-1, -1, -1};
				int faceTextures[3] = {-1, -1, -1};

				if(normalList.empty() && texCoordList.empty())
				{
					sscanf_s(line, "f %i %i %i", &(faceVertices[0]), &(faceVertices[1]), &(faceVertices[2]));
				}
				else if(!normalList.empty() && texCoordList.empty())
				{
					sscanf_s(line, "f %i//%i %i//%i %i//%i", &(faceVertices[0]), &(faceNormals[0]), &(faceVertices[1]), &(faceNormals[1]), &(faceVertices[2]), &(faceNormals[2]));
				}
				else if(normalList.empty() && !texCoordList.empty())
				{
					sscanf_s(line, "f %i/%i/ %i/%i/ %i/%i/", &(faceVertices[0]), &(faceTextures[0]), &(faceVertices[1]), &(faceTextures[1]), &(faceVertices[2]), &(faceTextures[2]));
				}
				else if(!normalList.empty() && !texCoordList.empty())
				{
					sscanf_s(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &(faceVertices[0]), &(faceTextures[0]), &(faceNormals[0]), &(faceVertices[1]), &(faceTextures[1]), &(faceNormals[1]), &(faceVertices[2]), &(faceTextures[2]), &(faceNormals[2]));
				}

				for(unsigned int i = 0; i < 3; ++i)
				{
					faceTrianglesIndexes.push_back( duplicatedVertices.size() );

					if(faceVertices[i] != -1)
					{
						duplicatedVertices.push_back( vertexList[ faceVertices[i] - 1 ] );
					}

					if(faceNormals[i] != -1)
					{
						duplicatedNormals.push_back( normalList[ faceNormals[i] - 1 ] );
					}

					if(faceTextures[i] != -1)
					{
						duplicatedTexCoords.push_back( texCoordList[ faceTextures[i] - 1 ] );
					}
				}
			}
			else if(line[0] == 'u')
			{
				addSubObject(faceTrianglesIndexes);
				setCurrentMaterial(line);
			}
		}

		addSubObject(faceTrianglesIndexes);

		std::cout << "Number of vertices: " << vertexList.size() << std::endl;
		std::cout << "Number of normals: " << normalList.size() << std::endl;
		std::cout << "Number of texture coordinates: " << texCoordList.size() << std::endl;

		m_NumberOfSharedVertices = duplicatedVertices.size();
		m_Positions = new GLfloat[3 * m_NumberOfSharedVertices];

		for(unsigned int i = 0; i < m_NumberOfSharedVertices; ++i)
		{
			m_Positions[3*i + 0] = duplicatedVertices[i].x;
			m_Positions[3*i + 1] = duplicatedVertices[i].y;
			m_Positions[3*i + 2] = duplicatedVertices[i].z;
		}

		m_NumberOfSharedNormals = duplicatedNormals.size();
		m_Normals = new GLfloat[3 * m_NumberOfSharedNormals];
	
		for(unsigned int i = 0; i < m_NumberOfSharedNormals; ++i)
		{
			m_Normals[3*i + 0] = duplicatedNormals[i].x;
			m_Normals[3*i + 1] = duplicatedNormals[i].y;
			m_Normals[3*i + 2] = duplicatedNormals[i].z;
		}

		m_NumberOfSharedTexCoords = duplicatedTexCoords.size();
		m_TexCoords = new GLfloat[2 * m_NumberOfSharedTexCoords];
	
		for(unsigned int i = 0; i < m_NumberOfSharedTexCoords; ++i)
		{
			m_TexCoords[2*i + 0] = duplicatedTexCoords[i].s;
			m_TexCoords[2*i + 1] = duplicatedTexCoords[i].t;
		}

		objStream.close();
	}
}

void BasicRenderableObject::addSubObject(std::vector<GLuint>& faceIndexes)
{
	if(!faceIndexes.empty())
	{
		if(m_CurrentMaterial >= 0)
		{
			std::cout << "New subobject with material " << m_Materials[m_CurrentMaterial]->GetName() << std::endl;

			// suppose only triangular faces
			unsigned int numberOfTriangles = faceIndexes.size() / 3;
			GLuint* subObjectIndexes = new GLuint[3 * numberOfTriangles];

			for(unsigned int i = 0; i < numberOfTriangles; i++)
			{
				for(unsigned int j = 0; j < 3; j++)
				{
					subObjectIndexes[3*i + j] = faceIndexes[3*i + j];
				}
			}

			m_SubObjectsNumberOfTriangles[m_CurrentMaterial] = numberOfTriangles;
			m_SubObjectsIndexes[m_CurrentMaterial] = subObjectIndexes;
		}
	}

	faceIndexes.clear();
}

void BasicRenderableObject::setCurrentMaterial(const char* line)
{
	std::ostringstream oss;
	oss << line;

	std::string materialLine = oss.str();
	unsigned int separatorPos = materialLine.find_first_of(" ");
	std::string materialName = materialLine.substr(separatorPos + 1, materialLine.size() - separatorPos);

	m_CurrentMaterial = -1;

	for(unsigned int i = 0; m_CurrentMaterial == -1 && i < m_Materials.size(); ++i)
	{
		if(m_Materials[i]->GetName() == materialName)
		{
			m_CurrentMaterial = i;
		}
	}
}

void BasicRenderableObject::generateFullScreenQuad()
{
	m_NumberOfSharedVertices = 4;
	m_NumberOfSharedNormals = 4;
	m_NumberOfSharedTexCoords = 4;
	m_SubObjectsNumberOfTriangles.push_back(2);

	m_Positions = new GLfloat[m_NumberOfSharedVertices * 3];

	m_Positions[0] = -1.0f; m_Positions[1] = -1.0f; m_Positions[2] = 0.0f;
	m_Positions[3] = -1.0f; m_Positions[4] = 1.0f; m_Positions[5] = 0.0f;
	m_Positions[6] = 1.0f; m_Positions[7] = 1.0f; m_Positions[8] = 0.0f;
	m_Positions[9] = 1.0f; m_Positions[10] = -1.0f; m_Positions[11] = 0.0f;

	m_Normals = new GLfloat[m_NumberOfSharedNormals * 3];

	m_Normals[0] = 0.0f; m_Normals[1] = 0.0f; m_Normals[2] = 1.0f;
	m_Normals[3] = 0.0f; m_Normals[4] = 0.0f; m_Normals[5] = 1.0f;
	m_Normals[6] = 0.0f; m_Normals[7] = 0.0f; m_Normals[8] = 1.0f;
	m_Normals[9] = 0.0f; m_Normals[10] = 0.0f; m_Normals[11] = 1.0f;

	m_TexCoords = new GLfloat[m_NumberOfSharedTexCoords * 2];

	m_TexCoords[0] = 0.0f; m_TexCoords[1] = 0.0f;
	m_TexCoords[2] = 0.0f; m_TexCoords[3] = 1.0f;
	m_TexCoords[4] = 1.0f; m_TexCoords[5] = 1.0f;
	m_TexCoords[6] = 1.0f; m_TexCoords[7] = 0.0f;

	GLuint* triangles = new GLuint[6];

	triangles[0] = 0; triangles[1] = 1; triangles[2] = 2; 
	triangles[3] = 0; triangles[4] = 2; triangles[5] = 3;

	m_SubObjectsIndexes.push_back(triangles);

	GLuint trianglesVBO;
	glGenBuffers(1, &trianglesVBO);
	m_SubObjectsVBOTriangles.push_back(trianglesVBO);

	m_CurrentMaterial = 0;
	m_Materials.push_back(new ObjectMaterial());
}

void BasicRenderableObject::fillInVBO()
{
	printGLErrors("BasicRenderableObject::fillInVBO() start");

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPosition);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_NumberOfSharedVertices * sizeof(GLfloat), m_Positions, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_NumberOfSharedNormals * sizeof(GLfloat), m_Normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoord);
	glBufferData(GL_ARRAY_BUFFER, 2 * m_NumberOfSharedTexCoords * sizeof(GLfloat), m_TexCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(unsigned int i = 0; i < m_SubObjectsVBOTriangles.size(); ++i)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SubObjectsVBOTriangles[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * m_SubObjectsNumberOfTriangles[i] * sizeof(GLuint), m_SubObjectsIndexes[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	printGLErrors("BasicRenderableObject::fillInVBO() end");
}

void BasicRenderableObject::createVertexArrayObject()
{
	printGLErrors("BasicRenderableObject::createVertexArrayObject() start");

	glGenVertexArrays(1, &m_Vao);

	glBindVertexArray(m_Vao);

	glEnableVertexAttribArray(m_ShaderProgram->VertexAttrib());
	glEnableVertexAttribArray(m_ShaderProgram->NormalAttrib());
	glEnableVertexAttribArray(m_ShaderProgram->TexCoordAttrib());

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPosition);
	glVertexAttribPointer(m_ShaderProgram->VertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
	glVertexAttribPointer(m_ShaderProgram->NormalAttrib(), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexCoord);
	glVertexAttribPointer(m_ShaderProgram->TexCoordAttrib(), 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	printGLErrors("BasicRenderableObject::createVertexArrayObject() end");
}

void BasicRenderableObject::setTextureTarget(GLuint texture)
{
	if(!m_Materials.empty())
	{
		m_Materials.front()->CreateAmbientTexture(texture);
	}
}

void BasicRenderableObject::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	m_ShaderProgram->startUseProgram();

	GLint indexOfProjectionMatrix = glGetUniformLocation(m_ShaderProgram->programId(), "projectionMatrix");
	GLint indexOfViewMatrix = glGetUniformLocation(m_ShaderProgram->programId(), "viewMatrix");

	glUniformMatrix4fv(indexOfProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(indexOfViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glBindVertexArray(m_Vao);

	for(unsigned int i = 0; i < m_SubObjectsVBOTriangles.size(); ++i)
	{
		GLint indexOfAmbientColor = glGetUniformLocation(m_ShaderProgram->programId(), "ambientColor");
		GLint indexOfDiffuseColor = glGetUniformLocation(m_ShaderProgram->programId(), "diffuseColor");
		GLint indexOfSpecularColor = glGetUniformLocation(m_ShaderProgram->programId(), "specularColor");
		GLint indexOfShininess = glGetUniformLocation(m_ShaderProgram->programId(), "shininess");
		GLint indexOfAmbientMap = glGetUniformLocation(m_ShaderProgram->programId(), "ambientMap");
		GLint indexOfDiffuseMap = glGetUniformLocation(m_ShaderProgram->programId(), "diffuseMap");
		GLint indexOfBumpMap = glGetUniformLocation(m_ShaderProgram->programId(), "bumpMap");

		if(indexOfAmbientColor >= 0)
		{
			glUniform3fv(indexOfAmbientColor, 1, glm::value_ptr(m_Materials[i]->GetAmbient()));
		}

		if(indexOfDiffuseColor >= 0)
		{
			glUniform3fv(indexOfDiffuseColor, 1, glm::value_ptr(m_Materials[i]->GetDiffuse()));
		}

		if(indexOfSpecularColor >= 0)
		{
			glUniform3fv(indexOfSpecularColor, 1, glm::value_ptr(m_Materials[i]->GetSpecular()));
		}

		if(indexOfShininess >= 0)
		{
			glUniform1f(indexOfShininess, m_Materials[i]->GetShininess());
		}

		unsigned int mapId = 0;

		if(indexOfAmbientMap >= 0 && m_Materials[i]->GetAmbientTextureId() > 0)
		{
			glUniform1i(indexOfAmbientMap, mapId);

			glActiveTexture(GL_TEXTURE0 + mapId);
			glBindTexture(GL_TEXTURE_2D, m_Materials[i]->GetAmbientTextureId());
			
			mapId++;
		}

		if(indexOfDiffuseMap >= 0 && m_Materials[i]->GetDiffuseTextureId() > 0)
		{
			glUniform1i(indexOfDiffuseMap, mapId);

			glActiveTexture(GL_TEXTURE0 + mapId);
			glBindTexture(GL_TEXTURE_2D, m_Materials[i]->GetDiffuseTextureId());
			
			mapId++;
		}

		if(indexOfBumpMap >= 0 && m_Materials[i]->GetBumpTextureId() > 0)
		{
			glUniform1i(indexOfBumpMap, mapId);

			glActiveTexture(GL_TEXTURE0 + mapId);
			glBindTexture(GL_TEXTURE_2D, m_Materials[i]->GetBumpTextureId());
			
			mapId++;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SubObjectsVBOTriangles[i]);
		glDrawElements(GL_TRIANGLES, 3 * m_SubObjectsNumberOfTriangles[i], GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_ShaderProgram->stopUseProgram();
}
