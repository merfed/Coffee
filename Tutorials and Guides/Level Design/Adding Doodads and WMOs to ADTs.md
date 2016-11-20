# Adding Doodads and WMOs to ADTs

In order to do this, you should be able to modify land and moving doodads using NoggIt and have both, knowledge in NoggIt and Cryects command line tools. Provided you know those I think you at least know what this is about in general. You've built some great land and now want to add some trees and stuff. You have mastered NoggIt since at this point the abilities of NoggIt end in general.

You now need those mighty tools. There are two apps. "fileInfo" and "loadInfo". Those apps extract the model-dat out of a adt-tile.

**FileInfo** - Writes information from an ADT to a text file.

	fileInfo <adt file>

**LoadInfo** - Writes the information in the text file back int othe ADT.

	loadInfo <adt file>

You now know the basic usage of the two tools. But if you try to do it, you may be hit by a big wall of text and numbers. This is what this guide tries to explain. Lets see what we got, if you get the fileInfo of a blizzard-adt.

**Example:**

	BlackwingLair_33_46

	MMDX    28
	0     world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock04.mdx
	1     world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock03.mdx
	2     world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock01.mdx
	3     world\generic\passivedoodads\lights\generaltorch01.mdx
	4     world\azeroth\burningsteppes\passivedoodads\volcanicvents\volcanicventsmall01.md x
	5     world\azeroth\burningsteppes\passivedoodads\smoke\ashtreesmoke01.mdx
	6     world\azeroth\burningsteppes\passivedoodads\volcanicvents\volcanicventlarge01.md x
	7     world\azeroth\burningsteppes\passivedoodads\volcanicvents\volcanicventlargeoff01 .mdx
	8     world\khazmodan\blackrock\passivedoodads\blackrocklavabubbles\blackrockstatuelav abubbles.mdx
	9     world\azeroth\burningsteppes\passivedoodads\trees\ashtree03.mdx
	10    world\azeroth\burningsteppes\passivedoodads\trees\burningmidtree01.mdx
	11    world\azeroth\burningsteppes\passivedoodads\trees\burningmidtree03.mdx
	12    world\khazmodan\blackrock\passivedoodads\blackrocklavabubbles\blackrockstatuelav abubble.mdx
	13    world\azeroth\burningsteppes\passivedoodads\lavabridge\lavabridge.mdx
	14    world\azeroth\burningsteppes\passivedoodads\lavaplugs\lavaplug01.mdx
	15    world\azeroth\burningsteppes\passivedoodads\lavaplugs\lavaplug02.mdx
	16    world\generic\dwarf\passive doodads\dwarfcorpse\corpseskeletondwarf.mdx
	17    world\azeroth\burningsteppes\passivedoodads\rocks\burningsteppesboulders03.mdx
	18    world\kalimdor\desolace\passivedoodads\spearwalls\centaurspearscone.mdx
	19    world\khazmodan\wetlands\passivedoodads\dragonbones\dragonbonesskull.mdx
	20    world\generic\dwarf\passive doodads\excavationbannerstands\excavationbannerstand.mdx
	21    world\khazmodan\wetlands\passivedoodads\totem\dragontotem01.mdx
	22    world\generic\human\passive doodads\meat\bloodymeat01.mdx
	23    world\azeroth\westfall\passivedoodads\westfallskeleton\westfallskeleton.mdx
	24    world\azeroth\westfall\passivedoodads\deadmule\deadmule.mdx
	25    world\azeroth\elwynn\passivedoodads\battlegladeskullhuman1\battlegladeskullhuman 1.mdx
	26    world\azeroth\elwynn\passivedoodads\battlegladeskullhuman2\battlegladeskullhuman 2.mdx
	27    world\generic\passivedoodads\flyinginsects\flyingsmallinsects.mdx
	
	MWMO    2
	0    world\wmo\azeroth\buildings\altarofstorms\altarofstorms.wmo
	1    world\wmo\dungeon\az_blackrock\blackrock_upper_guild.wmo
	
	MDDF    30
	0     569921    17702.980469    259.725525    24549.898438    0.000000    98.000000     0.000000    1024
	1     569922    17880.085938    269.850677    24558.773438    36.500000   20.000000    -21.500000   665
	2     569920    17716.037109    258.058868    24545.775391    0.000000    -40.500000    0.000000    1024
	3     569953    17947.330078    128.760712    25022.666016    1.000000    0.000000      5.500000    1024
	4     569960    17986.671875    131.697800    25003.968750    0.000000    0.000000      0.000000    1024
	5     569923    17886.742188    266.424561    24560.740234    0.000000    0.000000      0.000000    1024
	6     569961    17988.152344    131.091492    25006.794922    0.000000    0.000000      0.000000    1024
	7     569959    17990.595703    130.874908    25003.787109    0.000000    0.000000      0.000000    1966
	8     569927    17920.445312    595.126221    24640.335938    0.000000    0.000000      0.000000    368
	8     569945    18084.054688    129.256210    24907.351562    0.000000    14.500000     0.000000    327
	8     570056    18135.326172    128.850876    24898.150391    0.000000    -53.500000    0.000000    327
	9     569947    17955.601562    131.585724    24988.974609    0.000000    135.718246    0.000000    1043
	10    569949    17950.736328    131.011093    24995.861328    0.000000    231.535889    0.000000    1655
	11    569948    17963.355469    132.419922    24989.703125    0.000000    146.694946    0.000000    1312
	12    569931    17901.535156    181.571655    24689.031250    0.000000    0.000000      0.000000    1228
	13    569934    17779.123047    181.158859    24718.705078    2.000000    88.000000     -1.000000   1024
	14    569936    17926.072266    589.903870    24702.291016    0.000000    -119.000000   0.000000    1024
	15    569926    17927.619141    588.403992    24637.429688    0.000000    -74.500000    0.000000    1024
	16    569967    17971.099609    131.586487    25020.185547    0.000000    0.000000      0.000000    1024
	17    569937    18083.193359    587.571045    24731.113281    0.000000    0.000000      0.000000    1024
	18    569950    17963.777344    130.321564    25029.906250    0.000000    0.500000      0.000000    1024
	19    569951    17941.664062    152.397308    25012.550781    81.500000   55.500000     27.000000   1024
	20    569952    17941.578125    128.743118    25012.386719    0.000000    45.000000     0.000000    6543
	21    569958    17970.056641    130.800034    25007.867188    0.000000    -137.000000   0.000000    1024
	22    569962    17973.800781    131.726105    25019.173828    0.000000    0.000000      0.000000    1771
	23    569963    17973.246094    131.774490    25021.515625    -0.500000   0.000000      1.500000    1024
	24    569964    17970.898438    131.500885    25017.908203    0.000000    -45.000000    0.000000    1024
	25    569965    17970.906250    131.537170    25019.041016    -0.500000   0.000000      4.000000    1382
	26    569966    17972.539062    131.647232    25018.988281    1.000000    -61.500000    2.000000    1873
	27    569968    17972.972656    131.678940    25018.994141    0.000000    -40.000000    0.000000    1024
	
	MODF    2
	0    569928    17822.890625    181.911072    24680.835938    0.000000    -75.000000   0.000000    17773.910156    173.622940    24616.408203    17907.185547    232.914902    24734.855469    0
	1    570872    17888.740234    279.808685    25159.396484    0.000000    35.000000    0.000000    17842.607422    354.110413    24300.787109    18479.392578    528.917969    24973.732422    0

