// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "MyChar.h"

#include <string.h>

#include "WindowsWrapper.h"

#include "ArmsItem.h"
#include "Caret.h"
#include "Debug.h"
#include "Draw.h"
#include "Flags.h"
#include "Game.h"
#include "KeyControl.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Sound.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

#include "Shoot.h"

#include "nifi.h"
#include "math.h"

#include "Multi.h"

#include "Frame.h"


MYCHAR gMCP1;
MYCHAR* gMC = &gMCP1;
MYCHAR gMCP2;

int gCurMyChar = 0;

int cache_x = 0;
int cache_y = 0;
int prev_ret = 0;

MYCHAR* GetNearestMyChar(int x, int y)
{
	if(!nifiIsLinkedInline()) return &gMCP1;

	if(x == cache_x && y == cache_y)
	{
		if(prev_ret == 0) return &gMCP1;
		return &gMCP2;
	}

	if(gMCP1.respawnTimer) return &gMCP2;
	if(gMCP2.respawnTimer) return &gMCP1;

	int p1 = abs(x - gMC->x) + abs(y - gMC->y);
	int p2 = abs(x - gMCP2.x) + abs(y - gMCP2.y);
	//printf("%d %d\n", p1, p2);
	cache_x = x; cache_y = y;
	if(p1 < p2)	{prev_ret = 0; return &gMCP1;};
	prev_ret = 1;
	return &gMCP2;
}

int GetNearestMyCharNo(int x, int y)
{
	if(!nifiIsLinked()) return 0;

	if(gMC->respawnTimer) return 1;
	if(gMCP2.respawnTimer) return 0;

	int p1 = abs(x - gMC->x) + abs(y - gMC->y);
	int p2 = abs(x - gMCP2.x) + abs(y - gMCP2.y);
	if(p1 < p2) return 0;
	return 1;
}

MYCHAR* GetMyCharNo(int no)
{
	if(!nifiIsLinked()) return &gMCP1;

	if (no == 0) return &gMCP1;
	return &gMCP2;
}

bool SwapMyChar(void)
{
	if(!nifiIsLinked()) {gCurMyChar = 0; return false;}
	//have mercy
	if(gCurMyChar == 0)
	{
		gSelectedArmsP1 = gSelectedArms;
		gKeyTrgP1 = gKeyTrg;
		gKeyP1 = gKey;

		gCurMyChar = 1;
		gMC = &gMCP2;
		gKeyTrg = gKeyTrgP2;
		gKey = gKeyP2;
		gSelectedArms = gSelectedArmsP2;
		gArmsData = gArmsDataP2;
	}
	else
	{
		gKeyTrgP2 = gKeyTrg;
		gKeyP2 = gKey;
		gSelectedArmsP2 = gSelectedArms;

		gCurMyChar = 0;
		gMC = &gMCP1;
		gKeyTrg = gKeyTrgP1;
		gKey = gKeyP1;
		gSelectedArms = gSelectedArmsP1;
		gArmsData = gArmsDataP1;
	}
	return true;
}

void InitMyChar(void)
{
	gMC = &gMCP1;

	memset(&gMCP1, 0, sizeof(MYCHAR));
	gMCP1.cond = 0x80;
	gMCP1.direct = 2;

	gMCP1.view.back = 8 * 0x200;
	gMCP1.view.top = 8 * 0x200;
	gMCP1.view.front = 8 * 0x200;
	gMCP1.view.bottom = 8 * 0x200;

	gMCP1.hit.back = 5 * 0x200;
	gMCP1.hit.top = 8 * 0x200;
	gMCP1.hit.front = 5 * 0x200;
	gMCP1.hit.bottom = 8 * 0x200;

	gMCP1.life = 3;
	gMCP1.max_life = 3;
	gMCP1.unit = 0;

	memset(&gMCP2, 0, sizeof(MYCHAR));
	gMCP2.cond = 0x80;
	gMCP2.direct = 2;

	gMCP2.view.back = 8 * 0x200;
	gMCP2.view.top = 8 * 0x200;
	gMCP2.view.front = 8 * 0x200;
	gMCP2.view.bottom = 8 * 0x200;

	gMCP2.hit.back = 5 * 0x200;
	gMCP2.hit.top = 8 * 0x200;
	gMCP2.hit.front = 5 * 0x200;
	gMCP2.hit.bottom = 8 * 0x200;

	gMCP2.life = 3;
	gMCP2.max_life = 3;
	gMCP2.unit = 0;
	
	gCurMyChar = 0;

	cache_x = 0;
	cache_y = 0;
	prev_ret = 0;

	gArmsData = gArmsDataP1;
}

