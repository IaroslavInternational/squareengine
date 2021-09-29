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
	// hitbox - хитбокс объекта
	Object2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite, HitBox hitbox);
public:
	/* Главные методы для описания объекта */
	
	void			  SetName(std::string name);				// Установить имя объекта
	void			  SetPosition(DirectX::XMFLOAT2 position);	// Установить позицию объекта
	void			  SetSurface(Surface2D sprite);			// Установить плоскость изображения спрайта
	void			  SetHitBox(HitBox hb);						// Установить hitbox
	std::string		  GetName() const;							// Получить имя объекта
	DirectX::XMFLOAT2 GetPosition();							// Получить позицию объекта
	Surface2D&		  GetSurface();								// Получить плоскость изображения спрайта
	HitBox			  GetHitBox();								// Получить hitbox

	/***************************************/
protected:
	/* Внутренние методы */

	void CalculateDeltas();	// Рассчитать корректирующие отступы hitbox от блока спрайта персонажа

	/*********************/
protected:
	/* Переменные описания объекта */
	
	std::string		  name;		// Имя объекта
	DirectX::XMFLOAT2 position;	// Позиция объекта
	Surface2D		  sprite;	// Плоскость изображения спрайта

	/*******************************/
protected:
	/* Переменные описания работы с hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// Видимость hitbox
	float  dx;							// Корректирующий отступ hitbox от блока спрайта по оси x
	float  dy;							// Корректирующий отступ hitbox от блока спрайта по оси y

	/***************************************/
};
