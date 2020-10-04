#include <cstring>
#include <stdlib.h>

//File references
#include "../incbin.h"
#define INCBIN_PREFIX g

typedef struct {
	const unsigned char *data;
	unsigned int size;
	const char *path;
} FILEREF;

#include "fopen.h"


// we are in the "build" folder
INCBIN(Armspng, "../game_english/data/Arms.png");
INCBIN(ArmsImagepng, "../game_english/data/ArmsImage.png");
INCBIN(ArmsItemtsc, "../game_english/data/ArmsItem.tsc");
INCBIN(Bulletpng, "../game_english/data/Bullet.png");
INCBIN(Caretpng, "../game_english/data/Caret.png");
INCBIN(Credittsc, "../game_english/data/Credit.tsc");
INCBIN(Facepng, "../game_english/data/Face.png");
INCBIN(Fadepng, "../game_english/data/Fade.png");
INCBIN(Headtsc, "../game_english/data/Head.tsc");
INCBIN(ICON_MINIpng, "../game_english/data/ICON_MINI.png");
INCBIN(ItemImagepng, "../game_english/data/ItemImage.png");
INCBIN(Loadingpng, "../game_english/data/Loading.png");
INCBIN(MyCharpng, "../game_english/data/MyChar.png");
INCBIN(StageImagepng, "../game_english/data/StageImage.png");
INCBIN(StageSelecttsc, "../game_english/data/StageSelect.tsc");
INCBIN(TextBoxpng, "../game_english/data/TextBox.png");
INCBIN(Titlepng, "../game_english/data/Title.png");
INCBIN(bk0png, "../game_english/data/bk0.png");
INCBIN(bkBlackpng, "../game_english/data/bkBlack.png");
INCBIN(bkBluepng, "../game_english/data/bkBlue.png");
INCBIN(bkFallpng, "../game_english/data/bkFall.png");
INCBIN(bkFogpng, "../game_english/data/bkFog.png");
INCBIN(bkGardpng, "../game_english/data/bkGard.png");
INCBIN(bkGraypng, "../game_english/data/bkGray.png");
INCBIN(bkGreenpng, "../game_english/data/bkGreen.png");
INCBIN(bkMazepng, "../game_english/data/bkMaze.png");
INCBIN(bkMoonpng, "../game_english/data/bkMoon.png");
INCBIN(bkRedpng, "../game_english/data/bkRed.png");
INCBIN(bkWaterpng, "../game_english/data/bkWater.png");
INCBIN(castspng, "../game_english/data/casts.png");
INCBIN(npctbl, "../game_english/data/npc.tbl");
INCBIN(smalfontpng, "../game_english/data/smalfont.png");
INCBIN(stagetbl, "../game_english/data/stage.tbl");
INCBIN(Npc0png, "../game_english/data/Npc/Npc0.png");
INCBIN(NpcAlmo1png, "../game_english/data/Npc/NpcAlmo1.png");
INCBIN(NpcAlmo2png, "../game_english/data/Npc/NpcAlmo2.png");
INCBIN(NpcBallospng, "../game_english/data/Npc/NpcBallos.png");
INCBIN(NpcBllgpng, "../game_english/data/Npc/NpcBllg.png");
INCBIN(NpcCemetpng, "../game_english/data/Npc/NpcCemet.png");
INCBIN(NpcCentpng, "../game_english/data/Npc/NpcCent.png");
INCBIN(NpcCurlypng, "../game_english/data/Npc/NpcCurly.png");
INCBIN(NpcDarkpng, "../game_english/data/Npc/NpcDark.png");
INCBIN(NpcDrpng, "../game_english/data/Npc/NpcDr.png");
INCBIN(NpcEggs1png, "../game_english/data/Npc/NpcEggs1.png");
INCBIN(NpcEggs2png, "../game_english/data/Npc/NpcEggs2.png");
INCBIN(NpcFrogpng, "../game_english/data/Npc/NpcFrog.png");
INCBIN(NpcGuestpng, "../game_english/data/Npc/NpcGuest.png");
INCBIN(NpcHellpng, "../game_english/data/Npc/NpcHell.png");
INCBIN(NpcHeripng, "../game_english/data/Npc/NpcHeri.png");
INCBIN(NpcIronHpng, "../game_english/data/Npc/NpcIronH.png");
INCBIN(NpcIslandpng, "../game_english/data/Npc/NpcIsland.png");
INCBIN(NpcKingspng, "../game_english/data/Npc/NpcKings.png");
INCBIN(NpcMazepng, "../game_english/data/Npc/NpcMaze.png");
INCBIN(NpcMizapng, "../game_english/data/Npc/NpcMiza.png");
INCBIN(NpcMoonpng, "../game_english/data/Npc/NpcMoon.png");
INCBIN(NpcOmgpng, "../game_english/data/Npc/NpcOmg.png");
INCBIN(NpcPlantpng, "../game_english/data/Npc/NpcPlant.png");
INCBIN(NpcPresspng, "../game_english/data/Npc/NpcPress.png");
INCBIN(NpcPriestpng, "../game_english/data/Npc/NpcPriest.png");
INCBIN(NpcRavilpng, "../game_english/data/Npc/NpcRavil.png");
INCBIN(NpcRedpng, "../game_english/data/Npc/NpcRed.png");
INCBIN(NpcRegupng, "../game_english/data/Npc/NpcRegu.png");
INCBIN(NpcSandpng, "../game_english/data/Npc/NpcSand.png");
INCBIN(NpcStreampng, "../game_english/data/Npc/NpcStream.png");
INCBIN(NpcSympng, "../game_english/data/Npc/NpcSym.png");
INCBIN(NpcToropng, "../game_english/data/Npc/NpcToro.png");
INCBIN(NpcTwinDpng, "../game_english/data/Npc/NpcTwinD.png");
INCBIN(NpcWeedpng, "../game_english/data/Npc/NpcWeed.png");
INCBIN(NpcXpng, "../game_english/data/Npc/NpcX.png");
INCBIN(CREDIT01png, "../game_english/data/BITMAP/CREDIT01.png");
INCBIN(CREDIT02png, "../game_english/data/BITMAP/CREDIT02.png");
INCBIN(CREDIT03png, "../game_english/data/BITMAP/CREDIT03.png");
INCBIN(CREDIT04png, "../game_english/data/BITMAP/CREDIT04.png");
INCBIN(CREDIT05png, "../game_english/data/BITMAP/CREDIT05.png");
INCBIN(CREDIT06png, "../game_english/data/BITMAP/CREDIT06.png");
INCBIN(CREDIT07png, "../game_english/data/BITMAP/CREDIT07.png");
INCBIN(CREDIT08png, "../game_english/data/BITMAP/CREDIT08.png");
INCBIN(CREDIT09png, "../game_english/data/BITMAP/CREDIT09.png");
INCBIN(CREDIT10png, "../game_english/data/BITMAP/CREDIT10.png");
INCBIN(CREDIT11png, "../game_english/data/BITMAP/CREDIT11.png");
INCBIN(CREDIT12png, "../game_english/data/BITMAP/CREDIT12.png");
INCBIN(CREDIT14png, "../game_english/data/BITMAP/CREDIT14.png");
INCBIN(CREDIT15png, "../game_english/data/BITMAP/CREDIT15.png");
INCBIN(CREDIT16png, "../game_english/data/BITMAP/CREDIT16.png");
INCBIN(CREDIT17png, "../game_english/data/BITMAP/CREDIT17.png");
INCBIN(CREDIT18png, "../game_english/data/BITMAP/CREDIT18.png");
INCBIN(PIXELpng, "../game_english/data/BITMAP/PIXEL.png");
INCBIN(PIXEL_JPpng, "../game_english/data/BITMAP/PIXEL_JP.png");
INCBIN(001raw, "../game_english/data/Wave/001.raw");
INCBIN(002raw, "../game_english/data/Wave/002.raw");
INCBIN(003raw, "../game_english/data/Wave/003.raw");
INCBIN(004raw, "../game_english/data/Wave/004.raw");
INCBIN(005raw, "../game_english/data/Wave/005.raw");
INCBIN(006raw, "../game_english/data/Wave/006.raw");
INCBIN(011raw, "../game_english/data/Wave/011.raw");
INCBIN(012raw, "../game_english/data/Wave/012.raw");
INCBIN(014raw, "../game_english/data/Wave/014.raw");
INCBIN(015raw, "../game_english/data/Wave/015.raw");
INCBIN(016raw, "../game_english/data/Wave/016.raw");
INCBIN(017raw, "../game_english/data/Wave/017.raw");
INCBIN(018raw, "../game_english/data/Wave/018.raw");
INCBIN(020raw, "../game_english/data/Wave/020.raw");
INCBIN(021raw, "../game_english/data/Wave/021.raw");
INCBIN(022raw, "../game_english/data/Wave/022.raw");
INCBIN(023raw, "../game_english/data/Wave/023.raw");
INCBIN(024raw, "../game_english/data/Wave/024.raw");
INCBIN(025raw, "../game_english/data/Wave/025.raw");
INCBIN(026raw, "../game_english/data/Wave/026.raw");
INCBIN(027raw, "../game_english/data/Wave/027.raw");
INCBIN(028raw, "../game_english/data/Wave/028.raw");
INCBIN(029raw, "../game_english/data/Wave/029.raw");
INCBIN(030raw, "../game_english/data/Wave/030.raw");
INCBIN(031raw, "../game_english/data/Wave/031.raw");
INCBIN(032raw, "../game_english/data/Wave/032.raw");
INCBIN(033raw, "../game_english/data/Wave/033.raw");
INCBIN(034raw, "../game_english/data/Wave/034.raw");
INCBIN(035raw, "../game_english/data/Wave/035.raw");
INCBIN(037raw, "../game_english/data/Wave/037.raw");
INCBIN(038raw, "../game_english/data/Wave/038.raw");
INCBIN(039raw, "../game_english/data/Wave/039.raw");
INCBIN(040raw, "../game_english/data/Wave/040.raw");
INCBIN(041raw, "../game_english/data/Wave/041.raw");
INCBIN(042raw, "../game_english/data/Wave/042.raw");
INCBIN(043raw, "../game_english/data/Wave/043.raw");
INCBIN(044raw, "../game_english/data/Wave/044.raw");
INCBIN(045raw, "../game_english/data/Wave/045.raw");
INCBIN(046raw, "../game_english/data/Wave/046.raw");
INCBIN(047raw, "../game_english/data/Wave/047.raw");
INCBIN(048raw, "../game_english/data/Wave/048.raw");
INCBIN(049raw, "../game_english/data/Wave/049.raw");
INCBIN(050raw, "../game_english/data/Wave/050.raw");
INCBIN(051raw, "../game_english/data/Wave/051.raw");
INCBIN(052raw, "../game_english/data/Wave/052.raw");
INCBIN(053raw, "../game_english/data/Wave/053.raw");
INCBIN(054raw, "../game_english/data/Wave/054.raw");
INCBIN(055raw, "../game_english/data/Wave/055.raw");
INCBIN(056raw, "../game_english/data/Wave/056.raw");
INCBIN(057raw, "../game_english/data/Wave/057.raw");
INCBIN(058raw, "../game_english/data/Wave/058.raw");
INCBIN(059raw, "../game_english/data/Wave/059.raw");
INCBIN(060raw, "../game_english/data/Wave/060.raw");
INCBIN(061raw, "../game_english/data/Wave/061.raw");
INCBIN(062raw, "../game_english/data/Wave/062.raw");
INCBIN(063raw, "../game_english/data/Wave/063.raw");
INCBIN(064raw, "../game_english/data/Wave/064.raw");
INCBIN(065raw, "../game_english/data/Wave/065.raw");
INCBIN(070raw, "../game_english/data/Wave/070.raw");
INCBIN(071raw, "../game_english/data/Wave/071.raw");
INCBIN(072raw, "../game_english/data/Wave/072.raw");
INCBIN(100raw, "../game_english/data/Wave/100.raw");
INCBIN(101raw, "../game_english/data/Wave/101.raw");
INCBIN(102raw, "../game_english/data/Wave/102.raw");
INCBIN(103raw, "../game_english/data/Wave/103.raw");
INCBIN(104raw, "../game_english/data/Wave/104.raw");
INCBIN(105raw, "../game_english/data/Wave/105.raw");
INCBIN(106raw, "../game_english/data/Wave/106.raw");
INCBIN(107raw, "../game_english/data/Wave/107.raw");
INCBIN(108raw, "../game_english/data/Wave/108.raw");
INCBIN(109raw, "../game_english/data/Wave/109.raw");
INCBIN(110raw, "../game_english/data/Wave/110.raw");
INCBIN(111raw, "../game_english/data/Wave/111.raw");
INCBIN(112raw, "../game_english/data/Wave/112.raw");
INCBIN(113raw, "../game_english/data/Wave/113.raw");
INCBIN(114raw, "../game_english/data/Wave/114.raw");
INCBIN(115raw, "../game_english/data/Wave/115.raw");
INCBIN(116raw, "../game_english/data/Wave/116.raw");
INCBIN(117raw, "../game_english/data/Wave/117.raw");
INCBIN(150raw, "../game_english/data/Wave/150.raw");
INCBIN(151raw, "../game_english/data/Wave/151.raw");
INCBIN(152raw, "../game_english/data/Wave/152.raw");
INCBIN(153raw, "../game_english/data/Wave/153.raw");
INCBIN(154raw, "../game_english/data/Wave/154.raw");
INCBIN(155raw, "../game_english/data/Wave/155.raw");
INCBIN(156raw, "../game_english/data/Wave/156.raw");
INCBIN(157raw, "../game_english/data/Wave/157.raw");
INCBIN(0pxa, "../game_english/data/Stage/0.pxa");
INCBIN(0pxe, "../game_english/data/Stage/0.pxe");
INCBIN(0pxm, "../game_english/data/Stage/0.pxm");
INCBIN(0tsc, "../game_english/data/Stage/0.tsc");
INCBIN(555pxe, "../game_english/data/Stage/555.pxe");
INCBIN(Almondpxa, "../game_english/data/Stage/Almond.pxa");
INCBIN(Almondpxe, "../game_english/data/Stage/Almond.pxe");
INCBIN(Almondpxm, "../game_english/data/Stage/Almond.pxm");
INCBIN(Almondtsc, "../game_english/data/Stage/Almond.tsc");
INCBIN(Ballo1pxe, "../game_english/data/Stage/Ballo1.pxe");
INCBIN(Ballo1pxm, "../game_english/data/Stage/Ballo1.pxm");
INCBIN(Ballo1tsc, "../game_english/data/Stage/Ballo1.tsc");
INCBIN(Ballo2pxe, "../game_english/data/Stage/Ballo2.pxe");
INCBIN(Ballo2pxm, "../game_english/data/Stage/Ballo2.pxm");
INCBIN(Ballo2tsc, "../game_english/data/Stage/Ballo2.tsc");
INCBIN(Barrpxa, "../game_english/data/Stage/Barr.pxa");
INCBIN(Barrpxe, "../game_english/data/Stage/Barr.pxe");
INCBIN(Barrpxm, "../game_english/data/Stage/Barr.pxm");
INCBIN(Barrtsc, "../game_english/data/Stage/Barr.tsc");
INCBIN(Blcny1pxe, "../game_english/data/Stage/Blcny1.pxe");
INCBIN(Blcny1pxm, "../game_english/data/Stage/Blcny1.pxm");
INCBIN(Blcny1tsc, "../game_english/data/Stage/Blcny1.tsc");
INCBIN(Blcny2pxe, "../game_english/data/Stage/Blcny2.pxe");
INCBIN(Blcny2pxm, "../game_english/data/Stage/Blcny2.pxm");
INCBIN(Blcny2tsc, "../game_english/data/Stage/Blcny2.tsc");
INCBIN(Cavepxa, "../game_english/data/Stage/Cave.pxa");
INCBIN(Cavepxe, "../game_english/data/Stage/Cave.pxe");
INCBIN(Cavepxm, "../game_english/data/Stage/Cave.pxm");
INCBIN(Cavetsc, "../game_english/data/Stage/Cave.tsc");
INCBIN(Cemetpxe, "../game_english/data/Stage/Cemet.pxe");
INCBIN(Cemetpxm, "../game_english/data/Stage/Cemet.pxm");
INCBIN(Cemettsc, "../game_english/data/Stage/Cemet.tsc");
INCBIN(Centpxa, "../game_english/data/Stage/Cent.pxa");
INCBIN(Centpxe, "../game_english/data/Stage/Cent.pxe");
INCBIN(Centpxm, "../game_english/data/Stage/Cent.pxm");
INCBIN(Centtsc, "../game_english/data/Stage/Cent.tsc");
INCBIN(CentWpxe, "../game_english/data/Stage/CentW.pxe");
INCBIN(CentWpxm, "../game_english/data/Stage/CentW.pxm");
INCBIN(CentWtsc, "../game_english/data/Stage/CentW.tsc");
INCBIN(Chakopxe, "../game_english/data/Stage/Chako.pxe");
INCBIN(Chakopxm, "../game_english/data/Stage/Chako.pxm");
INCBIN(Chakotsc, "../game_english/data/Stage/Chako.tsc");
INCBIN(Clockpxe, "../game_english/data/Stage/Clock.pxe");
INCBIN(Clockpxm, "../game_english/data/Stage/Clock.pxm");
INCBIN(Clocktsc, "../game_english/data/Stage/Clock.tsc");
INCBIN(Comupxe, "../game_english/data/Stage/Comu.pxe");
INCBIN(Comupxm, "../game_english/data/Stage/Comu.pxm");
INCBIN(Comutsc, "../game_english/data/Stage/Comu.tsc");
INCBIN(Cookpxm, "../game_english/data/Stage/Cook.pxm");
INCBIN(Cthupxe, "../game_english/data/Stage/Cthu.pxe");
INCBIN(Cthupxm, "../game_english/data/Stage/Cthu.pxm");
INCBIN(Cthutsc, "../game_english/data/Stage/Cthu.tsc");
INCBIN(Cthu2pxe, "../game_english/data/Stage/Cthu2.pxe");
INCBIN(Cthu2pxm, "../game_english/data/Stage/Cthu2.pxm");
INCBIN(Cthu2tsc, "../game_english/data/Stage/Cthu2.tsc");
INCBIN(Curlypxe, "../game_english/data/Stage/Curly.pxe");
INCBIN(Curlypxm, "../game_english/data/Stage/Curly.pxm");
INCBIN(Curlytsc, "../game_english/data/Stage/Curly.tsc");
INCBIN(CurlySpxe, "../game_english/data/Stage/CurlyS.pxe");
INCBIN(CurlySpxm, "../game_english/data/Stage/CurlyS.pxm");
INCBIN(CurlyStsc, "../game_english/data/Stage/CurlyS.tsc");
INCBIN(Darkpxe, "../game_english/data/Stage/Dark.pxe");
INCBIN(Darkpxm, "../game_english/data/Stage/Dark.pxm");
INCBIN(Darktsc, "../game_english/data/Stage/Dark.tsc");
INCBIN(Drainpxe, "../game_english/data/Stage/Drain.pxe");
INCBIN(Drainpxm, "../game_english/data/Stage/Drain.pxm");
INCBIN(Draintsc, "../game_english/data/Stage/Drain.tsc");
INCBIN(EgEnd1pxe, "../game_english/data/Stage/EgEnd1.pxe");
INCBIN(EgEnd1pxm, "../game_english/data/Stage/EgEnd1.pxm");
INCBIN(EgEnd1tsc, "../game_english/data/Stage/EgEnd1.tsc");
INCBIN(EgEnd2pxe, "../game_english/data/Stage/EgEnd2.pxe");
INCBIN(EgEnd2pxm, "../game_english/data/Stage/EgEnd2.pxm");
INCBIN(EgEnd2tsc, "../game_english/data/Stage/EgEnd2.tsc");
INCBIN(Egg1pxe, "../game_english/data/Stage/Egg1.pxe");
INCBIN(Egg1pxm, "../game_english/data/Stage/Egg1.pxm");
INCBIN(Egg1tsc, "../game_english/data/Stage/Egg1.tsc");
INCBIN(Egg6pxe, "../game_english/data/Stage/Egg6.pxe");
INCBIN(Egg6pxm, "../game_english/data/Stage/Egg6.pxm");
INCBIN(Egg6tsc, "../game_english/data/Stage/Egg6.tsc");
INCBIN(EggInpxa, "../game_english/data/Stage/EggIn.pxa");
INCBIN(EggRpxe, "../game_english/data/Stage/EggR.pxe");
INCBIN(EggRpxm, "../game_english/data/Stage/EggR.pxm");
INCBIN(EggRtsc, "../game_english/data/Stage/EggR.tsc");
INCBIN(EggR2pxe, "../game_english/data/Stage/EggR2.pxe");
INCBIN(EggR2pxm, "../game_english/data/Stage/EggR2.pxm");
INCBIN(EggR2tsc, "../game_english/data/Stage/EggR2.tsc");
INCBIN(EggXpxa, "../game_english/data/Stage/EggX.pxa");
INCBIN(EggXpxe, "../game_english/data/Stage/EggX.pxe");
INCBIN(EggXpxm, "../game_english/data/Stage/EggX.pxm");
INCBIN(EggXtsc, "../game_english/data/Stage/EggX.tsc");
INCBIN(EggX2pxe, "../game_english/data/Stage/EggX2.pxe");
INCBIN(EggX2pxm, "../game_english/data/Stage/EggX2.pxm");
INCBIN(EggX2tsc, "../game_english/data/Stage/EggX2.tsc");
INCBIN(Eggspxa, "../game_english/data/Stage/Eggs.pxa");
INCBIN(Eggspxe, "../game_english/data/Stage/Eggs.pxe");
INCBIN(Eggspxm, "../game_english/data/Stage/Eggs.pxm");
INCBIN(Eggstsc, "../game_english/data/Stage/Eggs.tsc");
INCBIN(Eggs2pxe, "../game_english/data/Stage/Eggs2.pxe");
INCBIN(Eggs2pxm, "../game_english/data/Stage/Eggs2.pxm");
INCBIN(Eggs2tsc, "../game_english/data/Stage/Eggs2.tsc");
INCBIN(Fallpxa, "../game_english/data/Stage/Fall.pxa");
INCBIN(Fallpxe, "../game_english/data/Stage/Fall.pxe");
INCBIN(Fallpxm, "../game_english/data/Stage/Fall.pxm");
INCBIN(Falltsc, "../game_english/data/Stage/Fall.tsc");
INCBIN(Frogpxe, "../game_english/data/Stage/Frog.pxe");
INCBIN(Frogpxm, "../game_english/data/Stage/Frog.pxm");
INCBIN(Frogtsc, "../game_english/data/Stage/Frog.tsc");
INCBIN(Gardpxa, "../game_english/data/Stage/Gard.pxa");
INCBIN(Gardpxe, "../game_english/data/Stage/Gard.pxe");
INCBIN(Gardpxm, "../game_english/data/Stage/Gard.pxm");
INCBIN(Gardtsc, "../game_english/data/Stage/Gard.tsc");
INCBIN(Hellpxa, "../game_english/data/Stage/Hell.pxa");
INCBIN(Hell1pxe, "../game_english/data/Stage/Hell1.pxe");
INCBIN(Hell1pxm, "../game_english/data/Stage/Hell1.pxm");
INCBIN(Hell1tsc, "../game_english/data/Stage/Hell1.tsc");
INCBIN(Hell2pxe, "../game_english/data/Stage/Hell2.pxe");
INCBIN(Hell2pxm, "../game_english/data/Stage/Hell2.pxm");
INCBIN(Hell2tsc, "../game_english/data/Stage/Hell2.tsc");
INCBIN(Hell3pxe, "../game_english/data/Stage/Hell3.pxe");
INCBIN(Hell3pxm, "../game_english/data/Stage/Hell3.pxm");
INCBIN(Hell3tsc, "../game_english/data/Stage/Hell3.tsc");
INCBIN(Hell4pxe, "../game_english/data/Stage/Hell4.pxe");
INCBIN(Hell4pxm, "../game_english/data/Stage/Hell4.pxm");
INCBIN(Hell4tsc, "../game_english/data/Stage/Hell4.tsc");
INCBIN(Hell42pxe, "../game_english/data/Stage/Hell42.pxe");
INCBIN(Hell42pxm, "../game_english/data/Stage/Hell42.pxm");
INCBIN(Hell42tsc, "../game_english/data/Stage/Hell42.tsc");
INCBIN(Islandpxe, "../game_english/data/Stage/Island.pxe");
INCBIN(Islandpxm, "../game_english/data/Stage/Island.pxm");
INCBIN(Islandtsc, "../game_english/data/Stage/Island.tsc");
INCBIN(Itohpxe, "../game_english/data/Stage/Itoh.pxe");
INCBIN(Itohpxm, "../game_english/data/Stage/Itoh.pxm");
INCBIN(Itohtsc, "../game_english/data/Stage/Itoh.tsc");
INCBIN(Jailpxa, "../game_english/data/Stage/Jail.pxa");
INCBIN(Jail1pxe, "../game_english/data/Stage/Jail1.pxe");
INCBIN(Jail1pxm, "../game_english/data/Stage/Jail1.pxm");
INCBIN(Jail1tsc, "../game_english/data/Stage/Jail1.tsc");
INCBIN(Jail2pxe, "../game_english/data/Stage/Jail2.pxe");
INCBIN(Jail2pxm, "../game_english/data/Stage/Jail2.pxm");
INCBIN(Jail2tsc, "../game_english/data/Stage/Jail2.tsc");
INCBIN(Jenka1pxe, "../game_english/data/Stage/Jenka1.pxe");
INCBIN(Jenka1pxm, "../game_english/data/Stage/Jenka1.pxm");
INCBIN(Jenka1tsc, "../game_english/data/Stage/Jenka1.tsc");
INCBIN(Jenka2pxe, "../game_english/data/Stage/Jenka2.pxe");
INCBIN(Jenka2pxm, "../game_english/data/Stage/Jenka2.pxm");
INCBIN(Jenka2tsc, "../game_english/data/Stage/Jenka2.tsc");
INCBIN(Kingspxe, "../game_english/data/Stage/Kings.pxe");
INCBIN(Kingspxm, "../game_english/data/Stage/Kings.pxm");
INCBIN(Kingstsc, "../game_english/data/Stage/Kings.tsc");
INCBIN(Labopxa, "../game_english/data/Stage/Labo.pxa");
INCBIN(Littlepxe, "../game_english/data/Stage/Little.pxe");
INCBIN(Littlepxm, "../game_english/data/Stage/Little.pxm");
INCBIN(Littletsc, "../game_english/data/Stage/Little.tsc");
INCBIN(Loungepxe, "../game_english/data/Stage/Lounge.pxe");
INCBIN(Loungepxm, "../game_english/data/Stage/Lounge.pxm");
INCBIN(Loungetsc, "../game_english/data/Stage/Lounge.tsc");
INCBIN(Malcopxe, "../game_english/data/Stage/Malco.pxe");
INCBIN(Malcopxm, "../game_english/data/Stage/Malco.pxm");
INCBIN(Malcotsc, "../game_english/data/Stage/Malco.tsc");
INCBIN(Mapipxe, "../game_english/data/Stage/Mapi.pxe");
INCBIN(Mapipxm, "../game_english/data/Stage/Mapi.pxm");
INCBIN(Mapitsc, "../game_english/data/Stage/Mapi.tsc");
INCBIN(Mazepxa, "../game_english/data/Stage/Maze.pxa");
INCBIN(MazeApxe, "../game_english/data/Stage/MazeA.pxe");
INCBIN(MazeApxm, "../game_english/data/Stage/MazeA.pxm");
INCBIN(MazeAtsc, "../game_english/data/Stage/MazeA.tsc");
INCBIN(MazeBpxe, "../game_english/data/Stage/MazeB.pxe");
INCBIN(MazeBpxm, "../game_english/data/Stage/MazeB.pxm");
INCBIN(MazeBtsc, "../game_english/data/Stage/MazeB.tsc");
INCBIN(MazeDpxe, "../game_english/data/Stage/MazeD.pxe");
INCBIN(MazeDpxm, "../game_english/data/Stage/MazeD.pxm");
INCBIN(MazeDtsc, "../game_english/data/Stage/MazeD.tsc");
INCBIN(MazeHpxe, "../game_english/data/Stage/MazeH.pxe");
INCBIN(MazeHpxm, "../game_english/data/Stage/MazeH.pxm");
INCBIN(MazeHtsc, "../game_english/data/Stage/MazeH.tsc");
INCBIN(MazeIpxe, "../game_english/data/Stage/MazeI.pxe");
INCBIN(MazeIpxm, "../game_english/data/Stage/MazeI.pxm");
INCBIN(MazeItsc, "../game_english/data/Stage/MazeI.tsc");
INCBIN(MazeMpxe, "../game_english/data/Stage/MazeM.pxe");
INCBIN(MazeMpxm, "../game_english/data/Stage/MazeM.pxm");
INCBIN(MazeMtsc, "../game_english/data/Stage/MazeM.tsc");
INCBIN(MazeOpxe, "../game_english/data/Stage/MazeO.pxe");
INCBIN(MazeOpxm, "../game_english/data/Stage/MazeO.pxm");
INCBIN(MazeOtsc, "../game_english/data/Stage/MazeO.tsc");
INCBIN(MazeSpxe, "../game_english/data/Stage/MazeS.pxe");
INCBIN(MazeSpxm, "../game_english/data/Stage/MazeS.pxm");
INCBIN(MazeStsc, "../game_english/data/Stage/MazeS.tsc");
INCBIN(MazeWpxe, "../game_english/data/Stage/MazeW.pxe");
INCBIN(MazeWpxm, "../game_english/data/Stage/MazeW.pxm");
INCBIN(MazeWtsc, "../game_english/data/Stage/MazeW.tsc");
INCBIN(MiBoxpxe, "../game_english/data/Stage/MiBox.pxe");
INCBIN(MiBoxpxm, "../game_english/data/Stage/MiBox.pxm");
INCBIN(MiBoxtsc, "../game_english/data/Stage/MiBox.tsc");
INCBIN(Mimipxa, "../game_english/data/Stage/Mimi.pxa");
INCBIN(Mimipxe, "../game_english/data/Stage/Mimi.pxe");
INCBIN(Mimipxm, "../game_english/data/Stage/Mimi.pxm");
INCBIN(Mimitsc, "../game_english/data/Stage/Mimi.tsc");
INCBIN(Momopxe, "../game_english/data/Stage/Momo.pxe");
INCBIN(Momopxm, "../game_english/data/Stage/Momo.pxm");
INCBIN(Momotsc, "../game_english/data/Stage/Momo.tsc");
INCBIN(Newpxe, "../game_english/data/Stage/New.pxe");
INCBIN(Osidepxa, "../game_english/data/Stage/Oside.pxa");
INCBIN(Osidepxe, "../game_english/data/Stage/Oside.pxe");
INCBIN(Osidepxm, "../game_english/data/Stage/Oside.pxm");
INCBIN(Osidetsc, "../game_english/data/Stage/Oside.tsc");
INCBIN(Osteppxe, "../game_english/data/Stage/Ostep.pxe");
INCBIN(Osteppxm, "../game_english/data/Stage/Ostep.pxm");
INCBIN(Osteptsc, "../game_english/data/Stage/Ostep.tsc");
INCBIN(Penspxa, "../game_english/data/Stage/Pens.pxa");
INCBIN(Pens1pxe, "../game_english/data/Stage/Pens1.pxe");
INCBIN(Pens1pxm, "../game_english/data/Stage/Pens1.pxm");
INCBIN(Pens1tsc, "../game_english/data/Stage/Pens1.tsc");
INCBIN(Pens2pxe, "../game_english/data/Stage/Pens2.pxe");
INCBIN(Pens2pxm, "../game_english/data/Stage/Pens2.pxm");
INCBIN(Pens2tsc, "../game_english/data/Stage/Pens2.tsc");
INCBIN(Pixelpxe, "../game_english/data/Stage/Pixel.pxe");
INCBIN(Pixelpxm, "../game_english/data/Stage/Pixel.pxm");
INCBIN(Pixeltsc, "../game_english/data/Stage/Pixel.tsc");
INCBIN(Plantpxe, "../game_english/data/Stage/Plant.pxe");
INCBIN(Plantpxm, "../game_english/data/Stage/Plant.pxm");
INCBIN(Planttsc, "../game_english/data/Stage/Plant.tsc");
INCBIN(Polepxe, "../game_english/data/Stage/Pole.pxe");
INCBIN(Polepxm, "../game_english/data/Stage/Pole.pxm");
INCBIN(Poletsc, "../game_english/data/Stage/Pole.tsc");
INCBIN(Poolpxe, "../game_english/data/Stage/Pool.pxe");
INCBIN(Poolpxm, "../game_english/data/Stage/Pool.pxm");
INCBIN(Pooltsc, "../game_english/data/Stage/Pool.tsc");
INCBIN(Prefa1pxe, "../game_english/data/Stage/Prefa1.pxe");
INCBIN(Prefa1pxm, "../game_english/data/Stage/Prefa1.pxm");
INCBIN(Prefa1tsc, "../game_english/data/Stage/Prefa1.tsc");
INCBIN(Prefa2pxe, "../game_english/data/Stage/Prefa2.pxe");
INCBIN(Prefa2pxm, "../game_english/data/Stage/Prefa2.pxm");
INCBIN(Prefa2tsc, "../game_english/data/Stage/Prefa2.tsc");
INCBIN(Priso1pxe, "../game_english/data/Stage/Priso1.pxe");
INCBIN(Priso1pxm, "../game_english/data/Stage/Priso1.pxm");
INCBIN(Priso1tsc, "../game_english/data/Stage/Priso1.tsc");
INCBIN(Priso2pxe, "../game_english/data/Stage/Priso2.pxe");
INCBIN(Priso2pxm, "../game_english/data/Stage/Priso2.pxm");
INCBIN(Priso2tsc, "../game_english/data/Stage/Priso2.tsc");
INCBIN(Prt0png, "../game_english/data/Stage/Prt0.png");
INCBIN(PrtAlmondpng, "../game_english/data/Stage/PrtAlmond.png");
INCBIN(PrtBarrpng, "../game_english/data/Stage/PrtBarr.png");
INCBIN(PrtCavepng, "../game_english/data/Stage/PrtCave.png");
INCBIN(PrtCentpng, "../game_english/data/Stage/PrtCent.png");
INCBIN(PrtEggInpng, "../game_english/data/Stage/PrtEggIn.png");
INCBIN(PrtEggXpng, "../game_english/data/Stage/PrtEggX.png");
INCBIN(PrtEggspng, "../game_english/data/Stage/PrtEggs.png");
INCBIN(PrtFallpng, "../game_english/data/Stage/PrtFall.png");
INCBIN(PrtGardpng, "../game_english/data/Stage/PrtGard.png");
INCBIN(PrtHellpng, "../game_english/data/Stage/PrtHell.png");
INCBIN(PrtJailpng, "../game_english/data/Stage/PrtJail.png");
INCBIN(PrtLabopng, "../game_english/data/Stage/PrtLabo.png");
INCBIN(PrtMazepng, "../game_english/data/Stage/PrtMaze.png");
INCBIN(PrtMimipng, "../game_english/data/Stage/PrtMimi.png");
INCBIN(PrtOsidepng, "../game_english/data/Stage/PrtOside.png");
INCBIN(PrtPenspng, "../game_english/data/Stage/PrtPens.png");
INCBIN(PrtRiverpng, "../game_english/data/Stage/PrtRiver.png");
INCBIN(PrtSandpng, "../game_english/data/Stage/PrtSand.png");
INCBIN(PrtStorepng, "../game_english/data/Stage/PrtStore.png");
INCBIN(PrtWeedpng, "../game_english/data/Stage/PrtWeed.png");
INCBIN(PrtWhitepng, "../game_english/data/Stage/PrtWhite.png");
INCBIN(Ring1pxe, "../game_english/data/Stage/Ring1.pxe");
INCBIN(Ring1pxm, "../game_english/data/Stage/Ring1.pxm");
INCBIN(Ring1tsc, "../game_english/data/Stage/Ring1.tsc");
INCBIN(Ring2pxe, "../game_english/data/Stage/Ring2.pxe");
INCBIN(Ring2pxm, "../game_english/data/Stage/Ring2.pxm");
INCBIN(Ring2tsc, "../game_english/data/Stage/Ring2.tsc");
INCBIN(Ring3pxe, "../game_english/data/Stage/Ring3.pxe");
INCBIN(Ring3pxm, "../game_english/data/Stage/Ring3.pxm");
INCBIN(Ring3tsc, "../game_english/data/Stage/Ring3.tsc");
INCBIN(Riverpxa, "../game_english/data/Stage/River.pxa");
INCBIN(Riverpxe, "../game_english/data/Stage/River.pxe");
INCBIN(Riverpxm, "../game_english/data/Stage/River.pxm");
INCBIN(Rivertsc, "../game_english/data/Stage/River.tsc");
INCBIN(Sandpxa, "../game_english/data/Stage/Sand.pxa");
INCBIN(Sandpxe, "../game_english/data/Stage/Sand.pxe");
INCBIN(Sandpxm, "../game_english/data/Stage/Sand.pxm");
INCBIN(Sandtsc, "../game_english/data/Stage/Sand.tsc");
INCBIN(SandEpxe, "../game_english/data/Stage/SandE.pxe");
INCBIN(SandEpxm, "../game_english/data/Stage/SandE.pxm");
INCBIN(SandEtsc, "../game_english/data/Stage/SandE.tsc");
INCBIN(Santapxe, "../game_english/data/Stage/Santa.pxe");
INCBIN(Santapxm, "../game_english/data/Stage/Santa.pxm");
INCBIN(Santatsc, "../game_english/data/Stage/Santa.tsc");
INCBIN(Sheltpxa, "../game_english/data/Stage/Shelt.pxa");
INCBIN(Sheltpxe, "../game_english/data/Stage/Shelt.pxe");
INCBIN(Sheltpxm, "../game_english/data/Stage/Shelt.pxm");
INCBIN(Shelttsc, "../game_english/data/Stage/Shelt.tsc");
INCBIN(Startpxe, "../game_english/data/Stage/Start.pxe");
INCBIN(Startpxm, "../game_english/data/Stage/Start.pxm");
INCBIN(Starttsc, "../game_english/data/Stage/Start.tsc");
INCBIN(Statuepxe, "../game_english/data/Stage/Statue.pxe");
INCBIN(Statuepxm, "../game_english/data/Stage/Statue.pxm");
INCBIN(Statuetsc, "../game_english/data/Stage/Statue.tsc");
INCBIN(Storepxa, "../game_english/data/Stage/Store.pxa");
INCBIN(Streampxe, "../game_english/data/Stage/Stream.pxe");
INCBIN(Streampxm, "../game_english/data/Stage/Stream.pxm");
INCBIN(Streamtsc, "../game_english/data/Stage/Stream.tsc");
INCBIN(Weedpxa, "../game_english/data/Stage/Weed.pxa");
INCBIN(Weedpxe, "../game_english/data/Stage/Weed.pxe");
INCBIN(Weedpxm, "../game_english/data/Stage/Weed.pxm");
INCBIN(Weedtsc, "../game_english/data/Stage/Weed.tsc");
INCBIN(WeedBpxe, "../game_english/data/Stage/WeedB.pxe");
INCBIN(WeedBpxm, "../game_english/data/Stage/WeedB.pxm");
INCBIN(WeedBtsc, "../game_english/data/Stage/WeedB.tsc");
INCBIN(WeedDpxe, "../game_english/data/Stage/WeedD.pxe");
INCBIN(WeedDpxm, "../game_english/data/Stage/WeedD.pxm");
INCBIN(WeedDtsc, "../game_english/data/Stage/WeedD.tsc");
INCBIN(WeedSpxe, "../game_english/data/Stage/WeedS.pxe");
INCBIN(WeedSpxm, "../game_english/data/Stage/WeedS.pxm");
INCBIN(WeedStsc, "../game_english/data/Stage/WeedS.tsc");
INCBIN(Whitepxa, "../game_english/data/Stage/White.pxa");
INCBIN(e_Blcnpxe, "../game_english/data/Stage/e_Blcn.pxe");
INCBIN(e_Blcnpxm, "../game_english/data/Stage/e_Blcn.pxm");
INCBIN(e_Blcntsc, "../game_english/data/Stage/e_Blcn.tsc");
INCBIN(e_Cemepxe, "../game_english/data/Stage/e_Ceme.pxe");
INCBIN(e_Cemepxm, "../game_english/data/Stage/e_Ceme.pxm");
INCBIN(e_Cemetsc, "../game_english/data/Stage/e_Ceme.tsc");
INCBIN(e_Jenkpxe, "../game_english/data/Stage/e_Jenk.pxe");
INCBIN(e_Jenkpxm, "../game_english/data/Stage/e_Jenk.pxm");
INCBIN(e_Jenktsc, "../game_english/data/Stage/e_Jenk.tsc");
INCBIN(e_Labopxe, "../game_english/data/Stage/e_Labo.pxe");
INCBIN(e_Labopxm, "../game_english/data/Stage/e_Labo.pxm");
INCBIN(e_Labotsc, "../game_english/data/Stage/e_Labo.tsc");
INCBIN(e_Malcpxe, "../game_english/data/Stage/e_Malc.pxe");
INCBIN(e_Malcpxm, "../game_english/data/Stage/e_Malc.pxm");
INCBIN(e_Malctsc, "../game_english/data/Stage/e_Malc.tsc");
INCBIN(e_Mazepxe, "../game_english/data/Stage/e_Maze.pxe");
INCBIN(e_Mazepxm, "../game_english/data/Stage/e_Maze.pxm");
INCBIN(e_Mazetsc, "../game_english/data/Stage/e_Maze.tsc");
INCBIN(e_Skypxe, "../game_english/data/Stage/e_Sky.pxe");
INCBIN(e_Skypxm, "../game_english/data/Stage/e_Sky.pxm");
INCBIN(e_Skytsc, "../game_english/data/Stage/e_Sky.tsc");
INCBIN(ACCESSorg, "../game_english/data/ORG/ACCESS.org");
INCBIN(ANZENorg, "../game_english/data/ORG/ANZEN.org");
INCBIN(BALCONYorg, "../game_english/data/ORG/BALCONY.org");
INCBIN(BALLOSorg, "../game_english/data/ORG/BALLOS.org");
INCBIN(BDOWNorg, "../game_english/data/ORG/BDOWN.org");
INCBIN(CEMETERYorg, "../game_english/data/ORG/CEMETERY.org");
INCBIN(CURLYorg, "../game_english/data/ORG/CURLY.org");
INCBIN(DRorg, "../game_english/data/ORG/DR.org");
INCBIN(ENDINGorg, "../game_english/data/ORG/ENDING.org");
INCBIN(ESCAPEorg, "../game_english/data/ORG/ESCAPE.org");
INCBIN(FANFALE1org, "../game_english/data/ORG/FANFALE1.org");
INCBIN(FANFALE2org, "../game_english/data/ORG/FANFALE2.org");
INCBIN(FANFALE3org, "../game_english/data/ORG/FANFALE3.org");
INCBIN(FIREEYEorg, "../game_english/data/ORG/FIREEYE.org");
INCBIN(GAMEOVERorg, "../game_english/data/ORG/GAMEOVER.org");
INCBIN(GINSUKEorg, "../game_english/data/ORG/GINSUKE.org");
INCBIN(GRANDorg, "../game_english/data/ORG/GRAND.org");
INCBIN(GRAVITYorg, "../game_english/data/ORG/GRAVITY.org");
INCBIN(HELLorg, "../game_english/data/ORG/HELL.org");
INCBIN(IRONHorg, "../game_english/data/ORG/IRONH.org");
INCBIN(JENKAorg, "../game_english/data/ORG/JENKA.org");
INCBIN(JENKA2org, "../game_english/data/ORG/JENKA2.org");
INCBIN(KODOUorg, "../game_english/data/ORG/KODOU.org");
INCBIN(LASTBT3org, "../game_english/data/ORG/LASTBT3.org");
INCBIN(LASTBTLorg, "../game_english/data/ORG/LASTBTL.org");
INCBIN(LASTCAVEorg, "../game_english/data/ORG/LASTCAVE.org");
INCBIN(MARINEorg, "../game_english/data/ORG/MARINE.org");
INCBIN(MAZEorg, "../game_english/data/ORG/MAZE.org");
INCBIN(MDOWN2org, "../game_english/data/ORG/MDOWN2.org");
INCBIN(MURAorg, "../game_english/data/ORG/MURA.org");
INCBIN(OSIDEorg, "../game_english/data/ORG/OSIDE.org");
INCBIN(PLANTorg, "../game_english/data/ORG/PLANT.org");
INCBIN(QUIETorg, "../game_english/data/ORG/QUIET.org");
INCBIN(REQUIEMorg, "../game_english/data/ORG/REQUIEM.org");
INCBIN(TOROKOorg, "../game_english/data/ORG/TOROKO.org");
INCBIN(VIVIorg, "../game_english/data/ORG/VIVI.org");
INCBIN(WANPAK2org, "../game_english/data/ORG/WANPAK2.org");
INCBIN(WANPAKUorg, "../game_english/data/ORG/WANPAKU.org");
INCBIN(WEEDorg, "../game_english/data/ORG/WEED.org");
INCBIN(WHITEorg, "../game_english/data/ORG/WHITE.org");
INCBIN(XXXXorg, "../game_english/data/ORG/XXXX.org");
INCBIN(ZONBIEorg, "../game_english/data/ORG/ZONBIE.org");
INCBIN(wavedat, "../game_english/data/ORG/wave.dat");

