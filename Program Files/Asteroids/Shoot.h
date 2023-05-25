#pragma once
#include "raylib.h"
#include "Player.h"
#define PLAYER_MAX_PROJECTILES 20

typedef struct Shoot
{
	Vector2 s_pos;
	Vector2 s_speed;

	float s_radius;
	float s_rot;

	int s_lifeSpan;

	bool active;

	Color s_colour;

	void ProjFunc(Player player, Shoot s_shoot[]);
	void InitShoot(Shoot s_shoot[]);
} Shoot;