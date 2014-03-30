#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

from BLPImagePlugin import BLPImageFile as BLP
from FTEXImagePlugin import FTEXImageFile as FTEX
from DDSImagePlugin import DDSImageFile as DDS

from hashlib import md5

usage = "Usage: %s image.{blp,ftc,...}" % (sys.argv[0])

def main():
	try:
		args = sys.argv[1:]
	except IndexError:
		print usage
		exit()
	for f in args:
		flower = f.lower()
		if flower.endswith(".blp"):
			print md5
			BLP(f).save("out_%s.png" % md5(f).hexdigest())
		elif flower.endswith(".ftc") or flower.endswith(".ftu"):
			FTEX(f).save("out_%s.png" % md5(f).hexdigest())
		elif flower.endswith(".ffs"):
			DDS(f).save("out_%s.png" % md5(f).hexdigest())
		else:
			print "Unknown file format for %s..." % (f)
			print usage
	exit()

if __name__ == "__main__":
	main()
