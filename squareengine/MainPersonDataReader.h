#pragma once

#include <DirectXMath.h>

#include "EngineFunctions.hpp"

class MainPersonDataReader
{
public:
	MainPersonDataReader(std::string dataPath);
public:
	std::string		  dataPath;
	std::string		  name;
	DirectX::XMFLOAT2 position;
	std::string		  pathToSprite;
	DirectX::XMFLOAT4 hb_coord;
	float			  eff_d;
	float			  eff_t;
	float			  eff_a;
	float			  speed;
	size_t			  layer;
	size_t			  camMode;
};