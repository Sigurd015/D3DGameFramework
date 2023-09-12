#include "Battle.h"
#include "../Math/Vector.h"
#include "../Input/Input.h"
#include "../Screen/Renderer.h"
#include "../Common.h"

#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <conio.h>
#include "../Title/LoadingPage.h"
#include "../Title/TitleMenu.h"

enum Direction
{
	LEFT, RIGHT, UP, DOWN
};

struct Player
{
	Vector2 Pos;
	int BombCount;
	int Score;
};

struct Bomb
{
	Vector2 Pos = { -1,-1 };
	bool IsEnable = false;
	int RemainTime = BOMB_EXPLOSION_TIME;
};

struct BombExplosion
{
	Vector2 Pos;
	Direction Dir;
	int RemainTime = BOMB_EXPLOSION_EFFECT_FRAME;
	bool IsEnable = false;
};

struct BattleCanvasElement
{
	char* Text;
	int StartLine;
};

struct BattleData
{
	Player Player[MAX_PLAYER_COUNT];
	Bomb Bombs[MAX_BOMB_COUNT];
	int RemainTime = TURN_TIME;

	Vector2 Player1InitPos = { 2, 2 };
	Vector2 Player2InitPos = { MAP_WIDTH - 2, MAP_HEIGHT - 2 };

	int CurrentBombExplosionCount = 0;
	BombExplosion BombExplosions[MAX_BOMB_EXPLOSION_COUNT];

	int Map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,1,0,1,0,1,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,1,1,1,1,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0},
		{0,1,0,1,0,1,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,1,1,1,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0},
		{0,1,0,1,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,1,8,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0},
		{0,1,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0},
		{0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0},
		{0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,0},
		{0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,1,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,0},
		{0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,1,0,1,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,0},
		{0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,8,0,1,0,1,0,1,0,0},
		{0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, };

	BattleCanvasElement RemainingTime = { "|                      Remaining Time :  ", 1 };
	char* LineEnd = "|\n";
	char* LineStart = "|";
	BattleCanvasElement PlayerScore[2] = {
		{"|             Player1 Score :  ", 2},
		{"              Player2 Score :  ", 3}, };

	BattleCanvasElement GameOver[3] = {
		{{"|                              Game Over                                      |\n"}, 11},
		{{"|             Player1 Score :"}, 12} ,
		{{"|             Player2 Score :"}, 13} , };

	int MaxSceneWidth = 80;
	int CurrentFrameTime = 0;
};
BattleData s_Data;

void Battle_Init()
{
	s_Data.Player[0].Pos = s_Data.Player1InitPos;
	s_Data.Player[1].Pos = s_Data.Player2InitPos;
	s_Data.Player[0].Score = 0;
	s_Data.Player[1].Score = 0;
	s_Data.Player[0].BombCount = 0;
	s_Data.Player[1].BombCount = 0;
	s_Data.RemainTime = TURN_TIME;
	s_Data.Map[s_Data.Player[0].Pos.y][s_Data.Player[0].Pos.x] = PLAYER1;
	s_Data.Map[s_Data.Player[1].Pos.y][s_Data.Player[1].Pos.x] = PLAYER2;

	Game_SetFramTime(BATTLE_SCENE_FRAME_TIME);
}

bool MoveCheck(Vector2* pos)
{
	return s_Data.Map[pos->y][pos->x] == EMPTY;
}

bool BombExplosionCheck(Vector2* pos)
{
	return s_Data.Map[pos->y][pos->x] == EMPTY
		|| s_Data.Map[pos->y][pos->x] == PLAYER1
		|| s_Data.Map[pos->y][pos->x] == PLAYER2
		|| s_Data.Map[pos->y][pos->x] == DESTROYABLE_WALL;
}

bool UpdateMap(Vector2* before, Vector2* after, MapType type)
{
	if (MoveCheck(after))
	{
		s_Data.Map[before->y][before->x] = EMPTY;
		s_Data.Map[after->y][after->x] = type;
		return true;
	}
	return false;
}

