/*
 * FrameBuffer2DMultisampleManager.cpp
 *
 *  Created on: 05.02.2013
 *      Author: nopper
 */

#include "FrameBuffer2DMultisampleManager.h"

using namespace std;

using namespace boost;

FrameBuffer2DMultisampleManager* FrameBuffer2DMultisampleManager::instance = nullptr;

FrameBuffer2DMultisampleManager::FrameBuffer2DMultisampleManager() :
	allFrameBuffers(), allWindowFrameBuffers()
{
}

FrameBuffer2DMultisampleManager::~FrameBuffer2DMultisampleManager()
{
	auto walker = allFrameBuffers.begin();
	while (walker != allFrameBuffers.end())
	{
		walker->second.reset();
		walker++;
	}
	allFrameBuffers.clear();
	allWindowFrameBuffers.clear();
}

FrameBuffer2DMultisampleManager* FrameBuffer2DMultisampleManager::getInstance()
{
	if (!instance)
	{
		instance = new FrameBuffer2DMultisampleManager();
	}

	return instance;
}

void FrameBuffer2DMultisampleManager::terminate()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

bool FrameBuffer2DMultisampleManager::containsFrameBuffer(const string& key) const
{
	return allFrameBuffers.find(key) != allFrameBuffers.end();
}

const FrameBuffer2DMultisampleSP& FrameBuffer2DMultisampleManager::getFrameBuffer(const string& key) const
{
	return allFrameBuffers.at(key);
}

void FrameBuffer2DMultisampleManager::addFrameBuffer(const string& key, const FrameBuffer2DMultisampleSP& framBuffer2DMultisample, bool windowFrameBuffer)
{
	allFrameBuffers[key] = framBuffer2DMultisample;

	if (windowFrameBuffer)
	{
		allWindowFrameBuffers[key] = framBuffer2DMultisample;
	}
}

FrameBuffer2DMultisampleSP FrameBuffer2DMultisampleManager::createFrameBuffer(const string& key, int32_t width, int32_t height, bool windowFrameBuffer)
{
	auto walker = allFrameBuffers.find(key);

	if (walker == allFrameBuffers.end())
	{
		allFrameBuffers[key] = FrameBuffer2DMultisampleSP(new FrameBuffer2DMultisample(width, height));

		if (windowFrameBuffer)
		{
			allWindowFrameBuffers[key] = allFrameBuffers[key];
		}

		return allFrameBuffers[key];
	}

	return allFrameBuffers[key];
}

void FrameBuffer2DMultisampleManager::updateWidthHeight(int32_t width, int32_t height)
{
	auto walker = allWindowFrameBuffers.begin();

	while (walker != allWindowFrameBuffers.end())
	{
		walker->second->setWidthHeight(width, height);

		walker++;
	}
}
