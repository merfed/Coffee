/*
 * Header for .skin files.
 *
 * Version: 0.1    
 * WoW Build: 8820 
 *
 * Date: 22.08.2008
 *
 * You may not need the three structures in the middle, 
 * but if you want to get your vertex like this:
 * vertices[indices.Vertex[triangles[i].Indices[0]]
 * its pretty nice for readability in your code.
 *
 */

typedef unsigned short uint16;
typedef unsigned int uint32;

struct SkinHeader {
  uint32 Padding;		// Just some Padding.
  uint32 nIndices;		
  uint32 ofsIndices;		// Indices used in this View.
  uint32 nTriangles;		
  uint32 ofsTriangles;		// The triangles made with them.
  uint32 nProperties;		
  uint32 ofsProperties;		// Properties of the vertices.
  uint32 nSubmeshes;		
  uint32 ofsSubmeshes;		// Submeshes (Geosets) of this View.
  uint32 nTextureUnits;	
  uint32 ofsTextureUnits;	// Texture Units.
  uint32 LOD;			// LOD distance or something else
};

struct Index {
   uint16 Vertex;		// The vertex in the global vertex list.
};

struct Triangle {
   uint16 Indices[3];		// Three indices which make up a triangle.
};

struct VertexProperty {
   uint32 Properties;		// Usually 0? Purpose unknown.
};

struct Submesh {
   uint32 ID;			// Mesh part ID, see below.
   uint16 StartVertex;		// Starting vertex number.
   uint16 nVertices;		// Number of vertices.
   uint16 StartTriangle;		// Starting triangle index (that's 3* the number of triangles drawn so far).
   uint16 nTriangles;		// Number of triangle indices.
   uint16 nBones;		// Number of elements in the bone lookup table.
   uint16 StartBones;		// Starting index in the bone lookup table.
   uint16 Unknown;		
   uint16 RootBone;		// Not sure.
   float Position[3];		// Some Vector. Not sure about it being the Position.
   float Floats[4];		// New Floats since BC.
};
