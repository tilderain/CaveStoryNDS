Cave Story DS homebrew release v0.3 by rain - play Cave Story on your DS after 15 years

Controls:
	Press L+R+Start to access the escape menu.

	D-Pad: Move
	A: Jump
	B: Shoot
	X: Previous Weapon
	Y: Next Weapon
	Start: Inventory
	Select: Map

Additional notes
	On 3DS or DSi you will need the DSi version to run. (though if you're on a 3DS you probably want to play Clownacy's 3DS port)
	If it fails to run, you can try launching it with nds-hb-menu 0.5.0 or TWiLight Menu.

	Config.dat and Profile.dat will be saved to the root of the SD card.
	You can press Select during boot to see console output.

Known issues:
	The game will rarely crash when closing the lid.
	The game will slow down when hitting a missile or using the Spur in the Plantation and Sand Zone.
	There may be a loud noise during stage transition.

Thanks to:
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

Changelog:
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
