#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "Shoot.h"
#include "Meteor.h"
#include "Player.h"

#define PLAYER_BASE_SIZE 20.0f
#define PLAYER_SPEED 3.0f
#define PLAYER_MAX_PROJECTILES 30

#define METEORS_SPEED 2
#define MAX_LARGE_METEORS 6
#define MAX_MEDIUM_METEORS 8
#define MAX_SMALL_METEORS 16

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool pause = false;
static bool victory = false;

static bool medPointsSet = true;
static bool smallPointsSet = true;

static float p_shipHeight = 0.0f;

static Player player = { 0 };
static Shoot shoot[PLAYER_MAX_PROJECTILES] = { 0 };
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
static void UnloadGame();
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

	// De-Initialization.
	UnloadGame();

	CloseWindow();
}

void InitGame()
{

	neededPoints = 1;

	int posX, posY;
	int velX, velY;

	bool correctRange = false;

	victory = false;
	pause = false;

	p_shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);

	// Player Initialization.
	player.p_pos = Vector2{ screenWidth / 2, screenHeight / 2 - p_shipHeight / 2 };
	player.p_speed = Vector2{ 0,0 };
	player.p_accel = 0;
	player.p_rot = 0;
	player.p_collider = Vector3{ player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (p_shipHeight / 2.5f), player.p_pos.y - 
												  (float)cos(player.p_rot * DEG2RAD) * (p_shipHeight / 2.5f), 12 };
	player.p_colour = SKYBLUE;

	destroyedMeteorsCount = 0;


	// Shoot Initialization.
	for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
	{
		shoot[i].s_pos = Vector2{ 0,0 };
		shoot[i].s_speed = Vector2{ 0,0 };
		shoot[i].s_radius = 2;
		shoot[i].active = false;
		shoot[i].s_lifeSpan = 0;
		shoot[i].s_colour = RED;
	}

	for (int i = 0; i < MAX_LARGE_METEORS; i++)
	{
		posX = rand() % screenWidth / 2;

		while (!correctRange)
		{
			if (posX > 790 + largeMeteor[i].m_radius && posX < 0 - largeMeteor[i].m_radius)
			{
				posX = rand() % screenWidth / 2;
			}
			else
			{
				correctRange = true;
			}
		}

		correctRange = false;

		posY = rand() % screenHeight / 2;

		while (!correctRange)
		{
			if (posY > 440 + largeMeteor[i].m_radius && posY < 0 - largeMeteor[i].m_radius)
			{
				posY = rand() % screenHeight / 2;
			}
			else
			{
				correctRange = true;
			}
		}

		largeMeteor[i].m_pos = Vector2{ (float)posX, (float)posY };

		correctRange = false;
		velX = rand() / ((RAND_MAX) / METEORS_SPEED);
		velY = rand() / ((RAND_MAX) / METEORS_SPEED);

		while (!correctRange)
		{
			if (velX == 0 && velY == 0)
			{
				velX = rand() / ( -METEORS_SPEED / METEORS_SPEED);
				velY = rand() / ( -METEORS_SPEED / METEORS_SPEED);
			}
			else
			{
				correctRange = true;
			}
		}

		largeMeteor[i].m_speed = Vector2{ (float)velX, (float)velY };
		largeMeteor[i].m_radius = 30;
		largeMeteor[i].active = true;
		largeMeteor[i].m_colour = DARKGRAY;
	}

	for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
	{
		mediumMeteor[i].m_pos = Vector2{ -100, -100 };
		mediumMeteor[i].m_speed = Vector2{ 0,0 };
		mediumMeteor[i].m_radius = 20;
		mediumMeteor[i].active = false;
		mediumMeteor[i].m_colour = BLACK;
	}

	for (int i = 0; i < MAX_SMALL_METEORS; i++)
	{
		smallMeteor[i].m_pos = Vector2{ -100, -100 };
		smallMeteor[i].m_speed = Vector2{ 0,0 };
		smallMeteor[i].m_radius = 10;
		smallMeteor[i].active = false;
		smallMeteor[i].m_colour = DARKGRAY;
	}

	midMeteorsCount = 0;
	smallMeteorsCount = 0;

	
}

