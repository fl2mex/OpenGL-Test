#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_vertexPath;
	std::string m_fragmentPath;
	unsigned int m_ID;

	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);

public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformVec3f(const std::string& name, const glm::vec3 value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
};