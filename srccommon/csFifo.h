#pragma once

enum
{
	CSFIFO_ARM7_PRINT = 0,
};

struct CSFIFOMessage
{
	u16 type;
	union 
	{
		struct
		{
			char printData[32];
		};
		
	};
	
};