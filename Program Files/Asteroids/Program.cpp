#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "Shoot.h"
#include "Meteor.h"
#include "Player.h"


#define MAX_LARGE_METEORS 4
#define MAX_MEDIUM_METEORS 8
#define MAX_SMALL_METEORS 16

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool pause = false;
static bool victory = false;

static bool medPointsSet = true;
static bool smallPointsSet = true;


float Player::shipHeight = 0.0f;

static Shoot s_shoot[PLAYER_MAX_PROJECTILES];
Player player = {0};

static Meteor largeMeteor[MAX_LARGE_METEORS] = { 0 };
static Meteor mediumMeteor[MAX_MEDIUM_METEORS] = { 0 };
static Meteor smallMeteor[MAX_SMALL_METEORS] = { 0 };

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

static int neededPoints = 1;

static void InitGame();
static void Update();
static void DrawGame();
static void UpdateDrawFrame();

int main(void)
{
	InitWindow(screenWidth, screenHeight, "C++ Asteroids");

	InitGame();

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		// Update and draw the game.
		UpdateDrawFrame();
	}
	CloseWindow();
}

void InitGame()
{
	player.PlayerInit(player);

	neededPoints = 1;
	victory = false;
	pause = false;

	player.shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);

	// Player Initialization.
	player.p_pos = Vector2{ screenWidth / 2, screenHeight / 2 - player.shipHeight / 2 };
	player.p_speed = Vector2{ 0,0 };
	player.p_accel = 0;
	player.p_rot = 0;
	player.p_collider = Vector3{ player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (player.shipHeight / 2.5f), player.p_pos.y -
									  (float)cos(player.p_rot * DEG2RAD) * (player.shipHeight / 2.5f), 12};
	player.p_colour = SKYBLUE;

	destroyedMeteorsCount = 0;

	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		s_shoot[i].InitShoot(s_shoot);
	}
	
	for (int i = 0; i < MAX_LARGE_METEORS; i++)
	{
		largeMeteor[i].m_colour = DARKGRAY;
		largeMeteor[i].InitLargeMeteor();
	}

	for (int i = 0; i < MAX_SMALL_METEORS; i++)
	{
		smallMeteor[i].m_radius = 10;
		smallMeteor[i].m_colour = DARKGRAY;
		smallMeteor[i].InitMeteor();
	}

	for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
	{
		mediumMeteor[i].m_radius = 20;
		mediumMeteor[i].m_colour = BLACK;
		mediumMeteor[i].InitMeteor();
	}
	midMeteorsCount = 0;
	smallMeteorsCount = 0;
}

