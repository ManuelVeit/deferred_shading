#include "stdafx.h"
#include "Shader.h"



Shader::Shader(GLenum type)
{
	_shaderId = glCreateShader(type);
	std::cout << "Create shader id : " << _shaderId << std::endl;
}


Shader::~Shader()
{
	glDeleteShader(_shaderId);
}



bool Shader::compileShader(std::string filename)
{
	printGLErrors("compileShader::start");

	//Test si l'id du shader est valide
	if(_shaderId == 0)
	{
		std::cerr << "Shader id is not valid !" << std::endl;
		return false;
	}
	
	//Charge le fichier source
	std::string src = readFileSrc(filename);

	if(src.length() == 0)
	{
		std::cerr << "Unable to read shader source file" << filename << std::endl;
		return false;
	}

	printGLErrors("compileShader::fileread");

	std::cout << src << std::endl;

	const char *shaderSource = src.c_str();
	
	glShaderSource(_shaderId, 1, &shaderSource, NULL);

	printGLErrors("compileShader::Unable to attach source");

	//Compilation du shader
	glCompileShader(_shaderId);
	printInfoLogShader("Shader " + filename + " compilation result: ");
	
	return true;
}


/**
 * Ouvre un fichier source shader et le lit
 * @param filename fichier source
 * @return le code source si ok sinon une chaine vide
 */
std::string Shader::readFileSrc(std::string filename)
{
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if ( filename != std::string("") )
	{
		fopen_s(&fp, filename.c_str(), "rt" );

		if ( fp != NULL )
		{

			fseek( fp, 0, SEEK_END );
			count = ftell( fp );
			rewind( fp );

			if ( count > 0 )
			{
				content = new char[count+1];
				count = fread( content, sizeof( char ), count, fp );
				content[count] = '\0';
			}

			fclose( fp );
		}
		else
		{
			std::cout << "Shader : cannot open " <<  filename << std::endl;
		}
	}

	return std::string(content);
}


/**
* Affiche les erreurs liées aux shaders
*
* @param shader
* @return false en cas d'erreur
*/
bool Shader::printInfoLogShader(std::string msg)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 1)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(_shaderId, infologLength, &charsWritten, infoLog);

		std::cerr << msg <<  " : " << infoLog << std::endl;
		free(infoLog);
	}

	return (infologLength==1);
}



bool printGLErrors(const char* msg)
{
	GLenum errCode;
	
	if((errCode = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "-----------------------------------------" << std::endl;
		std::cerr << "OpenGL Error [" << msg << "]" << std::endl;
		std::cerr << "-----------------------------------------" << std::endl;
		
		return false;
	}
	
	return true;
}
