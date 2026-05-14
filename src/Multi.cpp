
//The majority of code is taken from here.
//https://github.com/Drenn1/GameYob/blob/master/platform/ds/arm9/source/nifi.cpp
//with minor alterations taken from here.
//https://github.com/RocketRobz/NesDS/blob/master/arm9/source/multi.c

#include "Multi.h"


//#include "../srccommon/wifi_shared.h"
//#include "../srccommon/wifi_arm9.h"



#include "nifi.h"
#include <stdio.h>

#include "Debug.h"

#include "gba.h"

#include "Game.h"

#include "Sound.h"

#include "KeyControl.h"

#include "Draw.h"

#include "Profile.h"

#include "Pause.h"

#include "MyChar.h"
#include "Main.h"
#include "Debug.h"
#include "Frame.h"

#include "Flags.h"

#include "gba.h"

#ifndef JAPANESE
char magic1 = 'C';
char magic2 = 'D';
char magic3 = 'E';
char magic4 = '1';
#else
char magic1 = 'C';
char magic2 = 'D';
char magic3 = 'J';
char magic4 = '1';
#endif

void nifiLinkTypeMenu();
void nifiHostMenu();
void nifiClientMenu();

inline int INT_AT(u8* ptr) {
    return *ptr | *(ptr+1)<<8 | *(ptr+2)<<16 | *(ptr+3)<<24;
}
inline void INT_TO(u8* ptr, int i) {
    *ptr = i&0xff;
    *(ptr+1) = (i>>8)&0xff;
    *(ptr+2) = (i>>16)&0xff;
    *(ptr+3) = (i>>24)&0xff;
}

enum LinkType {
    LINK_CABLE=0,
    LINK_SGB
};
enum {
    HEADER_HOSTID       = 0x04, // u32
    HEADER_DATASIZE     = 0x08, // u32
    HEADER_COMMAND      = 0x0c, // u8
    HEADER_CHECKSUM     = 0x0d, // u8
    HEADER_ACKNOWLEDGE  = 0x0e, // u8
	HEADER_ACK_SEQ		= 0x0f, // u8
};
const int PACKET_HEADER_SIZE = 0x10;

const int CLIENT_FRAME_LAG = 2;
const int FRAGMENT_SIZE = 0x200;

const int OLD_INPUTS_BUFFER_SIZE = CLIENT_FRAME_LAG + 2;

u8* fragmentBuffer = NULL;
u8 lastFragment;

bool nifiEnabled=true;
bool nifiInitialized = false;
volatile bool packetAcknowledged;

volatile bool foundClient;
volatile bool foundHost;
bool isClient = false;
bool isHost = false;

int nifiFrameCounter;
int nifiLinkType;
volatile bool receivedSram;

bool nifiPaused = false;

int* nifiInputDest;      // Where input for this DS goes
int* nifiOtherInputDest; // Where input from other DS goes

int nifiConsecutiveWaitingFrames = 0;
int nifiConsecutiveWaitingFramesPrev = 0;

volatile int status = 0;
volatile u32 hostId;

//char linkedFilename[MAX_FILENAME_LEN];
char linkedRomTitle[20];

volatile int receivedInput[32];
volatile bool receivedInputReady[32];

int oldInputs[OLD_INPUTS_BUFFER_SIZE];
char curAckSeq = 0;


bool WaitForDisconnect()
{
	char* Text = "No data recieved for a while.";
	char* Text2 = "Please wait or press L+R+START to disconnect.";
	PutText(&grcGame, 0, WINDOW_HEIGHT - 40, Text, RGB(255, 255, 255));
	PutText(&grcGame, 0, WINDOW_HEIGHT - 16, Text2, RGB(255, 255, 255));
	//glEnd2D();
	////glFlush(0);
	VBlankIntrWait();
	//glBegin2D();
	scanKeys();
	int keys = keysHeld();
	if (keys & KEY_L && keys & KEY_R && keys & KEY_START) 
	{
   		nifiConsecutiveWaitingFrames = 0;
   		printf("Connection lost!\n");
   		nifiStop();
   		printf("Nifi turned off.\n");
		gDisconnectTimer = 180;
		return true;
   }
	return false;
}

