# Cave Story DS
CSE2 (for Nintendo DS)

You can find a technical writeup of the port's development here (TODO, lol).

## Compilation
Acquire devKitPro and libnds+dswifi, then run make. Sound will probably be broken on most flashcarts, so you need to compile this rom with libnds commit 62c8ffd, or launch it with a compatible hbmenu.
For wifi to work you need to change dswifi's Wifi_RawTxFrame function. See https://github.com/CTurt/dsgmDSWiFi/blob/2080f4ab16781ad0165e1924ed83569513dcfb2e/arm9/source/wifi_arm9.c#L354 for details.
Don't forget to run make -j6 or something for fast as f*** compilation.

There are a few compile flags. 

make JPN=1 will compile for Japanese language.

make SD=1 will disable incbin for files and instead read from the data folder on the SD card, enabling support for bmp, wav, and mp3/xm reads.

make NITRO=1 will allow you to play the SD card version on emulator.

make DSI=1 will compile a DSi rom.

-DTWO_SCREENS will enable the broken two screen mode.

-DCYG_PROFILER will enable the profiler, needing -DREAD_FROM_SD flag and -finstrument-functions. (may inflate the size the binary too big to run though, or may just straight up)

See this page for more info. 
 
http://ands-pdf.blogspot.com/2009/03/profiling-on-ds.html

## Developing

For making mods, I recommend to use SD=1 compilation flag, because it's much much easier to work with.
Below is a list of considerations when working with the non-SD version.

All files are loaded into RAM on start. Please ensure that the size of the NDS file does not exceed around 2.6MB, or many flashcarts will fail to load it into memory, depending on the size of their firmware.

In order for changes to files to take effect, you must remove fopen.o in the build folder or use make clean.
I might implement a gzip system to make this easier/reduce the size of the files.
You can use incbingenerator.py to make a file list for fopen.cpp.

The PNGs are required to be 16-color palettized, with specific overrides for ones that require 256 colors. Note that you may need to include an additional color so that the palette size reaches 17. The positions in VRAM are manually defined in Draw.cpp. You can check vramtetris.xcf to visualize the VRAM. Transparency might mess with the color count. TextBox.bmp is specifically resized 4 pixels wider to fix the alignment. I haven't figured out the specific requirements for a compatible png yet, sorry. TODO is to make a tool to convert bmp into compatible png.


## Licensing

Being a decompilation, the majority of the code in this project is proprietary
and belongs to Daisuke "Pixel" Amaya.

Modifications and custom code are made available under the MIT licence. See
`LICENCE.txt` for details.

There is some code that is modified from libnds. These are the ARM7 files, gl2d, soundFifo, and vram allocation in Draw.cpp.

The WiFi code is taken mostly from Drenn1's GameYob (licensed under MIT), with minor changes from huiminghao's NesDS (no license).

## Thanks to

	CSE2- CuckyDev, Clownacy, GabrielRavier
	
	Lusty_- Testing
	Random people in the CSE2 Discord- Testing

	Aeon Genesis- Shih Tzu (TranslateDoggie), GideonZhi
		Cave Story english translation

	devKitPro
	gl2D

	lvandeve- lodepng

	Drenn1- wifi code
	huiminghao- wifi code

	Ravenworks- Inspiring the port

	Pixel


