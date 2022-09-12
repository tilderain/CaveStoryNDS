# Cave Story DS
<img src="/preview.jpg">

CSE2 (for Nintendo DS)



## Table of Contents

This repo has two main branches:

Branch | Description
--------|--------
[ds](https://www.github.com/tilderain/CaveStoryDS/tree/ds) | You are here :)
[ds2DGfx](https://www.github.com/tilderain/CaveStoryDS/tree/ds2DGfx) | An attempt at using the 2D hardware to render graphics on both screens.

## Compilation
Acquire devKitPro and libnds+dswifi, then run make. Sound will probably be broken on most flashcarts, so you need to compile this rom with libnds commit 62c8ffd, or launch it with a compatible hbmenu.
For wifi to work you need to change dswifi's Wifi_RawTxFrame function. See https://github.com/CTurt/dsgmDSWiFi/blob/2080f4ab16781ad0165e1924ed83569513dcfb2e/arm9/source/wifi_arm9.c#L354 for details.
Don't forget to run make -j6 or something for fast as f*** compilation.

There are a few compile flags. 

make JPN=1 will compile for Japanese language. Not compatible with SD card version.

make SD=1 will disable incbin for files and instead read from the data folder on the SD card, enabling support for BMP and wav file reading, and easy replacement of game files. 

make NITRO=1 will allow you to play the SD card version on emulator. It's ok for packaging the SD files into a single rom, but flashcart compatibility is low and file reading is slower than the other versions.

make DSI=1 will compile a DSi rom with an extended header. (it won't work on DS flashcarts natively)

make PROFILER=1 will build with profiler flags (broken).

-DTWO_SCREENS will enable the broken two screen mode.

-DCYG_PROFILER will enable the profiler, needing -DREAD_FROM_SD flag and -finstrument-functions. (may inflate the size the binary too big to run though. I think this project has too many functions for it to work now...)

See this page for more info. 
 
http://ands-pdf.blogspot.com/2009/03/profiling-on-ds.html

## Developing

For making mods, I recommend to use SD=1 compilation flag, because it's much much easier to work with.
Below is a list of considerations when working with the non-SD version.

All files are loaded into RAM on start. Please ensure that the size of the NDS file does not exceed around 2.6MB, or many flashcarts will fail to load it into memory, depending on the size of their firmware.

In order for changes to files to take effect, you must remove fopen.o in the build folder or use make clean.
I might implement a gzip system to make this easier/reduce the size of the files.
You can use incbingenerator.py to make a file list for fopen.cpp. Insert the output into fopenincbins.h.

The PNGs are required to be 16-color palettized, with specific overrides for ones that require 256 colors. Note that you may need to include an additional color so that the palette size reaches 17. The positions in VRAM are manually defined in Draw.cpp. You can check vramtetris.xcf to visualize the VRAM. Transparency might mess with the color count. TextBox.bmp is specifically resized 4 pixels wider to fix the alignment. I haven't figured out the specific requirements for a compatible png yet, sorry. TODO is to make a tool to convert bmp into compatible png, or just fix my png loading code...

You can also compress files when embedded using this tool. https://github.com/DS-Homebrew/nds-bootstrap/blob/master/lzss.c
The org files are already compressed.

By default sounds are loaded as .swav. You can get waveconv.exe from the official Nintendo DS SDK to convert wavs into ADPCM.

Tile 0 is not rendered by default. Check PutStage_Back() to change it.


## Licensing

Being a decompilation, the majority of the code in this project is proprietary
and belongs to Daisuke "Pixel" Amaya.

Modifications and custom code are made available under the MIT licence. See
`LICENCE.txt` for details.

There is some code that is modified from libnds. These are the ARM7 files, gl2d, soundFifo, and vram allocation in Draw.cpp.

The WiFi code is taken mostly from Drenn1's GameYob (licensed under MIT), with minor changes from huiminghao's NesDS (no license).

BMP loading code is taken from Nitro Engine (MIT license). (https://github.com/AntonioND/nitro-engine/blob/master/source/NEFormats.c)

## Thanks to

The CSE2 Decompilation Project, by CuckyDev and Clownacy

(and our optimization mascot GabrielRavier)

The unauthorized source code recreation that lets us do weird things to this game.

CuckyDev, who helped me personally by making the font and file loading code, helped with the sound code,

and made this port too funny to make.

Lusty_ from GBATemp, who helped test and was able to help me figure out the compatibility issue with flashcarts.

Random people in the CSE2 Discord, who tried the port in its early stages and helped me discover that I'm not crazy for making it.

Aeon Genesis: Shih Tzu (TranslateDoggie), and GideonZhi,

whose expert translation and ROM hacking skills brought Cave Story to a boatload of people.

devKitPro, the homebrew toolchain that lets us make games for Nintendo consoles without anyone's permission.

Martin Korth, for making N0$GBA and its fantastic debugging suite, as well as the GBA/DS/3DS documentation that shaped the scene.

The DeSmuME developers for making an emulator with GDB debugging support making this port a lot less of a pain in the ass to develop.

gl2D- by Relminator, for making a pretty good 3D library saving my headaches looking at the documentation.

lvandeve, for the lodepng library that lets me painlessly load pngs.

and AntonioND, for the great BMP loading from Nitro Engine as well.

mackron, for the sexy dr_wav loading library.

graphitemaster, for the sleek incbin file embed library.

CUE, for the sly NDS lzss compression library.

Drenn1, for the WiFi code from GameYob, and the forum posts on the devKitPro forums that probably let me avoid

a dozen hours of debugging the sound.

huiminghao, for the WiFi code from NesDS.

Arisutora, for MelonDS and the NiFi debugging support.

Ravenworks, who took on the Cave Story DS port in the first place, got destroyed by a publisher in the process, and inspired me 

<<<<<<< HEAD
to create the DS version that the people deserve.
=======
	to create the DS version that the people deserve.
>>>>>>> acda353b5a27ff3e9f893d21150c5c2ae408cb30

And Pixel, who created this wonderful thing that inspired a generation of gamers.

Thank you Pixel!

