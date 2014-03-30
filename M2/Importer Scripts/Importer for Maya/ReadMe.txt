Compiling / installing:
-----------------------

You need:
 * Scons
 * Maya 					(Tested with 8.0)
 * Visual Studio	(Tested with 2005)

To use this plugin you need to compile and install it. Under windows you it 
should be enough to just change the 'MayaPath' variable to in the file 
'SConstruct' to point to your maya installation directory. If you are compiling 
on other platforms, you need to also change the compiler options.

Running 'scons install' should compile and copy the plugin and its MEL script
to correct directories. Then you can load the plugin from plugin manager in 
maya.

Usage Notes:
------------

If you want textures to appear correctly, you need to flip them vertically (local 
axis).

===============================================================================

Thanks to the following people for M2 file format code:
	UfoZ & Darjk 	(WoW Model Viewer)

===============================================================================

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

===============================================================================
	
World of Warcraft® and World of Warcraft:Burning Crusade® 
are registered trademark of Blizzard Entertainment®. 

All game material and content is ©2006 Blizzard Entertainment®. 
All rights reserved.