# Converting WMO v14 to WMO v17 #

WMO version 14 pertains to the WMO files in the alpha client (2003), version 17 includes Vanilla/Burning Crusade/Wrath/Cataclysm.

## Rootfile ##
The rootfile is what coordinates all the parts of the model. It contains a list of textures or doodads (such as paintings, tables, etc), flags, how to render groups of vertices and more. The rootfile is the file without numbers at the end: *test.wmo*.

### MVER ###
The version changed of course: ```0x0E``` to ```0x11``` (14 to 17).

### MOHD ###
Data within this chunk is in the same order, bad thing is that the bounding box corners aren't there. In every WMO I've opened there were only ```0x00``` instead of the 3*2 floats.

You can actually calculate a nice bounding box by taking the minimum and maximum values in the MOGI chunk. Bounding box is, as the name suggests, a box. So it can be defined by only two points: the corners (top-left-background and bottom-right-foreground for example). These two points are floats, so we need two points then six floats. This bounding box will need to fit with the whole WMO, so we are going to look in the MOGI chunk because it stores bounding box corners for every part of the WMO. Taking the maximum and the minimum values should create, logically, a bounding box big enough to contain the whole WMO.
&mdash; *Relaxok*

### MOMT ###
The only thing that has changed in this chunk is that in version 17 there are 20 bytes at the end of each entry. I don't understand well what it is, but the wiki says that it's nulled when the game loads them. Pretty sure you can just put ```0x00``` there and it should be fine.

### MOGI ###
* v14 Entries: 40 bytes
* v17 Entries: 32 bytes

Quite simple, in version 14 each entry has in the the first position the absolute uint32 offset of the groupfile, and just after its size, uint32 too. Just delete these 8 bytes for each MOGI entry.

### MOSB ###
Skyboxes don't exist in version 14, so just create an empty one. Be careful, you need yo add an empty int32, it's not an empty chunk, it will look like mag/size/empty_int or ```"BSOM", 4, 0x00, 0x00, 0x00, 0x00```.

### MOVV / MOVB ###
Doesn't exist in version 14, and I actually never saw a single one not empty. So just add empty ones (really empty this time, size = 0 and nothing in the data).

### MOLT ###
There is some new information, but I'm not sure about what it is:
<pre>
	/* v17 MOLT */
0   flags                // in v14
4   color                // in v14
8   position 1           // in v14
12  position 2           // in v14
16  position 3           // in v14
20  intensity            // in v14
24  attenuation start    // new
28  attenuation end      // new
32  unknown float 1      // new
36  unknown float 2      // new
40  unknown float 3      // in v14
44  unknown float 4      // in v14
</pre>

The two last unknown floats seem indeed to be at the end of version 14, but this may not be true for all WMOs. So how do we fill in the four missing values for each entry? Use values that seem good. 0 for both attenuation values, or whatever you prefer. For the two missing floats I set -1 and -0.5, mostly because most version 17 WMO's have -1 and -0.5 for these values.

### MODS / MODN / MODD ###
I can't say anything about these, I didn't look too closely at them. They seem the same in version 17.

## Groupfile ##
These are the files that keep all the "raw" data such as vertice positions, normals, triangles, etc. They're named with a number (ex: test_000.wmo, test_001.wmo). Always three digits, and never more then 512 (hard coded limit).

### MOGP ###
This chunk contains all the chunks for one groupfile so take note of the given size. This header is the most important, and I'm not too sure how to read it. He're the first part where I think it's the same in version 14 and version 17.

<pre>
	/* v14 and v17 MOGP header - First part */
0   groupName
4   groupDescription       // this and above are indices
8   flags
12  bb 0.0                 // bounding box corners i think, as in MOGI
16  bb 0.1
20  bb 0.2
24  bb 1.0
28  bb 1.1
32  bb 1.2
</pre>

In version 14 the second part contains 23*4 bytes, the last 8 are padding. The uint32 before is an ID in a WMOAreaTable.dbc. The first 20*4 bytes are a mystery.