void AnimationMyChar(BOOL bKey)
{
	RECT rcLeft[12] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{0, 0, 16, 16},
		{32, 0, 48, 16},
		{0, 0, 16, 16},
		{48, 0, 64, 16},
		{64, 0, 80, 16},
		{48, 0, 64, 16},
		{80, 0, 96, 16},
		{48, 0, 64, 16},
		{96, 0, 112, 16},
		{112, 0, 128, 16},
	};

	RECT rcRight[12] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{0, 16, 16, 32},
		{32, 16, 48, 32},
		{0, 16, 16, 32},
		{48, 16, 64, 32},
		{64, 16, 80, 32},
		{48, 16, 64, 32},
		{80, 16, 96, 32},
		{48, 16, 64, 32},
		{96, 16, 112, 32},
		{112, 16, 128, 32},
	};

	if (gMC->cond & 2)
		return;

	if (gMC->flag & 8)
	{
		if (gMC->cond & 1)
		{
			gMC->ani_no = 11;
		}
		else if (gKey & gKeyUp && gKey & (gKeyLeft | gKeyRight) && bKey)
		{
			gMC->cond |= 4;

			if (++gMC->ani_wait > 4)
			{
				gMC->ani_wait = 0;

				if (++gMC->ani_no == 7 || gMC->ani_no == 9)
					PlaySoundObject(24, 1);
			}

			if (gMC->ani_no > 9 || gMC->ani_no < 6)
				gMC->ani_no = 6;
		}
		else if (gKey & (gKeyLeft | gKeyRight) && bKey)
		{
			gMC->cond |= 4;

			if (++gMC->ani_wait > 4)
			{
				gMC->ani_wait = 0;

				if (++gMC->ani_no == 2 || gMC->ani_no == 4)
					PlaySoundObject(24, 1);
			}

			if (gMC->ani_no > 4 || gMC->ani_no < 1)
				gMC->ani_no = 1;
		}
		else if (gKey & gKeyUp && bKey)
		{
			if (gMC->cond & 4)
				PlaySoundObject(24, 1);

			gMC->cond &= ~4;
			gMC->ani_no = 5;
		}
		else
		{
			if (gMC->cond & 4)
				PlaySoundObject(24, 1);

			gMC->cond &= ~4;
			gMC->ani_no = 0;
		}
	}
	else if (gMC->up)
	{
		gMC->ani_no = 6;
	}
	else if (gMC->down)
	{
		gMC->ani_no = 10;
	}
	else
	{
		if (gMC->ym > 0)
			gMC->ani_no = 1;
		else
			gMC->ani_no = 3;
	}

	if (gMC->direct == 0)
		gMC->rect = rcLeft[gMC->ani_no];
	else
		gMC->rect = rcRight[gMC->ani_no];
}

void ShowMyChar(BOOL bShow)
{
	if (bShow)
		gMC->cond &= ~2;
	else
		gMC->cond |= 2;

	if(nifiIsLinked())
	{
		if (bShow)
			gMCP2.cond &= ~2;
		else
			gMCP2.cond |= 2;

	}
}

