//--------------------------
//--------------------------
//  Cmd Text Adventure Game
//  Created by Yu Jiang 
//      2023.05.20
//--------------------------
//--------------------------
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS

int main()
{
#pragma region Global Variable Declaration
	char g_PlayerName[256];
	int g_PlayerNameLen = 0;

	srand((unsigned)time(nullptr));

	int g_SceneMaxWidth = 80;
	int g_SceneMaxHeight = 25;
	int g_LoadFrame = 0;

#pragma region Battle Setting
	int g_ThreatAccumulative = 10;
	int g_ThreatAccumulativeWhenCritical = 30;
	int g_MaxSkillPoint = 5;
	int g_SkillPointAccumulative = 2;
	int g_SkillPointAccumulativeWhenCritical = 3;
	int g_MaxShield = 50;
	int g_HpAccumulativeWhenPassLevel = 100;
#pragma endregion

#pragma region Player Status
	int g_PlayerMaxHp = 200;
	int g_PlayerMaxDamage = 40;
	int g_PlayerMinDamage = 10;
	int g_PlayerCriticalDamageMultiplier = 2;
	int g_PlayerSpeedStep = 3;
	int g_PlayerSkillMultiplier = 2;
	int g_PlayerSkillMaxTurn = 4;
	int g_PlayerSkillRequest = 5;
	int g_PlayerMaxSpeedStep = g_PlayerSpeedStep * g_PlayerSkillMultiplier;
	int g_PlayerThreat = 30;
	int g_PlayerDefense = 10;
#pragma endregion

#pragma region Player Members Status
	int g_TankMaxHp = 300;
	int g_TankMaxDamage = 20;
	int g_TankMinDamage = 10;
	int g_TankSpeedStep = 2;
	int g_TankTauntSkillThreat = 50;
	int g_TankTauntSkillShield = 30;
	int g_TankSkillRequest = 5;
	int g_TankThreat = 80;
	int g_TankDefense = 20;

	int g_HealerMaxHp = 100;
	int g_HealerMaxDamage = 15;
	int g_HealerMinDamage = 10;
	int g_HealerSpeedStep = 1;
	int g_HealerHealSkill = 100;
	int g_HealerHealSkillDeathMember = 10;
	int g_HealerSkillRequest = 5;
	int g_HealerThreat = 50;
	int g_HealerDefense = 5;
#pragma endregion

#pragma region Enemy Status
	int g_EnemyMaxHp = 300;
	int g_EnemyRangeSkillDamage = 50;
	int g_EnemyMaxDamage = 50;
	int g_EnemyMinDamage = 30;
	int g_EnemySpeedStep = 4;
	int g_EnemyRangeAttackTurn = 10;
#pragma endregion

#pragma endregion

#pragma region Title Scene
	int t_TitleFrame = 0;
	while (t_TitleFrame < 6)
	{
		printf("|-----------------------------------------------------------------------------|\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                  by                                         |\n");
		switch (t_TitleFrame)
		{
		case 0:
		{
			printf("|                           Freestyle Studio                                  |\n");
			break;
		}
		case 1:
		{
			printf("|                               Free Stud                                     |\n");
			break;
		}
		case 2:
		{
			printf("|                                Fre Stu                                      |\n");
			break;
		}
		case 3:
		{
			printf("|                                 Fr St                                       |\n");
			break;
		}
		case 4:
		{
			printf("|                                 F  S                                        |\n");
			break;
		}
		case 5:
		{
			printf("|                                  FS                                         |\n");
			break;
		}
		}
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|-----------------------------------------------------------------------------|\n");

		Sleep(500);
		system("cls");
		t_TitleFrame++;
	}

#pragma region Loading Scene
	system("cls");
	g_LoadFrame = 0;
	while (g_LoadFrame < 3)
	{
		printf("|-----------------------------------------------------------------------------|\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|              Tips: 下に（＜）があるときは入力できます                       |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		switch (g_LoadFrame)
		{
		case 0:
		{
			printf("|                                                                    ■□□      |\n");
			break;
		}
		case 1:
		{
			printf("|                                                                    ■■□      |\n");
			break;
		}
		case 2:
		{
			printf("|                                                                    ■■■      |\n");
			break;
		}
		}
		printf("|                                                                             |\n");
		printf("|-----------------------------------------------------------------------------|\n");

		Sleep(500);
		system("cls");
		g_LoadFrame++;
	}
#pragma endregion

	printf("|-----------------------------------------------------------------------------|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                           あなたの名前を                                    |\n");
	printf("|                           入力して下さい                                    |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|-----------------------------------------------------------------------------|\n>");
	scanf("%s", &g_PlayerName[0]);
	while (g_PlayerName[g_PlayerNameLen] != '\0')
	{
		g_PlayerNameLen++;
	}

#pragma region Loading Scene
	system("cls");
	g_LoadFrame = 0;
	while (g_LoadFrame < 3)
	{
		printf("|-----------------------------------------------------------------------------|\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		switch (g_LoadFrame)
		{
		case 0:
		{
			printf("|                                                                    ■□□      |\n");
			break;
		}
		case 1:
		{
			printf("|                                                                    ■■□      |\n");
			break;
		}
		case 2:
		{
			printf("|                                                                    ■■■      |\n");
			break;
		}
		}
		printf("|                                                                             |\n");
		printf("|-----------------------------------------------------------------------------|\n");
		Sleep(500);
		system("cls");
		g_LoadFrame++;
	}
#pragma endregion

	printf("|-----------------------------------------------------------------------------|\n");
	printf("|                                                                             |\n");
	printf("|        { { @ } }                                                            |\n");
	printf("|           8@8                                                               |\n");
	printf("|           888                                                               |\n");
	printf("|           8@8                                                               |\n");
	printf("|           )8(                                                               |\n");
	printf("|     (@)__/8@8\\__(@)                        1.Start                          |\n");
	printf("|    `~ ==-=):(=- ==~`                                                        |\n");
	printf("|           |.|                              2.Credits                        |\n");
	printf("|           |S|                                                               |\n");
	printf("|           |'|                              3.Exit                           |\n");
	printf("|           |.|                                                               |\n");
	printf("|           |P|                                                               |\n");
	printf("|           |'|                                                               |\n");
	printf("|           |.|                                                               |\n");
	printf("|           |U|                                                               |\n");
	printf("|           \\ /                                                               |\n");
	printf("|            ^                                                                |\n");
	printf("|  The Legend of ");
	printf("%s", &g_PlayerName[0]);
	for (size_t i = 17 + g_PlayerNameLen; i < g_SceneMaxWidth - 2; i++)
	{
		printf("%c", ' ');
	}
	printf("%s", "|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                FS 2023                                      |\n");
	printf("|-----------------------------------------------------------------------------|\n>");

	int t_Selected = 0;
	scanf("%d", &t_Selected);
	while (t_Selected != 1 && t_Selected != 2 && t_Selected != 3)
	{
		t_Selected = 0;
		printf("不明なタイプです。再入力してください\n>");
		scanf("%d", &t_Selected);
	}

	switch (t_Selected)
	{
	case 2:
	{
#pragma region Loading Scene
		system("cls");
		g_LoadFrame = 0;
		while (g_LoadFrame < 3)
		{
			printf("|-----------------------------------------------------------------------------|\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			switch (g_LoadFrame)
			{
			case 0:
			{
				printf("|                                                                    ■□□      |\n");
				break;
			}
			case 1:
			{
				printf("|                                                                    ■■□      |\n");
				break;
			}
			case 2:
			{
				printf("|                                                                    ■■■      |\n");
				break;
			}
			}
			printf("|                                                                             |\n");
			printf("|-----------------------------------------------------------------------------|\n");
			Sleep(500);
			system("cls");
			g_LoadFrame++;
		}
#pragma endregion

		int t_CreditsFrame = 0;
		int t_Credits1Pos = 21;
		int t_Credits2Pos = 21;
		int t_Credits3Pos = 21;
		int t_Credits4Pos = 21;
		int t_Credits5Pos = 21;
		int t_CreditsCount = 0;
		while (t_CreditsFrame < 32)
		{

			if (t_CreditsFrame == 0 || t_CreditsFrame == 31)
			{
				printf("|-----------------------------------------------------------------------------|\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|-----------------------------------------------------------------------------|\n");
			}
			else
			{
				switch (t_CreditsCount)
				{
				case 0:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits1Pos)
							printf("|                      Freestyle Studio                                       |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					t_Credits1Pos--;	
					if (t_Credits1Pos == 19)
						t_CreditsCount++;
					break;
				}
				case 1:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits1Pos)
							printf("|                      Freestyle Studio                                       |\n");
						else if (i == t_Credits2Pos)
							printf("|                     Program, Design, ...etc                                 |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");						
					t_Credits1Pos--;
					t_Credits2Pos--;	
					if (t_Credits2Pos == 19)
						t_CreditsCount++;
					break;
				}
				case 2:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits1Pos)
							printf("|                      Freestyle Studio                                       |\n");
						else if (i == t_Credits2Pos)
							printf("|                     Program, Design, ...etc                                 |\n");
						else if (i == t_Credits3Pos)
							printf("|                             YU JIANG                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					t_Credits1Pos--;
					t_Credits2Pos--;
					t_Credits3Pos--;
					if (t_Credits3Pos == 19)
						t_CreditsCount++;
					break;
				}
				case 3:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits1Pos)
							printf("|                      Freestyle Studio                                       |\n");
						else if (i == t_Credits2Pos)
							printf("|                     Program, Design, ...etc                                 |\n");
						else if (i == t_Credits3Pos)
							printf("|                             YU JIANG                                        |\n");
						else if (i == t_Credits4Pos)
							printf("|                        Special Thanks                                       |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					t_Credits1Pos--;
					t_Credits2Pos--;
					t_Credits3Pos--;
					t_Credits4Pos--;
					if (t_Credits4Pos == 19)
						t_CreditsCount++;
					break;
				}
				case 4:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits1Pos)
							printf("|                      Freestyle Studio                                       |\n");
						else if (i == t_Credits2Pos)
							printf("|                     Program, Design, ...etc                                 |\n");
						else if (i == t_Credits3Pos)
							printf("|                             YU JIANG                                        |\n");
						else if (i == t_Credits4Pos)
							printf("|                        Special Thanks                                       |\n");
						else if (i == t_Credits5Pos)
							printf("|                            HAL Tokyo                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					if (t_Credits1Pos == 0)
						t_CreditsCount++;
					t_Credits1Pos--;
					t_Credits2Pos--;
					t_Credits3Pos--;
					t_Credits4Pos--;
					t_Credits5Pos--;
					break;
				}
				case 5:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits2Pos)
							printf("|                     Program, Design, ...etc                                 |\n");
						else if (i == t_Credits3Pos)
							printf("|                             YU JIANG                                        |\n");
						else if (i == t_Credits4Pos)
							printf("|                        Special Thanks                                       |\n");
						else if (i == t_Credits5Pos)
							printf("|                            HAL Tokyo                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					if (t_Credits2Pos == 0)
						t_CreditsCount++;
					t_Credits2Pos--;
					t_Credits3Pos--;
					t_Credits4Pos--;
					t_Credits5Pos--;
					break;
				}
				case 6:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits3Pos)
							printf("|                             YU JIANG                                        |\n");
						else if (i == t_Credits4Pos)
							printf("|                        Special Thanks                                       |\n");
						else if (i == t_Credits5Pos)
							printf("|                            HAL Tokyo                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					if (t_Credits3Pos == 0)
						t_CreditsCount++;
					t_Credits3Pos--;
					t_Credits4Pos--;
					t_Credits5Pos--;
					break;
				}
				case 7:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits4Pos)
							printf("|                        Special Thanks                                       |\n");
						else if (i == t_Credits5Pos)
							printf("|                            HAL Tokyo                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					if (t_Credits4Pos == 0)
						t_CreditsCount++;
					t_Credits4Pos--;
					t_Credits5Pos--;			
					break;
				}
				case 8:
				{
					printf("|-----------------------------------------------------------------------------|\n");
					for (size_t i = 0; i < 22; i++)
					{
						if (i == t_Credits5Pos)
							printf("|                            HAL Tokyo                                        |\n");
						else
							printf("|                                                                             |\n");
					}
					printf("|-----------------------------------------------------------------------------|\n");
					t_Credits5Pos--;
					break;
				}
				}
			}
			Sleep(500);
			system("cls");
			t_CreditsFrame++;
		}
		exit(-1);
		break;
	}
	case 3:
	{
		exit(-1);
		break;
	}
	}
	system("cls");
