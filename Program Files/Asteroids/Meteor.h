#pragma once
#include "raylib.h"

#define METEORS_SPEED 2

typedef struct Meteor 
{
	Vector2 m_pos;
	Vector2 m_speed;

	float m_radius;

	static int posX, posY;
	static int velX, velY;
	
	bool active;
	bool correctRange;

	Color m_colour;

	void MeteorLogic();

	void InitMeteor();
	void InitLargeMeteor();

	void Draw();
	
	static float Test;

} Meteor;