void Update()
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

	if (gameOver == false && victory == false)
	{
		if (IsKeyPressed('P'))
		{
			pause = !pause;
		}

		if (!pause)
		{
			// Player Logic - Rotation.
			if (IsKeyDown(KEY_LEFT))
			{
				player.p_rot -= 5;
			}
			
			if (IsKeyDown(KEY_RIGHT))
			{
				player.p_rot += 5;
			}
			// END


			// Player Logic - Speed.
			//(DEG2RAD is basically PI / 180.0f) - Converting DEGREES to RADIANS.
			player.p_speed.x = (float)sin(player.p_rot * DEG2RAD) * PLAYER_SPEED;
			player.p_speed.y = (float)cos(player.p_rot * DEG2RAD) * PLAYER_SPEED;
			// END


			// Player Logic - Acceleration.
			if (IsKeyDown(KEY_UP))
			{
				if (player.p_accel < 1)
				{
					player.p_accel += 0.04f;
				}
			}
			else 
			{
				if (player.p_accel > 0)
				{
					player.p_accel -= 0.02f;
				}
				else if (player.p_accel < 0)
				{
					player.p_accel = 0;
				}
			}

			if (IsKeyDown(KEY_DOWN))
			{
				if (player.p_accel > 0)
				{
					player.p_accel -= 0.04f;
				}
				else if (player.p_accel < 0)
				{
					player.p_accel = 0;
				}
			}
			// END


			// Player Logic - Movement.
			player.p_pos.x += (player.p_speed.x * player.p_accel);
			player.p_pos.y -= (player.p_speed.y * player.p_accel);
			// END


			// Collision Logic - Player -> Walls.
			if (player.p_pos.x > screenWidth + p_shipHeight) 
			{
				player.p_pos.x = -(p_shipHeight);
			}
			else if (player.p_pos.x < -(p_shipHeight))
			{
				player.p_pos.x = screenWidth + p_shipHeight;
			}

			if (player.p_pos.y > (screenHeight + p_shipHeight))
			{
				player.p_pos.y = -(p_shipHeight);
			}
			else if (player.p_pos.y < -(p_shipHeight))
			{
				player.p_pos.y = screenHeight + p_shipHeight;
			}
			// END

			// Player Shooting Logic.
			if (IsKeyReleased(KEY_SPACE))
			{
				for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
				{
					if (!shoot[i].active)
					{
						shoot[i].s_pos = Vector2{ player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (p_shipHeight),
												 player.p_pos.y - (float)cos(player.p_rot * DEG2RAD) * (p_shipHeight) };
						shoot[i].active = true;
						shoot[i].s_speed.x = 1.5f * (float)sin(player.p_rot * DEG2RAD) * PLAYER_SPEED;
						shoot[i].s_speed.y = 1.5f * (float)cos(player.p_rot * DEG2RAD) * PLAYER_SPEED;
						shoot[i].s_rot = player.p_rot;
						break;
					}
				}
			}
			// END

			// Player Projectile Life Timer.
			for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
			{
				if (shoot[i].active)
				{
					shoot[i].s_lifeSpan++;
				}
			}
			// END

			// Player Projectile Logic.
			for (int i = 0; i < PLAYER_MAX_PROJECTILES; i++)
			{
				// Movement.
				shoot[i].s_pos.x += shoot[i].s_speed.x;
				shoot[i].s_pos.y -= shoot[i].s_speed.y;
				// END

				// Collision Logic - Player Projectiles -> Walls.
				if (shoot[i].s_pos.x > screenWidth + shoot[i].s_radius)
				{
					shoot[i].active = false;
					shoot[i].s_lifeSpan = 0;
				}
				else if (shoot[i].s_pos.x < 0 - shoot[i].s_radius)
				{
					shoot[i].active = false;
					shoot[i].s_lifeSpan = 0;
				}

				if (shoot[i].s_pos.y > screenHeight + shoot[i].s_radius)
				{
					shoot[i].active = false;
					shoot[i].s_lifeSpan = 0;
				}
				else if (shoot[i].s_pos.y < 0 - shoot[i].s_radius)
				{
					shoot[i].active = false;
					shoot[i].s_lifeSpan = 0;
				}
				// END


				// Life Of Player Projectile
				if (shoot[i].s_lifeSpan >= 60)
				{
					shoot[i].s_pos = Vector2{ 0,0 };
					shoot[i].s_speed = Vector2{ 0,0 };
					shoot[i].s_lifeSpan = 0;
					shoot[i].active = false;
				}
				// END
			}
			// END

			// Collision Logic - Player -> Meteor.
			player.p_collider = Vector3{ player.p_pos.x + (float)sin(player.p_rot * DEG2RAD) * (p_shipHeight),
										player.p_pos.y - (float)cos(player.p_rot * DEG2RAD) * (p_shipHeight / 2.5f), 12 };

			for (int a = 0; a < MAX_LARGE_METEORS; a++)
			{
				if (CheckCollisionCircles(Vector2{ player.p_collider.x, player.p_collider.y }, player.p_collider.z,
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
				if (shoot[i].active)
				{
					for (int a = 0; a < MAX_LARGE_METEORS; a++)
					{
						if (largeMeteor[a].active && CheckCollisionCircles(shoot[i].s_pos, shoot[i].s_radius, largeMeteor[a].m_pos, largeMeteor[a].m_radius))
						{
							shoot[i].active = false;
							shoot[i].s_lifeSpan = 0;
							largeMeteor[a].active = false;
							medPointsSet = false;
						
							for (int j = 0; j < 2; j++)
							{
								if (midMeteorsCount % 2 == 0)
								{
									mediumMeteor[midMeteorsCount].m_pos = Vector2{ largeMeteor[a].m_pos.x, largeMeteor[a].m_pos.y };
									mediumMeteor[midMeteorsCount].m_speed = (Vector2{ (float)cos(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1,
																			  (float)sin(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									mediumMeteor[midMeteorsCount].m_pos.x = largeMeteor[a].m_pos.x;
									mediumMeteor[midMeteorsCount].m_pos.y = largeMeteor[a].m_pos.y;
									mediumMeteor[midMeteorsCount].m_speed = Vector2{ (float)cos(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED,
																						 (float)sin(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED };
								}

								mediumMeteor[midMeteorsCount].active = true;
								midMeteorsCount++;
							}

							
							largeMeteor[a].m_colour = RED;
							a = MAX_LARGE_METEORS;
						}

						
					}
				}

				if (shoot[i].active)
				{
					for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
					{
						if (mediumMeteor[b].active && CheckCollisionCircles(shoot[i].s_pos, shoot[i].s_radius, mediumMeteor[b].m_pos, mediumMeteor[b].m_radius))
						{
							shoot[i].active = false;
							shoot[i].s_lifeSpan = 0;
							mediumMeteor[b].active = false;
							destroyedMeteorsCount++;
							smallPointsSet = false;
							for (int j = 0; j < 2; j++)
							{
								if (smallMeteorsCount % 2 == 0)
								{
									smallMeteor[smallMeteorsCount].m_pos = Vector2{ mediumMeteor[b].m_pos.x, mediumMeteor[b].m_pos.y };
									smallMeteor[smallMeteorsCount].m_speed = (Vector2{ (float)cos(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1,
																			  (float)sin(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED * -1 });
								}
								else
								{
									smallMeteor[smallMeteorsCount].m_pos = Vector2{ mediumMeteor[b].m_pos.x, mediumMeteor[b].m_pos.y };
									smallMeteor[smallMeteorsCount].m_speed = Vector2{ (float)cos(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED,
																					 (float)sin(shoot[i].s_rot * DEG2RAD) * METEORS_SPEED };
								}

								smallMeteor[smallMeteorsCount].active = true;
								smallMeteorsCount++;
							}

							mediumMeteor[b].m_colour = GREEN;
							b = MAX_MEDIUM_METEORS;
						}
					}

					
				}

				if (shoot[i].active)
				{
					for (int c = 0; c < MAX_SMALL_METEORS; c++)
					{
						if (smallMeteor[c].active && CheckCollisionCircles(shoot[i].s_pos, shoot[i].s_radius, smallMeteor[c].m_pos, smallMeteor[c].m_radius))
						{
							shoot[i].active = false;
							shoot[i].s_lifeSpan = 0;
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
		Vector2 v1 = { player.p_pos.x + sinf(player.p_rot * DEG2RAD) * (p_shipHeight), player.p_pos.y - cosf(player.p_rot * DEG2RAD) * (p_shipHeight) };
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
			if (shoot[i].active)
			{
				DrawCircleV(shoot[i].s_pos, shoot[i].s_radius, RED);
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

//Unload Game Variables.
void UnloadGame()
{
	//TODO - Unload all loaded data.
}

// Update and Draw (One Frame).
void UpdateDrawFrame()
{
	Update();
	DrawGame();
}