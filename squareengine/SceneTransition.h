#pragma once

#include <memory>

#include "Surface2D.h"

class Graphics;

class SceneTransition
{
public:
	SceneTransition(std::string dataPath);
public:
	void Draw(Graphics& gfx);
	void Appear(float dt);
	void Disappear(float dt);
public:
	bool Appeared();
	bool Disappeared();
	bool IsRunning();
private:
	std::string dataPath;
	bool		IsAppeared    = false;
	bool		IsDisappeared = false;
	float		depth		  = 1.0f;
	bool		IsRun		  = false;
private:
	std::unique_ptr<Surface2D> fade;
	std::unique_ptr<Surface2D> dawn;
};

