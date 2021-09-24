#pragma once

#include <DirectXMath.h>

class HitBox
{
public:
	HitBox(DirectX::XMFLOAT4 coordinates);
	HitBox(float leftTopX,     float leftTopY,
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
	DirectX::XMFLOAT4& GetCoordinates();				// Получить 4 координаты углов hitbox 

	/***************************************/
public:
	/* Методы операций с hitbox */
	
	bool IsCollide(HitBox& hb);	// Проверка столкновения с другим hitbox

	/****************************/
private:
	/* Переменные описания hitbox */
	
	DirectX::XMFLOAT4 coordinates; // 4 координаты углов hitbox

	/******************************/
};