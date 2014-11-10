#include "stdafx.h"

#include "Shader.h"
#include "ShaderProgram.h"




ShaderProgram::ShaderProgram()
{
	_programId = glCreateProgram();

	_vertShader = NULL;
	_geomShader = NULL;
	_fragShader = NULL;
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_programId);
}




/*!
    \fn ShaderProgram::attachGeometryShader(Shader* shader)
 */
bool ShaderProgram::attachGeometryShader(Shader* shader)
{
	_geomShader = shader;
	glAttachShader(_programId, shader->shaderId());

	return printGLErrors("attachment error for geometry shader");
}


/*!
    \fn ShaderProgram::attachVertexShader(Shader* shader)
 */
bool ShaderProgram::attachVertexShader(Shader* shader)
{
    _vertShader = shader;
	glAttachShader(_programId, shader->shaderId());

	return printGLErrors("attachment error for vertex shader");
}


/*!
    \fn ShaderProgram::attachFragmentShader(Shader* shader)
 */
bool ShaderProgram::attachFragmentShader(Shader* shader)
{
    _fragShader = shader;
	glAttachShader(_programId, shader->shaderId());

	return printGLErrors("attachment error for fragment shader");
}

/*!
    \fn ShaderProgram::linkProgram()
 */
bool ShaderProgram::linkProgram()
{
	glLinkProgram(_programId);

	//Print log if needed
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(_programId, infologLength, &charsWritten, infoLog);

		std::cerr << "Link message :" << infoLog;
		free(infoLog);
	}

	return (infologLength == 1);
}


/*!
    \fn ShaderProgram::detachFragmentShader()
 */
bool ShaderProgram::detachFragmentShader()
{
	glDetachShader(_programId, _fragShader->shaderId());

	return printGLErrors("Unable to detach fragment shader");
}


/*!
    \fn ShaderProgram::detachVertexShader()
 */
bool ShaderProgram::detachVertexShader()
{
    glDetachShader(_programId, _vertShader->shaderId());

	return printGLErrors("Unable to detach vertex shader");
}


/*!
    \fn ShaderProgram::detachGeometryShader()
 */
bool ShaderProgram::detachGeometryShader()
{
	glDetachShader(_programId, _geomShader->shaderId());

	return printGLErrors("Unable to detach geometry shader");
}


/*!
    \fn ShaderProgram::createStdShader()
 */
ShaderProgram* ShaderProgram::createStdShader()
{
	printGLErrors("createStdShader::start");

	//Création du vertex et du fragment shader
	Shader* vertexShader = new ::Shader(GL_VERTEX_SHADER);
	if(!vertexShader->compileShader("..\\BaseOpenGL\\simple.vert")) {
		delete vertexShader;

		return NULL;
	}

	printGLErrors("createStdShader::after_vertex_shader_compilation");

	Shader* fragmentShader = new ::Shader(GL_FRAGMENT_SHADER);
	if(!fragmentShader->compileShader("..\\BaseOpenGL\\simple.frag")) {
		delete vertexShader;
		delete fragmentShader;

		return NULL;
	}

	printGLErrors("createStdShader::after_fragment_shader_compilation");

	//Création du programme shader
	ShaderProgram* pgm = new ::ShaderProgram();

	//Attachement des shaders au programme
	pgm->attachVertexShader(vertexShader);
	pgm->attachFragmentShader(fragmentShader);
	pgm->linkProgram();

	return pgm;
}

ShaderProgram* ShaderProgram::createAmbientMapShader()
{
	printGLErrors("createStdDiffuseShader::start");

	//Création du vertex et du fragment shader
	Shader* vertexShader = new ::Shader(GL_VERTEX_SHADER);
	if(!vertexShader->compileShader("..\\BaseOpenGL\\ambientMap.vert")) {
		delete vertexShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_vertex_shader_compilation");

	Shader* fragmentShader = new ::Shader(GL_FRAGMENT_SHADER);
	if(!fragmentShader->compileShader("..\\BaseOpenGL\\ambientMap.frag")) {
		delete vertexShader;
		delete fragmentShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_fragment_shader_compilation");

	//Création du programme shader
	ShaderProgram* pgm = new ::ShaderProgram();

	//Attachement des shaders au programme
	pgm->attachVertexShader(vertexShader);
	pgm->attachFragmentShader(fragmentShader);
	pgm->linkProgram();

	return pgm;
}

ShaderProgram* ShaderProgram::createPostProcessShader()
{
	printGLErrors("createStdDiffuseShader::start");

	//Création du vertex et du fragment shader
	Shader* vertexShader = new ::Shader(GL_VERTEX_SHADER);
	if(!vertexShader->compileShader("..\\BaseOpenGL\\postProcess.vert")) {
		delete vertexShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_vertex_shader_compilation");

	Shader* fragmentShader = new ::Shader(GL_FRAGMENT_SHADER);
	if(!fragmentShader->compileShader("..\\BaseOpenGL\\postProcess.frag")) {
		delete vertexShader;
		delete fragmentShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_fragment_shader_compilation");

	//Création du programme shader
	ShaderProgram* pgm = new ::ShaderProgram();

	//Attachement des shaders au programme
	pgm->attachVertexShader(vertexShader);
	pgm->attachFragmentShader(fragmentShader);
	pgm->linkProgram();

	return pgm;
}

ShaderProgram* ShaderProgram::createDeferredShadingShader()
{
	printGLErrors("createStdDiffuseShader::start");

	//Création du vertex et du fragment shader
	Shader* vertexShader = new ::Shader(GL_VERTEX_SHADER);
	if(!vertexShader->compileShader("..\\BaseOpenGL\\deferredShading.vert")) {
		delete vertexShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_vertex_shader_compilation");

	Shader* fragmentShader = new ::Shader(GL_FRAGMENT_SHADER);
	if(!fragmentShader->compileShader("..\\BaseOpenGL\\deferredShading.frag")) {
		delete vertexShader;
		delete fragmentShader;

		return NULL;
	}

	printGLErrors("createStdDiffuseShader::after_fragment_shader_compilation");

	//Création du programme shader
	ShaderProgram* pgm = new ::ShaderProgram();

	//Attachement des shaders au programme
	pgm->attachVertexShader(vertexShader);
	pgm->attachFragmentShader(fragmentShader);
	pgm->linkProgram();

	return pgm;
}

void ShaderProgram::initAttribute()
{
	startUseProgram();

	m_VertexAttribute = glGetAttribLocation(programId(), "vertex_in");
	m_NormalAttribute = glGetAttribLocation(programId(), "normal_in");
	m_TexCoordAttribute = glGetAttribLocation(programId(), "uv_in");

	stopUseProgram();
}

GLuint ShaderProgram::VertexAttrib()
{
	return m_VertexAttribute;
}

GLuint ShaderProgram::NormalAttrib()
{
	return m_NormalAttribute;
}

GLuint ShaderProgram::TexCoordAttrib()
{
	return m_TexCoordAttribute;
}
