// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "NpcTbl.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "WindowsWrapper.h"

#include "File.h"
#include "Generic.h"
#include "NpcAct.h"

#include "fopen.h"

// Npc function table
const NPCFUNCTION gpNpcFuncTbl[361] = {
	ActNpc000,
	ActNpc001,
	ActNpc002,
	ActNpc003,
	ActNpc004,
	ActNpc005,
	ActNpc006,
	ActNpc007,
	ActNpc008,
	ActNpc009,
	ActNpc010,
	ActNpc011,
	ActNpc012,
	ActNpc013,
	ActNpc014,
	ActNpc015,
	ActNpc016,
	ActNpc017,
	ActNpc018,
	ActNpc019,
	ActNpc020,
	ActNpc021,
	ActNpc022,
	ActNpc023,
	ActNpc024,
	ActNpc025,
	ActNpc026,
	ActNpc027,
	ActNpc028,
	ActNpc029,
	ActNpc030,
	ActNpc031,
	ActNpc032,
	ActNpc033,
	ActNpc034,
	ActNpc035,
	ActNpc036,
	ActNpc037,
	ActNpc038,
	ActNpc039,
	ActNpc040,
	ActNpc041,
	ActNpc042,
	ActNpc043,
	ActNpc044,
	ActNpc045,
	ActNpc046,
	ActNpc047,
	ActNpc048,
	ActNpc049,
	ActNpc050,
	ActNpc051,
	ActNpc052,
	ActNpc053,
	ActNpc054,
	ActNpc055,
	ActNpc056,
	ActNpc057,
	ActNpc058,
	ActNpc059,
	ActNpc060,
	ActNpc061,
	ActNpc062,
	ActNpc063,
	ActNpc064,
	ActNpc065,
	ActNpc066,
	ActNpc067,
	ActNpc068,
	ActNpc069,
	ActNpc070,
	ActNpc071,
	ActNpc072,
	ActNpc073,
	ActNpc074,
	ActNpc075,
	ActNpc076,
	ActNpc077,
	ActNpc078,
	ActNpc079,
	ActNpc080,
	ActNpc081,
	ActNpc082,
	ActNpc083,
	ActNpc084,
	ActNpc085,
	ActNpc086,
	ActNpc087,
	ActNpc088,
	ActNpc089,
	ActNpc090,
	ActNpc091,
	ActNpc092,
	ActNpc093,
	ActNpc094,
	ActNpc095,
	ActNpc096,
	ActNpc097,
	ActNpc098,
	ActNpc099,
	ActNpc100,
	ActNpc101,
	ActNpc102,
	ActNpc103,
	ActNpc104,
	ActNpc105,
	ActNpc106,
	ActNpc107,
	ActNpc108,
	ActNpc109,
	ActNpc110,
	ActNpc111,
	ActNpc112,
	ActNpc113,
	ActNpc114,
	ActNpc115,
	ActNpc116,
	ActNpc117,
	ActNpc118,
	ActNpc119,
	ActNpc120,
	ActNpc121,
	ActNpc122,
	ActNpc123,
	ActNpc124,
	ActNpc125,
	ActNpc126,
	ActNpc127,
	ActNpc128,
	ActNpc129,
	ActNpc130,
	ActNpc131,
	ActNpc132,
	ActNpc133,
	ActNpc134,
	ActNpc135,
	ActNpc136,
	ActNpc137,
	ActNpc138,
	ActNpc139,
	ActNpc140,
	ActNpc141,
	ActNpc142,
	ActNpc143,
	ActNpc144,
	ActNpc145,
	ActNpc146,
	ActNpc147,
	ActNpc148,
	ActNpc149,
	ActNpc150,
	ActNpc151,
	ActNpc152,
	ActNpc153,
	ActNpc154,
	ActNpc155,
	ActNpc156,
	ActNpc157,
	ActNpc158,
	ActNpc159,
	ActNpc160,
	ActNpc161,
	ActNpc162,
	ActNpc163,
	ActNpc164,
	ActNpc165,
	ActNpc166,
	ActNpc167,
	ActNpc168,
	ActNpc169,
	ActNpc170,
	ActNpc171,
	ActNpc172,
	ActNpc173,
	ActNpc174,
	ActNpc175,
	ActNpc176,
	ActNpc177,
	ActNpc178,
	ActNpc179,
	ActNpc180,
	ActNpc181,
	ActNpc182,
	ActNpc183,
	ActNpc184,
	ActNpc185,
	ActNpc186,
	ActNpc187,
	ActNpc188,
	ActNpc189,
	ActNpc190,
	ActNpc191,
	ActNpc192,
	ActNpc193,
	ActNpc194,
	ActNpc195,
	ActNpc196,
	ActNpc197,
	ActNpc198,
	ActNpc199,
	ActNpc200,
	ActNpc201,
	ActNpc202,
	ActNpc203,
	ActNpc204,
	ActNpc205,
	ActNpc206,
	ActNpc207,
	ActNpc208,
	ActNpc209,
	ActNpc210,
	ActNpc211,
	ActNpc212,
	ActNpc213,
	ActNpc214,
	ActNpc215,
	ActNpc216,
	ActNpc217,
	ActNpc218,
	ActNpc219,
	ActNpc220,
	ActNpc221,
	ActNpc222,
	ActNpc223,
	ActNpc224,
	ActNpc225,
	ActNpc226,
	ActNpc227,
	ActNpc228,
	ActNpc229,
	ActNpc230,
	ActNpc231,
	ActNpc232,
	ActNpc233,
	ActNpc234,
	ActNpc235,
	ActNpc236,
	ActNpc237,
	ActNpc238,
	ActNpc239,
	ActNpc240,
	ActNpc241,
	ActNpc242,
	ActNpc243,
	ActNpc244,
	ActNpc245,
	ActNpc246,
	ActNpc247,
	ActNpc248,
	ActNpc249,
	ActNpc250,
	ActNpc251,
	ActNpc252,
	ActNpc253,
	ActNpc254,
	ActNpc255,
	ActNpc256,
	ActNpc257,
	ActNpc258,
	ActNpc259,
	ActNpc260,
	ActNpc261,
	ActNpc262,
	ActNpc263,
	ActNpc264,
	ActNpc265,
	ActNpc266,
	ActNpc267,
	ActNpc268,
	ActNpc269,
	ActNpc270,
	ActNpc271,
	ActNpc272,
	ActNpc273,
	ActNpc274,
	ActNpc275,
	ActNpc276,
	ActNpc277,
	ActNpc278,
	ActNpc279,
	ActNpc280,
	ActNpc281,
	ActNpc282,
	ActNpc283,
	ActNpc284,
	ActNpc285,
	ActNpc286,
	ActNpc287,
	ActNpc288,
	ActNpc289,
	ActNpc290,
	ActNpc291,
	ActNpc292,
	ActNpc293,
	ActNpc294,
	ActNpc295,
	ActNpc296,
	ActNpc297,
	ActNpc298,
	ActNpc299,
	ActNpc300,
	ActNpc301,
	ActNpc302,
	ActNpc303,
	ActNpc304,
	ActNpc305,
	ActNpc306,
	ActNpc307,
	ActNpc308,
	ActNpc309,
	ActNpc310,
	ActNpc311,
	ActNpc312,
	ActNpc313,
	ActNpc314,
	ActNpc315,
	ActNpc316,
	ActNpc317,
	ActNpc318,
	ActNpc319,
	ActNpc320,
	ActNpc321,
	ActNpc322,
	ActNpc323,
	ActNpc324,
	ActNpc325,
	ActNpc326,
	ActNpc327,
	ActNpc328,
	ActNpc329,
	ActNpc330,
	ActNpc331,
	ActNpc332,
	ActNpc333,
	ActNpc334,
	ActNpc335,
	ActNpc336,
	ActNpc337,
	ActNpc338,
	ActNpc339,
	ActNpc340,
	ActNpc341,
	ActNpc342,
	ActNpc343,
	ActNpc344,
	ActNpc345,
	ActNpc346,
	ActNpc347,
	ActNpc348,
	ActNpc349,
	ActNpc350,
	ActNpc351,
	ActNpc352,
	ActNpc353,
	ActNpc354,
	ActNpc355,
	ActNpc356,
	ActNpc357,
	ActNpc358,
	ActNpc359,
	ActNpc360,
};

