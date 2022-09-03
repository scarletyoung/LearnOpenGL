#include "shader.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <array>

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
	{
		return GL_VERTEX_SHADER;
	} else if (type == "fragment" || type == "pixel")
	{
		return GL_FRAGMENT_SHADER;
	}
	throw "Unkonwn shader type";
}

Shader::Shader(const std::string& filePath)
{
	auto source = ReadFile(filePath);
	auto shaderSource = Preprocess(source);
	Compile(shaderSource);
}

Shader::~Shader()
{
	glDeleteProgram(mRenderId);
}

void Shader::Bind() const
{
	glUseProgram(mRenderId);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	Bind();
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetInt(const std::string& name, int value) const
{
	Bind();
	glUniform1i(GetUniformLocation(name), value);
}

int Shader::GetUniformLocation(const std::string & name) const
{
	if (mLocationCache.find(name) != mLocationCache.end())
	{
		return mLocationCache[name];
	}
	int location = glGetUniformLocation(mRenderId, name.c_str());
	if (location == -1)
	{
		std::cout << "glunifrom name " << name << " not found" << std::endl;
		return location;
	}
	mLocationCache[name] = location;
	return location;
}

std::string Shader::ReadFile(const std::string& filepath)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);

	if (in)
	{
		auto iter = std::istreambuf_iterator<char>(in);
		std::string source(iter, std::istreambuf_iterator<char>());
		if (source.empty())
		{
			std::cout << "Could not read from file " << filepath << std::endl;
		}
		return source;
	} else
	{
		std::cout << "Could not open file " << filepath << std::endl;
	}
	return std::string();
}

const std::unordered_map<uint32_t, std::string> Shader::Preprocess(const std::string& source)
{
	std::unordered_map<uint32_t, std::string> shaderSources;
	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		if (eol == std::string::npos)
		{
			throw "Syntax error";
		}
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		if (!ShaderTypeFromString(type))
		{
			throw "Invalid shader type specified";
		}

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}
	return shaderSources;
}

void Shader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSource)
{
	GLuint program = glCreateProgram();
	if (shaderSource.size() > 2)
	{
		throw "Don't support more than two shaders!";
	}
	std::array<uint32_t, 2> shaderId;
	int index = 0;
	for (auto src : shaderSource)
	{
		GLuint shader = glCreateShader(src.first);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = src.second.c_str();
		glShaderSource(shader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);
			for (auto id : shaderId)
			{
				glDeleteShader(id);
			}

			std::cout << infoLog.data() << std::endl;
			return;
		}
		shaderId[index++] = shader;
	}

	mRenderId = program;
	for (auto id : shaderId)
	{
		glAttachShader(mRenderId, id);
	}

	// Link our program
	glLinkProgram(mRenderId);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(mRenderId, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mRenderId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mRenderId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(mRenderId);
		// Don't leak shaders either.
		for (auto id : shaderId)
		{
			glDeleteShader(id);
		}
		std::cout << infoLog.data() << std::endl;
		return;
	}
	for (auto id : shaderId)
	{
		glDetachShader(mRenderId, id);
	}
}
