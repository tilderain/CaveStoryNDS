# Cave Story DS
CSE2 (for Nintendo DS)

You can find a technical writeup of the port's development here (TODO, lol).

## Compilation
Acquire devKitPro and libnds, then run make. Sound will probably be broken on most flashcarts, in this case you can use a compatible hbmenu, or compile it with libnds commit 62c8ffd.
For wifi to work you need to change dswifi's Wifi_RawTxFrame function. See https://github.com/CTurt/dsgmDSWiFi/blob/2080f4ab16781ad0165e1924ed83569513dcfb2e/arm9/source/wifi_arm9.c#L354 for details.
Don't forget to run make -j6 or something for fast as f*** compilation.

There are a few compile flags. 

-DJAPANESE will compile for Japanese language.

-DTWO_SCREENS will enable the broken two screen mode.

-DREAD_FROM_SD will disable incbin for files and instead read from the data folder on the SD card. (I think it fails to read files half the time though)

-DCYG_PROFILER will enable the profiler, needing -DREAD_FROM_SD flag and -finstrument-functions. (may inflate the size the binary too big to run though)

See this page for more info. 
 
http://ands-pdf.blogspot.com/2009/03/profiling-on-ds.html

## Developing

All files are loaded into RAM on start. Please ensure that the size of the NDS file does not exceed around 2.6MB, or else many flashcarts will fail to load it into memory depending on the size of their firmware.

In order for changes to files to take effect, you must remove fopen.o in the build folder or use make clean.
I might implement a gzip system to make this easier/reduce the size of the files.
You can use incbingenerator.py to make a file list for fopen.cpp.

The PNGs are required to be 16-color palettized, with specific overrides for ones that require 256 colors. The positions in VRAM are manually defined in Draw.cpp. Transparency might mess with the color count. TextBox.bmp is specifically resized 4 pixels wider to fix the alignment. I haven't figured out the specific requirements for a compatible png yet, sorry. TODO is to make a tool to convert bmp into compatible png.

## Thanks to

	CSE2- CuckyDev, Clownacy, GabrielRavier
	Lusty_- Testing
	Random people in the CSE2 Discord- Testing

	Aeon Genesis- Cave Story english translation

	devKitPro
	gl2D

	Drenn1- wifi code
	huiminghao- wifi code

	Ravenworks- Inspiring the port

	Pixel


## Licensing

Being a decompilation, the majority of the code in this project is proprietary
and belongs to Daisuke "Pixel" Amaya.

Modifications and custom code are made available under the MIT licence. See
`LICENCE.txt` for details.

There is some code that is modified from libnds. These are the ARM7 files, gl2d, soundFifo, and vram allocation in Draw.cpp.

The WiFi code is taken mostly from Drenn1's GameYob, with minor changes from huiminghao's NesDS. The original repositories have no license.