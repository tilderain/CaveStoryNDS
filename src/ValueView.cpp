// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "ValueView.h"

#include <string.h>

#include "WindowsWrapper.h"

#include "Draw.h"

#define VALUEVIEW_MAX 0x20

VALUEVIEW gVV[VALUEVIEW_MAX];
int gVVIndex;

void ClearValueView(void)
{
	memset(gVV, 0, sizeof(gVV));
	gVVIndex = 0;
}

void SetValueView(int *px, int *py, int value)
{
	BOOL minus;
	int v;

	int index;
	int i;

	for (i = 0; i < VALUEVIEW_MAX; ++i)
	{
		if (gVV[i].flag && gVV[i].px == px)
		{
			if (gVV[i].value < 0 && value < 0)
				break;

			if (gVV[i].value > 0 && value > 0)
				break;
		}
	}

	if (i == VALUEVIEW_MAX)
	{
		index = gVVIndex++;

		if (gVVIndex == VALUEVIEW_MAX)
			gVVIndex = 0;

		gVV[index].count = 0;
		gVV[index].offset_y = 0;
		gVV[index].value = value;
	}
	else
	{
		index = i;

		gVV[index].count = 32;
		gVV[index].value += value;
		value = gVV[index].value;
	}

	// Get if negative or not
	if (value < 0)
	{
		value *= -1;
		minus = TRUE;
	}
	else
	{
		minus = FALSE;
	}

	// Get width
	v = value;

	int width;

	if (value > 999)
		width = 40;
	else if (value > 99)
		width = 32;
	else if (value > 9)
		width = 24;
	else
		width = 16;

	// Set properties
	gVV[index].flag = TRUE;
	gVV[index].px = px;
	gVV[index].py = py;
	gVV[index].rect.left = 40 - width;
	gVV[index].rect.top = 0;
	gVV[index].rect.right = 40;
	gVV[index].rect.bottom = 8 * (index + 1);
}

void ActValueView(void)
{
	int v;

	for (v = 0; v < VALUEVIEW_MAX; ++v)
	{
		if (gVV[v].flag == FALSE)
			continue;

		if (++gVV[v].count < 32)
			gVV[v].offset_y -= 0x100;

		if (gVV[v].count > 72)
			++gVV[v].rect.top;

		if (gVV[v].count > 80)
			gVV[v].flag = FALSE;
	}
}

void PutValueView(int flx, int fly)
{
	int offset_x;
	int v;

	for (v = 0; v < VALUEVIEW_MAX; ++v)
	{
		if (gVV[v].flag == FALSE)
			continue;

		RECT rect[20] = {
			{0, 56, 8, 64},
			{8, 56, 16, 64},
			{16, 56, 24, 64},
			{24, 56, 32, 64},
			{32, 56, 40, 64},
			{40, 56, 48, 64},
			{48, 56, 56, 64},
			{56, 56, 64, 64},
			{64, 56, 72, 64},
			{72, 56, 80, 64},
			{0, 64, 8, 72},
			{8, 64, 16, 72},
			{16, 64, 24, 72},
			{24, 64, 32, 72},
			{32, 64, 40, 72},
			{40, 64, 48, 72},
			{48, 64, 56, 72},
			{56, 64, 64, 72},
			{64, 64, 72, 72},
			{72, 64, 80, 72},
		};

		// Get digits
		int dig[4];
		int fig[4];
		dig[0] = 1;
		dig[1] = 10;
		dig[2] = 100;
		dig[3] = 1000;


		RECT rcPlus = {32, 48, 40, 56};
		RECT rcMinus = {40, 48, 48, 56};
		rcPlus.top += gVV[v].rect.top;
		rcMinus.top += gVV[v].rect.top;

		// Draw value
		//CortBox2(&gVV[v].rect, 0x000000, SURFACE_ID_VALUE_VIEW);
		int vv = gVV[v].value;
		BOOL minus;
		if (vv < 0)
		{
			vv *= -1;
			minus = TRUE;
		}
		else
		{
			minus = FALSE;
		}

		for (int i = 3; i >= 0; --i)
		{
			fig[i] = 0;

			while (vv >= dig[i])
			{
				vv -= dig[i];
				++fig[i];
			}
		}

		offset_x = (gVV[v].rect.right - gVV[v].rect.left) / 2;

		int x = (*gVV[v].px / 0x200) - offset_x - (flx / 0x200);
		int y = (*gVV[v].py / 0x200) + (gVV[v].offset_y / 0x200) - 4 - (fly / 0x200);


		if (minus)
			PutBitmap3(&grcGame, x, y, &rcMinus, SURFACE_ID_TEXT_BOX);
		else
			PutBitmap3(&grcGame, x, y, &rcPlus, SURFACE_ID_TEXT_BOX);

		BOOL sw = FALSE;

		for (int i = 3; i >= 0; i--)
		{
			if (!sw && i != 0 && fig[i] == 0)
				continue;

			sw = TRUE;

			if (minus)
				fig[i] += 10;

			rect[fig[i]].top += gVV[v].rect.top;

			PutBitmap3(&grcGame, x + ((3 - i) * 8) + 8 - gVV[v].rect.left, y, &rect[fig[i]], SURFACE_ID_TEXT_BOX);
		}

	}
}
