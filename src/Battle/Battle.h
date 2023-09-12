#pragma once
#pragma once
#define TURN_TIME 120 // •b

#define BOMB_EXPLOSION_TIME 3 // •b
#define BOMB_EXPLOSION_EFFECT_FRAME 5 
#define MAX_BOMB_EXPLOSION_COUNT 512

#define MAX_BOMB_COUNT_PER_PLAYER 3
#define MAX_BOMB_COUNT 6

#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define MAP_START_LINE 2
#define MAP_END_LINE 22
#define MAP_START_COLUMN 20
#define MAP_END_COLUMN 60

#define MAX_PLAYER_COUNT 2

#define BATTLE_SCENE_FRAME_TIME 50 // ƒ~ƒŠ•b

enum MapType
{
	WALL = 0,
	EMPTY = 1,
	BOMB_1 = 2,
	BOMB_2 = 3,
	BOMB_3 = 4,
	PLAYER1 = 5,
	PLAYER2 = 6,
	EXPLOSION_EFFECT = 7,
	DESTROYABLE_WALL = 8,
};

#define EMPTY_ICON ' '
#define WALL_ICON '='
#define PLAYER1_ICON '@'
#define PLAYER2_ICON '#'
#define BOMB_ICON_1 '1'
#define BOMB_ICON_2 '2'
#define BOMB_ICON_3 '3'
#define DESTROYABLE_WALL_ICON '+'
#define EXPLOSION_EFFECT_ICON 'X'

void Battle_Init();
void Battle_Draw();