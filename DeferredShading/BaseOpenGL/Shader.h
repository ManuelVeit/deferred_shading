#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>



class Shader
{
public:
	Shader(GLenum type);
	~Shader();

	bool compileShader(std::string filename);

	GLuint shaderId() const			{ return _shaderId; }

protected:
	GLuint _shaderId;

private:
	std::string readFileSrc(std::string filename);
	bool printInfoLogShader(std::string msg);
};

bool printGLErrors(const char* msg);

#endif