void PutMyChar(int fx, int fy)
{
	int arms_offset_y;

	if (!(gMC->cond & 0x80) || gMC->cond & 2)
		return;

	// Draw weapon
	gMC->rect_arms.left = (gArmsData[gSelectedArms].code % 13) * 24;
	gMC->rect_arms.right = gMC->rect_arms.left + 24;
	gMC->rect_arms.top = (gArmsData[gSelectedArms].code / 13) * 96;
	gMC->rect_arms.bottom = gMC->rect_arms.top + 16;

	if (gMC->direct == 2)
	{
		gMC->rect_arms.top += 16;
		gMC->rect_arms.bottom += 16;
	}

	if (gMC->up)
	{
		arms_offset_y = -4;
		gMC->rect_arms.top += 32;
		gMC->rect_arms.bottom += 32;
	}
	else if (gMC->down)
	{
		arms_offset_y = 4;
		gMC->rect_arms.top += 64;
		gMC->rect_arms.bottom += 64;
	}
	else
	{
		arms_offset_y = 0;
	}

	if (gMC->ani_no == 1 || gMC->ani_no == 3 || gMC->ani_no == 6 || gMC->ani_no == 8)
		++gMC->rect_arms.top;

	if (gMC->direct == 0)
		PutBitmap3(
			&grcGame,
			((gMC->x - gMC->view.front) / 0x200) - (fx / 0x200) - 8,
			((gMC->y - gMC->view.top) / 0x200) - (fy / 0x200) + arms_offset_y,
			&gMC->rect_arms,
			SURFACE_ID_ARMS);
	else
		PutBitmap3(
			&grcGame,
			((gMC->x - gMC->view.front) / 0x200) - (fx / 0x200),
			((gMC->y - gMC->view.top) / 0x200) - (fy / 0x200) + arms_offset_y,
			&gMC->rect_arms,
			SURFACE_ID_ARMS);

	if (gMC->shock / 2 % 2)
		return;

	// Draw player
	RECT rect = gMC->rect;
	if (gMC->equip & 0x40)
	{
		rect.top += 32;
		rect.bottom += 32;
	}
	SurfaceID id = (gCurMyChar ? SURFACE_ID_MY_CHAR2 : SURFACE_ID_MY_CHAR);
	PutBitmap3(&grcGame, ((gMC->x - gMC->view.front) / 0x200) - (fx / 0x200), ((gMC->y - gMC->view.top) / 0x200) - (fy / 0x200), &rect, id);

	// Draw air tank
	RECT rcBubble[2] = {
		{56, 96, 80, 120},
		{80, 96, 104, 120},
	};

	++gMC->bubble;
	if (gMC->equip & 0x10 && gMC->flag & 0x100)
		PutBitmap3(&grcGame, (gMC->x / 0x200) - 12 - (fx / 0x200), (gMC->y / 0x200) - 12 - (fy / 0x200), &rcBubble[gMC->bubble / 2 % 2], SURFACE_ID_CARET);
	else if (gMC->unit == 1)
		PutBitmap3(&grcGame, (gMC->x / 0x200) - 12 - (fx / 0x200), (gMC->y / 0x200) - 12 - (fy / 0x200), &rcBubble[gMC->bubble / 2 % 2], SURFACE_ID_CARET);
}

