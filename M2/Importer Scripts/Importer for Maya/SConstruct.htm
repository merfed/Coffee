import os

# Create environment
env = Environment()

# Change this to point where your Maya is installed

MayaPath = 'C:\Program Files\Alias\Maya8.0'
#MayaPath = '/usr/aw/maya'

# Compiler parameters
env.Append(CCFLAGS = '-D_WIN32 -D_BOOL /EHsc -DNT_PLUGIN -D_CRT_SECURE_NO_DEPRECATE')
env.Append(CPPPATH = MayaPath + '/include')
env.Append(LINKFLAGS = '/export:initializePlugin /export:uninitializePlugin')

# Linker parameters
env.Append(LIBPATH = MayaPath + '/lib')
env['LIBS'] = Split('OpenMaya Foundation')

# Targets
env.SharedLibrary(target = 'm2import', source = Split('Plugin.cpp Import.cpp'))

env.InstallAs(target = MayaPath + '/bin/plug-ins/m2import.mll', source = 'm2import.dll')
env.Install(dir = MayaPath + '/scripts/others', source = 'm2FileTranslatorOpts.mel')

env.Alias('install', [MayaPath + '/bin/plug-ins', MayaPath + '/scripts/others'])
