#include "Player.h"
#include <math.h>

void Player::PlayerInit(Player player)
{

}


void Player::p_update()
{
	// Player Logic - Rotation.
	if (IsKeyDown(KEY_LEFT))
	{
		Player::p_rot -= 5;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		Player::p_rot += 5;
	}
	// END


	// Player Logic - Speed.
	//(DEG2RAD is basically PI / 180.0f) - Converting DEGREES to RADIANS.
	Player::p_speed.x = (float)sin(Player::p_rot * DEG2RAD) * PLAYER_SPEED;
	Player::p_speed.y = (float)cos(Player::p_rot * DEG2RAD) * PLAYER_SPEED;
	// END


	// Player Logic - Acceleration.
	if (IsKeyDown(KEY_UP))
	{
		if (p_accel < 1)
		{
			p_accel += 0.04f;
		}
	}
	else
	{
		if (p_accel > 0)
		{
			p_accel -= 0.02f;
		}
		else if (p_accel < 0)
		{
			p_accel = 0;
		}
	}

	if (IsKeyDown(KEY_DOWN))
	{
		if (p_accel > 0)
		{
			p_accel -= 0.04f;
		}
		else if (p_accel < 0)
		{
			p_accel = 0;
		}
	}
	// END


	// Player Logic - Movement.
	p_pos.x += (p_speed.x * p_accel);
	p_pos.y -= (p_speed.y * p_accel);
	// END
}

void Player::p_wCol(Player player)
{
	// Collision Logic - Player -> Walls.
	if (Player::p_pos.x > 800 + shipHeight)
	{
		p_pos.x = -(shipHeight);
	}
	else if (p_pos.x < -(shipHeight))
	{
		p_pos.x = 800 + shipHeight;
	}

	if (p_pos.y > (450 + shipHeight))
	{
		p_pos.y = -(shipHeight);
	}
	else if (p_pos.y < -(shipHeight))
	{
		p_pos.y = 450 + shipHeight;
	}
	// END
}