void PlayerInputProcess()
{
	if (Input_IsKeyPressed(Key::P1_UP))
	{
		Vector2 before = s_Data.Player[0].Pos;
		Vector2 after = { s_Data.Player[0].Pos.x, s_Data.Player[0].Pos.y - 1 };
		if (UpdateMap(&before, &after, PLAYER1))
		{
			s_Data.Player[0].Pos = after;
		}
	}
	else if (Input_IsKeyPressed(Key::P1_DOWN))
	{
		Vector2 before = s_Data.Player[0].Pos;
		Vector2 after = { s_Data.Player[0].Pos.x,s_Data.Player[0].Pos.y + 1 };
		if (UpdateMap(&before, &after, PLAYER1))
		{
			s_Data.Player[0].Pos = after;
		}
	}

	if (Input_IsKeyPressed(Key::P1_LEFT))
	{
		Vector2 before = s_Data.Player[0].Pos;
		Vector2 after = { s_Data.Player[0].Pos.x - 1 , s_Data.Player[0].Pos.y };
		if (UpdateMap(&before, &after, PLAYER1))
		{
			s_Data.Player[0].Pos = after;
		}
	}
	else if (Input_IsKeyPressed(Key::P1_RIGHT))
	{
		Vector2 before = s_Data.Player[0].Pos;
		Vector2 after = { s_Data.Player[0].Pos.x + 1 , s_Data.Player[0].Pos.y };
		if (UpdateMap(&before, &after, PLAYER1))
		{
			s_Data.Player[0].Pos = after;
		}
	}

	if (Input_IsKeyPressed(Key::P1_BOMB))
	{
		if (s_Data.Player[0].BombCount < MAX_BOMB_COUNT_PER_PLAYER && !s_Data.Bombs[s_Data.Player[0].BombCount].IsEnable)
		{
			s_Data.Bombs[s_Data.Player[0].BombCount].Pos = s_Data.Player[0].Pos;
			s_Data.Bombs[s_Data.Player[0].BombCount].IsEnable = true;
			s_Data.Bombs[s_Data.Player[0].BombCount].RemainTime = BOMB_EXPLOSION_TIME;
			s_Data.Player[0].BombCount++;
		}
	}

	if (Input_IsKeyPressed(Key::P2_UP))
	{
		Vector2 before = s_Data.Player[1].Pos;
		Vector2 after = { s_Data.Player[1].Pos.x ,s_Data.Player[1].Pos.y - 1 };
		if (UpdateMap(&before, &after, PLAYER2))
		{
			s_Data.Player[1].Pos = after;
		}
	}
	else if (Input_IsKeyPressed(Key::P2_DOWN))
	{
		Vector2 before = s_Data.Player[1].Pos;
		Vector2 after = { s_Data.Player[1].Pos.x, s_Data.Player[1].Pos.y + 1 };
		if (UpdateMap(&before, &after, PLAYER2))
		{
			s_Data.Player[1].Pos = after;
		}
	}

	if (Input_IsKeyPressed(Key::P2_LEFT))
	{
		Vector2 before = s_Data.Player[1].Pos;
		Vector2 after = { s_Data.Player[1].Pos.x - 1, s_Data.Player[1].Pos.y };
		if (UpdateMap(&before, &after, PLAYER2))
		{
			s_Data.Player[1].Pos = after;
		}
	}
	else if (Input_IsKeyPressed(Key::P2_RIGHT))
	{
		Vector2 before = s_Data.Player[1].Pos;
		Vector2 after = { s_Data.Player[1].Pos.x + 1, s_Data.Player[1].Pos.y };
		if (UpdateMap(&before, &after, PLAYER2))
		{
			s_Data.Player[1].Pos = after;
		}
	}

	if (Input_IsKeyPressed(Key::P2_BOMB))
	{
		if (s_Data.Player[1].BombCount < MAX_BOMB_COUNT_PER_PLAYER && !s_Data.Bombs[s_Data.Player[1].BombCount + MAX_BOMB_COUNT_PER_PLAYER].IsEnable)
		{
			s_Data.Bombs[s_Data.Player[1].BombCount + MAX_BOMB_COUNT_PER_PLAYER].Pos = s_Data.Player[1].Pos;
			s_Data.Bombs[s_Data.Player[1].BombCount + MAX_BOMB_COUNT_PER_PLAYER].IsEnable = true;
			s_Data.Bombs[s_Data.Player[1].BombCount + MAX_BOMB_COUNT_PER_PLAYER].RemainTime = BOMB_EXPLOSION_TIME;
			s_Data.Player[1].BombCount++;
		}
	}
}

