#include "Player.h"
#include "Shoot.h"
#include <math.h>

void Shoot::InitShoot(Shoot s_shoot[])
{
	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		s_shoot[i] = { 0 };
	}
	//Shoot shoot[PLAYER_MAX_PROJECTILES] = s_shoot[PLAYER_MAX_PROJECTILES];

	// Shoot Initialization.
	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		s_shoot[i].s_pos = Vector2{ 0,0 };
		s_shoot[i].s_speed = Vector2{ 0,0 };
		s_shoot[i].s_radius = 2;
		s_shoot[i].active = false;
		s_shoot[i].s_lifeSpan = 0;
		s_shoot[i].s_colour = RED;
	}
}

void Shoot::ProjFunc(Player player, Shoot s_shoot[])
{
	// Player Shooting Logic.
	if (IsKeyReleased(KEY_SPACE))
	{
		for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
		{
			if (!s_shoot[i].active)
			{
				s_shoot[i].s_pos = Vector2{ player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (Player::shipHeight),
								 player.p_pos.y - (float)cos(player.p_rot * DEG2RAD) * (Player::shipHeight)};
				s_shoot[i].active = true;
				s_shoot[i].s_speed.x = 1.5f * (float)sin(player.p_rot * DEG2RAD) * PLAYER_SPEED;
				s_shoot[i].s_speed.y = 1.5f * (float)cos(player.p_rot * DEG2RAD) * PLAYER_SPEED;
				s_shoot[i].s_rot = player.p_rot;
				break;
			}
		}

		// Player Projectile Life Timer.
	}
	// END

	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		if (s_shoot[i].active)
		{
			s_shoot[i].s_lifeSpan++;
		}
	}
	// END

	// Player Projectile Logic.
	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		// Movement.
		s_shoot[i].s_pos.x += s_shoot[i].s_speed.x;
		s_shoot[i].s_pos.y -= s_shoot[i].s_speed.y;
		// END

		// Collision Logic - Player Projectiles -> Walls.
		if (s_shoot[i].s_pos.x > 800 + s_shoot[i].s_radius)
		{
			s_shoot[i].active = false;
			s_shoot[i].s_lifeSpan = 0;
		}
		else if (s_shoot[i].s_pos.x < 0 - s_shoot[i].s_radius)
		{
			s_shoot[i].active = false;
			s_shoot[i].s_lifeSpan = 0;
		}

		if (s_shoot[i].s_pos.y > 450 + s_shoot[i].s_radius)
		{
			s_shoot[i].active = false;
			s_shoot[i].s_lifeSpan = 0;
		}
		else if (s_shoot[i].s_pos.y < 0 - s_shoot[i].s_radius)
		{
			s_shoot[i].active = false;
			s_shoot[i].s_lifeSpan = 0;
		}
		// END


		// Life Of Player Projectile
		if (s_shoot[i].s_lifeSpan >= 60)
		{
			s_shoot[i].s_pos = Vector2{ 0,0 };
			s_shoot[i].s_speed = Vector2{ 0,0 };
			s_shoot[i].s_lifeSpan = 0;
			s_shoot[i].active = false;
		}
		// END
	}
	// END
}