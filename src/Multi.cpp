#include "Multi.h"

#include <nds.h>
//#include "../srccommon/wifi_shared.h"
//#include "../srccommon/wifi_arm9.h"



#include "nifi.h"
#include <stdio.h>
#include "dswifi9.h"

#include "nds.h"

#include "Game.h"

#include "Sound.h"

#include "KeyControl.h"

#include "Draw.h"

char magic1 = 'Y';
char magic2 = 'O';
char magic3 = 'B';
char magic4 = 'P';

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

        memcpy(buffer+PACKET_HEADER_SIZE, data, dataLen);

        buffer[HEADER_CHECKSUM] = nifiGetChecksum(buffer, dataLen);

        packetAcknowledged = false;
        if (Wifi_RawTxFrame(dataLen+PACKET_HEADER_SIZE, 0x0014, (unsigned short *)buffer) != 0) {
            printf("Nifi send error\n");
            errcode = 1;
        }
        if (acknowledge) {
            int attemptCounter = 0;
            while (!packetAcknowledged) {
                int frameCounter = 0;
                while (!packetAcknowledged && frameCounter < 10) {
                    swiWaitForVBlank();
                    frameCounter++;
                }
                if (!packetAcknowledged) {
                    if (attemptCounter > 10) {
                        errcode = 1;
                        printf("Connection lost.\n");
                        nifiStop();
                        break;
                    }
                    else
                        Wifi_RawTxFrame(dataLen+PACKET_HEADER_SIZE, 0x0014,
                                (unsigned short *)buffer);
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
            swiWaitForVBlank(); // Excessive?
            // Send it twice for safety
//             nifiSendPacket(NIFI_CMD_FRAGMENT, buffer,
//                     fragmentSize+0x10, acknowledge);
//             swiWaitForVBlank();
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

                if (nifiConsecutiveWaitingFrames >= (1000 / 60))
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
                /*if (nifiLinkType == LINK_SGB)
                    memcpy(gameboy->externRam, data, gameboy->getNumSramBanks()*0x2000);
                else if (gb2)
                    memcpy(gb2->externRam, data, gb2->getNumSramBanks()*0x2000);
                else
                    printf("GB2 NOT INITIALIZED!\n");*/
                printf("Received SRAM.\n");
                receivedSram = true;
            }
            break;

            // A command broken up into multiple packets

		case NIFI_CMD_HOST_START_GAME:
			if(isClient && status == CLIENT_CONNECTED){
				status = CLIENT_INGAME;
				gStartingNetplay = true;
				gCounter = 0;
				printf("Client: starting netplay\n");
			}
			break;
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

void packetHandler(int packetID, int readlength)
{
    static u32 pkt[4096/2];
    static u8* packet = (u8*)pkt;
    // static int bytesRead = 0; // Not used

    // Wifi_RxRawReadPacket:  Allows user code to read a packet from within the WifiPacketHandler function
    //  long packetID:		a non-unique identifier which locates the packet specified in the internal buffer
    //  long readlength:		number of bytes to read (actually reads (number+1)&~1 bytes)
    //  unsigned short * data:	location for the data to be read into
    
	// bytesRead = Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)data); // Not used
	Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)packet);
	
    if (verifyPacket(packet, readlength)) {
        if (*(packet+32+HEADER_ACKNOWLEDGE))
            nifiSendPacket(NIFI_CMD_ACKNOWLEDGE, 0, 0, false);
        u8* data = packetData(packet);

        if (packetCommand(packet) == NIFI_CMD_HOST) {
            if (isClient && status == CLIENT_WAITING) {
                foundHost = true;
                hostId = packetHostId(packet);
                nifiLinkType = data[0];

                char* filename = (char*)(data+8);
                char* romTitle = (char*)(data+8+strlen(filename)+1);
              //  strcpy(linkedFilename, filename);
                strcpy(linkedRomTitle, romTitle);
            }
        }
        else
            handlePacketCommand(packetCommand(packet), data);
    }
}


