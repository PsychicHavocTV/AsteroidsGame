#pragma once
#include "raylib.h"

typedef struct Meteor 
{
	Vector2 m_pos;
	Vector2 m_speed;

	float m_radius;

	bool active;

	Color m_colour;

	void MeteorLogic()
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

	void Draw()
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

} Meteor;