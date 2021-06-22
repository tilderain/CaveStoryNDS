#ifndef _multi_nesds__
#define _multi_nesds__

void nifiHostMenu();
void nifiHostWait();
void nifiClientMenu();
void nifiClientWait();

extern char gStartingNetplay;



//#include "../srccommon/wifi_shared.h"
//#include "../srccommon/wifi_arm9.h"

#include <nds.h>

#endif

#ifdef __cplusplus
extern "C"{
#endif

/*extern Wifi_MainStruct Wifi_Data_Struct;
extern WifiSyncHandler synchandler;
extern volatile Wifi_MainStruct * WifiData;
extern int Wifi_RawTxFrameNIFI(u16 datalen, u16 rate, u16 * data);
extern void Handler(int packetID, int readlength);*/

#ifdef __cplusplus
}
#endif