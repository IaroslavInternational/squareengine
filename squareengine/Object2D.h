#pragma once

#include <DirectXMath.h>
#include <string>

// Базовый класс для всех объектов на сцене. От этого класса наследуются остальные производные объекты.
class Object2D
{
public:
	// Конструктор объекта требует следующие поля:
	// name - имя объекта
	// position - позиция объекта
	Object2D(std::string name, DirectX::XMFLOAT2 position);
public:
	// Получить имя объекта
	std::string		  GetName() const;

	// Получить позицию объекта
	DirectX::XMFLOAT2 GetPosition();
	DirectX::XMFLOAT2* GetPositionPtr();

	// Установить имя объекта
	void			  SetName(std::string name);

	// Установить позицию объекта
	void			  SetPosition(DirectX::XMFLOAT2 position);
protected:
	// Имя объекта
	std::string		  name;
	
	// Позиция объекта
	DirectX::XMFLOAT2 position;
};
