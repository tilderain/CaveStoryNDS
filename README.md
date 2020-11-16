# Cave Story DS
CSE2 (for Nintendo DS)

## Compilation
Acquire devKitPro and libnds, then run make. Sound will probably be broken on most flashcarts, in this case you can use a compatible hbmenu, or compile it with libnds commit 62c8ffd.

There are a few compile flags. -DTWO_SCREENS will enable the broken two screen mode. -DREAD_FROM_SD will disable incbin for files and instead read from the data folder on the SD card. -DCYG_PROFILER will enable the profiler, needing -DREAD_FROM_SD flag and -finstrument-functions. See this page for more info. http://ands-pdf.blogspot.com/2009/03/profiling-on-ds.html

## Thanks to
CSE2- Clownacy, CuckyDev, GabrielRavier

CuckyDev- font code, fopen.c

Aeon Genesis- Cave Story english translation

devKitPro

gl2D

Pixel


## Licensing

Being a decompilation, the majority of the code in this project is proprietary
and belongs to Daisuke "Pixel" Amaya.

Modifications and custom code are made available under the MIT licence. See
`LICENCE.txt` for details.

There is some code that is modified from libnds. These are the ARM7 files, gl2d, soundFifo, and vram allocation in Draw.cpp.