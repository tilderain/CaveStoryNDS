SD card version information:
	The SD card version is *relatively* easy to mod. You can also run it if the normal version doesn't work.
	It requires the datacse2 folder to be in the SD card root.

	You can configure DeSmuME to test your mods easily.
	Go to Config--> Slot 1, and set it to R4. Check "use path of the loaded ROM".
	You should be able to play mods if datacse2 is in the same folder.

	The NitroFS version is also moddable, runs easily on emulators, and packages mods into a single file, although you need to use
	devkitPro's ndstool to extract and re-package the data files into the rom. The loading time is also slower.
	For consoles, NitroFS ver. needs to be launched with TWiLight Menu++, or nds-hb-menu 0.5.0. 

	As of now there is only support for English mods.
--Modding instructions:
	Grab a copy of Booster's Lab 0.5.0.0 or less, open your mod, and export a stage.tbl.
	You can now copy and paste the contents of your mod's data folder into the datacse2 folder.

	For the BMP files, you can read palettized 16-color bmps, compatible with freeware Cave Story.
	NpcGuest, NpcRegu, casts, and Face.bmp are allowed to be 256-color.

	I recommend to install ImageMagick and run this bash command for each folder of spritesheets.

	for f in *.bmp; do convert "$f" -colors 16 -compress none "$f"; done

	You then need to use a program to re-export each BMP. I use GIMP with "do not include color space information" checked in the export settings.

	To change the starting and intro map, check out mapstart/mapintro.txt in datacse2.
	The order is the Stage, event to run on entry, x and y position of the player.

	To read sounds, place the corresponding sound in the WAVE folder.
	It will attempt to read the following formats: 
		8-bit 22050hz unsigned WAV audio (as ".wav")
		Raw 8-bit 22050hz signed audio (as ".raw")
		ADPCM sound using Nintendo's waveconv.exe, from the NDS SDK (as ".swav")
	
Exporting custom sounds from mods easily:
	Grab SeaTone by sshisi here: https://www.cavestory.org/downloads/tone_pusher_rev1.zip
	Use pusher.exe to extract the sounds into a folder, then use the included batchpxttoraw.exe here to convert the pxts to .raw.
	You can then put the raws into the datacse2/WAVE folder.

	PxTone Collage is supported for music. You can play music from Kero Blaster and other things.
	Place them in the ORG folder like "CURLY.ptcop".
	Try not to use songs that are too large. Loading times aren't great unless you use sounds as WAV in the ptcop.