So what we see first are those massive blocks. But we notice that:
	
	MMDX , 28
	[...]
	
	MWMO , 2
	[...]
	
	MDDF , 30
	[...]
	
	MODF , 2
	[...]

The 4 characters are the "chunk"-identifier. They all start with a M for Map. The other characters are:

* MDX for model-filenames
* WMO for wmo-filenames
* DDF for DooDad-reFerence. Just say: where the models are. I actually dont know, what the **** that should mean xD * DDF ...
* ODF for ObjecDreFerence? Just the WMO-placement.

The number after them is the number of entries in that chunk. So we can see 28 different doodads in there, 2 wmos, with 30 doodad references and 2 WMO instances. Thats the basic overview of that file. Lets analyse the blocks itself.

## The Chunks

### MMDX and MWMO

So, here we got: 

	0    world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock04.mdx
	1    world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock03.mdx
	2    world\generic\ogre\passive doodads\ogremoundrocks\ogremoundrock01.mdx
	3    world\generic\passivedoodads\lights\generaltorch01.mdx
	4    world\azeroth\burningsteppes\passivedoodads\volcanicvents\volcanicventsmall01.mdx
	5    world\azeroth\burningsteppes\passivedoodads\smoke\ashtreesmoke01.mdx
	6    world\azeroth\burningsteppes\passivedoodads\volcanicvents\volcanicventlarge01.mdx

	---

	0    world\wmo\azeroth\buildings\altarofstorms\altarofstorms.wmo
	1    world\wmo\dungeon\az_blackrock\blackrock_upper_guild.wmo

