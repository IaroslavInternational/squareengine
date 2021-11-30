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
	void Refresh();
public:
	bool Appeared();
	bool Disappeared();
	bool IsRunning();
private:
	std::string dataPath;
	bool		IsAppeared    = false;
	bool		IsDisappeared = false;
	bool		IsRun		  = false;
	struct
	{
		float k;
		float max;
		float min;
		float speed;
	} depth;
private:
	std::unique_ptr<Surface2D> fade;
	std::unique_ptr<Surface2D> dawn;
};