#pragma endregion

#pragma region Loading Scene
	system("cls");
	g_LoadFrame = 0;
	while (g_LoadFrame < 3)
	{
		printf("|-----------------------------------------------------------------------------|\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		switch (g_LoadFrame)
		{
		case 0:
		{
			printf("|                                                                    ■□□      |\n");
			break;
		}
		case 1:
		{
			printf("|                                                                    ■■□      |\n");
			break;
		}
		case 2:
		{
			printf("|                                                                    ■■■      |\n");
			break;
		}
		}
		printf("|                                                                             |\n");
		printf("|-----------------------------------------------------------------------------|\n");
		Sleep(500);
		system("cls");
		g_LoadFrame++;
	}
#pragma endregion

#pragma region Game Scene

#pragma region Dialogue1
	printf("|-----------------------------------------------------------------------------|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|   =======================================================================   |\n");
	printf("|   =                                                                     =   |\n");
	printf("%s", "|   =  ");
	printf("%s", &g_PlayerName[0]);
	printf("%s", "と仲間たちは、");
	for (size_t i = 22 + g_PlayerNameLen; i < g_SceneMaxWidth - 5; i++)
	{
		printf("%c", ' ');
	}
	printf("%s", "=   |\n");
	printf("|   =  伝説の秘宝が眠るとされる塔に挑戦する決意を固めます。               =   |\n");
	printf("|   =                                                                     =   |\n");
	printf("|   =======================================================================   |\n");
	printf("|                                                                             |\n");
	printf("|-----------------------------------------------------------------------------|\n>");
	rewind(stdin);
	(void)_getch();
#pragma endregion

#pragma region Battle 1
	char t_BattleHpIcon = '>';
	char t_PlayerSpeedIcon = 'P';
	char t_EnemySpeedIcon = 'E';
	char t_TankSpeedIcon = 'T';
	char t_HealerSpeedIcon = 'H';
	int t_Level = 1;
	bool t_EntryScene = true;
	bool t_Running = true;
	int t_TurnCount = 0;
	int t_PlayerSpeed = 0;
	int t_EnemySpeed = 0;
	int t_TankSpeed = 0;
	int t_HealerSpeed = 0;
	int t_CurrentPlayerHp = g_PlayerMaxHp;
	int t_CurrentTankHp = g_TankMaxHp;
	int t_CurrentHealerHp = g_HealerMaxHp;
	int t_CurrentEnemyHp = g_EnemyMaxHp;
	int t_CurrentPlayerSkillPoint = 0;
	int t_CurrentHealerSkillPoint = 0;
	int t_CurrentTankSkillPoint = 0;
	int t_CurrentPlayerThreat = g_PlayerThreat;
	int t_CurrentHealerThreat = g_HealerThreat;
	int t_CurrentTankThreat = g_TankThreat;
	bool t_IsPlayerSpeedUp = false;
	int t_PlayerCurrentSpeedStep = g_PlayerSpeedStep;
	int t_PlayerSpeedUpRemaining = g_PlayerSkillMaxTurn;
	int t_CurrentPlayerShield = 0;
	int t_CurrentTankShield = 0;
	int t_CurrentHealerShield = 0;
	bool t_IsPlayerDead = false;
	bool t_IsTankDead = false;
	bool t_IsHealerDead = false;
	int t_TipsRandomMsg = rand() % 4;
	while (t_Running)
	{
		if (t_EntryScene)
		{
#pragma region Loading Scene
			system("cls");
			g_LoadFrame = 0;
			while (g_LoadFrame < 3)
			{
				printf("|-----------------------------------------------------------------------------|\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				switch (t_TipsRandomMsg)
				{
				case 0:
				{
					printf("|     Tips: スキルを発動するため、五つのスキルポイント（*）が必要です     　　|\n");
					break;
				}
				case 1:
				{
					printf("|              Tips: タンクは20％の被害軽減を持っています             　　　　|\n");
					break;
				}
				case 2:
				{
					printf("|              Tips: ヒーラーのスビートは最も遅いです                 　　　　|\n");
					break;
				}
				case 3:
				{
					printf("|              Tips: 毎10ターン、確定50の全体攻撃があります       　　　      |\n");
					break;
				}
				}
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				printf("|                                                                             |\n");
				switch (g_LoadFrame)
				{
				case 0:
				{
					printf("|                                                                    ■□□      |\n");
					break;
				}
				case 1:
				{
					printf("|                                                                    ■■□      |\n");
					break;
				}
				case 2:
				{
					printf("|                                                                    ■■■      |\n");
					break;
				}
				}
				printf("|                                                                             |\n");
				printf("|-----------------------------------------------------------------------------|\n");

				Sleep(500);
				system("cls");
				g_LoadFrame++;
			}
#pragma endregion

			printf("|-----------------------------------------------------------------------------|\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|             ==================================================              |\n");
			printf("|             =                                                =              |\n");
			printf("|             =                   ");
			printf("%03d層", t_Level);
			for (size_t i = 40; i < g_SceneMaxWidth - 20; i++)
			{
				printf("%c", ' ');
			}
			printf("    =              |\n");
			printf("|             =                                                =              |\n");
			printf("|             ==================================================              |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|                                                                             |\n");
			printf("|-----------------------------------------------------------------------------|\n>");
			rewind(stdin);
			(void)_getch();
			system("cls");
			t_EntryScene = false;
			continue;
		}

		if (t_IsPlayerSpeedUp)
		{
			if (t_PlayerCurrentSpeedStep < g_PlayerMaxSpeedStep)
				t_PlayerCurrentSpeedStep *= g_PlayerSkillMultiplier;
			t_PlayerSpeedUpRemaining--;
		}

		if (t_PlayerSpeedUpRemaining == 0)
		{
			t_PlayerCurrentSpeedStep = g_PlayerSpeedStep;
			t_IsPlayerSpeedUp = false;
		}

		if (!t_IsPlayerDead)
			t_PlayerSpeed += t_PlayerCurrentSpeedStep;

		if (!t_IsTankDead)
			t_TankSpeed += g_TankSpeedStep;

		if (!t_IsHealerDead)
			t_HealerSpeed += g_HealerSpeedStep;

		t_EnemySpeed += g_EnemySpeedStep;

		printf("|-----------------------------------------------------------------------------|\n");
		printf("|                                                                             |\n");
		printf("|        |");
		for (size_t i = 0; i < 10; i++)
		{
			if (t_EnemySpeed < 10 && i == t_EnemySpeed - 1)
				printf("%c", t_EnemySpeedIcon);
			else if (t_PlayerSpeed < 10 && i == t_PlayerSpeed - 1)
				printf("%c", t_PlayerSpeedIcon);
			else if (t_TankSpeed < 10 && i == t_TankSpeed - 1)
				printf("%c", t_TankSpeedIcon);
			else if (t_HealerSpeed < 10 && i == t_HealerSpeed - 1)
				printf("%c", t_HealerSpeedIcon);
			else
				printf("%c", '-');
		}
		printf("|                                                         |\n");
		printf("|                                                                             |\n");
		printf("|                                                                             |\n");
		printf("|        |");
		int t_CurrentEnemyHpPercentage = (int)((float)t_CurrentEnemyHp / (float)g_EnemyMaxHp * 10.0f + 0.5f);
		for (size_t i = 0; i < 10; i++)
		{
			if (i < t_CurrentEnemyHpPercentage)
				printf("%c", t_BattleHpIcon);
			else
				printf("%c", '-');
		}
		printf("%c", '|');
		printf("%c", ' ');
		printf("%03d/%03d", t_CurrentEnemyHp, g_EnemyMaxHp);
		printf("                                                 |\n");
		printf("|          aad888baa                                                          |\n");
		printf("|        d:?88888?::8b                      P  ");
		for (size_t i = 0; i < g_MaxSkillPoint; i++)
		{
			if (i < t_CurrentPlayerSkillPoint)
				printf("%c", '*');
			else
				printf("%c", ' ');
		}
		printf("%c", ' ');
		printf("%c", '|');
		int t_CurrentPlayerHpPercentage = (int)((float)t_CurrentPlayerHp / (float)g_PlayerMaxHp * 10.0f + 0.5f);
		for (size_t i = 0; i < 10; i++)
		{
			if (i < t_CurrentPlayerHpPercentage)
				printf("%c", t_BattleHpIcon);
			else
				printf("%c", '-');
		}
		printf("%c", '|');
		printf("%c", ' ');
		printf("%03d/%03d", t_CurrentPlayerHp, g_PlayerMaxHp);
		printf("     |\n");
		printf("|      d888:?8888??a888b                    ");
		if (t_IsPlayerSpeedUp)
			printf("突(%d)", t_PlayerSpeedUpRemaining);
		else
			printf("突(N)");
		printf("%c", ' ');
		printf("盾(%02d)", t_CurrentPlayerShield);
		printf("                      |\n");
		printf("|     dP:::888888888:::Yb                                                     |\n");
		printf("|    dP::::Y8888888P::::Yb                  T  ");
		for (size_t i = 0; i < g_MaxSkillPoint; i++)
		{
			if (i < t_CurrentTankSkillPoint)
				printf("%c", '*');
			else
				printf("%c", ' ');
		}
		printf("%c", ' ');
		printf("%c", '|');
		int t_CurrentTankHpPercentage = (int)((float)t_CurrentTankHp / (float)g_TankMaxHp * 10.0f + 0.5f);
		for (size_t i = 0; i < 10; i++)
		{
			if (i < t_CurrentTankHpPercentage)
				printf("%c", t_BattleHpIcon);
			else
				printf("%c", '-');
		}
		printf("%c", '|');
		printf("%c", ' ');
		printf("%03d/%03d", t_CurrentTankHp, g_TankMaxHp);
		printf("     |\n");
		printf("|   d8:::::YP8:T:88b::::::Yb                ");
		printf("盾(%02d)", t_CurrentTankShield);
		printf("                            |\n");
		printf("|  Y88888888P::|::Y88888888P                                                  |\n");
		printf("|       8888:::|:::8888                     H  ");
		for (size_t i = 0; i < g_MaxSkillPoint; i++)
		{
			if (i < t_CurrentHealerSkillPoint)
				printf("%c", '*');
			else
				printf("%c", ' ');
		}
		printf("%c", ' ');
		printf("%c", '|');
		int t_CurrentHealerHpPercentage = (int)((float)t_CurrentHealerHp / (float)g_HealerMaxHp * 10.0f + 0.5f);
		for (size_t i = 0; i < 10; i++)
		{
			if (i < t_CurrentHealerHpPercentage)
				printf("%c", t_BattleHpIcon);
			else
				printf("%c", '-');
		}
		printf("%c", '|');
		printf("%c", ' ');
		printf("%03d/%03d", t_CurrentHealerHp, g_HealerMaxHp);
		printf("     |\n");
		printf("|       888888888888888                     ");
		printf("盾(%02d)", t_CurrentHealerShield);
		printf("                            |\n");
		printf("|       88::88::88::888                                                       |\n");
		printf("|       88::88::P:::888                                                       |\n");
		printf("|       88::88::::::888                                                       |\n");
		printf("|                                                                             |\n");
		printf("|-----------------------------------------------------------------------------|\n");

		printf("Turn:%d,Threat:(P:%d,T:%d,H:%d)\n", t_TurnCount, t_CurrentPlayerThreat, t_CurrentTankThreat, t_CurrentHealerThreat);

		if (t_TurnCount % g_EnemyRangeAttackTurn == 0 && t_TurnCount != 0)
		{
			int t_EnemyDamage = g_EnemyRangeSkillDamage;
			if (t_CurrentPlayerShield == 0)
			{
				float damageReduction = g_PlayerDefense / (100.0f + g_PlayerDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentPlayerHp -= t_EnemyDamage;
			}
			else if (t_EnemyDamage > t_CurrentPlayerShield)
			{
				float damageReduction = g_PlayerDefense / (100.0f + g_PlayerDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentPlayerHp -= (t_EnemyDamage - t_CurrentPlayerShield);
				t_CurrentPlayerShield = 0;
			}
			else
				t_CurrentPlayerShield -= t_EnemyDamage;

			if (t_CurrentTankShield == 0)
			{
				float damageReduction = g_TankDefense / (100.0f + g_TankDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentTankHp -= t_EnemyDamage;
			}
			else if (t_EnemyDamage > t_CurrentTankShield)
			{
				float damageReduction = g_TankDefense / (100.0f + g_TankDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentTankHp -= (t_EnemyDamage - t_CurrentTankShield);
				t_CurrentTankShield = 0;
			}
			else
				t_CurrentTankShield -= t_EnemyDamage;

			if (t_CurrentHealerShield == 0)
			{
				float damageReduction = g_HealerDefense / (100.0f + g_HealerDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentHealerHp -= t_EnemyDamage;
			}
			else if (t_EnemyDamage > t_CurrentHealerShield)
			{
				float damageReduction = g_HealerDefense / (100.0f + g_HealerDefense);
				t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
				t_CurrentHealerHp -= (t_EnemyDamage - t_CurrentHealerShield);
				t_CurrentHealerShield = 0;
			}
			else
				t_CurrentHealerShield -= t_EnemyDamage;
			printf("エネミーがプレイヤー全体に %dのダメージを与えた\n>", t_EnemyDamage);
			rewind(stdin);
			(void)_getch();
			printf("\n");
		}

		if (t_EnemySpeed > 10)
		{
			t_EnemySpeed %= 10;
			int t_EnemyDamage = rand() % (g_EnemyMaxDamage - g_EnemyMinDamage);
			t_EnemyDamage += g_EnemyMinDamage;
			if ((t_CurrentPlayerThreat > t_CurrentHealerThreat) && (t_CurrentPlayerThreat > t_CurrentTankThreat))
			{
				if (t_CurrentPlayerShield == 0)
				{
					float damageReduction = g_PlayerDefense / (100.0f + g_PlayerDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentPlayerHp -= t_EnemyDamage;
				}
				else if (t_EnemyDamage > t_CurrentPlayerShield)
				{
					float damageReduction = g_PlayerDefense / (100.0f + g_PlayerDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentPlayerHp -= (t_EnemyDamage - t_CurrentPlayerShield);
					t_CurrentPlayerShield = 0;
				}
				else
					t_CurrentPlayerShield -= t_EnemyDamage;
				printf("エネミーがプレイヤーに %dのダメージを与えた\n>", t_EnemyDamage);
			}
			else if ((t_CurrentTankThreat > t_CurrentHealerThreat) && (t_CurrentTankThreat > t_CurrentPlayerThreat))
			{
				if (t_CurrentTankShield == 0)
				{
					float damageReduction = g_TankDefense / (100.0f + g_TankDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentTankHp -= t_EnemyDamage;
				}
				else if (t_EnemyDamage > t_CurrentTankShield)
				{
					float damageReduction = g_TankDefense / (100.0f + g_TankDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentTankHp -= (t_EnemyDamage - t_CurrentTankShield);
					t_CurrentTankShield = 0;
				}
				else
					t_CurrentTankShield -= t_EnemyDamage;
				printf("エネミーがタンクに %dのダメージを与えた\n>", t_EnemyDamage);
			}
			else if ((t_CurrentHealerThreat > t_CurrentTankThreat) && (t_CurrentHealerThreat > t_CurrentTankThreat))
			{
				if (t_CurrentHealerShield == 0)
				{
					float damageReduction = g_HealerDefense / (100.0f + g_HealerDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentHealerHp -= t_EnemyDamage;
				}
				else if (t_EnemyDamage > t_CurrentHealerShield)
				{
					float damageReduction = g_HealerDefense / (100.0f + g_HealerDefense);
					t_EnemyDamage = t_EnemyDamage * (1.0f - damageReduction);
					t_CurrentHealerHp -= (t_EnemyDamage - t_CurrentHealerShield);
					t_CurrentHealerShield = 0;
				}
				else
					t_CurrentHealerShield -= t_EnemyDamage;
				printf("エネミーがヒーラーに %dのダメージを与えた\n>", t_EnemyDamage);
			}
			else
			{
				int t_Target = rand() % 3;
				switch (t_Target)
				{
				case 0:
				{
					if (t_CurrentPlayerShield == 0)
						t_CurrentPlayerHp -= t_EnemyDamage;
					else if (t_EnemyDamage > t_CurrentPlayerShield)
					{
						t_CurrentPlayerHp -= (t_EnemyDamage - t_CurrentPlayerShield);
						t_CurrentPlayerShield = 0;
					}
					else
						t_CurrentPlayerShield -= t_EnemyDamage;
					printf("エネミーがプレイヤーに %dのダメージを与えた\n>", t_EnemyDamage);
					break;
				}
				case 1:
				{
					if (t_CurrentTankShield == 0)
						t_CurrentTankHp -= t_EnemyDamage;
					else if (t_EnemyDamage > t_CurrentTankShield)
					{
						t_CurrentTankHp -= (t_EnemyDamage - t_CurrentTankShield);
						t_CurrentTankShield = 0;
					}
					else
						t_CurrentTankShield -= t_EnemyDamage;
					printf("エネミーがタンクに %dのダメージを与えた\n>", t_EnemyDamage);
					break;
				}
				case 2:
				{
					if (t_CurrentHealerShield == 0)
						t_CurrentHealerHp -= t_EnemyDamage;
					else if (t_EnemyDamage > t_CurrentHealerShield)
					{
						t_CurrentHealerHp -= (t_EnemyDamage - t_CurrentHealerShield);
						t_CurrentHealerShield = 0;
					}
					else
						t_CurrentHealerShield -= t_EnemyDamage;
					printf("エネミーがヒーラーに %dのダメージを与えた\n>", t_EnemyDamage);
					break;
				}
				}
			}
			rewind(stdin);
			(void)_getch();
			printf("\n");
		}

		if (t_CurrentPlayerHp < 0)
			t_CurrentPlayerHp = 0;
		if (t_CurrentTankHp < 0)
			t_CurrentTankHp = 0;
		if (t_CurrentHealerHp < 0)
			t_CurrentHealerHp = 0;

		if (t_CurrentPlayerHp == 0)
		{
			t_IsPlayerDead = true;
			t_CurrentPlayerShield = 0;
			t_IsPlayerSpeedUp = false;
			t_PlayerCurrentSpeedStep = g_PlayerSpeedStep;
			t_CurrentPlayerSkillPoint = 0;
			t_CurrentPlayerThreat = 0;
			t_PlayerSpeed = 0;
		}

		if (t_CurrentTankHp == 0)
		{
			t_IsTankDead = true;
			t_CurrentTankShield = 0;
			t_CurrentTankSkillPoint = 0;
			t_CurrentTankThreat = 0;
			t_TankSpeed = 0;
		}

		if (t_CurrentHealerHp == 0)
		{
			t_IsHealerDead = true;
			t_CurrentHealerShield = 0;
			t_CurrentHealerSkillPoint = 0;
			t_CurrentHealerThreat = 0;
			t_HealerSpeed = 0;
		}

		if (t_CurrentPlayerHp == 0 && t_CurrentTankHp == 0 && t_CurrentHealerHp == 0)
		{
			printf("全滅、リトライしますか？　１：Y　２：N\n>");
			int t_Selected = 0;
			scanf("%d", &t_Selected);
			while (t_Selected != 1 && t_Selected != 2)
			{
				t_Selected = 0;
				printf("不明なタイプです。再入力してください\n>");
				scanf("%d", &t_Selected);
			}
			if (t_Selected == 1)
			{
				t_CurrentPlayerSkillPoint = 0;
				t_CurrentHealerSkillPoint = 0;
				t_CurrentTankSkillPoint = 0;
				t_CurrentPlayerThreat = g_PlayerThreat;
				t_CurrentHealerThreat = g_HealerThreat;
				t_CurrentTankThreat = g_TankThreat;
				t_EntryScene = true;
				t_CurrentEnemyHp = g_EnemyMaxHp;
				t_CurrentPlayerHp = g_PlayerMaxHp;
				t_CurrentTankHp = g_TankMaxHp;
				t_CurrentHealerHp = g_HealerMaxHp;
				t_PlayerSpeed = 0;
				t_EnemySpeed = 0;
				t_TankSpeed = 0;
				t_HealerSpeed = 0;
				t_TurnCount = 0;
			}
			else
			{
				t_Running = false;
				break;
			}
		}

		if (t_PlayerSpeed > 10 && !t_IsPlayerDead)
		{
			t_PlayerSpeed %= 10;
			printf("プレイヤー：１．攻撃　２．猪突猛進（4ターンスピードアップ）　３．逃げる\n");
			printf(">");
			int t_Selected = 0;
			scanf("%d", &t_Selected);
			while ((t_Selected != 1 && t_Selected != 2 && t_Selected != 3) || t_Selected == 3
				|| (t_CurrentPlayerSkillPoint < g_PlayerSkillRequest && t_Selected == 2) || (t_IsPlayerSpeedUp && t_Selected == 2))
			{
				if (t_Selected == 3)
					printf("この戦いは逃げられません！\n>");
				else if (t_Selected == 2)
				{
					if (t_IsPlayerSpeedUp)
						printf("すでに発動しているようです\n>");
					else if (t_CurrentPlayerSkillPoint < g_PlayerSkillRequest)
						printf("スキルポイントは足りないようです\n>");
				}
				else
					printf("不明なタイプです。再入力してください\n>");

				t_Selected = 0;
				scanf("%d", &t_Selected);
			}
			switch (t_Selected)
			{
			case 1:
			{
				int t_PlayerDamage = rand() % (g_PlayerMaxDamage - g_PlayerMinDamage);

				if (t_PlayerDamage != 0)
				{
					if (rand() % 2)
					{
						t_PlayerDamage = (g_PlayerMinDamage + t_PlayerDamage) * g_PlayerCriticalDamageMultiplier;
						t_CurrentEnemyHp = t_CurrentEnemyHp - t_PlayerDamage;
						printf("プレイヤーが %dの会心の一撃 を与えた\n>", t_PlayerDamage);

						if (t_CurrentPlayerSkillPoint < g_MaxSkillPoint)
							t_CurrentPlayerSkillPoint += g_SkillPointAccumulativeWhenCritical;

						t_CurrentPlayerThreat += g_ThreatAccumulativeWhenCritical;
					}
					else
					{
						t_PlayerDamage = g_PlayerMinDamage + t_PlayerDamage;
						t_CurrentEnemyHp = t_CurrentEnemyHp - t_PlayerDamage;
						printf("プレイヤーが %dのダメージ を与えた\n>", t_PlayerDamage);

						if (t_CurrentPlayerSkillPoint < g_MaxSkillPoint)
							t_CurrentPlayerSkillPoint += g_SkillPointAccumulative;

						t_CurrentPlayerThreat += g_ThreatAccumulative;
					}
				}
				else
				{
					if (rand() % 2)
						printf("プレイヤーの攻撃は外れた\n>");
					else
						printf("エネミーが攻撃を避けた\n>");
				}
				break;
			}
			case 2:
			{
				t_CurrentPlayerSkillPoint -= g_PlayerSkillRequest;
				t_IsPlayerSpeedUp = true;
				t_PlayerSpeedUpRemaining = g_PlayerSkillMaxTurn;
				printf("豪勇なる者よ、我が力に乗り込め！\n>");
				break;
			}
			}
			rewind(stdin);
			(void)_getch();
			printf("\n");
		}

		if (t_TankSpeed > 10 && !t_IsTankDead)
		{
			t_TankSpeed %= 10;
			printf("タンク：１．攻撃　２．堅如磐石（全体シールド+30、自身のヘイト+50）　３．逃げる\n");
			printf(">");
			int t_Selected = 0;
			scanf("%d", &t_Selected);
			while ((t_Selected != 1 && t_Selected != 2 && t_Selected != 3) || t_Selected == 3
				|| (t_CurrentTankSkillPoint < g_TankSkillRequest && t_Selected == 2))
			{
				if (t_Selected == 3)
					printf("この戦いは逃げられません！\n>");
				else if (t_Selected == 2)
					printf("スキルポイントは足りないようです\n>");
				else
					printf("不明なタイプです。再入力してください\n>");

				t_Selected = 0;
				scanf("%d", &t_Selected);
			}
			switch (t_Selected)
			{
			case 1:
			{
				int t_TankDamage = rand() % (g_TankMaxDamage - g_TankMinDamage);
				if (t_TankDamage != 0)
				{
					t_TankDamage = g_TankMinDamage + t_TankDamage;
					t_CurrentEnemyHp = t_CurrentEnemyHp - t_TankDamage;
					printf("タンクが %dのダメージ を与えた\n>", t_TankDamage);

					if (t_CurrentTankSkillPoint < g_MaxSkillPoint)
						t_CurrentTankSkillPoint += g_SkillPointAccumulative;

					t_CurrentTankThreat += g_ThreatAccumulative;
				}
				else
				{
					if (rand() % 2)
						printf("タンクの攻撃は外れた\n>");
					else
						printf("エネミーが攻撃を避けた\n>");
				}
				break;
			}
			case 2:
			{
				t_CurrentTankSkillPoint -= g_TankSkillRequest;
				t_CurrentPlayerShield += g_TankTauntSkillShield;
				t_CurrentTankShield += g_TankTauntSkillShield;
				t_CurrentHealerShield += g_TankTauntSkillShield;
				t_CurrentTankThreat += g_TankTauntSkillThreat;
				printf("われは岩のように,決して揺るがない!\n>");
				break;
			}
			}
			rewind(stdin);
			(void)_getch();
			printf("\n");
		}

		if (t_HealerSpeed > 10 && !t_IsHealerDead)
		{
			t_HealerSpeed %= 10;
			printf("ヒーラー：１．攻撃　２．聖術回生（全体HP+100、死んだメンバーが復活します（10HP））　３．逃げる\n");
			printf(">");
			int t_Selected = 0;
			scanf("%d", &t_Selected);
			while ((t_Selected != 1 && t_Selected != 2 && t_Selected != 3) || t_Selected == 3
				|| (t_CurrentHealerSkillPoint < g_HealerSkillRequest && t_Selected == 2))
			{
				if (t_Selected == 3)
					printf("この戦いは逃げられません！\n>");
				else if (t_Selected == 2)
					printf("スキルポイントは足りないようです\n>");
				else
					printf("不明なタイプです。再入力してください\n>");

				t_Selected = 0;
				scanf("%d", &t_Selected);
			}
			switch (t_Selected)
			{
			case 1:
			{
				int t_HealerDamage = rand() % (g_HealerMaxDamage - g_HealerMinDamage);
				if (t_HealerDamage != 0)
				{
					t_HealerDamage = g_HealerMinDamage + t_HealerDamage;
					t_CurrentEnemyHp = t_CurrentEnemyHp - t_HealerDamage;
					printf("ヒーラーが %dのダメージ を与えた\n>", t_HealerDamage);

					if (t_CurrentHealerSkillPoint < g_MaxSkillPoint)
						t_CurrentHealerSkillPoint += g_SkillPointAccumulative;

					t_CurrentHealerThreat += g_ThreatAccumulative;
				}
				else
				{
					if (rand() % 2)
						printf("ヒーラーの攻撃は外れた\n>");
					else
						printf("エネミーが攻撃を避けた\n>");
				}
				break;
			}
			case 2:
			{
				t_CurrentHealerSkillPoint -= g_HealerSkillRequest;

				if (t_IsPlayerDead)
					t_CurrentPlayerHp = g_HealerHealSkillDeathMember;
				else
					t_CurrentPlayerHp += g_HealerHealSkill;

				if (t_IsTankDead)
					t_CurrentTankHp = g_HealerHealSkillDeathMember;
				else
					t_CurrentTankHp += g_HealerHealSkill;

				printf("神の力で死者を目覚めさせます！\n>");
				break;
			}
			}
			rewind(stdin);
			(void)_getch();
			printf("\n");
		}

		if (t_CurrentEnemyHp < 0)
			t_CurrentEnemyHp = 0;

		if (t_CurrentPlayerHp > g_PlayerMaxHp)
			t_CurrentPlayerHp = g_PlayerMaxHp;
		if (t_CurrentTankHp > g_TankMaxHp)
			t_CurrentTankHp = g_TankMaxHp;
		if (t_CurrentHealerHp > g_HealerMaxHp)
			t_CurrentHealerHp = g_HealerMaxHp;

		if (t_CurrentEnemyHp == 0)
		{
			printf("エネミーは倒された\n");
			printf("次の層に挑戦しますか？　１：Y　２：N\n>");
			int t_Selected = 0;
			scanf("%d", &t_Selected);
			while (t_Selected != 1 && t_Selected != 2)
			{
				t_Selected = 0;
				printf("不明なタイプです。再入力してください\n>");
				scanf("%d", &t_Selected);
			}
			if (t_Selected == 1)
			{
				t_CurrentPlayerSkillPoint = 0;
				t_CurrentHealerSkillPoint = 0;
				t_CurrentTankSkillPoint = 0;
				t_CurrentPlayerThreat = g_PlayerThreat;
				t_CurrentHealerThreat = g_HealerThreat;
				t_CurrentTankThreat = g_TankThreat;
				t_CurrentPlayerHp += g_HpAccumulativeWhenPassLevel;
				t_CurrentTankHp += g_HpAccumulativeWhenPassLevel;
				t_CurrentHealerHp += g_HpAccumulativeWhenPassLevel;
				t_PlayerSpeed = 0;
				t_EnemySpeed = 0;
				t_TankSpeed = 0;
				t_HealerSpeed = 0;
				t_TurnCount = 0;
				t_EntryScene = true;
				g_EnemyMaxHp += 10;
				t_CurrentEnemyHp = g_EnemyMaxHp;
				t_Level++;
			}
			else
			{
				t_Running = false;
				break;
			}
		}

		t_TurnCount++;
		Sleep(500);
		system("cls");
	}

	printf("|-----------------------------------------------------------------------------|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|             ==================================================              |\n");
	printf("|             =                                                =              |\n");
	printf("|             =              ");
	printf("最高記録：%03d層", t_Level);
	for (size_t i = 45; i < g_SceneMaxWidth - 20; i++)
	{
		printf("%c", ' ');
	}
	printf("    =              |\n");
	printf("|             =                                                =              |\n");
	printf("|             ==================================================              |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|-----------------------------------------------------------------------------|\n>");
	rewind(stdin);
	(void)_getch();
	system("cls");

	printf("|-----------------------------------------------------------------------------|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|             ==================================================              |\n");
	printf("|             =                                                =              |\n");
	printf("|             =            Thanks for Playing                  =              |\n");
	printf("|             =                                                =              |\n");
	printf("|             ==================================================              |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|-----------------------------------------------------------------------------|\n>");
	rewind(stdin);
	(void)_getch();
	system("cls");
#pragma endregion

#pragma endregion
}