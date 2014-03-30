from adt.adt import *
from wmo.wmo import *

OffsetX = 533.333
OffsetY = 533.333


def convertMMDXtoMODN(mmdx):
	modn = FilenameChunk(1297040462)
	for i in mmdx.filenames:
		modn.filenames.append(i)
	return modn
	

def convertMDDFtoMODD(mddf):
	modd = MODDEntry()
	modd.nameIndex = adt.mmid.entries[mddf.index].Id
	modd.pos.x = (mddf.position.x - OffsetX ) 
	modd.pos.y = (mddf.position.z - OffsetY ) 
	modd.pos.z = mddf.position.y
	#modd.ori = mddf.rotation #todo: vec3 to quaternion
	modd.ori.x = 1
	modd.scale = mddf.scale / 1024.0
	return modd


adt = ADTFile()
root = WMOFile(True)
group = WMOFile(False)
adtname = "Azeroth_32_49.adt"
adt.read(adtname)
s = adtname.split("_")
t = []
for i in s:
	t.extend(i.split("."))
c = 0
for i in t:
	if(i.isdigit()):
		if(c == 0):
			OffsetX *= int(i)
			c += 1
		else:
			OffsetY *= int(i)
			c += 1
	if (c == 2):
		break
if (c != 2):
	print "Please insert IndexX:\n"
root.read("Root.wmo")
group.read("Root_000.wmo")


root.modn = convertMMDXtoMODN(adt.mmdx)
root.modd = EntryChunk(1297040452, MODDEntry)
for i in adt.mddf.entries:
	root.modd.addEntry(convertMDDFtoMODD(i))
	
root.mods.entries[0].nDoodads = root.modd.nEntries

group.mogp.modr = EntryChunk(1297040466, Lookup)
for i in range(root.modd.nEntries):
	t = Lookup()
	t.Id = i
	group.mogp.modr.addEntry(t)
	
group.mogp.flags |= WMO_DOODADS




root.write("D:\makempq\World\wmo\Kalimdor\Buildings\GoblinHut\KL_Tanaris_GoblinBld_C.wmo")
group.write("D:\makempq\World\wmo\Kalimdor\Buildings\GoblinHut\KL_Tanaris_GoblinBld_C_000.wmo")