<pre>
	/* v17 MOGP header - Second part - from the wiki page */
0x24 	uint16 		Index into the MOPR chunk
0x26 	uint16 		Number of items used from the MOPR chunk
0x28 	uint16 		Number of batches A
0x2A 	uint16 		Number of batches B
0x2C 	uint32 		Number of batches C
0x30 	uint8[4] 	Up to four indices into the WMO fog list
0x34 	uint32 		LiquidType related, see below in the MLIQ chunk.
0x38 	uint32 		WMO group ID (column 4 in WMOAreaTable.dbc)
0x3C 	uint32 		Always 0?
0x40 	uint32 		Always 0?
</pre>

Let's reorganize this, in the comments, a "line" is a group of 4 bytes in the 23*4 bytes, it's a bit easier to read.

<pre>
	/* v14 MOGP header - Second part - how i see it */
0   MOPR related             // looks like v14 2nd line
4   Batch related            // 2 uint16
8   Batch related            // 1 uint32 -- explanations below
12  Fog related              // looks like v14 3rd line
16  LiquidType related       // looks like v14 4th line
20  WMO group ID             // looks like v14 21th line
24  padding
28  padding
</pre>

As said in the wiki, batches AB and C (A, B for the two shorts and C for the int) can be read as a sum; the number of batches (in the MOBA chunk below). Problem is that we don't know how the developers divides the total number to put them in these three slots. I found that in some places in the version 14 dark data there is two values and their sum equals the number of MOBA entries. But, these values aren't always in the same place.

As promised, examples: Keep in mind that parts could have changed between alpha and wrath, so we must be careful when comparing these files:

<pre>
	/* v14 23*4 bytes - Undercity, MOGP @ 0x17AB649 */
00 00 18 00   // ?
70 01 08 00   // can be the MOPR data : an index and a small uint16
00 00 00 00   // (see next example)
0F 00 00 00   // 0f is the value often used for LiqType
00 00 FF 3F   // ?
00 00 79 00   // ?
FF 3F C6 00   // ?
79 00 10 00   // x79 + x10 = x89, number of batches... see what i mean ? ;)
00 00 00 00   // can be the "batch C" field
89 00 00 00   // there, number of batches and 0x00... repeated until the end
00 00 00 00
89 00 00 00
00 00 00 00
89 00 00 00
00 00 00 00
89 00 00 00
00 00 00 00
89 00 00 00
00 00 00 00
89 00 00 00
95 0C 00 00   // fits with the idea of an index in the DBC.
00 00 00 00   // padding
00 00 00 00
</pre>

Another alpha group file with the fitting version 17. Some information, such as the bounding box seem to be the same.

<pre>
	/* v14 23*4 bytes - Undercity, MOGP @ random place */
00 00 07 00   // ?
19 00 07 00
02 04 03 00   // looks the v17 below : seems nice, no ?
0F 00 00 00
00 00 00 00   // why not
00 00 00 00
00 00 00 00
00 00 00 00   // ...
00 00 00 00
00 00 00 00
00 00 00 00   // wtf, why so much 0x00
00 00 00 00
00 00 FF 3F
FF 3F FA 26   // omg what's this... seems to be MOIN nEntries o_O
53 00 4C 00   // here is the values to sum (x56 + x4C = x9F, number of MOBA entries)
00 00 00 00
9F 00 00 00
00 00 00 00
9F 00 00 00
FE 0B 00 00   // ok
00 00 00 00   // padding
00 00 00 00
	
	/* v17 8*4 bytes - always UC, fitting groupfile */
17 00 07 00   // first short is a little different, but why the fuck not ?
00 00 00 00
18 00 00 00   // all in batch C */me cries*
02 03 04 00   // Fog related, just 2nd and 3rd inverted, nice :D
0F 00 00 00
FE 0B 00 00   // same number, nice
00 00 00 00
00 00 00 00
</pre>

See the values to sum aren't in the same place.