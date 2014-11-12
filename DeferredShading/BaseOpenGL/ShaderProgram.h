#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H



#include "stdafx.h"
#include "Shader.h"



class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

	GLuint programId() const					{ return _programId; }

	Shader* vertShader() const					{ return _vertShader; }
	Shader* geomShader() const					{ return _geomShader; }
	Shader* fragShader() const					{ return _fragShader; }

    bool attachGeometryShader(Shader* shader);
    bool attachVertexShader(Shader* shader);
    bool attachFragmentShader(Shader* shader);

	bool linkProgram();
	
    bool detachFragmentShader();
    bool detachVertexShader();
    bool detachGeometryShader();

	void startUseProgram()						{ glUseProgram(_programId); }
	void stopUseProgram()						{ glUseProgram(0);	}

	static ShaderProgram* createGBufferBuilderShader();
	static ShaderProgram* createDirectionnalLightShader();

	void initAttribute();

	GLuint VertexAttrib();
	GLuint NormalAttrib();
	GLuint TexCoordAttrib();

protected:
	GLuint _programId;

	Shader* _vertShader;
	Shader* _geomShader;
	Shader* _fragShader;

public:
	GLuint m_VertexAttribute;
	GLuint m_NormalAttribute;
	GLuint m_TexCoordAttribute;
};

#endif