void ActMyChar_Normal(BOOL bKey)
{
	// Get speeds and accelerations
	int max_move;	// Unused
	int max_dash;
	int gravity1;
	int gravity2;
	int jump;
	int dash1;
	int dash2;
	int resist;

	int a, x;

	//Debug fly
	if (gDebug.bNoclip)
	{
		if (gKey & gKeyLeft)
		{
			gMC->x -= 0x1000;
			gMC->direct = 0;
		}
		if (gKey & gKeyRight)
		{
			gMC->x += 0x1000;
			gMC->direct = 2;
		}
		if (gKey & gKeyUp)
		{
			gMC->y -= 0x1000;
		}
		if (gKey & gKeyDown)
		{
			gMC->y += 0x1000;
		}

		gMC->tgt_x = gMC->x;
		gMC->tgt_y = gMC->y;

		return;
	}

	if (gMC->cond & 2)
		return;

	if (gMC->flag & 0x100)
	{
		max_dash = 0x196;
		max_move = 0x2FF;
		gravity1 = 0x28;
		gravity2 = 0x10;
		jump = 0x280;
		dash1 = 0x2A;
		dash2 = 0x10;
		resist = 0x19;
	}
	else
	{
		max_dash = 0x32C;
		max_move = 0x5FF;
		gravity1 = 0x50;
		gravity2 = 0x20;
		jump = 0x500;
		dash1 = 0x55;
		dash2 = 0x20;
		resist = 0x33;
	}

	// Don't create "?" effect
	gMC->ques = FALSE;

	// If can't control player, stop boosting
	if (!bKey)
		gMC->boost_sw = 0;

	// Movement on the ground
	if (gMC->flag & 8 || gMC->flag & 0x10 || gMC->flag & 0x20)
	{
		// Stop boosting and refuel
		gMC->boost_sw = 0;

		if (gMC->equip & 1)
		{
			gMC->boost_cnt = 50;
		}
		else if (gMC->equip & 0x20)
		{
			gMC->boost_cnt = 50;
		}
		else
		{
			gMC->boost_cnt = 0;
		}

		// Move in direction held
		if (bKey)
		{
			if (gKeyTrg == gKeyDown && gKey == gKeyDown && !(gMC->cond & 1) && !(g_GameFlags & 4))
			{
				gMC->cond |= 1;
				gMC->ques = TRUE;
			}
			else if (gKey == gKeyDown)
			{
				// There probably used to be commented-out code here
			}
			else
			{
				if (gKey & gKeyLeft && gMC->xm > -max_dash)
					gMC->xm -= dash1;
				if (gKey & gKeyRight && gMC->xm < max_dash)
					gMC->xm += dash1;

				if (gKey & gKeyLeft)
					gMC->direct = 0;
				if (gKey & gKeyRight)
					gMC->direct = 2;
			}
		}

		// Friction
		if (!(gMC->cond & 0x20))
		{
			if (gMC->xm < 0)
			{
				if (gMC->xm > -resist)
					gMC->xm = 0;
				else
					gMC->xm += resist;
			}
			if (gMC->xm > 0)
			{
				if (gMC->xm < resist)
					gMC->xm = 0;
				else
					gMC->xm -= resist;
			}
		}
	}
	else
	{
		// Start boosting
		if (bKey)
		{
			if (gMC->equip & 0x21 && gKeyTrg & gKeyJump && gMC->boost_cnt != 0)
			{
				// Booster 0.8
				if (gMC->equip & 1)
				{
					gMC->boost_sw = 1;

					if (gMC->ym > 0x100)
						gMC->ym /= 2;
				}

				// Booster 2.0
				if (gMC->equip & 0x20)
				{
					if (gKey & gKeyUp)
					{
						gMC->boost_sw = 2;
						gMC->xm = 0;
						gMC->ym = -0x5FF;
					}
					else if (gKey & gKeyLeft)
					{
						gMC->boost_sw = 1;
						gMC->ym = 0;
						gMC->xm = -0x5FF;
					}
					else if (gKey & gKeyRight)
					{
						gMC->boost_sw = 1;
						gMC->ym = 0;
						gMC->xm = 0x5FF;
					}
					else if (gKey & gKeyDown)
					{
						gMC->boost_sw = 3;
						gMC->xm = 0;
						gMC->ym = 0x5FF;
					}
					else
					{
						gMC->boost_sw = 2;
						gMC->xm = 0;
						gMC->ym = -0x5FF;
					}
				}
			}

			// Move left and right
			if (gKey & gKeyLeft && gMC->xm > -max_dash)
				gMC->xm -= dash2;
			if (gKey & gKeyRight && gMC->xm < max_dash)
				gMC->xm += dash2;

			if (gKey & gKeyLeft)
				gMC->direct = 0;
			if (gKey & gKeyRight)
				gMC->direct = 2;
		}

		// Slow down when stopped boosting (Booster 2.0)
		if (gMC->equip & 0x20 && gMC->boost_sw != 0 && (!(gKey & gKeyJump) || gMC->boost_cnt == 0))
		{
			if (gMC->boost_sw == 1)
				gMC->xm /= 2;
			else if (gMC->boost_sw == 2)
				gMC->ym /= 2;
		}

		// Stop boosting
		if (gMC->boost_cnt == 0 || !(gKey & gKeyJump))
			gMC->boost_sw = 0;
	}

	// Jumping
	if (bKey)
	{
		// Look up and down
		if (gKey & gKeyUp)
			gMC->up = TRUE;
		else
			gMC->up = FALSE;

		if (gKey & gKeyDown && !(gMC->flag & 8))
			gMC->down = TRUE;
		else
			gMC->down = FALSE;

		if (gKeyTrg & gKeyJump && (gMC->flag & 8 || gMC->flag & 0x10 || gMC->flag & 0x20))
		{
			if (gMC->flag & 0x2000)
			{
				// Another weird empty case needed for accurate assembly.
				// There probably used to be some commented-out code here.
			}
			else
			{
				gMC->ym = -jump;
				PlaySoundObject(15, 1);
			}
		}
	}

	// Stop interacting when moved
	if (bKey && gKey & (gKeyLeft | gKeyRight | gKeyUp | gKeyJump | gKeyShot))
		gMC->cond &= ~1;

	// Booster losing fuel
	if (gMC->boost_sw != 0 && gMC->boost_cnt != 0)
		--gMC->boost_cnt;

	// Wind / current forces
	if (gMC->flag & 0x1000)
		gMC->xm -= 0x88;
	if (gMC->flag & 0x2000)
		gMC->ym -= 0x80;
	if (gMC->flag & 0x4000)
		gMC->xm += 0x88;
	if (gMC->flag & 0x8000)
		gMC->ym += 0x55;

	// Booster 2.0 forces and effects
	if (gMC->equip & 0x20 && gMC->boost_sw != 0)
	{
		if (gMC->boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC->flag & 5)
				gMC->ym = -0x100;

			// Move in direction facing
			if (gMC->direct == 0)
				gMC->xm -= 0x20;
			if (gMC->direct == 2)
				gMC->xm += 0x20;

			// Boost particles (and sound)
			if (gKeyTrg & gKeyJump || gMC->boost_cnt % 3 == 1)
			{
				if (gMC->direct == 0)
					SetCaret(gMC->x + (2 * 0x200), gMC->y + (2 * 0x200), 7, 2);
				if (gMC->direct == 2)
					SetCaret(gMC->x - (2 * 0x200), gMC->y + (2 * 0x200), 7, 0);

				PlaySoundObject(113, 1);
			}
		}
		else if (gMC->boost_sw == 2)
		{
			// Move upwards
			gMC->ym -= 0x20;

			// Boost particles (and sound)
			if (gKeyTrg & gKeyJump || gMC->boost_cnt % 3 == 1)
			{
				SetCaret(gMC->x, gMC->y + (6 * 0x200), 7, 3);
				PlaySoundObject(113, 1);
			}
		}
		else if (gMC->boost_sw == 3 && (gKeyTrg & gKeyJump || gMC->boost_cnt % 3 == 1))
		{
			// Boost particles (and sound)
			SetCaret(gMC->x, gMC->y - (6 * 0x200), 7, 1);
			PlaySoundObject(113, 1);
		}
	}
	// Upwards wind/current
	else if (gMC->flag & 0x2000)
	{
		gMC->ym += gravity1;
	}
	// Booster 0.8
	else if (gMC->equip & 1 && gMC->boost_sw != 0 && gMC->ym > -0x400)
	{
		// Upwards force
		gMC->ym -= 0x20;

		if (gMC->boost_cnt % 3 == 0)
		{
			SetCaret(gMC->x, gMC->y + (gMC->hit.bottom / 2), 7, 3);
			PlaySoundObject(113, 1);
		}

		// Bounce off of ceiling
		if (gMC->flag & 2)
			gMC->ym = 0x200;
	}
	// Gravity while jump is held
	else if (gMC->ym < 0 && bKey && gKey & gKeyJump)
	{
		gMC->ym += gravity2;
	}
	// Normal gravity
	else
	{
		gMC->ym += gravity1;
	}

	// Keep player on slopes
	if (!bKey || !(gKeyTrg & gKeyJump))
	{
		if (gMC->flag & 0x10 && gMC->xm < 0)
			gMC->ym = -gMC->xm;
		if (gMC->flag & 0x20 && gMC->xm > 0)
			gMC->ym = gMC->xm;
		if (gMC->flag & 8 && gMC->flag & 0x80000 && gMC->xm < 0)
			gMC->ym = 0x400;
		if (gMC->flag & 8 && gMC->flag & 0x10000 && gMC->xm > 0)
			gMC->ym = 0x400;
		if (gMC->flag & 8 && gMC->flag & 0x20000 && gMC->flag & 0x40000)
			gMC->ym = 0x400;
	}

	if (0)
	{
		// There used to be an if-statement here that didn't do anything, but the compiler optimised it out.
		// We only know this was here because empty if-statements affect the register usage.
		// Since there's no code, we have no idea what the original condition actually was.
	}

	// Limit speed
	if (gMC->flag & 0x100 && !(gMC->flag & 0xF000))
	{
		if (gMC->xm < -0x2FF)
			gMC->xm = -0x2FF;
		if (gMC->xm > 0x2FF)
			gMC->xm = 0x2FF;
		if (gMC->ym < -0x2FF)
			gMC->ym = -0x2FF;
		if (gMC->ym > 0x2FF)
			gMC->ym = 0x2FF;
	}
	else
	{
		if (gMC->xm < -0x5FF)
			gMC->xm = -0x5FF;
		if (gMC->xm > 0x5FF)
			gMC->xm = 0x5FF;
		if (gMC->ym < -0x5FF)
			gMC->ym = -0x5FF;
		if (gMC->ym > 0x5FF)
			gMC->ym = 0x5FF;
	}

	// Water splashing
	if (!gMC->sprash && gMC->flag & 0x100)
	{
		int dir;

		if (gMC->flag & 0x800)
			dir = 2;
		else
			dir = 0;

		if (!(gMC->flag & 8) && gMC->ym > 0x200)
		{
			for (a = 0; a < 8; ++a)
			{
				x = gMC->x + (Random(-8, 8) * 0x200);
				SetNpChar(73, x, gMC->y, gMC->xm + Random(-0x200, 0x200), Random(-0x200, 0x80) - (gMC->ym / 2), dir, NULL, 0);
			}

			PlaySoundObject(56, 1);
		}
		else
		{
			if (gMC->xm > 0x200 || gMC->xm < -0x200)
			{
				for (a = 0; a < 8; ++a)
				{
					x = gMC->x + (Random(-8, 8) * 0x200);
					SetNpChar(73, x, gMC->y, gMC->xm + Random(-0x200, 0x200), Random(-0x200, 0x80), dir, NULL, 0);
				}

				PlaySoundObject(56, 1);
			}
		}

		gMC->sprash = TRUE;
	}

	if (!(gMC->flag & 0x100))
		gMC->sprash = FALSE;

	// Spike damage
	if (gMC->flag & 0x400)
		DamageMyChar(10);

	// Camera
	if (gMC->direct == 0)
	{
		gMC->index_x -= 0x200;
		if (gMC->index_x < -0x8000)
			gMC->index_x = -0x8000;
	}
	else
	{
		gMC->index_x += 0x200;
		if (gMC->index_x > 0x8000)
			gMC->index_x = 0x8000;
	}
	if (gKey & gKeyUp && bKey)
	{
		gMC->index_y -= 0x200;
		if (gMC->index_y < -0x8000)
			gMC->index_y = -0x8000;
	}
	else if (gKey & gKeyDown && bKey)
	{
		gMC->index_y += 0x200;
		if (gMC->index_y > 0x8000)
			gMC->index_y = 0x8000;
	}
	else
	{
		if (gMC->index_y > 0x200)
			gMC->index_y -= 0x200;
		if (gMC->index_y < -0x200)
			gMC->index_y += 0x200;
	}

	gMC->tgt_x = gMC->x + gMC->index_x;
	gMC->tgt_y = gMC->y + gMC->index_y;

	// Change position
	if (gMC->xm <= resist && gMC->xm >= -resist)
	{
		// This case is completely empty. This is most likely the result of commented-out code or some other change (so this is most likely inaccurate to the original source code)
	}
	else
	{
		gMC->x += gMC->xm;
	}

	gMC->y += gMC->ym;
}

