/*
 * main.cpp
 *
 *  Created on: 12.10.2009
 *      Author: Bastian
 */

/*
 * Small changes on bounding box normals : Mjollnà 2011-10-25
 */
 
/*
 * C++-ified by schlumpf 2011-10-27
 */
 
#include <libskin/skineditor.h>
#include <libm2/modeleditor.h>

#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

class file_t
{
  private:
    FILE* _handle;
    
  public:
    file_t (const std::string& filename, const std::string& flags)
    : _handle (fopen (filename.c_str(), flags.c_str()))
    {
      if (!_handle)
      {
        throw std::runtime_error ( "unable to open file "
                                 + filename
                                 + " with flags "
                                 + flags
                                 );
      }
    }
    
    file_t (FILE* handle)
    : _handle (handle)
    {
    }
    
    ~file_t()
    {
      fclose (_handle);
      _handle = NULL;
    }
    
    size_t size()
    {
      size_t pos (ftell (_handle));
      fseek (_handle, 0, SEEK_END);
      size_t filesize (ftell (_handle));
      fseek (_handle, pos, SEEK_SET);
      return filesize;
    }
    
    void read_from_disk (std::vector<char>& destination)
    {
      const size_t filesize (size());
      destination.resize (filesize);
      
      size_t pos (ftell (_handle));
      fseek (_handle, 0, SEEK_SET);
      fread (&destination[0], filesize, 1, _handle);
      fseek (_handle, pos, SEEK_SET);
    }
    
    void write_to_disk (std::vector<char>& data)
    {
      size_t pos (ftell (_handle));
      fseek (_handle, 0, SEEK_SET);
      fwrite (&data[0], data.size(), 1, _handle);
      fseek (_handle, pos, SEEK_SET);
    }
};

void add_collision (ModelEditor& me, SkinEditor& se)
{
	const int triangle_count (se.getnTriangles() / 3);
	const int index_count (se.getnIndices());
	
	const ModelVertex* vertices (me.getVertices());
	const Index* indices (se.getIndices());
	const Triangle* triangles (se.getTriangles());
	
	Vec3D* bVert (new Vec3D[index_count]);
	ModelBoundTriangle* bTri (new ModelBoundTriangle[triangle_count]);
	Vec3D* bNorm (new Vec3D[triangle_count]);
	
	for (int i (0); i < index_count; ++i)
	{
		bVert[i] = vertices[indices[i].Vertex].pos;
	}
	
	for (int i (0); i < triangle_count; ++i)
	{
		bTri[i].index[0] = indices[triangles[i].Indices[0]].Vertex;
		bTri[i].index[1] = indices[triangles[i].Indices[1]].Vertex;
		bTri[i].index[2] = indices[triangles[i].Indices[2]].Vertex;
		
		// Mjo : We add 1 surface normal to every triangle
		Vec3D U ( vertices[indices[triangles[i].Indices[1]].Vertex].pos
		        - vertices[indices[triangles[i].Indices[0]].Vertex].pos
		        );
		Vec3D V ( vertices[indices[triangles[i].Indices[2]].Vertex].pos
		        - vertices[indices[triangles[i].Indices[0]].Vertex].pos
		        );	
		bNorm[i] = U % V;
		bNorm[i].normalize();
	}
	
	BoundVolumeHelper bv;
	// Mjo : One normal per triangle (was : index_count)
	bv.nBNormals = triangle_count;
	bv.nBVertices = index_count;
	bv.nBTriangles = triangle_count;
	bv.BNormals = bNorm;
	bv.BVertices = bVert;
	bv.BTriangles = bTri;
	me.newBounds (bv);
	
	delete[] bNorm;
	delete[] bVert;
	delete[] bTri;
}

int main(int argc, char **argv)
{
  assert (argc == 2 && "specify filename as argument."); 

  const std::string filename (argv[1]);
	const std::string modelname (filename.substr (0, filename.length() - 3));
  
  std::vector<char> input_data;

  {
  	file_t input (filename, "rb+");
	  input.read_from_disk (input_data);
  }
  
  std::vector<char> skin_data;

  {
  	file_t input (modelname + "00.skin", "rb+");
	  input.read_from_disk (skin_data);
  }
	
	std::cout << "model and skin read." << std::endl;

	ModelEditor me (&input_data[0], input_data.size());
	SkinEditor se (&skin_data[0], skin_data.size());
	
	std::cout << "initialized." << std::endl;
	
	add_collision (me, se);
	
	std::cout << "added collision." << std::endl;

  {
    const char* output_data_raw (me.getFile());
    std::vector<char> output_data ( output_data_raw
                                  , output_data_raw + me.GetFileSize()
                                  );
    
    file_t output (filename, "wb");
    output.write_to_disk (output_data);
  }
  
  std::cout << "written to original file." << std::endl;
  
	return 0;
}