u8 nifiGetChecksum(u8* data, u32 dataLen) {
    u8 checksum = 0;
    for (int i=0; i<dataLen+PACKET_HEADER_SIZE; i++) {
        if (i == HEADER_CHECKSUM)
            continue;
        checksum *= 7;
        checksum += data[i];
    }
    return checksum;
}

bool keyJustPressed(int a)
{
	return false;
}

int nifiSendPacket(u8 command, u8* data, u32 dataLen, bool acknowledge)
{
    if (!nifiEnabled || !nifiInitialized)
        return 1;

    int errcode = 0;

    if (command == NIFI_CMD_FRAGMENT || dataLen <= FRAGMENT_SIZE) {
        u8* buffer = (u8*)malloc(dataLen + PACKET_HEADER_SIZE);
        if (!buffer) {
            printf("Nifi out of memory\n");
            return 1;
        }

        buffer[0] = magic1;
        buffer[1] = magic2;
        buffer[2] = magic3;
        buffer[3] = magic4;

        INT_TO(buffer+HEADER_HOSTID, hostId);
        buffer[HEADER_COMMAND] = command;
        INT_TO(buffer+HEADER_DATASIZE, dataLen);
        buffer[HEADER_ACKNOWLEDGE] = (acknowledge ? 1 : 0);
		if(acknowledge) buffer[HEADER_ACK_SEQ] = ++curAckSeq;

        memcpy(buffer+PACKET_HEADER_SIZE, data, dataLen);
        buffer[HEADER_CHECKSUM] = nifiGetChecksum(buffer, dataLen);



        packetAcknowledged = false;
       // if (Wifi_RawTxFrame(dataLen+PACKET_HEADER_SIZE, 0x0014, (unsigned short *)buffer) != 0) {
         //   printf("Nifi send error\n");
           // errcode = 1;
        //}
        if (acknowledge) {
            int attemptCounter = 0;
            while (!packetAcknowledged) {
                int frameCounter = 0;
                while (!packetAcknowledged && frameCounter < 10) {
                    VBlankIntrWait();
                    frameCounter++;
                }
                if (!packetAcknowledged) {
                    if (attemptCounter > 20) {
                        if(WaitForDisconnect()) break;
                    }
                   // Wifi_RawTxFrame(dataLen+PACKET_HEADER_SIZE, 0x0014,
                     //   (unsigned short *)buffer);
                }
                attemptCounter++;
            }
        }

        free(buffer);
    }
    else {
        u8* buffer = (u8*)malloc(FRAGMENT_SIZE + 0x10);

        u8 numFragments = (dataLen+(FRAGMENT_SIZE-1))/FRAGMENT_SIZE;

        for (int i=0; i<numFragments; i++) {
            int fragmentSize = FRAGMENT_SIZE;
            if (i == numFragments-1) {
                fragmentSize = dataLen % FRAGMENT_SIZE;
                if (fragmentSize == 0)
                    fragmentSize = FRAGMENT_SIZE;
            }

            INT_TO(buffer, dataLen);
            buffer[4] = command;
            buffer[5] = numFragments;
            buffer[6] = i;
            memcpy(buffer+0x10, data+i*FRAGMENT_SIZE, fragmentSize);

            printf("SEND %d\n", i);
            if (nifiSendPacket(NIFI_CMD_FRAGMENT, buffer,
                        fragmentSize+0x10, acknowledge)) {
                errcode = 1;
                break;
            }
            VBlankIntrWait(); // Excessive?
            // Send it twice for safety
//             nifiSendPacket(NIFI_CMD_FRAGMENT, buffer,
//                     fragmentSize+0x10, acknowledge);
//             VBlankIntrWait();
        }

        free(buffer);
    }

    return errcode;
}

