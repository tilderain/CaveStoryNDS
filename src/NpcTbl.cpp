#include <string>

#include <SDL_rwops.h>

#include "NpcTbl.h"
#include "NpcAct.h"

NPC_TABLE *gNpcTable;

bool LoadNpcTable(const char *path)
{
	SDL_RWops *fp = SDL_RWFromFile(path, "rb");

	if (fp == nullptr)
	{
		printf("LoadNpcTable failed\nSDL Error: %s\n", SDL_GetError());
		return false;
	}

	const size_t tblSize = SDL_RWsize(fp);

	const size_t npcCount = tblSize / 0x18;
	gNpcTable = (NPC_TABLE*)malloc(npcCount * sizeof(NPC_TABLE));

	for (size_t i = 0; i < npcCount; i++) //bits
		gNpcTable[i].bits = SDL_ReadLE16(fp);
	for (size_t i = 0; i < npcCount; i++) //life
		gNpcTable[i].life = SDL_ReadLE16(fp);
	for (size_t i = 0; i < npcCount; i++) //surf
		fp->read(fp, &gNpcTable[i].surf, 1, 1);
	for (size_t i = 0; i < npcCount; i++) //destroy_voice
		fp->read(fp, &gNpcTable[i].destroy_voice, 1, 1);
	for (size_t i = 0; i < npcCount; i++) //hit_voice
		fp->read(fp, &gNpcTable[i].hit_voice, 1, 1);
	for (size_t i = 0; i < npcCount; i++) //size
		fp->read(fp, &gNpcTable[i].size, 1, 1);
	for (size_t i = 0; i < npcCount; i++) //exp
		gNpcTable[i].exp = SDL_ReadLE32(fp);
	for (size_t i = 0; i < npcCount; i++) //damage
		gNpcTable[i].damage = SDL_ReadLE32(fp);
	for (size_t i = 0; i < npcCount; i++) //hit
		fp->read(fp, &gNpcTable[i].hit, 4, 1);
	for (size_t i = 0; i < npcCount; i++) //view
		fp->read(fp, &gNpcTable[i].view, 4, 1);
		
	SDL_RWclose(fp);	
	return true;
}

void ReleaseNpcTable()
{
	if (gNpcTable)
		free(gNpcTable);
}

//Npc function table
NPCFUNCTION gpNpcFuncTbl[361] =
{
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
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc218,
	ActNpc219,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc259,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc278,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc292,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc298,
	ActNpc299,
	ActNpc300,
	nullptr,
	ActNpc302,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc334,
	ActNpc335,
	ActNpc336,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	ActNpc355,
	nullptr,
	nullptr,
	nullptr,
	ActNpc359,
	nullptr,
};
