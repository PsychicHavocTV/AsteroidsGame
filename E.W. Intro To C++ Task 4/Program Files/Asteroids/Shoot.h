#pragma once
#include "raylib.h"

typedef struct Shoot
{
	Vector2 s_pos;
	Vector2 s_speed;

	float s_radius;
	float s_rot;

	int s_lifeSpan;

	bool active;

	Color s_colour;
} Shoot;