u32 packetHostId(u8* packet) {
    return (u32)INT_AT(packet+32+HEADER_HOSTID);
}
bool verifyPacket(u8* packet, int len) {
    if (len >= 32+PACKET_HEADER_SIZE &&
            packet[32+0] == magic1 &&
            packet[32+1] == magic2 &&
            packet[32+2] == magic3 &&
            packet[32+3] == magic4 &&
            ((isClient && status == CLIENT_WAITING) ||
             packetHostId(packet) == hostId)) {

         u8 checksum =
             nifiGetChecksum(packet+32, INT_AT(packet+32+HEADER_DATASIZE));
         if (checksum == packet[32+HEADER_CHECKSUM])
             return true;
         else
             printf("Nifi bad packet checksum\n");
    }
    return false;
}
u8 packetCommand(u8* packet) {
    return packet[32+HEADER_COMMAND];
}
u8* packetData(u8* packet) {
    return packet+32+PACKET_HEADER_SIZE;
}

void handlePacketCommand(int command, u8* data) {
    switch(command) {
        case NIFI_CMD_ACKNOWLEDGE:
			if(curAckSeq == data[0])
            	packetAcknowledged = true;
            break;
        case NIFI_CMD_CLIENT:
            if (isHost && status == HOST_WAITING) {
                foundClient = true;

                char* filename = (char*)(data+8);
                char* romTitle = (char*)(data+8+strlen(filename)+1);
                //strcpy(linkedFilename, filename);
                //strcpy(linkedRomTitle, romTitle);

                printf("Link romTitle: %s\n", linkedRomTitle);
                //printf("Link filename: %s\n", linkedFilename);
            }
            break;

        case NIFI_CMD_INPUT_FOR_CLIENT:
		case NIFI_CMD_INPUT_FOR_HOST:
            if ((isHost && command == NIFI_CMD_INPUT_FOR_HOST) || (isClient && command == NIFI_CMD_INPUT_FOR_CLIENT)) {
                int num = data[0];
                int frame1 = INT_AT(data+1);

                if (nifiConsecutiveWaitingFrames >= (10000 / 60))
                    printf("Received packet: %x\n", frame1);

                for (int i=0; i<num; i++) {
                    int frame = frame1+i;

                    if (frame >= gCounter) {
                        if (receivedInputReady[frame&31]) {
                            if (receivedInput[frame&31] != INT_AT(data+5+(i*4)))
                                printf("MISMATCH %x, %d, %d\n", frame, receivedInput[frame&31], INT_AT(data+5+(i*4)));
                        }
                        else {
                            receivedInputReady[frame&31] = true;
                            receivedInput[frame&31] = INT_AT(data+5+(i*4));
                        }
                    }
                }
            }
            break;
        case NIFI_CMD_TRANSFER_SRAM:
            {
				if(isClient){
                /*if (nifiLinkType == LINK_SGB)
                    memcpy(gameboy->externRam, data, gameboy->getNumSramBanks()*0x2000);
               	else if (gb2)
                    memcpy(gb2->externRam, data, gb2->getNumSramBanks()*0x2000);
                else
                    printf("GB2 NOT INITIALIZED!\n");*/
					memcpy(&profile, data, sizeof(profile));
               		printf("Received SRAM.\n");
                	receivedSram = true;
				}
            }
            break;

            // A command broken up into multiple packets

		case NIFI_CMD_HOST_START_GAME:
			if(isClient && status == CLIENT_CONNECTED){
				status = CLIENT_INGAME;
				gStartingNetplay = (receivedSram ? NETPLAY_START_LOAD : NETPLAY_START_NORMAL);
				gCounter = 0;

				gb50Fps = data[0];
				gDebug.bEnabled = data[1];
				gRespawnEnabled = data[2];
				gEnemyHPMultiplier = data[3];
				gEnemyDamageMultiplier = data[4];

				memcpy(&gSkipFlag, &data[5], sizeof(gSkipFlag));

				printf("\ndebug enabled: %d\n", gDebug.bEnabled);
				printf("50fps: %d\n", gb50Fps);
				printf("respawn: %d\n", gRespawnEnabled);
				printf("HP: %d\n", gEnemyHPMultiplier);
				printf("DMG: %d\n\n", gEnemyDamageMultiplier);

				printf("Client: starting netplay\n");

				ResetDebugOptions();
			}
			break;

		case NIFI_CMD_DISCONNECT:
			if(isClient)
			{
				nifiStop();
				gDisconnectTimer = 180;
			}
        case NIFI_CMD_FRAGMENT:
            {
                u32 totalSize = INT_AT(data);
                u8 command = data[4];
                u8 numFragments = data[5];
                u8 fragment = data[6];

                int fragmentSize = FRAGMENT_SIZE;
                if (fragment == numFragments-1) {
                    fragmentSize = totalSize % FRAGMENT_SIZE;
                    if (fragmentSize == 0)
                        fragmentSize = FRAGMENT_SIZE;
                }

                if (fragmentBuffer == NULL && fragment != 0) {
                    printf("NULL Buffer.\n");
                    return;
                }
                if (fragment == 0) {
                    if (fragmentBuffer != NULL)
                        free(fragmentBuffer);
                    fragmentBuffer = (u8*)malloc(totalSize);
                    if (fragmentBuffer == 0) {
                        printf("Nifi not enough memory\n");
                        return;
                    }
                }
                else if (lastFragment > fragment) {
                    if (fragmentBuffer != NULL) {
                        free(fragmentBuffer);
                        fragmentBuffer = NULL;
                    }
                    printf("Fragment mismatch\n");
                    lastFragment = -1;
                    return;
                }

                if (fragment == 0 || lastFragment+1 == fragment)
                    memcpy(fragmentBuffer+fragment*FRAGMENT_SIZE, data+0x10, fragmentSize);

                lastFragment = fragment;

                if (fragment == numFragments-1) {
                    handlePacketCommand(command, fragmentBuffer);
                    free(fragmentBuffer);
                    fragmentBuffer = NULL;
                    lastFragment = -1;
                }
            }
            break;
    }
}
EWRAM_DATA u32 pkt[4096/2];

