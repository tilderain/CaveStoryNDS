// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"
#include "MyChar.h"

typedef struct BULLET
{
	int flag;
	int code_bullet;
	int bbits;
	int cond;
	int x;
	int y;
	int xm;
	int ym;
	int tgt_x;
	int tgt_y;
	int act_no;
	int act_wait;
	int ani_wait;
	int ani_no;
	unsigned char direct;
	RECT rect;
	int count1;
	int count2;
	int life_count;
	int damage;
	int life;
	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;
	struct
	{
		int front;
		int top;
		int back;
		int bottom;
	} view;
	char owner;
} BULLET;

typedef struct BULLET_TABLE
{
	signed char damage;
	signed char life;
	int life_count;
	int bbits;
	int enemyXL;
	int enemyYL;
	int blockXL;
	int blockYL;
	struct
	{
		int front;
		int top;
		int back;
		int bottom;
	} view;
} BULLET_TABLE;

#define BULLET_MAX 0x40
#define BULLET_MAX2P 0x80
extern BULLET gBul[BULLET_MAX2P];

void InitBullet(void);
int CountArmsBullet(int arms_code);
int CountBulletNum(int bullet_code);
void DeleteBullet(int code);
void ClearBullet(void);
void PutBullet(int fx, int fy);
void SetBullet(int no, int x, int y, int dir, signed char owner=gCurMyChar);
void ActBullet(void);

int GetMaxBullet(void);

BOOL IsActiveSomeBullet(void);
