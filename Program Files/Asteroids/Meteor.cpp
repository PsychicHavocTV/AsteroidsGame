#include "Meteor.h"
#include <math.h>
#include <stdlib.h>

int Meteor::posX;
int Meteor::posY;
int Meteor::velX;
int Meteor::velY;

void Meteor::MeteorLogic()
{
	if (active)
	{
		// Movement.
		m_pos.x += m_speed.x;
		m_pos.y += m_speed.y;
		// END

		// Collision Logic - Meteor -> Wall.
		if (m_pos.x > 790 + m_radius)
		{
			m_pos.x = -(m_radius);
		}
		else if (m_pos.x < 0 - m_radius)
		{
			m_pos.x = 790 + m_radius;
		}

		if (m_pos.y > 440 + m_radius)
		{
			m_pos.y = -(m_radius);
		}
		else if (m_pos.y < 0 - m_radius)
		{
			m_pos.y = 440 + m_radius;
		}
		// END
	}
}

void Meteor::InitMeteor()
{
	m_pos = Vector2{ -100, -100 };
	m_speed = Vector2{ 0,0 };
	active = false;
}

void Meteor::InitLargeMeteor()
{
	correctRange = false;

	posX = rand() % 790 / 2;

	while (!correctRange)
	{
		if (posX > 790 + m_radius && posX < 0 - m_radius)
		{
			posX = rand() % 790 / 2;
		}
		else
		{
			correctRange = true;
		}
	}

	correctRange = false;

	posY = rand() % 440 / 2;

	while (!correctRange)
	{
		if (posY > 440 + m_radius && posY < 0 - m_radius)
		{
			posY = rand() % 440 / 2;
		}
		else
		{
			correctRange = true;
		}
	}

	m_pos = Vector2{ (float)posX, (float)posY };

	correctRange = false;
	velX = rand() / ((RAND_MAX) / METEORS_SPEED);
	velY = rand() / ((RAND_MAX) / METEORS_SPEED);

	while (!correctRange)
	{
		if (velX == 0 && velY == 0)
		{
			velX = rand() / (-METEORS_SPEED / METEORS_SPEED);
			velY = rand() / (-METEORS_SPEED / METEORS_SPEED);
		}
		else
		{
			correctRange = true;
		}
	}

	m_speed = Vector2{ (float)velX, (float)velY };
	m_radius = 30;
	active = true;
}

void Meteor::Draw()
{
	{
		if (active)
		{
			DrawCircleV(m_pos, m_radius, m_colour);
		}
		else
		{
			DrawCircleV(m_pos, m_radius, Fade(m_colour, 0.3f));
		}
	}
}