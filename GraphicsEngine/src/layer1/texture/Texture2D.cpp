/*
 * Texture2D.cpp
 *
 *  Created on: 22.11.2012
 *      Author: nopper
 */

#include "Texture2D.h"

using namespace boost;

Texture2D::Texture2D(GLint internalFormat, int32_t width, int32_t height, GLenum format, GLenum type, const uint8_t* pixels, uint32_t sizeOfData, bool mipMap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, float anisotropic) :
	TextureStandard(GL_TEXTURE_2D, internalFormat, width, height, format, type, sizeOfData, mipMap, minFilter, magFilter, wrapS, wrapT, anisotropic), pixelData(width, width, format, type, pixels, sizeOfData)
{
	init();
}

Texture2D::~Texture2D()
{
	freePixels();
}

bool Texture2D::init()
{
	if (width < 1 || height < 1)
	{
		return false;
	}

	if (!textureName)
	{
		glGenTextures(1, &textureName);

		if (!textureName)
		{
			return false;
		}
	}

	glBindTexture(target, textureName);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, pixelData.getPixels());

	if (mipMap)
	{
		glGenerateMipmap(target);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);

	return true;
}

void Texture2D::freePixels()
{
	pixelData.freePixels();
}

