#this script generates the list of files in fopen.cpp

path = "game_english"
path2 = "game_japanese"

import os

rootsEn = []
filesEn = []

rootsJp = []
filesJp = []

for root,dir,files in os.walk(path2):
	for file in sorted(files):
		filesJp.append(file)
		rootsJp.append(root)

for root,dir,files in os.walk(path):
	for file in sorted(files):
		if file not in filesJp:
			if ".raw" not in file and ".wav" not in file and "waveconv" not in file:
				print('INCBIN(' + file.replace(".", "") + ', "../' + root + "/" + file + '");')
		else:
			filesEn.append(file)
			rootsEn.append(root)

print("\n#ifndef JAPANESE\n")
for i in range(len(rootsEn)):
	print('INCBIN(' + filesEn[i].replace(".", "") + ', "../' + rootsEn[i] + "/" + filesEn[i] + '");')
print("\n#else")
for i in range(len(rootsJp)):
	print('INCBIN(' + filesJp[i].replace(".", "") + ', "../' + rootsJp[i] + "/" + filesJp[i] + '");')
print("\n#endif")



print("\nconst FILEREF files[] = {")

for root,dir,files in os.walk(path):
	for file in sorted(files):
			if ".raw" not in file and ".wav" not in file and "waveconv" not in file:
				name = file.replace(".", "")
				name = "g" + name
				print("\t{" + name + "Data, " + name + 'Size, "' + root.replace("game_english/", "") + "/" + file + '"},')

print("};")