void ActMyChar_Stream(BOOL bKey)
{
	gMC->up = FALSE;
	gMC->down = FALSE;

	if (bKey)
	{
		if (gKey & (gKeyLeft | gKeyRight))
		{
			if (gKey & gKeyLeft)
				gMC->xm -= 0x100;

			if (gKey & gKeyRight)
				gMC->xm += 0x100;
		}
		else if (gMC->xm < 0x80 && gMC->xm > -0x80)
		{
			gMC->xm = 0;
		}
		else if (gMC->xm > 0)
		{
			gMC->xm -= 0x80;
		}
		else if (gMC->xm < 0)
		{
			gMC->xm += 0x80;
		}

		if (gKey & (gKeyUp | gKeyDown))
		{
			if (gKey & gKeyUp)
				gMC->ym -= 0x100;

			if (gKey & gKeyDown)
				gMC->ym += 0x100;
		}
		else if (gMC->ym < 0x80 && gMC->ym > -0x80)
		{
			gMC->ym = 0;
		}
		else if (gMC->ym > 0)
		{
			gMC->ym -= 0x80;
		}
		else if (gMC->ym < 0)
		{
			gMC->ym += 0x80;
		}
	}
	else
	{
		if (gMC->xm < 0x80 && gMC->xm > -0x40)
			gMC->xm = 0;
		else if (gMC->xm > 0)
			gMC->xm -= 0x80;
		else if (gMC->xm < 0)
			gMC->xm += 0x80;

		if (gMC->ym < 0x80 && gMC->ym > -0x40)
			gMC->ym = 0;
		else if (gMC->ym > 0)
			gMC->ym -= 0x80;
		else if (gMC->ym < 0)
			gMC->ym += 0x80;
	}

	if (gMC->ym < -0x200 && gMC->flag & 2)
		SetCaret(gMC->x, gMC->y - gMC->hit.top, 13, 5);
	if (gMC->ym > 0x200 && gMC->flag & 8)
		SetCaret(gMC->x, gMC->y + gMC->hit.bottom, 13, 5);

	if (gMC->xm > 0x400)
		gMC->xm = 0x400;
	if (gMC->xm < -0x400)
		gMC->xm = -0x400;

	if (gMC->ym > 0x400)
		gMC->ym = 0x400;
	if (gMC->ym < -0x400)
		gMC->ym = -0x400;

	if ((gKey & (gKeyLeft | gKeyUp)) == (gKeyLeft | gKeyUp))
	{
		if (gMC->xm < -780)
			gMC->xm = -780;
		if (gMC->ym < -780)
			gMC->ym = -780;
	}

	if ((gKey & (gKeyRight | gKeyUp)) == (gKeyRight | gKeyUp))
	{
		if (gMC->xm > 780)
			gMC->xm = 780;
		if (gMC->ym < -780)
			gMC->ym = -780;
	}

	if ((gKey & (gKeyLeft | gKeyDown)) == (gKeyLeft | gKeyDown))
	{
		if (gMC->xm < -780)
			gMC->xm = -780;
		if (gMC->ym > 780)
			gMC->ym = 780;
	}

	if ((gKey & (gKeyRight | gKeyDown)) == (gKeyRight | gKeyDown))
	{
		if (gMC->xm > 780)
			gMC->xm = 780;
		if (gMC->ym > 780)
			gMC->ym = 780;
	}

	gMC->x += gMC->xm;
	gMC->y += gMC->ym;
}

