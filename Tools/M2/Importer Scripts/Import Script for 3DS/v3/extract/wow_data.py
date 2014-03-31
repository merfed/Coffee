
#import resources

from location import *
#import os
import os.path

archive_dir = ''

from platform import system
if system()=='Windows':
	import _winreg
	k = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,'SOFTWARE\\Blizzard Entertainment\\World of Warcraft')
	archive_dir, t = _winreg.QueryValueEx(k,'InstallPath')
	_winreg.CloseKey(k)
	archive_dir += 'Data'
else:
	archive_dir = 'Data'


# from glob import glob
# archive_names = glob(archive_dir+'/*.MPQ')

archive_names = [
	'patch.MPQ',
	'terrain.MPQ',
	'texture.MPQ',
	'wmo.MPQ',
	'model.MPQ',
	'interface.MPQ',
	'dbc.MPQ',
	'misc.MPQ',
	'base.MPQ',
	'speech.MPQ',
	'sound.MPQ',
	'fonts.MPQ',
]

archives = {}
#archive_priority_order = []

wow_data = Locator()
wow_data.openlocation(archive_dir)
for n in archive_names:
	try:
		archives[n] = wow_data.openlocation(os.path.join(archive_dir, n))
	except IOError:
		print 'cannot open wow archive:',n

if __name__=='__main__':
	print len(wow_data.namelist())

