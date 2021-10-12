#pragma once

#include <DirectXMath.h>
#include <string>

#include "HitBox.h"
#include "Surface2D.h"

// Базовый класс для всех объектов на сцене. 
// От этого класса наследуются остальные производные объекты.
class Object2D
{
public:
	// Конструктор объекта требует следующие поля:
	// name - имя объекта
	// position - позиция объекта
	Object2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite);
public:
	/* Главные методы для описания объекта */
	
	void			  Translate(DirectX::XMFLOAT2 delta);		//
	void			  SetName(std::string name);				// Установить имя объекта
	void			  SetPosition(DirectX::XMFLOAT2 position);	// Установить позицию объекта
	void			  SetSurface(Surface2D sprite);				// Установить плоскость изображения спрайта
	std::string		  GetName() const;							// Получить имя объекта
	DirectX::XMFLOAT2 GetPosition();							// Получить позицию объекта
	Surface2D&		  GetSurface();								// Получить плоскость изображения спрайта

	/***************************************/
protected:
	/* Переменные описания объекта */
	
	std::string		  name;		// Имя объекта
	DirectX::XMFLOAT2 position;	// Позиция объекта
	Surface2D		  image;	// Плоскость изображения спрайта

	/*******************************/
};
