#this script compresses everything using lzss in the Embedded files version.

path = "game_english"
path2 = "game_english_bmp"

import os
import distutils.dir_util

#Get all of the files from game_english_bmp
distutils.dir_util.copy_tree(path2, path)

filesEn = []

for root,dir,files in os.walk(path):
	for file in sorted(files):
			f = './' + root + "/" + file
			print(f)
			filesEn.append(f)
os.system("./lzss -ewf " + " ".join(filesEn))

#Now, rename all files that don't end with lzss
basepath = './game_english/datacse2/'
addDirs = [name for name in os.listdir(basepath) if os.path.isdir(basepath + name)]
addDirs.append("")
print(addDirs)
for d in addDirs:
	dirmod = basepath + d + "/"
	for f in os.listdir(dirmod):
		if not f.endswith('.lzss') and os.path.isfile(dirmod + f):
			os.rename(dirmod + f, dirmod + f + ".lzss")
