// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "NpcHit.h"

#include "WindowsWrapper.h"

#include "Back.h"
#include "Bullet.h"
#include "Caret.h"
#include "Flags.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "TextScr.h"
#include "ValueView.h"

void JadgeHitNpCharBlock(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->y - npc->hit.top < (y * 0x10 + 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 5) * 0x200
		&& npc->x - npc->hit.back < (x * 0x10 + 8) * 0x200
		&& npc->x - npc->hit.back > x * 0x10 * 0x200)
	{
		npc->x = ((x * 0x10 + 8) * 0x200) + npc->hit.back;
		hit |= 1;
	}

	if (npc->y - npc->hit.top < (y * 0x10 + 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 8) * 0x200
		&& npc->x + npc->hit.back < x * 0x10 * 0x200)
	{
		npc->x = ((x * 0x10 - 8) * 0x200) - npc->hit.back;
		hit |= 4;
	}

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 5) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200
		&& npc->y - npc->hit.top > y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 + 8) * 0x200) + npc->hit.top;
		npc->ym = 0;
		hit |= 2;
	}

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom < y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 - 8) * 0x200) - npc->hit.bottom;
		npc->ym = 0;
		hit |= 8;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleA(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleB(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleC(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleD(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 + npc->hit.top;

		// Halt momentum
		if (npc->ym < 0)
			npc->ym = 0;

		// Set that hit a ceiling
		hit |= 2;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleE(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x10000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleF(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x20000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x >= (x * 0x10 - 8) * 0x200	// Note that this function uses '>='. I'm not sure if this is a bug.
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) + ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x28;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleG(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x40000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) + 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharTriangleH(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	hit |= 0x80000;

	if (npc->x < (x * 0x10 + 8) * 0x200
		&& npc->x > (x * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200)
	{
		// Clip
		npc->y = (y * 0x10 * 0x200) - ((npc->x - (x * 0x10 * 0x200)) / 2) - 0x800 - npc->hit.bottom;

		// Halt momentum
		if (npc->ym > 0)
			npc->ym = 0;

		// Set that hit this slope
		hit |= 0x18;
	}

	npc->flag |= hit;
}

void JudgeHitNpCharWater(NPCHAR *npc, int x, int y)
{
	int hit = 0;

	if (npc->x - npc->hit.back < (x * 0x10 + 6) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 6) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 + 6) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 6) * 0x200)
		hit |= 0x100;

	npc->flag |= hit;
}

__attribute__((hot))
void HitNpCharMap(void)
{
	int x, y;
	int judg;

	const int offx[] = {0, 1, 0, 1, 2, 2, 2, 0, 1};
	const int offy[] = {0, 0, 1, 1, 0, 1, 2, 2, 2};

	int i, j;

	for (i = 0; i < gActiveNPCCount; ++i)
	{

		if (gActiveNPC[i]->bits & NPC_IGNORE_SOLIDITY)
			continue;

		if (gActiveNPC[i]->size >= 3)
		{
			judg = 9;
			x = (gActiveNPC[i]->x - 0x1000) / 0x10 / 0x200;
			y = (gActiveNPC[i]->y - 0x1000) / 0x10 / 0x200;
		}
		else
		{
			judg = 4;
			x = gActiveNPC[i]->x / 0x10 / 0x200;
			y = gActiveNPC[i]->y / 0x10 / 0x200;
		}

		gActiveNPC[i]->flag = 0;

		for (j = 0; j < judg; ++j)
		{
			switch (GetAttribute(x + offx[j], y + offy[j]))
			{
				// No NPC block
				case 0x44:
					if (gActiveNPC[i]->bits & NPC_IGNORE_TILE_44)
						break;
					// Fallthrough
				// Block
				case 0x03:
				case 0x05:
				case 0x41:
				case 0x43:
					JadgeHitNpCharBlock(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				// Slopes
				case 0x50:
					JudgeHitNpCharTriangleA(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x51:
					JudgeHitNpCharTriangleB(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x52:
					JudgeHitNpCharTriangleC(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x53:
					JudgeHitNpCharTriangleD(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x54:
					JudgeHitNpCharTriangleE(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x55:
					JudgeHitNpCharTriangleF(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x56:
					JudgeHitNpCharTriangleG(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x57:
					JudgeHitNpCharTriangleH(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water
				case 0x02:
				case 0x60:
				case 0x62:
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water block
				case 0x04:
				case 0x61:
				case 0x64:
					JadgeHitNpCharBlock(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				// Water slopes
				case 0x70:
					JudgeHitNpCharTriangleA(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x71:
					JudgeHitNpCharTriangleB(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x72:
					JudgeHitNpCharTriangleC(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x73:
					JudgeHitNpCharTriangleD(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x74:
					JudgeHitNpCharTriangleE(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x75:
					JudgeHitNpCharTriangleF(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x76:
					JudgeHitNpCharTriangleG(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0x77:
					JudgeHitNpCharTriangleH(gActiveNPC[i], x + offx[j], y + offy[j]);
					JudgeHitNpCharWater(gActiveNPC[i], x + offx[j], y + offy[j]);
					break;

				case 0xA0:
					gActiveNPC[i]->flag |= 0x100;
					// Fallthrough
				case 0x80:
					gActiveNPC[i]->flag |= 0x1000;
					break;

				case 0xA1:
					gActiveNPC[i]->flag |= 0x100;
					// Fallthrough
				case 0x81:
					gActiveNPC[i]->flag |= 0x2000;
					break;

				case 0xA2:
					gActiveNPC[i]->flag |= 0x100;
					// Fallthrough
				case 0x82:
					gActiveNPC[i]->flag |= 0x4000;
					break;

				case 0xA3:
					gActiveNPC[i]->flag |= 0x100;
					// Fallthrough
				case 0x83:
					gActiveNPC[i]->flag |= 0x8000;
					break;
			}

			if (gActiveNPC[i]->y > gWaterY + 0x800)
				gActiveNPC[i]->flag |= 0x100;
		}
	}
}

void LoseNpChar(NPCHAR *npc, BOOL bVanish)
{
	int val;

	// Play death sound
	PlaySoundObject(npc->destroy_voice, 1);

	// Create smoke
	switch (npc->size)
	{
		case 1:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 3);
			break;

		case 2:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 7);
			break;

		case 3:
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 12);
			break;
	}

	// Create drop
	if (npc->exp != 0)
	{
		switch (Random(1, 5))
		{
			case 1:
				// Spawn health
				if (npc->exp > 6)
					val = 6;
				else
					val = 2;

				SetLifeObject(npc->x, npc->y, val);

				break;

			case 2:
				// Spawn missile launcher ammo
				if (npc->exp > 6)
					val = 3;
				else
					val = 1;

				if (SetBulletObject(npc->x, npc->y, val))
					break;

				// Fallthrough
			default:
				// Spawn weapon energy
				SetExpObjects(npc->x, npc->y, npc->exp);
				break;
		}
	}

	// Set flag
	SetNPCFlag(npc->code_flag);

	// Create value view
	if (npc->bits & NPC_SHOW_DAMAGE)
	{
		if ((npc->bits & NPC_SHOW_DAMAGE) && npc->damage_view)	// npc->bits & NPC_SHOW_DAMAGE is already verified at this point, so this is redundant
			SetValueView(&npc->x, &npc->y, npc->damage_view);
		if (bVanish)
			VanishNpChar(npc);
	}
	else
	{
		RemoveFromActiveNPCList(npc);
		npc->cond = 0;
	}
}

static BOOL DamageNpchar(int n, int b)
{
	// Damage NPC
	if (gActiveNPC[n]->bits & NPC_SHOOTABLE)
	{
		gActiveNPC[n]->life -= gBul[b].damage;

		if (gActiveNPC[n]->life < 1)
		{
			gActiveNPC[n]->life = 0;

			if (gActiveNPC[n]->bits & NPC_SHOW_DAMAGE)
				gActiveNPC[n]->damage_view -= gBul[b].damage;

			if ((gMC.cond & 0x80) && gActiveNPC[n]->bits & NPC_EVENT_WHEN_KILLED)
				StartTextScript(gActiveNPC[n]->code_event);
			else
			{
				//gActiveNPC[n]->cond |= 8; //wtf pixel
				LoseNpChar(gActiveNPC[n], TRUE);
				--gBul[b].life;
				return TRUE;
			}

		}
		else
		{
			if (gActiveNPC[n]->shock < 14)
			{
				SetCaret((gBul[b].x + gActiveNPC[n]->x) / 2, (gBul[b].y + gActiveNPC[n]->y) / 2, 11, 0);
				SetCaret((gBul[b].x + gActiveNPC[n]->x) / 2, (gBul[b].y + gActiveNPC[n]->y) / 2, 11, 0);
				SetCaret((gBul[b].x + gActiveNPC[n]->x) / 2, (gBul[b].y + gActiveNPC[n]->y) / 2, 11, 0);
				PlaySoundObject(gActiveNPC[n]->hit_voice, 1);
				gActiveNPC[n]->shock = 16;
			}

			if (gActiveNPC[n]->bits & NPC_SHOW_DAMAGE)
				gActiveNPC[n]->damage_view -= gBul[b].damage;
		}
	}
	else if (!(gBul[b].bbits & 0x10))
	{
		// Hit invulnerable NPC
		SetCaret((gBul[b].x + gActiveNPC[n]->x) / 2, (gBul[b].y + gActiveNPC[n]->y) / 2, 2, 2);
		PlaySoundObject(31, 1);
		gBul[b].life = 0;
		return FALSE;
	}

	--gBul[b].life;
	return FALSE;
}

__attribute__((hot))
void HitNpCharBullet(void)
{
	//int n, b;
	//BOOL bHit;
	for (int b = 0; b < BULLET_MAX; ++b)
	{
		if (!(gBul[b].cond & 0x80))
			continue;

		if (gBul[b].damage == -1)
			continue;

		for (int n = 0; n < gActiveNPCCount; ++n)
		{
			if (!(gActiveNPC[n]->cond & 0x80))
				continue;

			if (gActiveNPC[n]->bits & NPC_SHOOTABLE && gActiveNPC[n]->bits & NPC_INTERACTABLE)
				continue;
	
			// Check if bullet touches npc
			if (gActiveNPC[n]->bits & NPC_SHOOTABLE
				&& gActiveNPC[n]->x - gActiveNPC[n]->hit.back < gBul[b].x + gBul[b].enemyXL
				&& gActiveNPC[n]->x + gActiveNPC[n]->hit.back > gBul[b].x - gBul[b].enemyXL
				&& gActiveNPC[n]->y - gActiveNPC[n]->hit.top < gBul[b].y + gBul[b].enemyYL
				&& gActiveNPC[n]->y + gActiveNPC[n]->hit.bottom > gBul[b].y - gBul[b].enemyYL)
			{
				if(DamageNpchar(n, b))
					--n;
			}
			else if (gActiveNPC[n]->bits & NPC_INVULNERABLE
				&& gActiveNPC[n]->x - gActiveNPC[n]->hit.back < gBul[b].x + gBul[b].blockXL
				&& gActiveNPC[n]->x + gActiveNPC[n]->hit.back > gBul[b].x - gBul[b].blockXL
				&& gActiveNPC[n]->y - gActiveNPC[n]->hit.top < gBul[b].y + gBul[b].blockYL
				&& gActiveNPC[n]->y + gActiveNPC[n]->hit.bottom > gBul[b].y - gBul[b].blockYL)
			{
				if(DamageNpchar(n, b))
					--n;
			}
		}
	}
}