void packetHandler(int packetID, int readlength)
{

    static u8* packet = (u8*)pkt;
    // static int bytesRead = 0; // Not used

    // Wifi_RxRawReadPacket:  Allows user code to read a packet from within the WifiPacketHandler function
    //  long packetID:		a non-unique identifier which locates the packet specified in the internal buffer
    //  long readlength:		number of bytes to read (actually reads (number+1)&~1 bytes)
    //  unsigned short * data:	location for the data to be read into
    
	// bytesRead = Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)data); // Not used
	//Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)packet);
	
    if (verifyPacket(packet, readlength)) {
        if (*(packet+32+HEADER_ACKNOWLEDGE))
            nifiSendPacket(NIFI_CMD_ACKNOWLEDGE, packet+32+HEADER_ACK_SEQ, 1, false);
        u8* data = packetData(packet);

        if (packetCommand(packet) == NIFI_CMD_HOST) {
            if (isClient && status == CLIENT_WAITING) {
                foundHost = true;
                hostId = packetHostId(packet);

                nifiLinkType = data[0];
				nifiChannel = data[4];


            }
        }
        else
            handlePacketCommand(packetCommand(packet), data);
    }
}


void Timer_10ms(void) {
	//Wifi_Timer(10);
}

void nifiStop() {

    isClient = false;
    isHost = false;
    disableNifi();
    nifiUnpause();
	gStartingNetplay = 0;

	SetFrameTargetMyChar(gFrame.wait);
}

static int wifiInited = false;

void enableNifi()
{

}

void disableNifi() {

}

void nifiInterLinkMenu() {
    int selection = 0;
}

void nifiLinkTypeMenu() {
    int selection = 0;

}

void nifiSendSram() {
    //nifiSendPacket(NIFI_CMD_TRANSFER_SRAM, gameboy->externRam,
    //        gameboy->getNumSramBanks()*0x2000, false);
    printf("Sent SRAM.\n");
}

int nifiReceiveSram() {
    nifiConsecutiveWaitingFrames = 0;
    while (!receivedSram) {
        VBlankIntrWait();
        nifiConsecutiveWaitingFrames++;
        if (nifiConsecutiveWaitingFrames >= 60*5) {
            return 1;
        }
    }
    return 0;
}