void AirProcess(void)
{
	if (gDebug.bGodmode)
		return;

	if (gMC->equip & 0x10)
	{
		gMC->air = 1000;
		gMC->air_get = 0;
	}
	else
	{
		if (!(gMC->flag & 0x100))
		{
			gMC->air = 1000;
		}
		else
		{
			if (--gMC->air <= 0)
			{
				if (GetNPCFlag(4000))
				{
					// Core cutscene
					StartTextScript(1100);
				}
				else
				{
					// Drown
					if(!nifiIsLinked() || nifiIsLinked() && !gRespawnEnabled)
					{
						StartTextScript(41);

						if (gMC->direct == 0)
							SetCaret(gMC->x, gMC->y, 8, 0);
						else
							SetCaret(gMC->x, gMC->y, 8, 2);

						gMC->cond &= ~0x80;
					}
					else
					{
						if(gCurMyChar == 0 && gMCP2.respawnTimer)
						{
							StartTextScript(41);

							if (gMC->direct == 0)
								SetCaret(gMC->x, gMC->y, 8, 0);
							else
								SetCaret(gMC->x, gMC->y, 8, 2);
						}
						else if(gCurMyChar==1 && gMCP1.respawnTimer)
						{
							StartTextScript(41);

							if (gMC->direct == 0)
								SetCaret(gMC->x, gMC->y, 8, 0);
							else
								SetCaret(gMC->x, gMC->y, 8, 2);
						}
						else
						{
							gMC->respawnTimer = 120;
						}
						gMC->cond &= ~0x80;
					}
					
				}
			}
		}

		if (gMC->flag & 0x100)
		{
			gMC->air_get = 60;
		}
		else
		{
			if (gMC->air_get != 0)
				--gMC->air_get;
		}
	}
}

