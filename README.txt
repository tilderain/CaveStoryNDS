Cave Story DS homebrew release v0.4 by rain - play Cave Story on your DS after 15 years

Controls:
	Press L+R+Start to access the escape menu.

	D-Pad: Move
	A: Jump
	B: Shoot
	X: Next Weapon
	Y: Previous Weapon
	Start: Inventory
	Select: Map
	R: Fast Forward during cutscenes

Additional notes:
	On 3DS or DSi you will need the DSi version to run. (if you're on a 3DS you can also play Clownacy's 3DS port)
	If it fails to run, you can try launching it with nds-hb-menu 0.5.0 or TWiLight Menu.

	Config.dat and Profile.dat will be saved to the root of the SD card.
	Profile.dat is compatible with freeware Cave Story.

	You can hold Start during boot to start with display on the bottom screen.
	You can hold Select during boot to see console output.
	You can press L+R+Select to toggle console output.

	Emulators can use temporary save points if there is no SD card connected.
	You can also eject the card and play normally on the non SD card version.

Known issues:
	The game will rarely crash when closing the lid.
	The game will slow down in the Plantation and Sand Zone when hitting a missile or using the Spur.
	Performance will lower in multiplayer when using the Spur or missiles.
	There may be a loud noise during stage transition.

SD card version information:

	as an alternative version to run, or mainly to mod.
	As of now there is only support for English mods.

	Grab a copy of Booster's Lab 0.5.0.0 and export a stage.tbl.

	For TSC, you can use the <MIM and <PHY commands.

	You can read palettized bmps, compatible with freeware Cave Story. 
	NpcGuest is in the 2nd 256 color slot.
	the 0 index color is transparent.

	To read sounds, place the corresponding sound as decimal
	It will attempt to read the following formats in order: 
		8-bit 22050hz unsigned WAV audio (as ".wav")
		Raw 8-bit 22050hz signed audio (as ".raw")
		ADPCM sound using Nintendo's waveconv.exe, from the NDS SDK (as ".swav")

Thanks to:
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

	Drenn1, for the WiFi code from GameYob, and the forum posts on the devKitPro forums that probably let me avoid
		a dozen hours of debugging the sound.
	huiminghao, for the WiFi code from NesDS.
	Arisutora, for MelonDS and the NiFi debugging support.

	Ravenworks, who took on the Cave Story DS port in the first place, got destroyed by a publisher in the process, and inspired me 
		to create the DS version that the people deserve.

	And Pixel, who created this wonderful thing that inspired a generation of gamers.
	Thank you Pixel!

Changelog:

v0.4:
	This version adds the option of the SD card ROM, which supports basic mods.
	See above for instructions on how to use it.

	- Added a setting to use an alternate set of percussion for songs.
	- You can now fast forward during cutscenes and events by holding down the R button.
	- Added a few songs to the sound test.
	- 50fps mode is now smoother. (Switched to a timer system instead of adding an additional wait)
	- Disabled file opening after ejecting card, so saves and config menu won't just crash.
	- You can toggle the console output with L+R+Select.
	- Holding down the Start button during boot will set display on the bottom screen.
	- Updated credits in the readme.
	- Add version to game subtitle and config menu.
	- Fixed missing tiles in Egg Corridor?.
	- Fixed arms graphic being corrupted after credits.
	- Fixed credits graphics corruption after viewing it twice.
	- Improved look of credit illustrations.
	- Fixed face portrait appearing for a bit before sliding.
	- Reduced flicker and save a few frames when uploading texture (use DMA).
	- Upload face portrait and Japanese text to texture only during vblank, reducing flicker.
	- Fix ending cloud parallax.
	- Multi: Fixed NPCs targeting dead players.
	- Multi: Optimized performance a bit.
	- Multi: reduced flicker.
	- Multi. Changed to 2 frame delay for both host and client.
	- JPN: Add some missing characters to the font. (whoops)
	- JPN: Fixed credit text being cut off.
	- JPN: Fixed some missing credit portraits.
v0.3:
	Local multiplayer is here! Explore the caves with a friend over wireless.
	Thanks to Drenn1 and huiminghao's wonderful Nifi code from GameYob and NesDS respectively.
	If you experience lag, try selecting another channel.
	Note that the client will not write the profile to the SD card during netplay
	(so you can take out the flashcart after loading on one console, and put it in the other to play with only one flashcart for both DSes).
	Take care not to fire the Spur in a big area.

	-Moved input processing to beginning of frame, saving 1 frame of input lag.
	-Fixed bosses damage frames not appearing half the time.
	-Fix camera position in H**l ending event.
	-Tile 0 is no longer rendered.
	-Added some "fun" ROMs to play.
	v0.3a:
		Fix shooting while dead in netplay.
v0.2:
	-Japanese language support!
	-Reduced filesize, so it can run on more flashcarts. (ADPCM sound with waveconv.exe)
	-Minimap works now.
	-60/50 FPS toggle.
	-Transparent textbox toggle.
	-Resize inventory screen.
	-Draw stage on inventory, teleporter, and map screen.
	-Emulators can save temporarily.
	-Fix L2/L3 missile sprite.
	-Fix camera position in Balcony heli event, and Passage.
	-Added event jump to cheat menu.
	-Text will appear when an error occurs.
v0.1:
	Initial release.
