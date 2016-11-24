/* Author : Mjollnà, 2012-08-12 */

/* Help/documentation : 

http://www.pxr.dk/wowdev/wiki/
http://modcraft.superparanoid.de/viewtopic.php?f=59&t=828 (010 templates)
http://modcraft.superparanoid.de/viewtopic.php?f=20&t=937 (Tutorial for M2 Cata -> LK)

Thanks to Schlumpf, Zim4ik and Morfium :)

 */

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

struct Vec3F
{
  float x;
  float y;
  float z;
};

struct cataHeader
{
  char magic[4];
  int version;
  int lName;
  int ofsName;
  int GlobalModelFlags;
  int nGlobalSequences;
  int ofsGlobalSequences;
  int nAnimations;
  int ofsAnimations;
  int nAnimationLookup;
  int ofsAnimationLookup;
  int nBones;
  int ofsBones;
  int nKeyBoneLookup;
  int ofsKeyBoneLookup;
  int nVertices;
  int ofsVertices;
  int nViews;
  int nColors;
  int ofsColors;
  int nTextures;
  int ofsTextures;
  int nTransparency;
  int ofsTransparency;
  int nUVAnimation;
  int ofsUVAnimation;
  int nTexReplace;
  int ofsTexReplace;
  int nRenderFlags;
  int ofsRenderFlags;
  int nBoneLookupTable;
  int ofsBoneLookupTable;
  int nTexLookup;
  int ofsTexLookup;
  int nTexUnits;
  int ofsTexUnits;
  int nTransLookup;
  int ofsTransLookup;
  int nUVAnimLookup;
  int ofsUVAnimLookup;	
  Vec3F VertexBox[2];
  float VertexRadius;	
  Vec3F BoundingBox[2];	
  float BoundingRadius;	
  int nBoundingTriangles;
  int ofsBoundingTriangles;
  int nBoundingVertices;
  int ofsBoundingVertices;
  int nBoundingNormals;
  int ofsBoundingNormals;
  int nAttachments;
  int ofsAttachments;
  int nAttachLookup;
  int ofsAttachLookup;
  int nEvents;
  int ofsEvents;
  int nLights;
  int ofsLights;
  int nCameras;
  int ofsCameras;
  int nCameraLookup;
  int ofsCameraLookup;
  int nRibbonEmitters;
  int ofsRibbonEmitters;
  int nParticleEmitters;
  int ofsParticleEmitters;
};

struct lkCamera // Total size = 0x64
{
  int Type;
  float FOV;  
  char otherStuff[0x5C]; // 0x64 - (Type + FOV)
};

struct cataCamera // Total size = 0x74
{
  int Type;
  char otherStuff[0x5C]; 
  char AnimBlock4[0x14]; // Stuff we're gonna get rid of
};

struct skinHeader
{
  int magic;
  int nIndices;		
  int ofsIndices;
  int nTriangles;		
  int ofsTriangles;
  int nProperties;		
  int ofsProperties;
  int nSubmeshes;		
  int ofsSubmeshes;
  int nTextureUnits;	
  int ofsTextureUnits;
  int LOD;
};

struct submesh
{
  int ID;		
  short StartVertex;		
  short nVertices;	
  short StartTriangle;	
  short nTriangles;	
  short nBones;	
  short StartBones;
  short Unknown;		
  short RootBone;	
  float Position[3];
  float Floats[4];
};

struct textureUnit
{
  char flags;
  char flags2;
  unsigned short order;
  short submesh;
  short submesh2;
  short colorIndex;
  short renderFlags;
  short texunit;
  short d4;
  short textureid;
  short texunit2;
  short transid;
  short texanimid;
};

void fixM2File(char * filename)
{
  std::ifstream cataM2File;
  cataM2File.open( filename, std::ios::binary );
  
  cataHeader header;
  cataM2File.read( reinterpret_cast<char*>(&header), sizeof(header) );
 
  std::vector<cataCamera> cataCams (0);  
  cataM2File.seekg( header.ofsCameras );
  
  for ( int numCams = 0 ; numCams < header.nCameras ; ++numCams )
  {
    cataCamera singleCam;
	cataM2File.read( reinterpret_cast<char*>(&singleCam), sizeof(cataCamera) );
    cataCams.push_back( singleCam );
  }  
  
  const int headerSize (0x130);
  
  cataM2File.seekg( 0, std::ios::end );
  const long fullFileSize ( cataM2File.tellg() );
  const long restOfFileSize (fullFileSize - headerSize);
  cataM2File.seekg( headerSize, std::ios::beg );
 
  char * restOfFile;
  restOfFile = new char[restOfFileSize];
  
  cataM2File.read( restOfFile, restOfFileSize ); 
 
  cataM2File.close();

  /* Start of tutorial, Zim4ik part */
  header.version = 264;
  header.nTexUnits = 1;
  header.ofsTexUnits = header.ofsTransLookup;
  
  /* Cameras fix, Schlumpf's help */
  std::vector<lkCamera> lkCams (0);
  
  if ( header.nCameras > 0 )
  {
    header.ofsCameras = fullFileSize;
	
	for ( int numCams = 0 ; numCams < header.nCameras ; ++numCams )
	{
	  lkCamera singleLkCam;
	  
	  singleLkCam.Type = cataCams[numCams].Type;
	  
	  for (int i = 0 ; i < 0x5C ; ++i)
	    singleLkCam.otherStuff[i] = cataCams[numCams].otherStuff[i];
	  
	  if ( cataCams[numCams].Type == 0 )
	    singleLkCam.FOV = 0.7;
      else
	    singleLkCam.FOV = 0.97;

	  lkCams.push_back( singleLkCam );
	}
  }
  
  std::ofstream lkOutput;
  std::string outputFileName ( filename );
  outputFileName.append( "_lk" );
  lkOutput.open( outputFileName.c_str(), std::ios::binary );
  
  lkOutput.write( reinterpret_cast<char*>(&header), sizeof(header) );
  lkOutput.write( restOfFile, restOfFileSize );
  lkOutput.write( reinterpret_cast<char*>(&lkCams[0]), sizeof(lkCamera) * lkCams.size() );
  
  lkOutput.close();
  
  delete[] restOfFile;
}

