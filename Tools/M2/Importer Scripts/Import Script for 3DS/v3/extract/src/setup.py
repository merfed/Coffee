from distutils.core import setup
from distutils.extension import Extension
from Pyrex.Distutils import build_ext

from sys import argv
argv += ['build_ext', '--inplace']

from platform import system
if system()=='Windows':
	mpq_lib = ['mpq']
	mpq_obj = []
	link_args = ['/NODEFAULTLIB:LIBC'] #wtf?
else:
	mpq_lib = [] #['stdc++']
	mpq_obj = ['libmpq.a']
	link_args = []

setup(
	name = 'wow',
	ext_modules=[ 
		Extension('mpq', ['mpq.pyx'], libraries=mpq_lib, extra_objects=mpq_obj, extra_link_args=link_args),
		Extension('vec', ['vec.pyx'], libraries=[]),
	],
	cmdclass = {'build_ext': build_ext}
)