void Update()
{
	if (gameOver == false && victory == false)
	{
		if (medPointsSet == false || smallPointsSet == false)
		{
			if (medPointsSet == false)
			{
				neededPoints += 2;
				medPointsSet = true;
			}

			if (smallPointsSet == false)
			{
				neededPoints += 2;
				smallPointsSet = true;
			}
		}

		if (IsKeyPressed('P'))
		{
			pause = !pause;
		}

		if (!pause)
		{
			player.p_update();
			player.p_wCol(player);
			for (int i = 0; i < 1; i++)
			{
				s_shoot[i].ProjFunc(player, s_shoot);
			}

			// Collision Logic - Player -> Meteor.
			player.p_collider = Vector3{player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (player.shipHeight),
										player.p_pos.y - (float)cos(player.p_rot * DEG2RAD) * (player.shipHeight / 2.5f), 12};

			for (int a = 0; a < MAX_LARGE_METEORS; a++)
			{
				if (CheckCollisionCircles(Vector2{ player.p_collider.x, player.p_collider.y}, player.p_collider.z,
					largeMeteor[a].m_pos, largeMeteor[a].m_radius) && largeMeteor[a].active)
				{
					gameOver = true;
				}
			}

			for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
			{
				if (CheckCollisionCircles(Vector2{ player.p_collider.x, player.p_collider.y }, player.p_collider.z,
										  mediumMeteor[a].m_pos, mediumMeteor[a].m_radius) && mediumMeteor[a].active)
				{
					gameOver = true;
				}
			}

			for (int a = 0; a < MAX_LARGE_METEORS; a++)
			{
				if (CheckCollisionCircles(Vector2{ player.p_collider.x, player.p_collider.y }, player.p_collider.z,
										  smallMeteor[a].m_pos, smallMeteor[a].m_radius) && smallMeteor[a].active)
				{
					gameOver = true;
				}
			}
			// END

			// Meteors logic - Large Meteors.
			for (int i = 0; i < MAX_LARGE_METEORS; i++)
			{
				largeMeteor[i].MeteorLogic();
			}
			// END

			// Meteors Logic - Medium Meteors.
			for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
			{
				mediumMeteor[i].MeteorLogic();
			}
			// END

			// Meteors Logic - Small Meteors.
			for (int i = 0; i < MAX_SMALL_METEORS; i++)
			{
				smallMeteor[i].MeteorLogic();
			}
			// END

			// Collision logic - Player Projectiles -> Meteors
 			for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
			{
				if (s_shoot[i].active)
				{
					for (int a = 0; a < MAX_LARGE_METEORS; a++)
					{
						if (largeMeteor[a].active && CheckCollisionCircles(s_shoot[i].s_pos, s_shoot[i].s_radius, largeMeteor[a].m_pos, largeMeteor[a].m_radius))
						{
							s_shoot[i].active = false;
							s_shoot[i].s_lifeSpan = 0;
							largeMeteor[a].active = false;
							medPointsSet = false;
						
							for (int j = 0; j < 2; j++)
							{
								if (midMeteorsCount % 2 == 0)
								{
									mediumMeteor[midMeteorsCount].m_pos = Vector2{ largeMeteor[a].m_pos.x, largeMeteor[a].m_pos.y };
									mediumMeteor[midMeteorsCount].m_speed = (Vector2{ (float)cos(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1,
																			  (float)sin(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									mediumMeteor[midMeteorsCount].m_pos.x = largeMeteor[a].m_pos.x;
									mediumMeteor[midMeteorsCount].m_pos.y = largeMeteor[a].m_pos.y;
									mediumMeteor[midMeteorsCount].m_speed = Vector2{ (float)cos(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED,
																						 (float)sin(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED };
								}

								mediumMeteor[midMeteorsCount].active = true;
								midMeteorsCount++;
							}
							
							largeMeteor[a].m_colour = RED;
							a = MAX_LARGE_METEORS;
						}
					}
				}

				if (s_shoot[i].active)
				{
					for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
					{
						if (mediumMeteor[b].active && CheckCollisionCircles(s_shoot[i].s_pos, s_shoot[i].s_radius, mediumMeteor[b].m_pos, mediumMeteor[b].m_radius))
						{
							s_shoot[i].active = false;
							s_shoot[i].s_lifeSpan = 0;
							mediumMeteor[b].active = false;
							destroyedMeteorsCount++;
							smallPointsSet = false;
							for (int j = 0; j < 2; j++)
							{
								if (smallMeteorsCount % 2 == 0)
								{
									smallMeteor[smallMeteorsCount].m_pos = Vector2{ mediumMeteor[b].m_pos.x, mediumMeteor[b].m_pos.y };
									smallMeteor[smallMeteorsCount].m_speed = (Vector2{ (float)cos(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1,
																			  (float)sin(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									smallMeteor[smallMeteorsCount].m_pos = Vector2{ mediumMeteor[b].m_pos.x, mediumMeteor[b].m_pos.y };
									smallMeteor[smallMeteorsCount].m_speed = Vector2{ (float)cos(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED,
																					 (float)sin(s_shoot[i].s_rot * DEG2RAD) * METEORS_SPEED };
								}

								smallMeteor[smallMeteorsCount].active = true;
								smallMeteorsCount++;
							}
							mediumMeteor[b].m_colour = GREEN;
							b = MAX_MEDIUM_METEORS;
						}
					}
				}

				if (s_shoot[i].active)
				{
					for (int c = 0; c < MAX_SMALL_METEORS; c++)
					{
						if (smallMeteor[c].active && CheckCollisionCircles(s_shoot[i].s_pos, s_shoot[i].s_radius, smallMeteor[c].m_pos, smallMeteor[c].m_radius))
						{
							s_shoot[i].active = false;
							s_shoot[i].s_lifeSpan = 0;
							smallMeteor[c].active = false;
							destroyedMeteorsCount++;
							smallMeteor[c].m_colour = YELLOW;
							c = MAX_SMALL_METEORS;
						}
					}
				}
			}

			if (neededPoints > 1)
			{
				if (destroyedMeteorsCount >= neededPoints - 1 && medPointsSet == true && smallPointsSet == true)
				{
					victory = true;
				}
			}
			else
			{
				if (IsKeyReleased(KEY_ENTER))
				{
					InitGame();
					gameOver = false;
				}
			}
			// END

		}
	}
	else 
	{
		if (IsKeyReleased(KEY_ENTER))
		{
			InitGame();
			gameOver = false;
		}

	}
}


void DrawGame()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	if (!gameOver)
	{

		//Draw Spaceship.
		Vector2 v1 = { player.p_pos.x + sinf(player.p_rot * DEG2RAD) * (player.shipHeight), player.p_pos.y - cosf(player.p_rot * DEG2RAD) * (player.shipHeight) };
		Vector2 v2 = { player.p_pos.x - cosf(player.p_rot * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.p_pos.y - sinf(player.p_rot * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		Vector2 v3 = { player.p_pos.x + cosf(player.p_rot * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.p_pos.y + sinf(player.p_rot * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
		DrawTriangle(v1, v2, v3, PURPLE);

		//Draw Meteors.
		for (int i = 0; i < MAX_LARGE_METEORS; i++)
		{
			largeMeteor[i].Draw();
		}

		for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
		{
			mediumMeteor[i].Draw();
		}

		for (int i = 0; i < MAX_SMALL_METEORS; i++)
		{
			smallMeteor[i].Draw();
		}

		// Draw Player Projectiles
		for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
		{
			if (s_shoot[i].active)
			{
				DrawCircleV(s_shoot[i].s_pos, s_shoot[i].s_radius, RED);
			}

			if (victory)
			{
				DrawText("Victory!", screenWidth / 2 - MeasureText("Victory!", 80) / 2, screenWidth / 2 - MeasureText("Victory!", 80), 80, MAGENTA);
			}

			if (pause)
			{
				DrawText("Game Paused.", screenWidth / 2 - MeasureText("Game Paused.", 40) / 2, screenHeight / 2 - 40, 40, DARKBLUE);
			}
		}
	}
	else
	{
		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, BLACK);
	}

	EndDrawing();

}

// Update and Draw (One Frame).
void UpdateDrawFrame()
{
	Update();
	DrawGame();
}