void BombProcess()
{
	for (int i = 0; i < MAX_BOMB_COUNT; i++)
	{
		if (s_Data.Bombs[i].IsEnable)
		{
			switch (s_Data.Bombs[i].RemainTime)
			{
			case 3:
				s_Data.Map[s_Data.Bombs[i].Pos.y][s_Data.Bombs[i].Pos.x] = BOMB_3;
				break;
			case 2:
				s_Data.Map[s_Data.Bombs[i].Pos.y][s_Data.Bombs[i].Pos.x] = BOMB_2;
				break;
			case 1:
				s_Data.Map[s_Data.Bombs[i].Pos.y][s_Data.Bombs[i].Pos.x] = BOMB_1;
				break;
			}

			if (s_Data.Bombs[i].RemainTime == 0)
			{
				s_Data.Map[s_Data.Bombs[i].Pos.y][s_Data.Bombs[i].Pos.x] = EMPTY;

				if (s_Data.CurrentBombExplosionCount >= MAX_BOMB_EXPLOSION_COUNT)
				{
					s_Data.CurrentBombExplosionCount = 0;
				}

				{
					Vector2 bombPos = { s_Data.Bombs[i].Pos.x - 1, s_Data.Bombs[i].Pos.y };
					if (BombExplosionCheck(&bombPos))
					{
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Pos = bombPos;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].IsEnable = true;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Dir = LEFT;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].RemainTime = BOMB_EXPLOSION_TIME;
						s_Data.Map[bombPos.y][bombPos.x] = EXPLOSION_EFFECT;
						s_Data.CurrentBombExplosionCount++;
					}
				}

				{
					Vector2 bombPos = { s_Data.Bombs[i].Pos.x + 1, s_Data.Bombs[i].Pos.y };
					if (BombExplosionCheck(&bombPos))
					{
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Pos = bombPos;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].IsEnable = true;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Dir = RIGHT;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].RemainTime = BOMB_EXPLOSION_TIME;
						s_Data.Map[bombPos.y][bombPos.x] = EXPLOSION_EFFECT;
						s_Data.CurrentBombExplosionCount++;
					}
				}

				{
					Vector2 bombPos = { s_Data.Bombs[i].Pos.x, s_Data.Bombs[i].Pos.y - 1 };
					if (BombExplosionCheck(&bombPos))
					{
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Pos = bombPos;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].IsEnable = true;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Dir = UP;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].RemainTime = BOMB_EXPLOSION_TIME;
						s_Data.Map[bombPos.y][bombPos.x] = EXPLOSION_EFFECT;
						s_Data.CurrentBombExplosionCount++;
					}
				}

				{
					Vector2 bombPos = { s_Data.Bombs[i].Pos.x, s_Data.Bombs[i].Pos.y + 1 };
					if (BombExplosionCheck(&bombPos))
					{
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Pos = bombPos;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].IsEnable = true;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].Dir = DOWN;
						s_Data.BombExplosions[s_Data.CurrentBombExplosionCount].RemainTime = BOMB_EXPLOSION_TIME;
						s_Data.Map[bombPos.y][bombPos.x] = EXPLOSION_EFFECT;
						s_Data.CurrentBombExplosionCount++;
					}
				}

				s_Data.Bombs[i].Pos = { -1, -1 };
				s_Data.Bombs[i].IsEnable = false;
				if (i < MAX_BOMB_COUNT_PER_PLAYER)
					s_Data.Player[0].BombCount--;
				else
					s_Data.Player[1].BombCount--;
			}
		}
	}

	for (int i = 0; i < s_Data.CurrentBombExplosionCount; i++)
	{
		if (s_Data.BombExplosions[i].IsEnable)
		{
			Vector2* bombPos = &s_Data.BombExplosions[i].Pos;

			if (s_Data.BombExplosions[i].RemainTime == 0)
			{
				s_Data.Map[bombPos->y][bombPos->x] = EMPTY;
				s_Data.BombExplosions[i].IsEnable = false;
				continue;
			}

			{
				Vector2* p1Pos = &s_Data.Player[0].Pos;
				Vector2* p2Pos = &s_Data.Player[1].Pos;

				if (Vector2_IsEqual(bombPos, p1Pos))
				{
					s_Data.Player[0].Pos = s_Data.Player1InitPos;
					s_Data.Map[p1Pos->y][p1Pos->x] = PLAYER1;
					s_Data.Player[1].Score++;
				}

				if (Vector2_IsEqual(bombPos, p2Pos))
				{
					s_Data.Player[1].Pos = s_Data.Player2InitPos;
					s_Data.Map[p2Pos->y][p2Pos->x] = PLAYER2;
					s_Data.Player[0].Score++;
				}

				if (s_Data.Map[bombPos->x][bombPos->y] == DESTROYABLE_WALL)
				{
					s_Data.Map[bombPos->x][bombPos->y] = EMPTY;
				}
			}

			Vector2 pos;
			switch (s_Data.BombExplosions[i].Dir)
			{
			case Direction::UP: pos = { bombPos->x, bombPos->y - 1 }; break;
			case Direction::DOWN: pos = { bombPos->x, bombPos->y + 1 }; break;
			case Direction::LEFT: pos = { bombPos->x - 1, bombPos->y }; break;
			case Direction::RIGHT: pos = { bombPos->x + 1, bombPos->y }; break;
			}

			if (BombExplosionCheck(&pos))
			{
				s_Data.Map[bombPos->y][bombPos->x] = EMPTY;
				s_Data.Map[pos.y][pos.x] = EXPLOSION_EFFECT;
				s_Data.BombExplosions[i].Pos = pos;
			}
			else
			{
				s_Data.Map[bombPos->y][bombPos->x] = EMPTY;
				s_Data.BombExplosions[i].RemainTime = 0;
				s_Data.BombExplosions[i].IsEnable = false;
			}
			s_Data.BombExplosions[i].RemainTime--;
		}
	}
}