void ActMyChar(BOOL bKey)
{
	if(gMC->respawnTimer > 0)
	{
		gMC->respawnTimer--;
		if(gMC->respawnTimer==1)
		{
			if(gCurMyChar==0)
			{
				gMC->x = gMCP2.x;
				gMC->y = gMCP2.y;
				if(gMCP2.life <= 1)
				{
					gMC->respawnTimer++;
					if(nifiIsHost())
					{
						gFrame.x = gMCP2.x - (WINDOW_WIDTH * 0x200 / 2);
						gFrame.y = gMCP2.y - (WINDOW_HEIGHT * 0x200 / 2);
					}
				}

		
			}
			else
			{
				gMC->x = gMCP1.x;
				gMC->y = gMCP1.y;
				if(gMCP1.life <= 1)
				{
					gMC->respawnTimer++;
					if(nifiIsClient())
					{
						gFrame.x = gMCP1.x - (WINDOW_WIDTH * 0x200 / 2);
						gFrame.y = gMCP1.y - (WINDOW_HEIGHT * 0x200 / 2);
					}

				}

			}

		}
		if(gMC->respawnTimer==0)
		{
			gMC->shock = 150;
			gMC->cond = 0x80;
			gMC->air = 1000;
			if(gCurMyChar==0)
			{
				gMC->x = gMCP2.x;
				gMC->y = gMCP2.y;
				gMC->life = gMCP2.life / 2;
				gMCP2.life /= 2;
			}
			else
			{
				gMC->x = gMCP1.x;
				gMC->y = gMCP1.y;
				gMC->life = gMCP1.life / 2;
				gMCP1.life /= 2;
			}
			
		}
	}
	if (!(gMC->cond & 0x80))
		return;

	if (gMC->exp_wait != 0)
		--gMC->exp_wait;

	if (gMC->shock != 0)
	{
		--gMC->shock;
	}
	else if (gMC->exp_count != 0)
	{
		if(gCurMyChar == 0)
			SetValueView(&gMC->x, &gMC->y, gMC->exp_count);
		else
			SetValueView(&gMCP2.x, &gMCP2.y, gMC->exp_count);
		gMC->exp_count = 0;
	}

	switch (gMC->unit)
	{
		case 0:
			if (!(g_GameFlags & 4) && bKey)
				AirProcess();

			ActMyChar_Normal(bKey);
			break;

		case 1:
			ActMyChar_Stream(bKey);
			break;
	}

	gMC->cond &= ~0x20;
}

