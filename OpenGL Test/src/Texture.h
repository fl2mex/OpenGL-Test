#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_ID;
	std::string m_FilePath;
	unsigned char* m_Buffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};