int loadOtherRom() {
    /*if (nifiLinkType != LINK_CABLE)
        return 0;
    if (strcmp(gameboy->getRomFile()->getRomTitle(), linkedRomTitle) == 0)
        return 0;
    if (!file_exists(linkedFilename))
        return 1;

    gameboy->getRomFile()->halfMemoryMode();

    if (gb2->getRomFile() != NULL && gameboy->getRomFile() != gb2->getRomFile())
        delete gb2->getRomFile();
    gb2->setRomFile(new RomFile(linkedFilename, true));

    // Init again since we switched out the rom
    gb2->init();
    // run loadSave() to make sure externRam is sized correctly
    gb2->loadSave(-1);
*/
    return 0;
}

int nifiStartLink() {
    bool waitForSram = false;
    bool sendSram = false;

    nifiFrameCounter = -1;

	memset(oldInputs, 0, sizeof(oldInputs));
	memset((void*)&receivedInputReady, 0, sizeof(receivedInputReady));
	memset((void*)&receivedInput, 0, sizeof(receivedInput));

   // mgr_reset();
    if (nifiLinkType == LINK_CABLE) {
        printf("Start Gb2\n");
       // mgr_startGb2(NULL);
    //    if (loadOtherRom() != 0) {
    //        printf("Error loading \"%s\".\n");
    //        return 1;
    }

    if (isHost) {
       // if (nifiLinkType == LINK_CABLE)
       //     mgr_setInternalClockGb(gameboy);

        // Fill in first few frames of client's input
        /*for (int i=0; i<OLD_INPUTS_BUFFER_SIZE; i++) {
            receivedInputReady[i] = true;
            receivedInput[i] = 0xff;
        }*/

        // Set input destinations
       /* if (nifiLinkType == LINK_SGB) {
            nifiInputDest = &gameboy->controllers[0];
            nifiOtherInputDest = &gameboy->controllers[1];
        }
        else if (nifiLinkType == LINK_CABLE) {
            nifiInputDest = &gameboy->controllers[0];
            nifiOtherInputDest = &gb2->controllers[0];
        }*/

        nifiInputDest = &gKey;
        nifiOtherInputDest = &gKeyP2;

        // Sram transfers
        sendSram = true;
    }
    else if (isClient) {
       // if (nifiLinkType == LINK_CABLE)
         //   mgr_setInternalClockGb(gb2);

        // First few frames of input are skipped, so fill them in
        //for (int i=0; i<OLD_INPUTS_BUFFER_SIZE; i++)
        //    oldInputs[i] = 0xff;

        // Set input destinations
       // if (nifiLinkType == LINK_SGB) {
       //     nifiInputDest = &gameboy->controllers[1];
       //     nifiOtherInputDest = &gameboy->controllers[0];
       // }
      //  else if (nifiLinkType == LINK_CABLE) {
      //      nifiInputDest = &gameboy->controllers[0];
      //      nifiOtherInputDest = &gb2->controllers[0];
	    // }

		nifiInputDest = &gKeyP2;
        nifiOtherInputDest = &gKey;
        // Sram transfers
        waitForSram = true;
    //    if (nifiLinkType == LINK_CABLE)
    //        sendSram = true;
    }

    /*if (isHost) {
        if (sendSram && gameboy->getNumSramBanks())
            nifiSendSram();
        VBlankIntrWait();
        if (waitForSram && gameboy->getNumSramBanks()) {
            if (nifiReceiveSram())
                return 1;
        }
    }
    else {
        if (waitForSram && gameboy->getNumSramBanks()) {
            if (nifiReceiveSram())
                return 1;
        }
        if (sendSram && gameboy->getNumSramBanks())
            nifiSendSram();
    }

    nifiConsecutiveWaitingFrames = 0;*/
    return 0;
}


bool nifiIsHost() { return isHost; }
bool nifiIsClient() { return isClient; }
bool nifiIsLinked() { return isHost || isClient; }

int nifiGetStatus()
{
	return status;
}

void nifiSetStatus(int stat)
{
	status = stat;
}

int nifiWasPaused = -1;
void nifiPause() {
    /*if (nifiWasPaused == -1) {
        nifiWasPaused = mgr_isPaused();
    }
    mgr_pause();*/
	nifiPaused = true;
}
void nifiUnpause() {
   /* if (nifiWasPaused == -1 || !nifiWasPaused) {
        mgr_unpause();
    }
    nifiWasPaused = -1;*/
	nifiPaused = false;
}

