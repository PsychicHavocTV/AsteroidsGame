#pragma once
#include "raylib.h"

typedef struct Player 
{
	Vector2 p_pos;
	Vector2 p_speed;

	Vector3 p_collider;
	Color p_colour;

	float p_accel;
	float p_rot;
} Player;