void fixSkinFile(const std::string & filename)
{
  std::ifstream skinFile;
  skinFile.open( filename.c_str(), std::ios::binary );
  
  skinFile.seekg( 0, std::ios::end );
  const long fullFileSize ( skinFile.tellg() );
  
  if ( fullFileSize > 0 )
  {
    skinFile.seekg( 0, std::ios::beg );
    skinHeader skinHdr;
    skinFile.read( reinterpret_cast<char*>(&skinHdr), sizeof(skinHdr) ); 

	skinFile.seekg( 0, std::ios::beg );
	
	char * startOfFile;
	const long startOfFileSize ( skinHdr.ofsSubmeshes );
	
	startOfFile = new char[startOfFileSize];
    skinFile.read( startOfFile, startOfFileSize ); 
	
    std::vector<submesh> subs (0);  
  
    for ( int numSubs = 0 ; numSubs < skinHdr.nSubmeshes ; ++numSubs )
    {
      submesh singleSubmesh;
	  skinFile.read( reinterpret_cast<char*>(&singleSubmesh), sizeof(submesh) );
      subs.push_back( singleSubmesh );
    }  
	
    std::vector<textureUnit> texUnits (0);  
  
    for ( int numTexUnits = 0 ; numTexUnits < skinHdr.nTextureUnits ; ++numTexUnits )
    {
      textureUnit singleTexUnit;
	  skinFile.read( reinterpret_cast<char*>(&singleTexUnit), sizeof(textureUnit) );
      texUnits.push_back( singleTexUnit );
    }  	
	
	const long endOfFileSize ( fullFileSize - skinFile.tellg() );	
	char * endOfFile;
	
	endOfFile = new char[endOfFileSize];
    skinFile.read( endOfFile, endOfFileSize ); 	
	
    skinFile.close();	

    /* My addition for dynamic shadows */
	
    for ( int i = 0 ; i < subs.size() ; ++i )
    {
	  if ( subs[i].Unknown == 0 )
	    subs[i].Unknown = 1;
    }
	
    /* Morfium's post on page 2 : http://modcraft.superparanoid.de/posting.php?mode=quote&f=20&p=14081 */ 	

    for ( int i = 0 ; i < texUnits.size() ; ++i )
    {
	  texUnits[i].d4 = 1;
	  
	  if ( (texUnits[i].flags != 0) || (texUnits[i].flags != 16) )
	    texUnits[i].flags = 16;
		
	  if ( texUnits[i].order > 50 ) // I hope that one is ok...
	    texUnits[i].order = 1;	    
    }	
	
    std::ofstream lkOutput;
    std::string outputFileName ( filename );
    outputFileName.append( "_lk" );
    lkOutput.open( outputFileName.c_str(), std::ios::binary );
  
    lkOutput.write( startOfFile, startOfFileSize );
	lkOutput.write( reinterpret_cast<char*>(&subs[0]), sizeof(submesh) * subs.size() );
    lkOutput.write( reinterpret_cast<char*>(&texUnits[0]), sizeof(textureUnit) * texUnits.size() );
    lkOutput.write( endOfFile, endOfFileSize );
  
    lkOutput.close();	
	
	delete[] startOfFile;
	delete[] endOfFile;
  }
  else
  {
    skinFile.close();
  }
}

int main( int argc, char **argv )
{
  fixM2File( argv[1] );
  
  std::string skinGenFileName ( argv[1] );
  skinGenFileName = skinGenFileName.substr( 0, skinGenFileName.size() - 3 );
  
  std::string skinFileNameZero ( skinGenFileName );
  skinFileNameZero.append( "00.skin" );
  
  std::string skinFileNameOne ( skinGenFileName );
  skinFileNameOne.append( "01.skin" );
  
  std::string skinFileNameTwo ( skinGenFileName );
  skinFileNameTwo.append( "02.skin" );
  
  std::string skinFileNameThree ( skinGenFileName );
  skinFileNameThree.append( "03.skin" );    
  
  fixSkinFile( skinFileNameZero );
  fixSkinFile( skinFileNameOne );
  fixSkinFile( skinFileNameTwo );
  fixSkinFile( skinFileNameThree );

  return 0;
}