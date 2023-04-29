#pragma once
#include <GL/glew.h>

#include <string>

#include "stb_image/stb_image.h"

class Texture
{
public:
	Texture(const std::string& path)
		: m_ID(0), m_FilePath(path), m_Buffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
	{
		m_Buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, STBI_rgb_alpha);

		GLenum format;
		switch (m_BPP)
		{
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = GL_RGBA;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_Buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (m_Buffer)
			stbi_image_free(m_Buffer);
	}
	~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}
	void Bind(unsigned int slot = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	unsigned int m_ID;
	std::string m_FilePath;
	unsigned char* m_Buffer;
	int m_Width, m_Height, m_BPP;
};