void GetMyCharPosition(int *x, int *y)
{
	*x = gMC->x;
	*y = gMC->y;
}

void SetMyCharPosition(int x, int y)
{
	gMC->x = x;
	gMC->y = y;
	gMC->tgt_x = gMC->x;
	gMC->tgt_y = gMC->y;
	gMC->index_x = 0;
	gMC->index_y = 0;
	gMC->xm = 0;
	gMC->ym = 0;
	gMC->cond &= ~1;
	InitStar();

	if (nifiIsLinked())
	{
		gMCP2.x = x;
		gMCP2.y = y;
		gMCP2.tgt_x = gMCP2.x;
		gMCP2.tgt_y = gMCP2.y;
		gMCP2.index_x = 0;
		gMCP2.index_y = 0;
		gMCP2.xm = 0;
		gMCP2.ym = 0;
		gMCP2.cond &= ~1;
	}
}

void MoveMyChar(int x, int y)
{
	gMC->x = x;
	gMC->y = y;
	if(nifiIsLinked())
	{
		gMCP2.x = x;
		gMCP2.y = y;
	}
}

void ZeroMyCharXMove(void)
{
	gMC->xm = 0;
	gMCP2.xm = 0;
}

int GetUnitMyChar(void)
{
	return gMC->unit;
}

void SetMyCharDirect(unsigned char dir)
{
	int i;

	if (dir == 3)
	{
		gMC->cond |= 1;
	}
	else
	{
		gMC->cond &= ~1;

		if (dir < 10)
		{
			gMC->direct = dir;
		}
		else
		{
			for (i = 0; i < NPC_MAX; ++i)
				if (gNPC[i]->code_event == dir)
					break;

			if (i == NPC_MAX)
				return;

			if (gMC->x > gNPC[i]->x)
				gMC->direct = 0;
			else
				gMC->direct = 2;
		}
	}

	gMC->xm = 0;
	AnimationMyChar(FALSE);
}

void ChangeMyUnit(unsigned char a)
{
	gMC->unit = a;
	gMCP2.unit = a;
}

void PitMyChar(void)
{
	gMC->y += 2 * 0x10 * 0x200;	// Shove player two tiles down. I wonder what this was meant for?
}

void EquipItem(int flag, BOOL b)
{
	if (b)
		gMC->equip |= flag;
	else
		gMC->equip &= ~flag;
	if (b)
		gMCP2.equip |= flag;
	else
		gMCP2.equip &= ~flag;
}

void ResetCheck(void)
{
	gMC->cond &= ~1;
}

int noise_no;
unsigned int noise_freq;

void SetNoise(int no, int freq)
{
	noise_freq = freq;
	noise_no = no;

	switch (noise_no)
	{
		case 1:
			ChangeSoundFrequency(40, noise_freq);
			ChangeSoundFrequency(41, noise_freq + 100);
			PlaySoundObject(40, -1);
			PlaySoundObject(41, -1);
			break;

		case 2:
			PlaySoundObject(58, -1);
			break;
	}
}

void CutNoise(void)
{
	noise_no = 0;
	PlaySoundObject(40, 0);
	PlaySoundObject(41, 0);
	PlaySoundObject(58, 0);
}

void ResetNoise(void)
{
	switch (noise_no)
	{
		case 1:
			ChangeSoundFrequency(40, noise_freq);
			ChangeSoundFrequency(41, noise_freq + 100);
			PlaySoundObject(40, -1);
			PlaySoundObject(41, -1);
			break;

		case 2:
			PlaySoundObject(58, -1);
			break;
	}
}

void SleepNoise(void)
{
	PlaySoundObject(40, 0);
	PlaySoundObject(41, 0);
	PlaySoundObject(58, 0);
}