void Timer_10ms(void) {
	Wifi_Timer(10);
}

void nifiStop() {
    isClient = false;
    isHost = false;
    disableNifi();
    nifiUnpause();
}

void enableNifi()
{
    if (nifiInitialized)
        return;

	//Wifi_SetRawPacketMode(PACKET_MODE_NIFI);

	Wifi_InitDefault(false);

// Wifi_SetPromiscuousMode: Allows the DS to enter or leave a "promsicuous" mode, in which 
//   all data that can be received is forwarded to the arm9 for user processing.
//   Best used with Wifi_RawSetPacketHandler, to allow user code to use the data
//   (well, the lib won't use 'em, so they're just wasting CPU otherwise.)
//  int enable:  0 to disable promiscuous mode, nonzero to engage
	Wifi_SetPromiscuousMode(1);

// Wifi_RawSetPacketHandler: Set a handler to process all raw incoming packets
//  WifiPacketHandler wphfunc:  Pointer to packet handler (see WifiPacketHandler definition for more info)
	Wifi_RawSetPacketHandler(packetHandler);

// Wifi_SetChannel: If the wifi system is not connected or connecting to an access point, instruct
//   the chipset to change channel
//  int channel: the channel to change to, in the range of 1-13
	Wifi_SetChannel(10);

	if(1) {
		//for secial configuration for wifi
		irqDisable(IRQ_TIMER3);
		irqSet(IRQ_TIMER3, Timer_10ms); // replace timer IRQ
		// re-set timer3
		TIMER3_CR = 0;
		TIMER3_DATA = -(6553 / 5); // 6553.1 * 256 / 5 cycles = ~10ms;
		TIMER3_CR = 0x00C2; // enable, irq, 1/256 clock
		irqEnable(IRQ_TIMER3);
	}


// Wifi_EnableWifi: Instructs the ARM7 to go into a basic "active" mode, not actually
//   associated to an AP, but actively receiving and potentially transmitting
	Wifi_EnableWifi();

    nifiInitialized = true;
}

void disableNifi() {
    if (!nifiInitialized)
        return;

    Wifi_DisableWifi();
    nifiInitialized = false;

	status = CLIENT_NONE;
}

void nifiInterLinkMenu() {
    int selection = 0;
    receivedSram = false;

    
}

void nifiLinkTypeMenu() {
    int selection = 0;

}

void nifiSendSram() {
    //nifiSendPacket(NIFI_CMD_TRANSFER_SRAM, gameboy->externRam,
            //gameboy->getNumSramBanks()*0x2000, false);
    printf("Sent SRAM.\n");
}