NPC_TABLE *gNpcTable;

BOOL LoadNpcTable(const char *path)
{
	FILE_e *fp;
	int n;
	size_t size;
	int num;

	fp = fopen_embed(path, "rb");
	if (fp == NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
		return FALSE;
	}

#ifndef READ_FROM_SD
	size = fp->size;
#else
	size = GetFileSizeLong(fp);
#endif

	if (size == -1)
		return FALSE;

	num = (int)(size / 0x18);

	gNpcTable = (NPC_TABLE*)malloc(num * sizeof(NPC_TABLE));
	if (gNpcTable == NULL)
		return FALSE;


	for (n = 0; n < num; ++n) // bits
		gNpcTable[n].bits = File_ReadLE16(fp);
	for (n = 0; n < num; ++n) // life
		gNpcTable[n].life = File_ReadLE16(fp);
	for (n = 0; n < num; ++n) // surf
		fread_embed(&gNpcTable[n].surf, 1, 1, fp);
	for (n = 0; n < num; ++n) // destroy_voice
		fread_embed(&gNpcTable[n].destroy_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // hit_voice
		fread_embed(&gNpcTable[n].hit_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // size
		fread_embed(&gNpcTable[n].size, 1, 1, fp);
	for (n = 0; n < num; ++n) // exp
		gNpcTable[n].exp = File_ReadLE32(fp);
	for (n = 0; n < num; ++n) // damage
		gNpcTable[n].damage = File_ReadLE32(fp);
	for (n = 0; n < num; ++n) // hit
		fread_embed(&gNpcTable[n].hit, 4, 1, fp);
	for (n = 0; n < num; ++n) // view
		fread_embed(&gNpcTable[n].view, 4, 1, fp);

	fclose_embed(fp);
	return TRUE;
}

void ReleaseNpcTable(void)
{
	if (gNpcTable != NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
	}
}
