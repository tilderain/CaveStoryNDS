// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "NpcAct.h"

#include <stddef.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Flash.h"
#include "Frame.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Sound.h"
#include "Triangle.h"

// Toroko
void ActNpc060(NPCHAR *npc)
{
	RECT rcLeft[8] = {
		{0, 64, 16, 80},
		{16, 64, 32, 80},
		{32, 64, 48, 80},
		{16, 64, 32, 80},
		{48, 64, 64, 80},
		{16, 64, 32, 80},
		{112, 64, 128, 80},
		{128, 64, 144, 80},
	};

	RECT rcRight[8] = {
		{0, 80, 16, 96},
		{16, 80, 32, 96},
		{32, 80, 48, 96},
		{16, 80, 32, 96},
		{48, 80, 64, 96},
		{16, 80, 32, 96},
		{112, 80, 128, 96},
		{128, 80, 144, 96},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (16 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (16 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (16 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (16 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				if (npc->x > GetNearestMyChar(npc->x, npc->y)->x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->flag & 1)
			{
				npc->direct = 2;
				npc->xm = 0x200;
			}

			if (npc->flag & 4)
			{
				npc->direct = 0;
				npc->xm = -0x200;
			}

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 6:
			npc->act_no = 7;
			npc->act_wait = 0;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 7:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 3;

			break;

		case 8:
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->act_no = 9;
			npc->ym = -0x200;
			// Fallthrough
		case 9:
			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 6;
			npc->ym = -0x400;
			PlaySoundObject(50, 1);

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			break;

		case 11:
			if (npc->act_wait++ != 0 && npc->flag & 8)
			{
				npc->act_no = 12;
				npc->ani_no = 7;
				npc->bits |= NPC_INTERACTABLE;
			}

			break;

		case 12:
			npc->xm = 0;
			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// King
void ActNpc061(NPCHAR *npc)
{
	int i;

	RECT rcLeft[11] = {
		{224, 32, 240, 48},
		{240, 32, 256, 48},
		{256, 32, 272, 48},
		{272, 32, 288, 48},
		{288, 32, 304, 48},
		{224, 32, 240, 48},
		{304, 32, 320, 48},
		{224, 32, 240, 48},
		{272, 32, 288, 48},
		{0, 0, 0, 0},
		{112, 32, 128, 48},
	};

	RECT rcRight[11] = {
		{224, 48, 240, 64},
		{240, 48, 256, 64},
		{256, 48, 272, 64},
		{272, 48, 288, 64},
		{288, 48, 304, 64},
		{224, 48, 240, 64},
		{304, 48, 320, 64},
		{224, 48, 240, 64},
		{272, 48, 288, 64},
		{0, 0, 0, 0},
		{112, 32, 128, 48},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 5:
			npc->ani_no = 3;
			npc->xm = 0;
			break;

		case 6:
			npc->act_no = 7;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 7:
			npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 5;

			break;

		case 8:
			npc->act_no = 9;
			npc->ani_no = 4;
			npc->ani_wait = 0;
			// Fallthrough
		case 9:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 4;
			npc->ani_wait = 0;
			// Fallthrough
		case 11:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 20:
			SetNpChar(145, 0, 0, 0, 0, 2, npc, 0x100);
			npc->ani_no = 0;
			npc->act_no = 0;
			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = 0;
			// Fallthrough
		case 31:
			npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x600;
			else
				npc->xm = 0x600;

			if (npc->flag & 1)
			{
				npc->direct = 2;
				npc->act_no = 7;
				npc->act_wait = 0;
				npc->ani_wait = 0;
				npc->ym = -0x400;
				npc->xm = 0x200;
				PlaySoundObject(71, 1);
				SetDestroyNpChar(npc->x, npc->y, 0x800, 4);
			}

			break;

		case 40:
			npc->act_no = 42;
			npc->act_wait = 0;
			npc->ani_no = 8;
			PlaySoundObject(29, 1);
			// Fallthrough
		case 42:
			if (++npc->ani_no > 9)
				npc->ani_no = 8;

			if (++npc->act_wait > 100)
			{
				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

				npc->act_no = 50;
				npc->surf = SURFACE_ID_NPC_SYM;
				npc->ani_no = 10;
			}

			break;

		case 60:
			npc->ani_no = 6;
			npc->act_no = 61;
			npc->ym = -0x5FF;
			npc->xm = 0x400;
			npc->count2 = 1;
			break;

		case 61:
			npc->ym += 0x40;

			if (npc->flag & 8)
			{
				npc->act_no = 0;
				npc->count2 = 0;
				npc->xm = 0;
			}

			break;
	}

	if (npc->act_no < 30 || npc->act_no >= 40)
	{
		npc->ym += 0x40;

		if (npc->xm > 0x400)
			npc->xm = 0x400;
		if (npc->xm < -0x400)
			npc->xm = -0x400;

		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Kazuma at computer
void ActNpc062(NPCHAR *npc)
{
	RECT rcLeft[3] = {
		{272, 192, 288, 216},
		{288, 192, 304, 216},
		{304, 192, 320, 216},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->x -= 4 * 0x200;
			npc->y += 16 * 0x200;
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (Random(0, 80) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (Random(0, 120) == 10)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 40)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 3:
			if (++npc->act_wait > 80)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	npc->rect = rcLeft[npc->ani_no];
}

// Toroko with stick
void ActNpc063(NPCHAR *npc)
{
	RECT rcLeft[6] = {
		{64, 64, 80, 80},
		{80, 64, 96, 80},
		{64, 64, 80, 80},
		{96, 64, 112, 80},
		{112, 64, 128, 80},
		{128, 64, 144, 80},
	};

	RECT rcRight[6] = {
		{64, 80, 80, 96},
		{80, 80, 96, 96},
		{64, 80, 80, 96},
		{96, 80, 112, 96},
		{112, 80, 128, 96},
		{128, 80, 144, 96},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 1:
			if (npc->ym > 0)
				npc->bits &= ~NPC_IGNORE_SOLIDITY;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 0;

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 2;

			break;

		case 2:
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 3:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 0;

			if (++npc->act_wait > 50)
			{
				npc->act_wait = 40;
				npc->xm *= -1;

				if (npc->direct == 0)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			if (npc->act_wait > 35)
				npc->bits |= NPC_SHOOTABLE;

			if (npc->direct == 0)
				npc->xm -= 0x40;
			else
				npc->xm += 0x40;

			if (npc->shock)
			{
				npc->act_no = 4;
				npc->ani_no = 4;
				npc->ym = -0x400;
				npc->bits &= ~NPC_SHOOTABLE;
				npc->damage = 0;
			}

			break;

		case 4:
			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
			{
				npc->act_no = 5;
				npc->bits |= NPC_INTERACTABLE;
			}

			break;

		case 5:
			npc->xm = 0;
			npc->ani_no = 5;
			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// First Cave Critter
void ActNpc064(NPCHAR *npc)
{
	RECT rcLeft[3] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{32, 0, 48, 16},
	};

	RECT rcRight[3] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{32, 16, 48, 32},
	};

	switch (npc->act_no)
	{
		case 0: // Initialize
			npc->y += 3 * 0x200;
			npc->act_no = 1;
			// Fallthrough
		case 1: // Waiting
			// Look at player
			if (npc->x > GetNearestMyChar(npc->x, npc->y)->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_x < 100)
				++npc->tgt_x;

			// Open eyes near player
			if (npc->act_wait >= 8 && npc->x - (112 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (112 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (80 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (80 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				npc->ani_no = 1;
			}
			else
			{
				if (npc->act_wait < 8)
					++npc->act_wait;

				npc->ani_no = 0;
			}

			// Jump if attacked
			if (npc->shock)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			// Jump if player is nearby
			if (npc->act_wait >= 8 && npc->tgt_x >= 100 && npc->x - (64 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (64 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (80 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (48 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}
			break;

		case 2: // Going to jump
			if (++npc->act_wait > 8)
			{
				// Set jump state
				npc->act_no = 3;
				npc->ani_no = 2;

				// Jump
				npc->ym = -0x5FF;
				PlaySoundObject(30, 1);

				// Jump in facing direction
				if (npc->direct == 0)
					npc->xm = -0x100;
				else
					npc->xm = 0x100;
			}
			break;

		case 3: // Jumping
			// Land
			if (npc->flag & 8)
			{
				npc->xm = 0;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->act_no = 1;
				PlaySoundObject(23, 1);
			}
			break;
	}

	// Gravity
	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	// Move
	npc->x += npc->xm;
	npc->y += npc->ym;

	// Set framerect
	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// First Cave Bat
void ActNpc065(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = 120;
			npc->act_no = 1;
			npc->act_wait = Random(0, 50);
			// Fallthrough

		case 1:
			if (++npc->act_wait < 50)
				break;

			npc->act_wait = 0;
			npc->act_no = 2;
			npc->ym = 0x300;

			break;

		case 2:
			if (GetNearestMyChar(npc->x, npc->y)->x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x300)
				npc->ym = 0x300;
			if (npc->ym < -0x300)
				npc->ym = -0x300;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rect_left[4] = {
		{32, 32, 48, 48},
		{48, 32, 64, 48},
		{64, 32, 80, 48},
		{80, 32, 96, 48},
	};

	RECT rect_right[4] = {
		{32, 48, 48, 64},
		{48, 48, 64, 64},
		{64, 48, 80, 64},
		{80, 48, 96, 64},
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Misery bubble
void ActNpc066(NPCHAR *npc)
{
	RECT rect[4] = {
		{32, 192, 56, 216},
		{56, 192, 80, 216},
		{32, 216, 56, 240},
		{56, 216, 80, 240},
	};

	switch (npc->act_no)
	{
		case 0:
			int a;
			for (a = 0; a < 0x200; ++a)
				if (gNPC[a]->code_event == 1000)
					break;

			if (a == 0x200)
				break;

			npc->tgt_x = gNPC[a]->x;
			npc->tgt_y = gNPC[a]->y;
			npc->count1 = a;

			unsigned char deg;
			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			npc->xm = GetCos(deg) * 2;
			npc->ym = GetSin(deg) * 2;

			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->x - (3 * 0x200) < npc->tgt_x && npc->x + (3 * 0x200) > npc->tgt_x && npc->y - (3 * 0x200) < npc->tgt_y && npc->y + (3 * 0x200) > npc->tgt_y)
			{
				npc->act_no = 2;
				npc->ani_no = 2;
				gNPC[npc->count1]->cond = 0;
				PlaySoundObject(21, 1);
			}

			break;

		case 2:
			npc->xm -= 0x20;
			npc->ym -= 0x20;

			if (npc->xm < -0x5FF)
				npc->xm = -0x5FF;
			if (npc->ym < -0x5FF)
				npc->ym = -0x5FF;

			if (npc->y < -8 * 0x200)
				npc->cond = 0;

			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}

// Misery (floating)
void ActNpc067(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->ani_no = 0;
			PlaySoundObject(29, 1);
			// Fallthrough
		case 1:
			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);

			if (++npc->act_wait == 0x20)
				npc->act_no = 10;

			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ym = 0x200;
			// Fallthrough

		case 11:
			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			break;

		case 13:
			npc->ani_no = 1;

			npc->ym += 0x40;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			if (npc->flag & 8)
			{
				PlaySoundObject(23, 1);
				npc->ym = 0;
				npc->act_no = 14;
				npc->bits |= NPC_IGNORE_SOLIDITY;
				npc->ani_no = 2;
			}

			break;

		case 15:
			npc->act_no = 16;
			npc->act_wait = 0;
			npc->ani_no = 4;
			// Fallthrough
		case 16:
			if (++npc->act_wait == 30)
			{
				PlaySoundObject(21, 1);
				SetNpChar(66, npc->x, npc->y - (16 * 0x200), 0, 0, 0, npc, 0);
			}

			if (npc->act_wait == 50)
				npc->act_no = 14;

			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 0;
			npc->ym = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			npc->ym -= 0x20;

			if (npc->y < -8 * 0x200)
				npc->cond = 0;

			break;

		case 25:
			npc->act_no = 26;
			npc->act_wait = 0;
			npc->ani_no = 5;
			npc->ani_wait = 0;
			// Fallthrough
		case 26:
			if (++npc->ani_no > 7)
				npc->ani_no = 5;

			if (++npc->act_wait == 30)
			{
				PlaySoundObject(101, 1);
				SetFlash(0, 0, 2);
				npc->act_no = 27;
				npc->ani_no = 7;
			}

			break;

		case 27:
			if (++npc->act_wait == 50)
				npc->act_no = 14;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rcLeft[8] = {
		{80, 0, 96, 16},
		{96, 0, 112, 16},
		{112, 0, 128, 16},
		{128, 0, 144, 16},
		{144, 0, 160, 16},
		{160, 0, 176, 16},
		{176, 0, 192, 16},
		{144, 0, 160, 16},
	};

	RECT rcRight[8] = {
		{80, 16, 96, 32},
		{96, 16, 112, 32},
		{112, 16, 128, 32},
		{128, 16, 144, 32},
		{144, 16, 160, 32},
		{160, 16, 176, 32},
		{176, 16, 192, 32},
		{144, 16, 160, 32},
	};

	if (npc->act_no == 11)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 1;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 0;
		}
	}

	if (npc->act_no == 14)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 3;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 2;
		}
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1 && npc->ani_wait < 32)
		npc->rect.bottom = (++npc->ani_wait / 2) + npc->rect.bottom - 16;
}

// Balrog (running)
void ActNpc068(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->act_wait = 30;

			if (npc->x > GetNearestMyChar(npc->x, npc->y)->x)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 1:
			if (--npc->act_wait)
				break;

			npc->act_no = 2;
			++npc->count1;

			break;

		case 2:
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 3:
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;

				if (++npc->ani_no == 2 || npc->ani_no == 4)
					PlaySoundObject(23, 1);
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->xm -= 0x10;
			else
				npc->xm += 0x10;

			if (npc->act_wait >= 8 && npc->x - (12 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (12 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (12 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (8 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				npc->act_no = 10;
				npc->ani_no = 5;
				GetNearestMyChar(npc->x, npc->y)->cond |= 2;
				int mc = GetNearestMyCharNo(npc->x, npc->y);
				if(mc == 1)
					SwapMyChar();
				
				DamageMyChar(2);
				if(mc == 1)
					SwapMyChar();
				break;
			}

			++npc->act_wait;

			if (npc->flag & 5 || npc->act_wait > 75)
			{
				npc->act_no = 9;
				npc->ani_no = 0;
				break;
			}

			if ((npc->count1 % 3) == 0 && npc->act_wait > 25)
			{
				npc->act_no = 4;
				npc->ani_no = 7;
				npc->ym = -0x400;
				break;
			}

			break;

		case 4:
			if (npc->flag & 8)
			{
				npc->act_no = 9;
				npc->ani_no = 8;
				SetQuake(30);
				PlaySoundObject(26, 1);
			}

			if (npc->act_wait >= 8 && npc->x - (12 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (12 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (12 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (8 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				npc->act_no = 10;
				npc->ani_no = 5;
				GetNearestMyChar(npc->x, npc->y)->cond |= 2;
				int mc = GetNearestMyCharNo(npc->x, npc->y);
				if(mc == 1)
					SwapMyChar();				
				DamageMyChar(2);
				if(mc == 1)
					SwapMyChar();						
			}

			break;

		case 9:
			npc->xm = (npc->xm * 4) / 5;

			if (npc->xm != 0)
				break;

			npc->act_no = 0;

			break;

		case 10:
			GetNearestMyChar(npc->x, npc->y)->x = npc->x;
			GetNearestMyChar(npc->x, npc->y)->y = npc->y;

			npc->xm = (npc->xm * 4) / 5;

			if (npc->xm != 0)
				break;

			npc->act_no = 11;
			npc->act_wait = 0;
			npc->ani_no = 5;
			npc->ani_wait = 0;

			break;

		case 11:
			GetNearestMyChar(npc->x, npc->y)->x = npc->x;
			GetNearestMyChar(npc->x, npc->y)->y = npc->y;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 5;

			if (++npc->act_wait > 100)
				npc->act_no = 20;

			break;

		case 20:
			PlaySoundObject(25, 1);
			GetNearestMyChar(npc->x, npc->y)->cond &= ~2;

			if (npc->direct == 0)
			{
				GetNearestMyChar(npc->x, npc->y)->x += 4 * 0x200;
				GetNearestMyChar(npc->x, npc->y)->y -= 8 * 0x200;
				GetNearestMyChar(npc->x, npc->y)->xm = 0x5FF;
				GetNearestMyChar(npc->x, npc->y)->ym = -0x200;
				GetNearestMyChar(npc->x, npc->y)->direct = 2;
				npc->direct = 2;
			}
			else
			{
				GetNearestMyChar(npc->x, npc->y)->x -= 4 * 0x200;
				GetNearestMyChar(npc->x, npc->y)->y -= 8 * 0x200;
				GetNearestMyChar(npc->x, npc->y)->xm = -0x5FF;
				GetNearestMyChar(npc->x, npc->y)->ym = -0x200;
				GetNearestMyChar(npc->x, npc->y)->direct = 0;
				npc->direct = 0;
			}

			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 7;
			// Fallthrough
		case 21:
			if (++npc->act_wait < 50)
				break;

			npc->act_no = 0;
			break;
	}

	npc->ym += 0x20;

	if (npc->xm < -0x400)
		npc->xm = -0x400;
	if (npc->xm > 0x400)
		npc->xm = 0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rect_left[9] = {
		{0, 0, 40, 24},
		{0, 48, 40, 72},
		{0, 0, 40, 24},
		{40, 48, 80, 72},
		{0, 0, 40, 24},
		{80, 48, 120, 72},
		{120, 48, 160, 72},
		{120, 0, 160, 24},
		{80, 0, 120, 24},
	};

	RECT rect_right[9] = {
		{0, 24, 40, 48},
		{0, 72, 40, 96},
		{0, 24, 40, 48},
		{40, 72, 80, 96},
		{0, 24, 40, 48},
		{80, 72, 120, 96},
		{120, 72, 160, 96},
		{120, 24, 160, 48},
		{80, 24, 120, 48},
	};

	if (npc->direct == 0)
		npc->rect = rect_left[npc->ani_no];
	else
		npc->rect = rect_right[npc->ani_no];
}

// Pignon
void ActNpc069(NPCHAR *npc)
{
	RECT rcLeft[6] = {
		{48, 0, 64, 16},
		{64, 0, 80, 16},
		{80, 0, 96, 16},
		{96, 0, 112, 16},
		{48, 0, 64, 16},
		{112, 0, 128, 16},
	};

	RECT rcRight[6] = {
		{48, 16, 64, 32},
		{64, 16, 80, 32},
		{80, 16, 96, 32},
		{96, 16, 112, 32},
		{48, 16, 64, 32},
		{112, 16, 128, 32},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 100) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
				break;
			}

			if (Random(0, 150) == 1)
			{
				if (npc->direct == 0)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			if (Random(0, 150) == 1)
			{
				npc->act_no = 3;
				npc->act_wait = 50;
				npc->ani_no = 0;
				break;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (--npc->act_wait == 0)
				npc->act_no = 0;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 2;

			if (npc->flag & 1)
			{
				npc->direct = 2;
				npc->xm = 0x200;
			}

			if (npc->flag & 4)
			{
				npc->direct = 0;
				npc->xm = -0x200;
			}

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			break;

		case 5:
			if (npc->flag & 8)
				npc->act_no = 0;

			break;
	}

	switch (npc->act_no)
	{
		case 1:
		case 2:
		case 4:
			if (npc->shock)
			{
				npc->ym = -0x200;
				npc->ani_no = 5;
				npc->act_no = 5;
			}

			break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Sparkle
void ActNpc070(NPCHAR *npc)
{
	RECT rect[4] = {
		{96, 48, 112, 64},
		{112, 48, 128, 64},
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	if (++npc->ani_wait > 3)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];
}

// Chinfish
void ActNpc071(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->ym = 0x80;
			// Fallthrough
		case 1:
			if (npc->tgt_y < npc->y)
				npc->ym -= 8;
			if (npc->tgt_y > npc->y)
				npc->ym += 8;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	RECT rcLeft[3] = {
		{64, 32, 80, 48},
		{80, 32, 96, 48},
		{96, 32, 112, 48},
	};

	RECT rcRight[3] = {
		{64, 48, 80, 64},
		{80, 48, 96, 64},
		{96, 48, 112, 64},
	};

	if (++npc->ani_wait > 4)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	if (npc->shock)
		npc->ani_no = 2;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Sprinkler
void ActNpc072(NPCHAR *npc)
{
	if (npc->direct == 0)
	{
		if (++npc->ani_wait > 1)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 1)
		{
			npc->ani_no = 0;
			return;
		}

		if (GetNearestMyChar(npc->x, npc->y)->x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && GetNearestMyChar(npc->x, npc->y)->x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && GetNearestMyChar(npc->x, npc->y)->y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200) && GetNearestMyChar(npc->x, npc->y)->y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200))
		{
			if (++npc->act_no % 2)
				SetNpChar(73, npc->x, npc->y, Random(-0x200, 0x200) * 2, Random(-0x200, 0x80) * 3, 0, NULL, 0x100);

			SetNpChar(73, npc->x, npc->y, Random(-0x200, 0x200) * 2, Random(-0x200, 0x80) * 3, 0, NULL, 0x100);
		}
	}

	RECT rect[2] = {
		{224, 48, 240, 64},
		{240, 48, 256, 64},
	};

	npc->rect = rect[npc->ani_no];
}

// Water droplet
void ActNpc073(NPCHAR *npc)
{
	RECT rect[5] = {
		{72, 16, 74, 18},
		{74, 16, 76, 18},
		{76, 16, 78, 18},
		{78, 16, 80, 18},
		{80, 16, 82, 18},
	};

	npc->ym += 0x20;
	npc->ani_no = Random(0, 4);

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

	if (npc->direct == 2)
	{
		npc->rect.top += 2;
		npc->rect.bottom += 2;
	}

	if (++npc->act_wait > 10)
	{
		if (npc->flag & 1)
			npc->cond = 0;
		if (npc->flag & 4)
			npc->cond = 0;
		if (npc->flag & 8)
			npc->cond = 0;
		if (npc->flag & 0x100)
			npc->cond = 0;
	}

	if (npc->y > gMap.length * 0x200 * 0x10)
		npc->cond = 0;
}

// Jack
void ActNpc074(NPCHAR *npc)
{
	RECT rcLeft[6] = {
		{64, 0, 80, 16},
		{80, 0, 96, 16},
		{96, 0, 112, 16},
		{64, 0, 80, 16},
		{112, 0, 128, 16},
		{64, 0, 80, 16},
	};

	RECT rcRight[6] = {
		{64, 16, 80, 32},
		{80, 16, 96, 32},
		{96, 16, 112, 32},
		{64, 16, 80, 32},
		{112, 16, 128, 32},
		{64, 16, 80, 32},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 8:
			npc->act_no = 9;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 9:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Kanpachi (fishing)
void ActNpc075(NPCHAR *npc)
{
	RECT rcLeft[2] = {
		{272, 32, 296, 56},
		{296, 32, 320, 56},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (npc->x - (48 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (48 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (48 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (16 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
				npc->ani_no = 1;
			else
				npc->ani_no = 0;

			break;
	}

	npc->rect = rcLeft[npc->ani_no];
}

// Flowers
void ActNpc076(NPCHAR *npc)
{
	npc->rect.left = npc->code_event * 16;
	npc->rect.top = 0;
	npc->rect.right = npc->rect.left + 16;
	npc->rect.bottom = 16;
}

// Yamashita
void ActNpc077(NPCHAR *npc)
{
	RECT rc[3] = {
		{0, 16, 48, 48},
		{48, 16, 96, 48},
		{96, 16, 144, 48},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rc[npc->ani_no];
	else
		npc->rect = rc[2];
}

// Pot
void ActNpc078(NPCHAR *npc)
{
	RECT rc[2] = {
		{160, 48, 176, 64},
		{176, 48, 192, 64},
	};

	if (npc->direct == 0)
		npc->rect = rc[0];
	else
		npc->rect = rc[1];
}

// Mahin
void ActNpc079(NPCHAR *npc)
{
	RECT rcLeft[3] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{32, 0, 48, 16},
	};

	RECT rcRight[3] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{32, 16, 48, 32},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			break;

		case 2:
			npc->ani_no = 0;

			if (Random(0, 120) == 10)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (32 * 0x200) < GetNearestMyChar(npc->x, npc->y)->x && npc->x + (32 * 0x200) > GetNearestMyChar(npc->x, npc->y)->x && npc->y - (32 * 0x200) < GetNearestMyChar(npc->x, npc->y)->y && npc->y + (16 * 0x200) > GetNearestMyChar(npc->x, npc->y)->y)
			{
				if (npc->x > GetNearestMyChar(npc->x, npc->y)->x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 3:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
			}

			break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}
