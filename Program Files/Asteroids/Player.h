#pragma once
#include "raylib.h"
#define PLAYER_BASE_SIZE 10.0f
#define PLAYER_SPEED 3.0f


typedef struct Player 
{
	Vector2 p_pos;
	Vector2 p_speed;

	Vector3 p_collider;
	Color p_colour;

	float p_accel;
	float p_rot;

	static float shipHeight;

	void p_update();
	void p_wCol(Player player);
	void PlayerInit(Player player);



} Player;