int nifiReceiveSram() {
    nifiConsecutiveWaitingFrames = 0;
    while (!receivedSram) {
        swiWaitForVBlank();
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
        for (int i=0; i<CLIENT_FRAME_LAG; i++) {
            receivedInputReady[i] = true;
            receivedInput[i] = 0xff;
        }

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
        for (int i=0; i<OLD_INPUTS_BUFFER_SIZE; i++)
            oldInputs[i] = 0xff;

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
        swiWaitForVBlank();
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

void nifiHostMenu() {
    enableNifi();
    //clearConsole();

    foundClient = false;
    isHost = true;
    isClient = false;
    status = HOST_WAITING;
    hostId = rand();

    printf("Waiting for client...\n");
    printf("Host ID: %d\n\n", hostId);
}

void nifiClientMenu() {
    enableNifi();
    consoleClear();
    printf("Waiting for host...\n\n");

    foundHost = false;
    isClient = true;
    isHost = false;
    status = CLIENT_WAITING;
}

void nifiHostWait()
{
	static int count = 0;
	if (!foundClient) {

        int bufferSize = 8 + 20 + 1;
        u8 buffer[bufferSize];

        buffer[0] = nifiLinkType;
		if(count++ % 5 == 0)
        	nifiSendPacket(NIFI_CMD_HOST, buffer, bufferSize, false);
    }

	if (foundClient && status != HOST_CONNECTED && status != HOST_INGAME) {
        printf("Found client.\n");
        status = HOST_CONNECTED;

		if (nifiStartLink() != 0)
       		printf("Link failed.\n");
    	else
        	printf("Starting link.\n");

   			for (int i=0; i<10; i++) swiWaitForVBlank();
			PlaySoundObject(65, 1);

    }


}

void nifiClientWait()
{
	if (foundHost && status != CLIENT_CONNECTED && status != CLIENT_INGAME) {
        swiWaitForVBlank();

		int bufferSize = 8 + 20 + 1;
        u8 buffer[bufferSize];

        nifiSendPacket(NIFI_CMD_CLIENT, buffer, bufferSize, false);

        printf("Connected to host.\nHost Id: %d\n", hostId);
        status = CLIENT_CONNECTED;
		if (nifiStartLink() != 0)
            printf("Link failed.\n");
        else
            printf("Starting link.\n");

		for (int i=0; i<10; i++) swiWaitForVBlank();
		PlaySoundObject(65, 1);

    }
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

void nifiUpdateInput() {
    int* inputDest;
    int* otherInputDest = nifiOtherInputDest;
    if (nifiIsLinked()) ///TODO
        inputDest = nifiInputDest;
    else
        inputDest = &gKey;

    u32 bfr[4*4];
    u8* buffer = (u8*)bfr;

    u32 actualFrame = gCounter;
    u32 inputFrame = gCounter;
    bool frameHasPassed = nifiFrameCounter != gCounter;
    if (nifiFrameCounter == -1)
        printf("Start at %d", gCounter);
    if (frameHasPassed && nifiFrameCounter > 0)
        receivedInputReady[(nifiFrameCounter-1)&31] = false;
    nifiFrameCounter = gCounter;

    if (nifiIsClient())
        inputFrame += CLIENT_FRAME_LAG;

    u8 olderInput = oldInputs[OLD_INPUTS_BUFFER_SIZE-CLIENT_FRAME_LAG];

    if (nifiIsLinked()) {
        if (frameHasPassed) {
            for (int i=0; i<OLD_INPUTS_BUFFER_SIZE-1; i++)
                oldInputs[i] = oldInputs[i+1];
            oldInputs[OLD_INPUTS_BUFFER_SIZE-1] = gKey; //TODO
        }

        // Send input to other ds
        INT_TO(buffer+1, inputFrame-OLD_INPUTS_BUFFER_SIZE+1);
        for (int i=0; i<OLD_INPUTS_BUFFER_SIZE; i++)
            INT_TO(buffer+5+(i*4), oldInputs[i]);
        buffer[0] = OLD_INPUTS_BUFFER_SIZE;
		u8 type = isHost ? NIFI_CMD_INPUT_FOR_CLIENT : NIFI_CMD_INPUT_FOR_HOST;
        nifiSendPacket(type, buffer, 5+(OLD_INPUTS_BUFFER_SIZE*4), false);

        // Set other controller's input
		while (!receivedInputReady[actualFrame&31])
		{
			swiDelay(10000);
			nifiConsecutiveWaitingFrames++;
			if(nifiConsecutiveWaitingFrames % (4000 / 60) == 0)
			{
				printf("NIFI NOT READY %x\n", nifiFrameCounter);
			}
			if(nifiConsecutiveWaitingFrames >= 7 * 1000)
			{
				char* Text = "No input recieved for a while.";
				char* Text2 = "Please wait or press L+R+START to disconnect.";
				PutText(&grcGame, 0, WINDOW_HEIGHT - 40, Text, RGB(255, 255, 255));
				PutText(&grcGame, 0, WINDOW_HEIGHT - 16, Text2, RGB(255, 255, 255));
				glEnd2D();
				glFlush(0);
				swiWaitForVBlank();
				glBegin2D();
				scanKeys();
				int keys = keysHeld();
				if (keys & KEY_L && keys & KEY_R && keys & KEY_START) 
				{
            		nifiConsecutiveWaitingFrames = 0;
            		printf("Connection lost!\n");
           	 		nifiStop();
           			printf("Nifi turned off.\n");
					break;
        		}
			}

			if(nifiConsecutiveWaitingFrames % 50 == 0)
			{
				nifiSendPacket(type, buffer, 5+(OLD_INPUTS_BUFFER_SIZE*4), false);
			}

		}
		
        *otherInputDest = receivedInput[actualFrame&31];
        nifiUnpause();
		nifiConsecutiveWaitingFramesPrev = nifiConsecutiveWaitingFrames;
        nifiConsecutiveWaitingFrames = 0;
        

    }

    if (!nifiIsLinked() || nifiIsHost()) {
        *inputDest = gKey;     /////TODO
    }
    else if (nifiIsClient()) {
        *inputDest = olderInput;
    }
}


int Wifi_RawTxFrameNIFI(u16 datalen, u16 rate, u16 * data) {
/*	int base,framelen, hdrlen, writelen;
	int copytotal, copyexpect;
	u16 framehdr[6 + 12 + 2];
	framelen=datalen + 8 + (WifiData->wepmode7 ? 4 : 0);

	if(framelen + 40>Wifi_TxBufferWordsAvailable()*2) { // error, can't send this much!
		return -1; //?
	}

	framehdr[0]=0;
	framehdr[1]=0;
	framehdr[2]=0;
	framehdr[3]=0;
	framehdr[4]=0; // rate, will be filled in by the arm7.
	hdrlen=18;
	framehdr[6]=0x0208;
	framehdr[7]=0;

	// MACs.
	memset(framehdr + 8, 0xFF, 18);

	if(WifiData->wepmode7)
	{
		framehdr[6] |=0x4000;
		hdrlen=20;
	}
	framehdr[17] = 0;
	framehdr[18] = 0; // wep IV, will be filled in if needed on the arm7 side.
	framehdr[19] = 0;

	framehdr[5]=framelen+hdrlen * 2 - 12 + 4;
	copyexpect= ((framelen+hdrlen * 2 - 12 + 4) + 12 - 4 + 1)/2;
	copytotal=0;

	WifiData->stats[WSTAT_TXQUEUEDPACKETS]++;
	WifiData->stats[WSTAT_TXQUEUEDBYTES] += framelen + hdrlen * 2;

	base = WifiData->txbufOut;
	Wifi_TxBufferWrite(base,hdrlen,framehdr);
	base += hdrlen;
	copytotal += hdrlen;
	if(base >= (WIFI_TXBUFFER_SIZE / 2)) base -= WIFI_TXBUFFER_SIZE / 2;

	// add LLC header
	framehdr[0]=0xAAAA;
	framehdr[1]=0x0003;
	framehdr[2]=0x0000;
	unsigned short protocol = 0x08FE;
	framehdr[3] = ((protocol >> 8) & 0xFF) | ((protocol << 8) & 0xFF00);

	Wifi_TxBufferWrite(base, 4, framehdr);
	base += 4;
	copytotal += 4;
	if(base>=(WIFI_TXBUFFER_SIZE/2)) base -= WIFI_TXBUFFER_SIZE/2;

	writelen = datalen;
	if(writelen) {
		Wifi_TxBufferWrite(base,(writelen+1)/2,data);
		base += (writelen + 1) / 2;
		copytotal += (writelen + 1) / 2;
		if(base>=(WIFI_TXBUFFER_SIZE/2)) base -= WIFI_TXBUFFER_SIZE/2;
	}
	if(WifiData->wepmode7)
	{ // add required extra bytes
		base += 2;
		copytotal += 2;
		if(base >= (WIFI_TXBUFFER_SIZE / 2)) base -= WIFI_TXBUFFER_SIZE / 2;
	}
	WifiData->txbufOut = base; // update fifo out pos, done sending packet.

	if(copytotal!=copyexpect)
	{
		//corrupted frame sent
	}
	if(synchandler) synchandler();
	return 0;*/
	return 0;
}