char MapToChar(int type)
{
	switch (type)
	{
	case EMPTY:
		return EMPTY_ICON;
	case WALL:
		return WALL_ICON;
	case BOMB_1:
		return BOMB_ICON_1;
	case BOMB_2:
		return BOMB_ICON_2;
	case BOMB_3:
		return BOMB_ICON_3;
	case EXPLOSION_EFFECT:
		return EXPLOSION_EFFECT_ICON;
	case PLAYER1:
		return PLAYER1_ICON;
	case PLAYER2:
		return PLAYER2_ICON;
	case DESTROYABLE_WALL:
		return DESTROYABLE_WALL_ICON;
	}
}

void DrawMap(char* result, int height)
{
	strcat(result, "|                  ");
	for (size_t i = 0; i < MAP_WIDTH; i++)
	{
		char tempArray[2] = { MapToChar(s_Data.Map[height][i]), '\0' };
		strcat(result, tempArray);
	}
	strcat(result, "                   |\n");
}

void Battle_Draw()
{
	PlayerInputProcess();
	BombProcess();

	char result[SCREEN_BUFFER_SIZE] = "";
	int len = 0;
	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.RemainingTime.StartLine)
		{
			strcat(result, s_Data.RemainingTime.Text);
			char time[20];
			sprintf(time, "%d", s_Data.RemainTime);
			strcat(result, time);
			int len = s_Data.MaxSceneWidth - (strlen(s_Data.RemainingTime.Text) + strlen(time));
			for (size_t i = 2; i < len; i++)
			{
				strcat(result, " ");
			}
			strcat(result, s_Data.LineEnd);
		}
		else if (i == s_Data.PlayerScore[0].StartLine)
		{
			strcat(result, s_Data.PlayerScore[0].Text);
			char p1Score[20];
			sprintf(p1Score, "%d", s_Data.Player[0].Score);
			strcat(result, p1Score);

			strcat(result, s_Data.PlayerScore[1].Text);
			char p2Score[20];
			sprintf(p2Score, "%d", s_Data.Player[1].Score);
			strcat(result, p2Score);

			int len = s_Data.MaxSceneWidth - (strlen(s_Data.PlayerScore[0].Text) + strlen(s_Data.PlayerScore[1].Text)
				+ strlen(p1Score) + strlen(p2Score));
			for (size_t i = 2; i < len; i++)
			{
				strcat(result, " ");
			}
			strcat(result, s_Data.LineEnd);
		}
		else if (i >= MAP_START_LINE && i <= MAP_END_LINE)
		{
			DrawMap(result, len);
			len++;
		}
		else
			strcat(result, CANVAS_LINE);
	}

	Renderer_UpdateData(result);

	s_Data.CurrentFrameTime += Game_GetFramTime();
	if (s_Data.CurrentFrameTime >= 1000)
	{
		s_Data.CurrentFrameTime = 0;
		s_Data.RemainTime--;

		for (int i = 0; i < MAX_BOMB_COUNT; i++)
		{
			if (s_Data.Bombs[i].IsEnable)
			{
				s_Data.Bombs[i].RemainTime--;
			}
		}

		if (s_Data.RemainTime == 0)
		{
			char result[SCREEN_BUFFER_SIZE] = "";

			for (int i = 0; i < CANVAS_MAX_LINE; i++)
			{
				if (i == 0 || i == CANVAS_MAX_LINE - 1)
					strcat(result, CANVAS_BORDER_LINE);
				else if (i == s_Data.GameOver[0].StartLine)
				{
					strcat(result, s_Data.GameOver[0].Text);
				}
				else if (i == s_Data.GameOver[1].StartLine)
				{
					strcat(result, s_Data.GameOver[1].Text);
					char score[20];
					sprintf(score, "%d", s_Data.Player[0].Score);
					strcat(result, score);

					int len = s_Data.MaxSceneWidth - (strlen(s_Data.GameOver[1].Text) + strlen(score));
					for (size_t i = 2; i < len; i++)
					{
						strcat(result, " ");
					}
					strcat(result, s_Data.LineEnd);

				}
				else if (i == s_Data.GameOver[2].StartLine)
				{
					strcat(result, s_Data.GameOver[2].Text);
					char score[20];
					sprintf(score, "%d", s_Data.Player[1].Score);
					strcat(result, score);

					int len = s_Data.MaxSceneWidth - (strlen(s_Data.GameOver[2].Text) + strlen(score));
					for (size_t i = 2; i < len; i++)
					{
						strcat(result, " ");
					}
					strcat(result, s_Data.LineEnd);
				}
				else
					strcat(result, CANVAS_LINE);
			}
			strcat(result, "-->");
			Renderer_UpdateData(result);
			Renderer_Draw();  // このフレームをすぐにレンダリングする

			(void)_getch();

			LoadingPage_Init();
			LoadingPage_SetDeparture(GameState::GAME_STATE_MENU_SCENE);
			Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);
			TitleMenu_SetMenuState(TitleMenuState::TITLE_IDLE);
			Game_RsetFramTime();
		}

	}
}