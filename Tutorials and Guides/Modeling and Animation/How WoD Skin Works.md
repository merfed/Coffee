# How Warlords of Draenor .skin Works
They used a trick to go over the 65536 limit of the StartTriangle field.
Now, there is also a Level field. It is set to 0, then, when StartTriangle resets from 65000+ to 0+, Level is set to 1.

<pre>
1. submesh:
- ID: Mesh part ID
- Level: 0
- startTriangle: (startTriangle + nTriangles) of previous Submesh.
- nTriangles: number of triangles.

2. if ((startTriangle + nTriangles) of previous Submesh) > 65535:
- ID: Mesh part ID
- Level: 1
- startTriangle: ((startTriangle + nTriangles) of previous Submesh) - 65536. (resets)
</pre>

<pre>
ex:
1.
- ID: 13
- Level: 0
- startTriangle: 64272
- nTriangles: 1356
= 64272 + 1356 = 65628

2.
- ID: 14
- Level: 1
- startTriangle: 92 (65628 - 65536) (resets)
- nTriangles: 1083
</pre>

## Credits
Vesageough