const FILEREF files[] = {
	{gArmspngData, gArmspngSize, "data/Arms.png"},
	{gArmsImagepngData, gArmsImagepngSize, "data/ArmsImage.png"},
	{gArmsItemtscData, gArmsItemtscSize, "data/ArmsItem.tsc"},
	{gBulletpngData, gBulletpngSize, "data/Bullet.png"},
	{gCaretpngData, gCaretpngSize, "data/Caret.png"},
	{gCredittscData, gCredittscSize, "data/Credit.tsc"},
	{gFacepngData, gFacepngSize, "data/Face.png"},
	{gFadepngData, gFadepngSize, "data/Fade.png"},
	{gHeadtscData, gHeadtscSize, "data/Head.tsc"},
	{gICON_MINIpngData, gICON_MINIpngSize, "data/ICON_MINI.png"},
	{gItemImagepngData, gItemImagepngSize, "data/ItemImage.png"},
	{gLoadingpngData, gLoadingpngSize, "data/Loading.png"},
	{gMyCharpngData, gMyCharpngSize, "data/MyChar.png"},
	{gStageImagepngData, gStageImagepngSize, "data/StageImage.png"},
	{gStageSelecttscData, gStageSelecttscSize, "data/StageSelect.tsc"},
	{gTextBoxpngData, gTextBoxpngSize, "data/TextBox.png"},
	{gTitlepngData, gTitlepngSize, "data/Title.png"},
	{gbk0pngData, gbk0pngSize, "data/bk0.png"},
	{gbkBlackpngData, gbkBlackpngSize, "data/bkBlack.png"},
	{gbkBluepngData, gbkBluepngSize, "data/bkBlue.png"},
	{gbkFallpngData, gbkFallpngSize, "data/bkFall.png"},
	{gbkFogpngData, gbkFogpngSize, "data/bkFog.png"},
	{gbkGardpngData, gbkGardpngSize, "data/bkGard.png"},
	{gbkGraypngData, gbkGraypngSize, "data/bkGray.png"},
	{gbkGreenpngData, gbkGreenpngSize, "data/bkGreen.png"},
	{gbkMazepngData, gbkMazepngSize, "data/bkMaze.png"},
	{gbkMoonpngData, gbkMoonpngSize, "data/bkMoon.png"},
	{gbkRedpngData, gbkRedpngSize, "data/bkRed.png"},
	{gbkWaterpngData, gbkWaterpngSize, "data/bkWater.png"},
	{gcastspngData, gcastspngSize, "data/casts.png"},
	{gnpctblData, gnpctblSize, "data/npc.tbl"},
	{gsmalfontpngData, gsmalfontpngSize, "data/smalfont.png"},
	{gstagetblData, gstagetblSize, "data/stage.tbl"},
	{gNpc0pngData, gNpc0pngSize, "data/Npc/Npc0.png"},
	{gNpcAlmo1pngData, gNpcAlmo1pngSize, "data/Npc/NpcAlmo1.png"},
	{gNpcAlmo2pngData, gNpcAlmo2pngSize, "data/Npc/NpcAlmo2.png"},
	{gNpcBallospngData, gNpcBallospngSize, "data/Npc/NpcBallos.png"},
	{gNpcBllgpngData, gNpcBllgpngSize, "data/Npc/NpcBllg.png"},
	{gNpcCemetpngData, gNpcCemetpngSize, "data/Npc/NpcCemet.png"},
	{gNpcCentpngData, gNpcCentpngSize, "data/Npc/NpcCent.png"},
	{gNpcCurlypngData, gNpcCurlypngSize, "data/Npc/NpcCurly.png"},
	{gNpcDarkpngData, gNpcDarkpngSize, "data/Npc/NpcDark.png"},
	{gNpcDrpngData, gNpcDrpngSize, "data/Npc/NpcDr.png"},
	{gNpcEggs1pngData, gNpcEggs1pngSize, "data/Npc/NpcEggs1.png"},
	{gNpcEggs2pngData, gNpcEggs2pngSize, "data/Npc/NpcEggs2.png"},
	{gNpcFrogpngData, gNpcFrogpngSize, "data/Npc/NpcFrog.png"},
	{gNpcGuestpngData, gNpcGuestpngSize, "data/Npc/NpcGuest.png"},
	{gNpcHellpngData, gNpcHellpngSize, "data/Npc/NpcHell.png"},
	{gNpcHeripngData, gNpcHeripngSize, "data/Npc/NpcHeri.png"},
	{gNpcIronHpngData, gNpcIronHpngSize, "data/Npc/NpcIronH.png"},
	{gNpcIslandpngData, gNpcIslandpngSize, "data/Npc/NpcIsland.png"},
	{gNpcKingspngData, gNpcKingspngSize, "data/Npc/NpcKings.png"},
	{gNpcMazepngData, gNpcMazepngSize, "data/Npc/NpcMaze.png"},
	{gNpcMizapngData, gNpcMizapngSize, "data/Npc/NpcMiza.png"},
	{gNpcMoonpngData, gNpcMoonpngSize, "data/Npc/NpcMoon.png"},
	{gNpcOmgpngData, gNpcOmgpngSize, "data/Npc/NpcOmg.png"},
	{gNpcPlantpngData, gNpcPlantpngSize, "data/Npc/NpcPlant.png"},
	{gNpcPresspngData, gNpcPresspngSize, "data/Npc/NpcPress.png"},
	{gNpcPriestpngData, gNpcPriestpngSize, "data/Npc/NpcPriest.png"},
	{gNpcRavilpngData, gNpcRavilpngSize, "data/Npc/NpcRavil.png"},
	{gNpcRedpngData, gNpcRedpngSize, "data/Npc/NpcRed.png"},
	{gNpcRegupngData, gNpcRegupngSize, "data/Npc/NpcRegu.png"},
	{gNpcSandpngData, gNpcSandpngSize, "data/Npc/NpcSand.png"},
	{gNpcStreampngData, gNpcStreampngSize, "data/Npc/NpcStream.png"},
	{gNpcSympngData, gNpcSympngSize, "data/Npc/NpcSym.png"},
	{gNpcToropngData, gNpcToropngSize, "data/Npc/NpcToro.png"},
	{gNpcTwinDpngData, gNpcTwinDpngSize, "data/Npc/NpcTwinD.png"},
	{gNpcWeedpngData, gNpcWeedpngSize, "data/Npc/NpcWeed.png"},
	{gNpcXpngData, gNpcXpngSize, "data/Npc/NpcX.png"},
	{gCREDIT01pngData, gCREDIT01pngSize, "data/BITMAP/CREDIT01.png"},
	{gCREDIT02pngData, gCREDIT02pngSize, "data/BITMAP/CREDIT02.png"},
	{gCREDIT03pngData, gCREDIT03pngSize, "data/BITMAP/CREDIT03.png"},
	{gCREDIT04pngData, gCREDIT04pngSize, "data/BITMAP/CREDIT04.png"},
	{gCREDIT05pngData, gCREDIT05pngSize, "data/BITMAP/CREDIT05.png"},
	{gCREDIT06pngData, gCREDIT06pngSize, "data/BITMAP/CREDIT06.png"},
	{gCREDIT07pngData, gCREDIT07pngSize, "data/BITMAP/CREDIT07.png"},
	{gCREDIT08pngData, gCREDIT08pngSize, "data/BITMAP/CREDIT08.png"},
	{gCREDIT09pngData, gCREDIT09pngSize, "data/BITMAP/CREDIT09.png"},
	{gCREDIT10pngData, gCREDIT10pngSize, "data/BITMAP/CREDIT10.png"},
	{gCREDIT11pngData, gCREDIT11pngSize, "data/BITMAP/CREDIT11.png"},
	{gCREDIT12pngData, gCREDIT12pngSize, "data/BITMAP/CREDIT12.png"},
	{gCREDIT14pngData, gCREDIT14pngSize, "data/BITMAP/CREDIT14.png"},
	{gCREDIT15pngData, gCREDIT15pngSize, "data/BITMAP/CREDIT15.png"},
	{gCREDIT16pngData, gCREDIT16pngSize, "data/BITMAP/CREDIT16.png"},
	{gCREDIT17pngData, gCREDIT17pngSize, "data/BITMAP/CREDIT17.png"},
	{gCREDIT18pngData, gCREDIT18pngSize, "data/BITMAP/CREDIT18.png"},
	{gPIXELpngData, gPIXELpngSize, "data/BITMAP/PIXEL.png"},
	{gPIXEL_JPpngData, gPIXEL_JPpngSize, "data/BITMAP/PIXEL_JP.png"},
	{g001rawData, g001rawSize, "data/Wave/001.raw"},
	{g002rawData, g002rawSize, "data/Wave/002.raw"},
	{g003rawData, g003rawSize, "data/Wave/003.raw"},
	{g004rawData, g004rawSize, "data/Wave/004.raw"},
	{g005rawData, g005rawSize, "data/Wave/005.raw"},
	{g006rawData, g006rawSize, "data/Wave/006.raw"},
	{g011rawData, g011rawSize, "data/Wave/011.raw"},
	{g012rawData, g012rawSize, "data/Wave/012.raw"},
	{g014rawData, g014rawSize, "data/Wave/014.raw"},
	{g015rawData, g015rawSize, "data/Wave/015.raw"},
	{g016rawData, g016rawSize, "data/Wave/016.raw"},
	{g017rawData, g017rawSize, "data/Wave/017.raw"},
	{g018rawData, g018rawSize, "data/Wave/018.raw"},
	{g020rawData, g020rawSize, "data/Wave/020.raw"},
	{g021rawData, g021rawSize, "data/Wave/021.raw"},
	{g022rawData, g022rawSize, "data/Wave/022.raw"},
	{g023rawData, g023rawSize, "data/Wave/023.raw"},
	{g024rawData, g024rawSize, "data/Wave/024.raw"},
	{g025rawData, g025rawSize, "data/Wave/025.raw"},
	{g026rawData, g026rawSize, "data/Wave/026.raw"},
	{g027rawData, g027rawSize, "data/Wave/027.raw"},
	{g028rawData, g028rawSize, "data/Wave/028.raw"},
	{g029rawData, g029rawSize, "data/Wave/029.raw"},
	{g030rawData, g030rawSize, "data/Wave/030.raw"},
	{g031rawData, g031rawSize, "data/Wave/031.raw"},
	{g032rawData, g032rawSize, "data/Wave/032.raw"},
	{g033rawData, g033rawSize, "data/Wave/033.raw"},
	{g034rawData, g034rawSize, "data/Wave/034.raw"},
	{g035rawData, g035rawSize, "data/Wave/035.raw"},
	{g037rawData, g037rawSize, "data/Wave/037.raw"},
	{g038rawData, g038rawSize, "data/Wave/038.raw"},
	{g039rawData, g039rawSize, "data/Wave/039.raw"},
	{g040rawData, g040rawSize, "data/Wave/040.raw"},
	{g041rawData, g041rawSize, "data/Wave/041.raw"},
	{g042rawData, g042rawSize, "data/Wave/042.raw"},
	{g043rawData, g043rawSize, "data/Wave/043.raw"},
	{g044rawData, g044rawSize, "data/Wave/044.raw"},
	{g045rawData, g045rawSize, "data/Wave/045.raw"},
	{g046rawData, g046rawSize, "data/Wave/046.raw"},
	{g047rawData, g047rawSize, "data/Wave/047.raw"},
	{g048rawData, g048rawSize, "data/Wave/048.raw"},
	{g049rawData, g049rawSize, "data/Wave/049.raw"},
	{g050rawData, g050rawSize, "data/Wave/050.raw"},
	{g051rawData, g051rawSize, "data/Wave/051.raw"},
	{g052rawData, g052rawSize, "data/Wave/052.raw"},
	{g053rawData, g053rawSize, "data/Wave/053.raw"},
	{g054rawData, g054rawSize, "data/Wave/054.raw"},
	{g055rawData, g055rawSize, "data/Wave/055.raw"},
	{g056rawData, g056rawSize, "data/Wave/056.raw"},
	{g057rawData, g057rawSize, "data/Wave/057.raw"},
	{g058rawData, g058rawSize, "data/Wave/058.raw"},
	{g059rawData, g059rawSize, "data/Wave/059.raw"},
	{g060rawData, g060rawSize, "data/Wave/060.raw"},
	{g061rawData, g061rawSize, "data/Wave/061.raw"},
	{g062rawData, g062rawSize, "data/Wave/062.raw"},
	{g063rawData, g063rawSize, "data/Wave/063.raw"},
	{g064rawData, g064rawSize, "data/Wave/064.raw"},
	{g065rawData, g065rawSize, "data/Wave/065.raw"},
	{g070rawData, g070rawSize, "data/Wave/070.raw"},
	{g071rawData, g071rawSize, "data/Wave/071.raw"},
	{g072rawData, g072rawSize, "data/Wave/072.raw"},
	{g100rawData, g100rawSize, "data/Wave/100.raw"},
	{g101rawData, g101rawSize, "data/Wave/101.raw"},
	{g102rawData, g102rawSize, "data/Wave/102.raw"},
	{g103rawData, g103rawSize, "data/Wave/103.raw"},
	{g104rawData, g104rawSize, "data/Wave/104.raw"},
	{g105rawData, g105rawSize, "data/Wave/105.raw"},
	{g106rawData, g106rawSize, "data/Wave/106.raw"},
	{g107rawData, g107rawSize, "data/Wave/107.raw"},
	{g108rawData, g108rawSize, "data/Wave/108.raw"},
	{g109rawData, g109rawSize, "data/Wave/109.raw"},
	{g110rawData, g110rawSize, "data/Wave/110.raw"},
	{g111rawData, g111rawSize, "data/Wave/111.raw"},
	{g112rawData, g112rawSize, "data/Wave/112.raw"},
	{g113rawData, g113rawSize, "data/Wave/113.raw"},
	{g114rawData, g114rawSize, "data/Wave/114.raw"},
	{g115rawData, g115rawSize, "data/Wave/115.raw"},
	{g116rawData, g116rawSize, "data/Wave/116.raw"},
	{g117rawData, g117rawSize, "data/Wave/117.raw"},
	{g150rawData, g150rawSize, "data/Wave/150.raw"},
	{g151rawData, g151rawSize, "data/Wave/151.raw"},
	{g152rawData, g152rawSize, "data/Wave/152.raw"},
	{g153rawData, g153rawSize, "data/Wave/153.raw"},
	{g154rawData, g154rawSize, "data/Wave/154.raw"},
	{g155rawData, g155rawSize, "data/Wave/155.raw"},
	{g156rawData, g156rawSize, "data/Wave/156.raw"},
	{g157rawData, g157rawSize, "data/Wave/157.raw"},
	{g0pxaData, g0pxaSize, "data/Stage/0.pxa"},
	{g0pxeData, g0pxeSize, "data/Stage/0.pxe"},
	{g0pxmData, g0pxmSize, "data/Stage/0.pxm"},
	{g0tscData, g0tscSize, "data/Stage/0.tsc"},
	{g555pxeData, g555pxeSize, "data/Stage/555.pxe"},
	{gAlmondpxaData, gAlmondpxaSize, "data/Stage/Almond.pxa"},
	{gAlmondpxeData, gAlmondpxeSize, "data/Stage/Almond.pxe"},
	{gAlmondpxmData, gAlmondpxmSize, "data/Stage/Almond.pxm"},
	{gAlmondtscData, gAlmondtscSize, "data/Stage/Almond.tsc"},
	{gBallo1pxeData, gBallo1pxeSize, "data/Stage/Ballo1.pxe"},
	{gBallo1pxmData, gBallo1pxmSize, "data/Stage/Ballo1.pxm"},
	{gBallo1tscData, gBallo1tscSize, "data/Stage/Ballo1.tsc"},
	{gBallo2pxeData, gBallo2pxeSize, "data/Stage/Ballo2.pxe"},
	{gBallo2pxmData, gBallo2pxmSize, "data/Stage/Ballo2.pxm"},
	{gBallo2tscData, gBallo2tscSize, "data/Stage/Ballo2.tsc"},
	{gBarrpxaData, gBarrpxaSize, "data/Stage/Barr.pxa"},
	{gBarrpxeData, gBarrpxeSize, "data/Stage/Barr.pxe"},
	{gBarrpxmData, gBarrpxmSize, "data/Stage/Barr.pxm"},
	{gBarrtscData, gBarrtscSize, "data/Stage/Barr.tsc"},
	{gBlcny1pxeData, gBlcny1pxeSize, "data/Stage/Blcny1.pxe"},
	{gBlcny1pxmData, gBlcny1pxmSize, "data/Stage/Blcny1.pxm"},
	{gBlcny1tscData, gBlcny1tscSize, "data/Stage/Blcny1.tsc"},
	{gBlcny2pxeData, gBlcny2pxeSize, "data/Stage/Blcny2.pxe"},
	{gBlcny2pxmData, gBlcny2pxmSize, "data/Stage/Blcny2.pxm"},
	{gBlcny2tscData, gBlcny2tscSize, "data/Stage/Blcny2.tsc"},
	{gCavepxaData, gCavepxaSize, "data/Stage/Cave.pxa"},
	{gCavepxeData, gCavepxeSize, "data/Stage/Cave.pxe"},
	{gCavepxmData, gCavepxmSize, "data/Stage/Cave.pxm"},
	{gCavetscData, gCavetscSize, "data/Stage/Cave.tsc"},
	{gCemetpxeData, gCemetpxeSize, "data/Stage/Cemet.pxe"},
	{gCemetpxmData, gCemetpxmSize, "data/Stage/Cemet.pxm"},
	{gCemettscData, gCemettscSize, "data/Stage/Cemet.tsc"},
	{gCentpxaData, gCentpxaSize, "data/Stage/Cent.pxa"},
	{gCentpxeData, gCentpxeSize, "data/Stage/Cent.pxe"},
	{gCentpxmData, gCentpxmSize, "data/Stage/Cent.pxm"},
	{gCenttscData, gCenttscSize, "data/Stage/Cent.tsc"},
	{gCentWpxeData, gCentWpxeSize, "data/Stage/CentW.pxe"},
	{gCentWpxmData, gCentWpxmSize, "data/Stage/CentW.pxm"},
	{gCentWtscData, gCentWtscSize, "data/Stage/CentW.tsc"},
	{gChakopxeData, gChakopxeSize, "data/Stage/Chako.pxe"},
	{gChakopxmData, gChakopxmSize, "data/Stage/Chako.pxm"},
	{gChakotscData, gChakotscSize, "data/Stage/Chako.tsc"},
	{gClockpxeData, gClockpxeSize, "data/Stage/Clock.pxe"},
	{gClockpxmData, gClockpxmSize, "data/Stage/Clock.pxm"},
	{gClocktscData, gClocktscSize, "data/Stage/Clock.tsc"},
	{gComupxeData, gComupxeSize, "data/Stage/Comu.pxe"},
	{gComupxmData, gComupxmSize, "data/Stage/Comu.pxm"},
	{gComutscData, gComutscSize, "data/Stage/Comu.tsc"},
	{gCookpxmData, gCookpxmSize, "data/Stage/Cook.pxm"},
	{gCthupxeData, gCthupxeSize, "data/Stage/Cthu.pxe"},
	{gCthupxmData, gCthupxmSize, "data/Stage/Cthu.pxm"},
	{gCthutscData, gCthutscSize, "data/Stage/Cthu.tsc"},
	{gCthu2pxeData, gCthu2pxeSize, "data/Stage/Cthu2.pxe"},
	{gCthu2pxmData, gCthu2pxmSize, "data/Stage/Cthu2.pxm"},
	{gCthu2tscData, gCthu2tscSize, "data/Stage/Cthu2.tsc"},
	{gCurlypxeData, gCurlypxeSize, "data/Stage/Curly.pxe"},
	{gCurlypxmData, gCurlypxmSize, "data/Stage/Curly.pxm"},
	{gCurlytscData, gCurlytscSize, "data/Stage/Curly.tsc"},
	{gCurlySpxeData, gCurlySpxeSize, "data/Stage/CurlyS.pxe"},
	{gCurlySpxmData, gCurlySpxmSize, "data/Stage/CurlyS.pxm"},
	{gCurlyStscData, gCurlyStscSize, "data/Stage/CurlyS.tsc"},
	{gDarkpxeData, gDarkpxeSize, "data/Stage/Dark.pxe"},
	{gDarkpxmData, gDarkpxmSize, "data/Stage/Dark.pxm"},
	{gDarktscData, gDarktscSize, "data/Stage/Dark.tsc"},
	{gDrainpxeData, gDrainpxeSize, "data/Stage/Drain.pxe"},
	{gDrainpxmData, gDrainpxmSize, "data/Stage/Drain.pxm"},
	{gDraintscData, gDraintscSize, "data/Stage/Drain.tsc"},
	{gEgEnd1pxeData, gEgEnd1pxeSize, "data/Stage/EgEnd1.pxe"},
	{gEgEnd1pxmData, gEgEnd1pxmSize, "data/Stage/EgEnd1.pxm"},
	{gEgEnd1tscData, gEgEnd1tscSize, "data/Stage/EgEnd1.tsc"},
	{gEgEnd2pxeData, gEgEnd2pxeSize, "data/Stage/EgEnd2.pxe"},
	{gEgEnd2pxmData, gEgEnd2pxmSize, "data/Stage/EgEnd2.pxm"},
	{gEgEnd2tscData, gEgEnd2tscSize, "data/Stage/EgEnd2.tsc"},
	{gEgg1pxeData, gEgg1pxeSize, "data/Stage/Egg1.pxe"},
	{gEgg1pxmData, gEgg1pxmSize, "data/Stage/Egg1.pxm"},
	{gEgg1tscData, gEgg1tscSize, "data/Stage/Egg1.tsc"},
	{gEgg6pxeData, gEgg6pxeSize, "data/Stage/Egg6.pxe"},
	{gEgg6pxmData, gEgg6pxmSize, "data/Stage/Egg6.pxm"},
	{gEgg6tscData, gEgg6tscSize, "data/Stage/Egg6.tsc"},
	{gEggInpxaData, gEggInpxaSize, "data/Stage/EggIn.pxa"},
	{gEggRpxeData, gEggRpxeSize, "data/Stage/EggR.pxe"},
	{gEggRpxmData, gEggRpxmSize, "data/Stage/EggR.pxm"},
	{gEggRtscData, gEggRtscSize, "data/Stage/EggR.tsc"},
	{gEggR2pxeData, gEggR2pxeSize, "data/Stage/EggR2.pxe"},
	{gEggR2pxmData, gEggR2pxmSize, "data/Stage/EggR2.pxm"},
	{gEggR2tscData, gEggR2tscSize, "data/Stage/EggR2.tsc"},
	{gEggXpxaData, gEggXpxaSize, "data/Stage/EggX.pxa"},
	{gEggXpxeData, gEggXpxeSize, "data/Stage/EggX.pxe"},
	{gEggXpxmData, gEggXpxmSize, "data/Stage/EggX.pxm"},
	{gEggXtscData, gEggXtscSize, "data/Stage/EggX.tsc"},
	{gEggX2pxeData, gEggX2pxeSize, "data/Stage/EggX2.pxe"},
	{gEggX2pxmData, gEggX2pxmSize, "data/Stage/EggX2.pxm"},
	{gEggX2tscData, gEggX2tscSize, "data/Stage/EggX2.tsc"},
	{gEggspxaData, gEggspxaSize, "data/Stage/Eggs.pxa"},
	{gEggspxeData, gEggspxeSize, "data/Stage/Eggs.pxe"},
	{gEggspxmData, gEggspxmSize, "data/Stage/Eggs.pxm"},
	{gEggstscData, gEggstscSize, "data/Stage/Eggs.tsc"},
	{gEggs2pxeData, gEggs2pxeSize, "data/Stage/Eggs2.pxe"},
	{gEggs2pxmData, gEggs2pxmSize, "data/Stage/Eggs2.pxm"},
	{gEggs2tscData, gEggs2tscSize, "data/Stage/Eggs2.tsc"},
	{gFallpxaData, gFallpxaSize, "data/Stage/Fall.pxa"},
	{gFallpxeData, gFallpxeSize, "data/Stage/Fall.pxe"},
	{gFallpxmData, gFallpxmSize, "data/Stage/Fall.pxm"},
	{gFalltscData, gFalltscSize, "data/Stage/Fall.tsc"},
	{gFrogpxeData, gFrogpxeSize, "data/Stage/Frog.pxe"},
	{gFrogpxmData, gFrogpxmSize, "data/Stage/Frog.pxm"},
	{gFrogtscData, gFrogtscSize, "data/Stage/Frog.tsc"},
	{gGardpxaData, gGardpxaSize, "data/Stage/Gard.pxa"},
	{gGardpxeData, gGardpxeSize, "data/Stage/Gard.pxe"},
	{gGardpxmData, gGardpxmSize, "data/Stage/Gard.pxm"},
	{gGardtscData, gGardtscSize, "data/Stage/Gard.tsc"},
	{gHellpxaData, gHellpxaSize, "data/Stage/Hell.pxa"},
	{gHell1pxeData, gHell1pxeSize, "data/Stage/Hell1.pxe"},
	{gHell1pxmData, gHell1pxmSize, "data/Stage/Hell1.pxm"},
	{gHell1tscData, gHell1tscSize, "data/Stage/Hell1.tsc"},
	{gHell2pxeData, gHell2pxeSize, "data/Stage/Hell2.pxe"},
	{gHell2pxmData, gHell2pxmSize, "data/Stage/Hell2.pxm"},
	{gHell2tscData, gHell2tscSize, "data/Stage/Hell2.tsc"},
	{gHell3pxeData, gHell3pxeSize, "data/Stage/Hell3.pxe"},
	{gHell3pxmData, gHell3pxmSize, "data/Stage/Hell3.pxm"},
	{gHell3tscData, gHell3tscSize, "data/Stage/Hell3.tsc"},
	{gHell4pxeData, gHell4pxeSize, "data/Stage/Hell4.pxe"},
	{gHell4pxmData, gHell4pxmSize, "data/Stage/Hell4.pxm"},
	{gHell4tscData, gHell4tscSize, "data/Stage/Hell4.tsc"},
	{gHell42pxeData, gHell42pxeSize, "data/Stage/Hell42.pxe"},
	{gHell42pxmData, gHell42pxmSize, "data/Stage/Hell42.pxm"},
	{gHell42tscData, gHell42tscSize, "data/Stage/Hell42.tsc"},
	{gIslandpxeData, gIslandpxeSize, "data/Stage/Island.pxe"},
	{gIslandpxmData, gIslandpxmSize, "data/Stage/Island.pxm"},
	{gIslandtscData, gIslandtscSize, "data/Stage/Island.tsc"},
	{gItohpxeData, gItohpxeSize, "data/Stage/Itoh.pxe"},
	{gItohpxmData, gItohpxmSize, "data/Stage/Itoh.pxm"},
	{gItohtscData, gItohtscSize, "data/Stage/Itoh.tsc"},
	{gJailpxaData, gJailpxaSize, "data/Stage/Jail.pxa"},
	{gJail1pxeData, gJail1pxeSize, "data/Stage/Jail1.pxe"},
	{gJail1pxmData, gJail1pxmSize, "data/Stage/Jail1.pxm"},
	{gJail1tscData, gJail1tscSize, "data/Stage/Jail1.tsc"},
	{gJail2pxeData, gJail2pxeSize, "data/Stage/Jail2.pxe"},
	{gJail2pxmData, gJail2pxmSize, "data/Stage/Jail2.pxm"},
	{gJail2tscData, gJail2tscSize, "data/Stage/Jail2.tsc"},
	{gJenka1pxeData, gJenka1pxeSize, "data/Stage/Jenka1.pxe"},
	{gJenka1pxmData, gJenka1pxmSize, "data/Stage/Jenka1.pxm"},
	{gJenka1tscData, gJenka1tscSize, "data/Stage/Jenka1.tsc"},
	{gJenka2pxeData, gJenka2pxeSize, "data/Stage/Jenka2.pxe"},
	{gJenka2pxmData, gJenka2pxmSize, "data/Stage/Jenka2.pxm"},
	{gJenka2tscData, gJenka2tscSize, "data/Stage/Jenka2.tsc"},
	{gKingspxeData, gKingspxeSize, "data/Stage/Kings.pxe"},
	{gKingspxmData, gKingspxmSize, "data/Stage/Kings.pxm"},
	{gKingstscData, gKingstscSize, "data/Stage/Kings.tsc"},
	{gLabopxaData, gLabopxaSize, "data/Stage/Labo.pxa"},
	{gLittlepxeData, gLittlepxeSize, "data/Stage/Little.pxe"},
	{gLittlepxmData, gLittlepxmSize, "data/Stage/Little.pxm"},
	{gLittletscData, gLittletscSize, "data/Stage/Little.tsc"},
	{gLoungepxeData, gLoungepxeSize, "data/Stage/Lounge.pxe"},
	{gLoungepxmData, gLoungepxmSize, "data/Stage/Lounge.pxm"},
	{gLoungetscData, gLoungetscSize, "data/Stage/Lounge.tsc"},
	{gMalcopxeData, gMalcopxeSize, "data/Stage/Malco.pxe"},
	{gMalcopxmData, gMalcopxmSize, "data/Stage/Malco.pxm"},
	{gMalcotscData, gMalcotscSize, "data/Stage/Malco.tsc"},
	{gMapipxeData, gMapipxeSize, "data/Stage/Mapi.pxe"},
	{gMapipxmData, gMapipxmSize, "data/Stage/Mapi.pxm"},
	{gMapitscData, gMapitscSize, "data/Stage/Mapi.tsc"},
	{gMazepxaData, gMazepxaSize, "data/Stage/Maze.pxa"},
	{gMazeApxeData, gMazeApxeSize, "data/Stage/MazeA.pxe"},
	{gMazeApxmData, gMazeApxmSize, "data/Stage/MazeA.pxm"},
	{gMazeAtscData, gMazeAtscSize, "data/Stage/MazeA.tsc"},
	{gMazeBpxeData, gMazeBpxeSize, "data/Stage/MazeB.pxe"},
	{gMazeBpxmData, gMazeBpxmSize, "data/Stage/MazeB.pxm"},
	{gMazeBtscData, gMazeBtscSize, "data/Stage/MazeB.tsc"},
	{gMazeDpxeData, gMazeDpxeSize, "data/Stage/MazeD.pxe"},
	{gMazeDpxmData, gMazeDpxmSize, "data/Stage/MazeD.pxm"},
	{gMazeDtscData, gMazeDtscSize, "data/Stage/MazeD.tsc"},
	{gMazeHpxeData, gMazeHpxeSize, "data/Stage/MazeH.pxe"},
	{gMazeHpxmData, gMazeHpxmSize, "data/Stage/MazeH.pxm"},
	{gMazeHtscData, gMazeHtscSize, "data/Stage/MazeH.tsc"},
	{gMazeIpxeData, gMazeIpxeSize, "data/Stage/MazeI.pxe"},
	{gMazeIpxmData, gMazeIpxmSize, "data/Stage/MazeI.pxm"},
	{gMazeItscData, gMazeItscSize, "data/Stage/MazeI.tsc"},
	{gMazeMpxeData, gMazeMpxeSize, "data/Stage/MazeM.pxe"},
	{gMazeMpxmData, gMazeMpxmSize, "data/Stage/MazeM.pxm"},
	{gMazeMtscData, gMazeMtscSize, "data/Stage/MazeM.tsc"},
	{gMazeOpxeData, gMazeOpxeSize, "data/Stage/MazeO.pxe"},
	{gMazeOpxmData, gMazeOpxmSize, "data/Stage/MazeO.pxm"},
	{gMazeOtscData, gMazeOtscSize, "data/Stage/MazeO.tsc"},
	{gMazeSpxeData, gMazeSpxeSize, "data/Stage/MazeS.pxe"},
	{gMazeSpxmData, gMazeSpxmSize, "data/Stage/MazeS.pxm"},
	{gMazeStscData, gMazeStscSize, "data/Stage/MazeS.tsc"},
	{gMazeWpxeData, gMazeWpxeSize, "data/Stage/MazeW.pxe"},
	{gMazeWpxmData, gMazeWpxmSize, "data/Stage/MazeW.pxm"},
	{gMazeWtscData, gMazeWtscSize, "data/Stage/MazeW.tsc"},
	{gMiBoxpxeData, gMiBoxpxeSize, "data/Stage/MiBox.pxe"},
	{gMiBoxpxmData, gMiBoxpxmSize, "data/Stage/MiBox.pxm"},
	{gMiBoxtscData, gMiBoxtscSize, "data/Stage/MiBox.tsc"},
	{gMimipxaData, gMimipxaSize, "data/Stage/Mimi.pxa"},
	{gMimipxeData, gMimipxeSize, "data/Stage/Mimi.pxe"},
	{gMimipxmData, gMimipxmSize, "data/Stage/Mimi.pxm"},
	{gMimitscData, gMimitscSize, "data/Stage/Mimi.tsc"},
	{gMomopxeData, gMomopxeSize, "data/Stage/Momo.pxe"},
	{gMomopxmData, gMomopxmSize, "data/Stage/Momo.pxm"},
	{gMomotscData, gMomotscSize, "data/Stage/Momo.tsc"},
	{gNewpxeData, gNewpxeSize, "data/Stage/New.pxe"},
	{gOsidepxaData, gOsidepxaSize, "data/Stage/Oside.pxa"},
	{gOsidepxeData, gOsidepxeSize, "data/Stage/Oside.pxe"},
	{gOsidepxmData, gOsidepxmSize, "data/Stage/Oside.pxm"},
	{gOsidetscData, gOsidetscSize, "data/Stage/Oside.tsc"},
	{gOsteppxeData, gOsteppxeSize, "data/Stage/Ostep.pxe"},
	{gOsteppxmData, gOsteppxmSize, "data/Stage/Ostep.pxm"},
	{gOsteptscData, gOsteptscSize, "data/Stage/Ostep.tsc"},
	{gPenspxaData, gPenspxaSize, "data/Stage/Pens.pxa"},
	{gPens1pxeData, gPens1pxeSize, "data/Stage/Pens1.pxe"},
	{gPens1pxmData, gPens1pxmSize, "data/Stage/Pens1.pxm"},
	{gPens1tscData, gPens1tscSize, "data/Stage/Pens1.tsc"},
	{gPens2pxeData, gPens2pxeSize, "data/Stage/Pens2.pxe"},
	{gPens2pxmData, gPens2pxmSize, "data/Stage/Pens2.pxm"},
	{gPens2tscData, gPens2tscSize, "data/Stage/Pens2.tsc"},
	{gPixelpxeData, gPixelpxeSize, "data/Stage/Pixel.pxe"},
	{gPixelpxmData, gPixelpxmSize, "data/Stage/Pixel.pxm"},
	{gPixeltscData, gPixeltscSize, "data/Stage/Pixel.tsc"},
	{gPlantpxeData, gPlantpxeSize, "data/Stage/Plant.pxe"},
	{gPlantpxmData, gPlantpxmSize, "data/Stage/Plant.pxm"},
	{gPlanttscData, gPlanttscSize, "data/Stage/Plant.tsc"},
	{gPolepxeData, gPolepxeSize, "data/Stage/Pole.pxe"},
	{gPolepxmData, gPolepxmSize, "data/Stage/Pole.pxm"},
	{gPoletscData, gPoletscSize, "data/Stage/Pole.tsc"},
	{gPoolpxeData, gPoolpxeSize, "data/Stage/Pool.pxe"},
	{gPoolpxmData, gPoolpxmSize, "data/Stage/Pool.pxm"},
	{gPooltscData, gPooltscSize, "data/Stage/Pool.tsc"},
	{gPrefa1pxeData, gPrefa1pxeSize, "data/Stage/Prefa1.pxe"},
	{gPrefa1pxmData, gPrefa1pxmSize, "data/Stage/Prefa1.pxm"},
	{gPrefa1tscData, gPrefa1tscSize, "data/Stage/Prefa1.tsc"},
	{gPrefa2pxeData, gPrefa2pxeSize, "data/Stage/Prefa2.pxe"},
	{gPrefa2pxmData, gPrefa2pxmSize, "data/Stage/Prefa2.pxm"},
	{gPrefa2tscData, gPrefa2tscSize, "data/Stage/Prefa2.tsc"},
	{gPriso1pxeData, gPriso1pxeSize, "data/Stage/Priso1.pxe"},
	{gPriso1pxmData, gPriso1pxmSize, "data/Stage/Priso1.pxm"},
	{gPriso1tscData, gPriso1tscSize, "data/Stage/Priso1.tsc"},
	{gPriso2pxeData, gPriso2pxeSize, "data/Stage/Priso2.pxe"},
	{gPriso2pxmData, gPriso2pxmSize, "data/Stage/Priso2.pxm"},
	{gPriso2tscData, gPriso2tscSize, "data/Stage/Priso2.tsc"},
	{gPrt0pngData, gPrt0pngSize, "data/Stage/Prt0.png"},
	{gPrtAlmondpngData, gPrtAlmondpngSize, "data/Stage/PrtAlmond.png"},
	{gPrtBarrpngData, gPrtBarrpngSize, "data/Stage/PrtBarr.png"},
	{gPrtCavepngData, gPrtCavepngSize, "data/Stage/PrtCave.png"},
	{gPrtCentpngData, gPrtCentpngSize, "data/Stage/PrtCent.png"},
	{gPrtEggInpngData, gPrtEggInpngSize, "data/Stage/PrtEggIn.png"},
	{gPrtEggXpngData, gPrtEggXpngSize, "data/Stage/PrtEggX.png"},
	{gPrtEggspngData, gPrtEggspngSize, "data/Stage/PrtEggs.png"},
	{gPrtFallpngData, gPrtFallpngSize, "data/Stage/PrtFall.png"},
	{gPrtGardpngData, gPrtGardpngSize, "data/Stage/PrtGard.png"},
	{gPrtHellpngData, gPrtHellpngSize, "data/Stage/PrtHell.png"},
	{gPrtJailpngData, gPrtJailpngSize, "data/Stage/PrtJail.png"},
	{gPrtLabopngData, gPrtLabopngSize, "data/Stage/PrtLabo.png"},
	{gPrtMazepngData, gPrtMazepngSize, "data/Stage/PrtMaze.png"},
	{gPrtMimipngData, gPrtMimipngSize, "data/Stage/PrtMimi.png"},
	{gPrtOsidepngData, gPrtOsidepngSize, "data/Stage/PrtOside.png"},
	{gPrtPenspngData, gPrtPenspngSize, "data/Stage/PrtPens.png"},
	{gPrtRiverpngData, gPrtRiverpngSize, "data/Stage/PrtRiver.png"},
	{gPrtSandpngData, gPrtSandpngSize, "data/Stage/PrtSand.png"},
	{gPrtStorepngData, gPrtStorepngSize, "data/Stage/PrtStore.png"},
	{gPrtWeedpngData, gPrtWeedpngSize, "data/Stage/PrtWeed.png"},
	{gPrtWhitepngData, gPrtWhitepngSize, "data/Stage/PrtWhite.png"},
	{gRing1pxeData, gRing1pxeSize, "data/Stage/Ring1.pxe"},
	{gRing1pxmData, gRing1pxmSize, "data/Stage/Ring1.pxm"},
	{gRing1tscData, gRing1tscSize, "data/Stage/Ring1.tsc"},
	{gRing2pxeData, gRing2pxeSize, "data/Stage/Ring2.pxe"},
	{gRing2pxmData, gRing2pxmSize, "data/Stage/Ring2.pxm"},
	{gRing2tscData, gRing2tscSize, "data/Stage/Ring2.tsc"},
	{gRing3pxeData, gRing3pxeSize, "data/Stage/Ring3.pxe"},
	{gRing3pxmData, gRing3pxmSize, "data/Stage/Ring3.pxm"},
	{gRing3tscData, gRing3tscSize, "data/Stage/Ring3.tsc"},
	{gRiverpxaData, gRiverpxaSize, "data/Stage/River.pxa"},
	{gRiverpxeData, gRiverpxeSize, "data/Stage/River.pxe"},
	{gRiverpxmData, gRiverpxmSize, "data/Stage/River.pxm"},
	{gRivertscData, gRivertscSize, "data/Stage/River.tsc"},
	{gSandpxaData, gSandpxaSize, "data/Stage/Sand.pxa"},
	{gSandpxeData, gSandpxeSize, "data/Stage/Sand.pxe"},
	{gSandpxmData, gSandpxmSize, "data/Stage/Sand.pxm"},
	{gSandtscData, gSandtscSize, "data/Stage/Sand.tsc"},
	{gSandEpxeData, gSandEpxeSize, "data/Stage/SandE.pxe"},
	{gSandEpxmData, gSandEpxmSize, "data/Stage/SandE.pxm"},
	{gSandEtscData, gSandEtscSize, "data/Stage/SandE.tsc"},
	{gSantapxeData, gSantapxeSize, "data/Stage/Santa.pxe"},
	{gSantapxmData, gSantapxmSize, "data/Stage/Santa.pxm"},
	{gSantatscData, gSantatscSize, "data/Stage/Santa.tsc"},
	{gSheltpxaData, gSheltpxaSize, "data/Stage/Shelt.pxa"},
	{gSheltpxeData, gSheltpxeSize, "data/Stage/Shelt.pxe"},
	{gSheltpxmData, gSheltpxmSize, "data/Stage/Shelt.pxm"},
	{gShelttscData, gShelttscSize, "data/Stage/Shelt.tsc"},
	{gStartpxeData, gStartpxeSize, "data/Stage/Start.pxe"},
	{gStartpxmData, gStartpxmSize, "data/Stage/Start.pxm"},
	{gStarttscData, gStarttscSize, "data/Stage/Start.tsc"},
	{gStatuepxeData, gStatuepxeSize, "data/Stage/Statue.pxe"},
	{gStatuepxmData, gStatuepxmSize, "data/Stage/Statue.pxm"},
	{gStatuetscData, gStatuetscSize, "data/Stage/Statue.tsc"},
	{gStorepxaData, gStorepxaSize, "data/Stage/Store.pxa"},
	{gStreampxeData, gStreampxeSize, "data/Stage/Stream.pxe"},
	{gStreampxmData, gStreampxmSize, "data/Stage/Stream.pxm"},
	{gStreamtscData, gStreamtscSize, "data/Stage/Stream.tsc"},
	{gWeedpxaData, gWeedpxaSize, "data/Stage/Weed.pxa"},
	{gWeedpxeData, gWeedpxeSize, "data/Stage/Weed.pxe"},
	{gWeedpxmData, gWeedpxmSize, "data/Stage/Weed.pxm"},
	{gWeedtscData, gWeedtscSize, "data/Stage/Weed.tsc"},
	{gWeedBpxeData, gWeedBpxeSize, "data/Stage/WeedB.pxe"},
	{gWeedBpxmData, gWeedBpxmSize, "data/Stage/WeedB.pxm"},
	{gWeedBtscData, gWeedBtscSize, "data/Stage/WeedB.tsc"},
	{gWeedDpxeData, gWeedDpxeSize, "data/Stage/WeedD.pxe"},
	{gWeedDpxmData, gWeedDpxmSize, "data/Stage/WeedD.pxm"},
	{gWeedDtscData, gWeedDtscSize, "data/Stage/WeedD.tsc"},
	{gWeedSpxeData, gWeedSpxeSize, "data/Stage/WeedS.pxe"},
	{gWeedSpxmData, gWeedSpxmSize, "data/Stage/WeedS.pxm"},
	{gWeedStscData, gWeedStscSize, "data/Stage/WeedS.tsc"},
	{gWhitepxaData, gWhitepxaSize, "data/Stage/White.pxa"},
	{ge_BlcnpxeData, ge_BlcnpxeSize, "data/Stage/e_Blcn.pxe"},
	{ge_BlcnpxmData, ge_BlcnpxmSize, "data/Stage/e_Blcn.pxm"},
	{ge_BlcntscData, ge_BlcntscSize, "data/Stage/e_Blcn.tsc"},
	{ge_CemepxeData, ge_CemepxeSize, "data/Stage/e_Ceme.pxe"},
	{ge_CemepxmData, ge_CemepxmSize, "data/Stage/e_Ceme.pxm"},
	{ge_CemetscData, ge_CemetscSize, "data/Stage/e_Ceme.tsc"},
	{ge_JenkpxeData, ge_JenkpxeSize, "data/Stage/e_Jenk.pxe"},
	{ge_JenkpxmData, ge_JenkpxmSize, "data/Stage/e_Jenk.pxm"},
	{ge_JenktscData, ge_JenktscSize, "data/Stage/e_Jenk.tsc"},
	{ge_LabopxeData, ge_LabopxeSize, "data/Stage/e_Labo.pxe"},
	{ge_LabopxmData, ge_LabopxmSize, "data/Stage/e_Labo.pxm"},
	{ge_LabotscData, ge_LabotscSize, "data/Stage/e_Labo.tsc"},
	{ge_MalcpxeData, ge_MalcpxeSize, "data/Stage/e_Malc.pxe"},
	{ge_MalcpxmData, ge_MalcpxmSize, "data/Stage/e_Malc.pxm"},
	{ge_MalctscData, ge_MalctscSize, "data/Stage/e_Malc.tsc"},
	{ge_MazepxeData, ge_MazepxeSize, "data/Stage/e_Maze.pxe"},
	{ge_MazepxmData, ge_MazepxmSize, "data/Stage/e_Maze.pxm"},
	{ge_MazetscData, ge_MazetscSize, "data/Stage/e_Maze.tsc"},
	{ge_SkypxeData, ge_SkypxeSize, "data/Stage/e_Sky.pxe"},
	{ge_SkypxmData, ge_SkypxmSize, "data/Stage/e_Sky.pxm"},
	{ge_SkytscData, ge_SkytscSize, "data/Stage/e_Sky.tsc"},
	{gACCESSorgData, gACCESSorgSize, "data/ORG/ACCESS.org"},
	{gANZENorgData, gANZENorgSize, "data/ORG/ANZEN.org"},
	{gBALCONYorgData, gBALCONYorgSize, "data/ORG/BALCONY.org"},
	{gBALLOSorgData, gBALLOSorgSize, "data/ORG/BALLOS.org"},
	{gBDOWNorgData, gBDOWNorgSize, "data/ORG/BDOWN.org"},
	{gCEMETERYorgData, gCEMETERYorgSize, "data/ORG/CEMETERY.org"},
	{gCURLYorgData, gCURLYorgSize, "data/ORG/CURLY.org"},
	{gDRorgData, gDRorgSize, "data/ORG/DR.org"},
	{gENDINGorgData, gENDINGorgSize, "data/ORG/ENDING.org"},
	{gESCAPEorgData, gESCAPEorgSize, "data/ORG/ESCAPE.org"},
	{gFANFALE1orgData, gFANFALE1orgSize, "data/ORG/FANFALE1.org"},
	{gFANFALE2orgData, gFANFALE2orgSize, "data/ORG/FANFALE2.org"},
	{gFANFALE3orgData, gFANFALE3orgSize, "data/ORG/FANFALE3.org"},
	{gFIREEYEorgData, gFIREEYEorgSize, "data/ORG/FIREEYE.org"},
	{gGAMEOVERorgData, gGAMEOVERorgSize, "data/ORG/GAMEOVER.org"},
	{gGINSUKEorgData, gGINSUKEorgSize, "data/ORG/GINSUKE.org"},
	{gGRANDorgData, gGRANDorgSize, "data/ORG/GRAND.org"},
	{gGRAVITYorgData, gGRAVITYorgSize, "data/ORG/GRAVITY.org"},
	{gHELLorgData, gHELLorgSize, "data/ORG/HELL.org"},
	{gIRONHorgData, gIRONHorgSize, "data/ORG/IRONH.org"},
	{gJENKAorgData, gJENKAorgSize, "data/ORG/JENKA.org"},
	{gJENKA2orgData, gJENKA2orgSize, "data/ORG/JENKA2.org"},
	{gKODOUorgData, gKODOUorgSize, "data/ORG/KODOU.org"},
	{gLASTBT3orgData, gLASTBT3orgSize, "data/ORG/LASTBT3.org"},
	{gLASTBTLorgData, gLASTBTLorgSize, "data/ORG/LASTBTL.org"},
	{gLASTCAVEorgData, gLASTCAVEorgSize, "data/ORG/LASTCAVE.org"},
	{gMARINEorgData, gMARINEorgSize, "data/ORG/MARINE.org"},
	{gMAZEorgData, gMAZEorgSize, "data/ORG/MAZE.org"},
	{gMDOWN2orgData, gMDOWN2orgSize, "data/ORG/MDOWN2.org"},
	{gMURAorgData, gMURAorgSize, "data/ORG/MURA.org"},
	{gOSIDEorgData, gOSIDEorgSize, "data/ORG/OSIDE.org"},
	{gPLANTorgData, gPLANTorgSize, "data/ORG/PLANT.org"},
	{gQUIETorgData, gQUIETorgSize, "data/ORG/QUIET.org"},
	{gREQUIEMorgData, gREQUIEMorgSize, "data/ORG/REQUIEM.org"},
	{gTOROKOorgData, gTOROKOorgSize, "data/ORG/TOROKO.org"},
	{gVIVIorgData, gVIVIorgSize, "data/ORG/VIVI.org"},
	{gWANPAK2orgData, gWANPAK2orgSize, "data/ORG/WANPAK2.org"},
	{gWANPAKUorgData, gWANPAKUorgSize, "data/ORG/WANPAKU.org"},
	{gWEEDorgData, gWEEDorgSize, "data/ORG/WEED.org"},
	{gWHITEorgData, gWHITEorgSize, "data/ORG/WHITE.org"},
	{gXXXXorgData, gXXXXorgSize, "data/ORG/XXXX.org"},
	{gZONBIEorgData, gZONBIEorgSize, "data/ORG/ZONBIE.org"},
	{gwavedatData, gwavedatSize, "data/ORG/wave.dat"},
};

//File struct functions


FILE_e *fopen_embed(const char *file, const char *mode)
{
	//Get our referenced file
	const FILEREF *fileref = NULL;
	FILE_e *fp;
	int i;
	
	for (i = 0; i < sizeof(files) / sizeof(FILEREF); i++)
	{
		if (!strcasecmp(file, files[i].path))
		{
			fileref = &files[i];
			break;
		}
			
	}
	
	if (fileref == NULL)
		return NULL;
	
	//Allocate file struct and construct
	fp = (FILE_e*)malloc(sizeof(FILE_e));
	fp->file = fileref->data;
	fp->point = fp->file;
	fp->size = fileref->size;
	fp->position = 0;
	
	return fp;
}

void fclose_embed(FILE_e *fp)
{
	//Free file struct
	free(fp);
}

size_t fread_embed(void *ptr, size_t size, size_t nmemb, FILE_e *fp)
{
	//Lazy memcpy
	memcpy(ptr, fp->point + fp->position, size * nmemb);
	fp->position += size * nmemb;
	return nmemb;
}
