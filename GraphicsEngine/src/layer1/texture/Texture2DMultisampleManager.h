/*
 * Texture2DMultisampleManager.h
 *
 *  Created on: 04.02.2013
 *      Author: nopper
 */

#ifndef TEXTURE2DMULTISAMPLEMANAGER_H_
#define TEXTURE2DMULTISAMPLEMANAGER_H_

#include "../../UsedLibs.h"

#include "Texture2DMultisample.h"

class Texture2DMultisampleManager
{
private:

	static Texture2DMultisampleManager* instance;

	std::map<std::string, Texture2DMultisampleSP> allTextures;

private:

	Texture2DMultisampleManager();
	virtual ~Texture2DMultisampleManager();

public:

	static Texture2DMultisampleManager* getInstance();

	static void terminate();

	void addTexture(const std::string& key, const Texture2DMultisampleSP& texture);

	Texture2DMultisampleSP createTexture(const std::string& key, boost::int32_t samples, GLint internalFormat, boost::int32_t width, boost::int32_t height, bool fixedsamplelocations = false);

};

#endif /* TEXTURE2DMULTISAMPLEMANAGER_H_ */
