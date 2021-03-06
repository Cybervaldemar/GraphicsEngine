/*
 * ViewportManager.cpp
 *
 *  Created on: 23.04.2011
 *      Author: Norbert Nopper
 */

#include "ViewportManager.h"

using namespace std;

ViewportManager* ViewportManager::instance;

const string ViewportManager::DEFAULT_VIEWPORT_KEY = "default";

ViewportManager::ViewportManager() :
	allViewports()
{
	allViewports[DEFAULT_VIEWPORT_KEY] = ViewportSP(new Viewport(0, 0, 1, 1));
}

ViewportManager::~ViewportManager()
{
	map<string, ViewportSP>::iterator walker = allViewports.begin();
	while (walker != allViewports.end())
	{
		walker->second.reset();
		walker++;
	}
	allViewports.clear();
}

ViewportManager* ViewportManager::getInstance()
{
	if (!instance)
	{
		instance = new ViewportManager();
	}

	return instance;
}

void ViewportManager::terminate()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

const ViewportSP& ViewportManager::getDefaultViewport() const
{
	return allViewports.at(DEFAULT_VIEWPORT_KEY);
}

bool ViewportManager::containsViewport(const string& key) const
{
	return allViewports.find(key) != allViewports.end();
}

const ViewportSP& ViewportManager::getViewport(const string& key) const
{
	return allViewports.at(key);
}

void ViewportManager::setViewport(const string& key, const ViewportSP& viewport)
{
	BOOST_ASSERT(DEFAULT_VIEWPORT_KEY.compare(key) != 0);

	allViewports[key] = viewport;
}
