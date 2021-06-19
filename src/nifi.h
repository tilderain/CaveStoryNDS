#pragma once

extern volatile int linkReceivedData;
extern volatile int linkSendData;
extern volatile bool transferWaiting;
extern volatile bool receivedPacket;
extern volatile int nifiSendid;
// Don't write directly
extern bool nifiEnabled;
extern bool nifiPaused;

void enableNifi();
void disableNifi();
//void sendPacketByte(u8 command, u8 data);

enum NifiCmd {
    NIFI_CMD_HOST=0,
    NIFI_CMD_CLIENT,
    NIFI_CMD_ACKNOWLEDGE,
    NIFI_CMD_INPUT,
    NIFI_CMD_TRANSFER_SRAM,

	NIFI_CMD_HOST_START_GAME,

    NIFI_CMD_FRAGMENT
};

int nifiSendPacket(u8 command, u8* data, u32 dataLen, bool acknowledge = false);

void nifiStop();

void nifiInterLinkMenu();

bool nifiIsHost();
bool nifiIsClient();
bool nifiIsLinked();


enum ClientStatus {
	CLIENT_NONE=0,
    CLIENT_IDLE=1,
    CLIENT_WAITING,
    CLIENT_CONNECTING,
    CLIENT_CONNECTED,
	CLIENT_INGAME
};
enum HostStatus {
    HOST_IDLE=5,
    HOST_WAITING,
    HOST_CONNECTED,
	HOST_INGAME
};

int nifiGetStatus();
void nifiSetStatus(int stat);

void nifiPause();
void nifiUnpause();

void nifiUpdateInput();
