import os


ndsname = "CSE2-DS-New.nds"
elfname = "CSE2-DS-New.elf"

releasefolder = "release04/"
elffolder = "ELF/"

sdfolder = "SD card version (moddable)/"

os.system("make clean")
os.system("make -j8")

os.rename(ndsname, releasefolder + "CaveStoryDS-v0.4EN.nds")

os.rename(elfname, releasefolder + elffolder +  "CaveStoryDS-v0.4EN.elf")

os.rename("srcarm7/CSE2-arm7.elf", releasefolder + elffolder + "CSE2-arm7.elf")

os.system("make -j8 DSI=1")

os.rename(ndsname, releasefolder + "CaveStoryDSi-v0.4EN.nds")

os.system("make clean")
os.system("make -j8 JPN=1")

os.rename(ndsname, releasefolder + "CaveStoryDS-v0.4JP.nds")

os.rename(elfname, releasefolder + elffolder +  "CaveStoryDS-v0.4JP.elf")

os.system("make -j8 JPN=1 DSI=1")

os.rename(ndsname, releasefolder + "CaveStoryDSi-v0.4JP.nds")


os.system("make clean")
os.system("make -j8 SD=1")

os.rename(ndsname, releasefolder + sdfolder + "CaveStoryDS-v0.4EN-SDCard.nds")
os.rename(elfname, releasefolder + elffolder + "CaveStoryDS-v0.4EN-SDCard.elf")

os.system("make -j8 SD=1 DSI=1")

os.rename(ndsname, releasefolder + sdfolder + "CaveStoryDSi-v0.4EN-SDCard.nds")


os.system("make clean")
os.system("make -j8 NITRO=1")

os.rename(ndsname, releasefolder + sdfolder + "CaveStoryDS-v0.4EN-NitroFS.nds")
os.rename(elfname, releasefolder + elffolder + "CaveStoryDS-v0.4EN-NitroFS.elf")

os.system("make -j8 NITRO=1 DSI=1")

os.rename(ndsname, releasefolder + sdfolder + "CaveStoryDSi-v0.4EN-NitroFS.nds")


