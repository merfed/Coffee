Complete read-me on how to convert a m2 into 3ds max, export it out,
or just make a new m2 all together! -By Krazzee

	All right, go into MyWarcraftStudio and find an m2 you want to edit with and 
extract it to a folder. Ok, now open 3DSMAX, and look at the
top of your screen, see that toolbar? Well, click on MAXScript and then Open Script. Now,
find your m2_import.ms that came with this file and select it and click open. All right,
now look for a part of it that looks like this:

-- GLOBALS AND STRUCTURES
--

-- this is where this script, and the extract directory should be located  (overwritten by UI)
global basePath = "c:\\3dsmax6\\scripts"

-- model filename (overwritten by UI)
global filename = "World\\Generic\\NightElf\\Passive doodads\\Statues\\StatueNEPriestess.m2"

now change where the global filename equals the NE Priestess Statue, change that to 
whatever you want, but make sure that if your model is in C:\Program Files\World of Warcraft\Data\Creatures\Rabbit\Rabbit.m2
that you change it to "C:\\Program Files\\World of Warcraft\\Data\\Creatures\\Rabbit\\Rabbit.m2" (Use the double \\'s and the ""'s !!!!)

Click on file, and click on Evaluate all, click on it only once! Now click Open... where it
says Model filename or path in mpq and browse for your m2. At the bottom you may notice
that it says "Replaceable textures (path in MPQ or blank)" and also "Creature Skin 1-3"
I have tried to get it to work with blps but it doesn't seem to load them. If you can, so
much the better. Now click import. You can't see anything at the moment because it is too
small, so look to your bottom-right and click on the icon with the square in front of the
grid, "Zoom Extents All Selected", it will zoom in all the windows to your m2. Well now
you can edit it.

When you are done Editing it, click on File, on the main toolbar, then go down and click
on Export... and export it as a 3DS file. Now comes the fun part, making your 3ds file
into a m2!

This is where the proggy 3dsToM2.exe comes in handy. You go to start button, then click on
run, then type in CMD, a dos window should come up. here is what you type (Note: do not 
hit enter until I say so, SPACE means hit the space bar):

Location of 3dsToM2.exe
SPACE
Location of the ****.3ds
SPACE
Location where you want the program to create the .m2
SPACE
The scale of the object, as in 1 is small, 100 is huge
Enter

And Voila, there it is, the brand new .m2 all shiny and squeaky clean. Shove it in a mpq
and Enjoy.

	-Krazzee
