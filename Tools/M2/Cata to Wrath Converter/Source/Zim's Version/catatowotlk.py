#! /usr/bin/python
from m2 import *
from skin import *
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OKGREEN = ''
        self.WARNING = ''
        self.FAIL = ''
        self.ENDC = ''

print "ONLY FOR PERSONAL USAGE"
#get the filename and open the files
print "Please insert Filename:"
filename = raw_input()
sname = filename[0:len(filename) -3] + "00.skin"
m2 = M2File(filename)
skin = SkinFile(sname)
#add texunit
tu = Lookup()
tu.Id = 1
m2.tex_units.append(tu)
texunit = m2.hdr.tex_units.count#save for later use
m2.hdr.tex_units.count += 1
m2.hdr.version -= 1
#######
#Note:
#Translation = Vec3()
#Rotation = Quat() (and wow uses Short-Quaternions)
#Scaling = Vec3()
#######
layer = Material()
layer.texunit = texunit
layer.texunit2 = texunit
m2.write(filename)
skin.write(sname)

raw_input ("Your model converted. Thank you for using my script. Zim")