You see, both times a running number in front and then a filename. Thats it.  The first number has to be a number that has NOT been used before. I'm not sure if it has to be x = x+1 but you always should do that. So to add a new model in there (or in your own file), we add a new filename:

	MMDX    n+1
	[...]
	x    world\azeroth\westfall\passivedoodads\deadcow\deadcow.mdx

The important thing is that we increase the count of the block-entries too! For WMOs it'd be the same. Also be sure, that you always reference the files as .MDX!

### MDDF

	0    569921    17702.980469    259.725525    24549.898438    0.000000    98.000000    0.000000    1024
	1    569922    17880.085938    269.850677    24558.773438    36.500000   20.000000    -21.500000  665
	2    569920    17716.037109    258.058868    24545.775391    0.000000    -40.500000   0.000000    1024
	3    569953    17947.330078    128.760712    25022.666016    1.000000    0.000000     5.500000    1024
	4    569960    17986.671875    131.697800    25003.968750    0.000000    0.000000     0.000000    1024
	5    569923    17886.742188    266.424561    24560.740234    0.000000    0.000000     0.000000    1024
	6    569961    17988.152344    131.091492    25006.794922    0.000000    0.000000     0.000000    1024
	7    569959    17990.595703    130.874908    25003.787109    0.000000    0.000000     0.000000    1966
	8    569927    17920.445312    595.126221    24640.335938    0.000000    0.000000     0.000000    368

Lets start. The first number is the count from above again. This time, there may be several entries for the same model! So you can use one model more than one time! Just add another entry. The second one: A unique number for every entry in the maps! Just try high numbers there. >800000 shouldn't be used anywhere. It really has to be unique in at least that tile and the ones on the edges. So we get 3 floats. That is the position! In the order X, Y, Z where Y is up to the sky. (Or X, Z, Y for z-up systems!). This position is global. Which means not limited to that tile. This is the reason you can drag the doodads on the shore on your tile. They are defined absolute! You will have to get a position that is somewhere on your tile. Just do that by getting there with NoggIt and reading the coordinates on the bottom left side. Then the rotation. A, B, C. If you don't know, what this means, just do that in NoggIt afterwards. Size. 1024 is standard. You can do that again in NoggIt too.

Lets add our new model added above.

	x    world\azeroth\westfall\passivedoodads\deadcow\deadcow.mdx

Well ... Get a new one:

	x    800000+t    x    y    z    0.0, 0.0    0.0    1024

* x = x. Same as the used above.
* t = random number. Just get that field to some high value.
* x, y, z - your position.

Be sure you always use that unique ID and floats! That means those ".0"s. To make a specific example:

	15	879478	17107.515625	146.708588	16173.349609	0.000000	-73.000000	0.000000	1679

### MODF
This one is nice too. Nearly the same as above that MDDF one. 

	WMOid, 	uniqueID, x, 			  y, 			z, 				a, 			b, 			  c, 		  lowerext_x, 	  lowerext_y, 	lowerext_z, 	upperext_x, 	upperext_y,   upperext_z, 	  size
	0    	569928    17822.890625    181.911072    24680.835938    0.000000    -75.000000    0.000000    17773.910156    173.622940    24616.408203    17907.185547    232.914902    24734.855469    0
	1    	570872    17888.740234    279.808685    25159.396484    0.000000    35.000000     0.000000    17842.607422    354.110413    24300.787109    18479.392578    528.917969    24973.732422    0

The size is not the size. Its the used doodad-set. You ever wondered, where all those things in the houses come from in the inns etc? They aren't placed individually each time. That are those doodad-sets. They define a set of doodads that is shown in that WMO. A WMO can have more than 1 doodad-set. That just depends on the used WMO. 
To have a look on them, just open the WMO in the model viewer. You can choose the set there. The standard one is 0.

So the *ext_* is used for WoW to have some reference, where that WMO is since its not just a single point. The problem there is: You don't know those values. So you just fill in some number that nearly matches the x, y and z but wont be the same. Just do it +-1.0 .. That works most of the time.

**Example:**

	3	681933	17597.925781	135.435043	16067.248047	0.000000	286.292725	0.000000	17548.787109	130.644745	16018.109375	17647.064453	182.614853	16116.388672	0

*Originally written by Schlumpf*