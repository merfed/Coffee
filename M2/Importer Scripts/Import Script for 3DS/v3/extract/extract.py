from wow_data import wow_data
import os
import os.path
import sys
from location import Locator

def wow2fs(wowname):
    return wowname.replace('\\', os.path.sep)

def extract_wowfile(wowname, filename, dir=''):
    if not os.path.isdir(dir):
        os.makedirs(dir)
    f = file(os.path.join(dir, filename), 'wb')
    f.write(wow_data.read(wowname))
    f.close()

def extract_fullpath(wowname, dir=''):
    wowdir, filename = os.path.split(wow2fs(wowname))
    extract_wowfile(wowname, filename, os.path.join(dir, wowdir))

def extract_nameonly(wowname, dir=''):
    wowdir, filename = os.path.split(wow2fs(wowname))
    extract_wowfile(wowname, filename, dir)

def extract(wowname, dir='', fullpath=False):
    if fullpath:
        extract_fullpath(wowname, dir)
    else:
        extract_nameonly(wowname, dir)


if __name__=='__main__':
    #import random
    #fn = random.choice(wow_data.namelist())
    #extract(fn, fullpath=True)
	first=True
	for fn in sys.argv:
		if first:
			first=False
		else:
			extract(fn, "Data", True)
  
