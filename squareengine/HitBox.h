#pragma once

#include <DirectXMath.h>
#include <string>

class HitBox
{
public:
	HitBox(std::string name, DirectX::XMFLOAT4 coordinates);
	HitBox(std::string name, float leftTopX,     float leftTopY,
							 float rightBottomX, float rightBottomY);
public:
	/* Операторы */
	
	HitBox operator-(DirectX::XMFLOAT2 delta);
	HitBox operator+(DirectX::XMFLOAT2 delta);

	/*************/
public:
	/* Основные методы для работы с hitbox */
	
	void			   Update(DirectX::XMFLOAT2 delta);	// Обновить 4 координаты углов hitbox корректирующими отступами
	void			   Update(float dx, float dy);		// Обновить 4 координаты углов hitbox корректирующими отступами
	void			   UpdateX(float dx);				// Обновить 2 координаты углов hitbox корректирующими отступами
	void			   UpdateY(float dy);				// Обновить 2 координаты углов hitbox корректирующими отступами
	std::string		   GetName();						// Получить имя
	DirectX::XMFLOAT4& GetCoordinates();				// Получить 4 координаты углов hitbox 

	/***************************************/
public:
	/* Методы операций с hitbox */
	
	bool IsCollide(HitBox hb);	// Проверка столкновения с другим hitbox

	/****************************/
private:
	/* Переменные описания hitbox */
	
	std::string		  name;
	DirectX::XMFLOAT4 coordinates; // 4 координаты углов hitbox

	/******************************/
};