path = "game_english"

import os

for root,dir,files in os.walk(path):
	for file in sorted(files):
		print('INCBIN(' + file.replace(".", "") + ', "../' + root + "/" + file + '");')

print("\nconst FILEREF files[] = {")

for root,dir,files in os.walk(path):
	for file in sorted(files):
		name = file.replace(".", "")
		name = "g" + name
		print("\t{" + name + "Data, " + name + 'Size, "' + root.replace("game_english/", "") + "/" + file + '"},')

print("};")