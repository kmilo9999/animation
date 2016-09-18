#include <iostream>
#include "Texture.h"
#include "Image.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}


bool Texture::Load()
{

	Image image;
	std::size_t found = m_fileName.find("dds");
	if (found != std::string::npos)
	{
		replace(m_fileName, "dds", "png");
	}
	
	found = m_fileName.find("C:/Users/Nathan/Documents/New Project/Assets/Textures/soldier/");
	if (found != std::string::npos)
	{
		replace(m_fileName, "C:/Users/Nathan/Documents/New Project/Assets/Textures/soldier/", "Tex/");
	}

	if (!image.LoadFromFile(m_fileName.c_str()))
	{
		cerr << "Image could not be load " << endl;
		return 0;
	}
	GLenum glErr;
	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		char *file = "Texture.cpp";
		printf("glError in file %s: %s\n",
			file, gluErrorString(glErr));
		return false;;
	}
	int Mode = GL_RGB;
	if (image.GetFormat() == 4)
	{
		Mode = GL_RGBA;
	}

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, Mode, image.GetHeight(), image.GetWidth(), 0, Mode, GL_UNSIGNED_BYTE, image.GetPixels());
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glBindTexture(m_textureTarget, 0);

	
	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		char *file = "Texture.cpp";
		printf("glError in file %s: %s\n",
			file, gluErrorString(glErr));
		return false;;
	}
    
    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}

bool Texture::replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}