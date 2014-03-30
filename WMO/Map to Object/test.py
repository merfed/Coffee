from wmo.wmo import *


root = WMOFile(True)
root.read("Root2.wmo")
for i in root.modn.filenames:
	print i