#pragma once

#include <vector>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>


namespace M2Lib
{


// unused
inline bool CompareFloatsSimilar( Float32 A, Float32 B, Float32 Margin )
{
	return ( abs( A - B ) <= Margin );
}


#pragma pack(push,1)


// used when importing an M2I and building an M2. each triangle is given an index in addition to the 3 vertices. the index is used for quick lookup when dealing out triangles between bone partitions when building skins.
class CTriangle
{
public:
	UInt32 Index;			// this triangle's global index, or unique ID.
	UInt16 Vertices[3];		// indices into the global vertex list.


public:
	CTriangle()
	{
		Index = 0;

		Vertices[0] = 0;
		Vertices[1] = 0;
		Vertices[2] = 0;
	}

	CTriangle( const CTriangle& Other )
	{
		*this = Other;
	}

	CTriangle& operator = ( const CTriangle& Other )
	{
		Index = Other.Index;

		Vertices[0] = Other.Vertices[0];
		Vertices[1] = Other.Vertices[1];
		Vertices[2] = Other.Vertices[2];

		return *this;
	}

};


// vertices are in this format.
class CVertex
{
public:
	Float32 Position[3];
	UInt8 BoneWeights[4];
	UInt8 Bones[4];
	Float32 Normal[3];
	Float32 Texture[2];
	Float32 Padding[2];		// make vertex 48 bytes in size, which benefits vetex caching (i think).


public:
	CVertex()
	{
		Position[0] = 0.0f;
		Position[1] = 0.0f;
		Position[2] = 0.0f;

		BoneWeights[0] = 0;
		BoneWeights[1] = 0;
		BoneWeights[2] = 0;
		BoneWeights[3] = 0;

		Bones[0] = 0;
		Bones[1] = 0;
		Bones[2] = 0;
		Bones[3] = 0;

		Normal[0] = 0.0f;
		Normal[1] = 0.0f;
		Normal[2] = 0.0f;

		Texture[0] = 0.0f;
		Texture[1] = 0.0f;

		Padding[0] = 0.0f;
		Padding[1] = 0.0f;
	}

	CVertex( const CVertex& Other )
	{
		*this = Other;
	}

	CVertex& operator = ( const CVertex& Other )
	{
		Position[0] = Other.Position[0];
		Position[1] = Other.Position[1];
		Position[2] = Other.Position[2];

		BoneWeights[0] = Other.BoneWeights[0];
		BoneWeights[1] = Other.BoneWeights[1];
		BoneWeights[2] = Other.BoneWeights[2];
		BoneWeights[3] = Other.BoneWeights[3];

		Bones[0] = Other.Bones[0];
		Bones[1] = Other.Bones[1];
		Bones[2] = Other.Bones[2];
		Bones[3] = Other.Bones[3];

		Normal[0] = Other.Normal[0];
		Normal[1] = Other.Normal[1];
		Normal[2] = Other.Normal[2];

		Texture[0] = Other.Texture[0];
		Texture[1] = Other.Texture[1];

		Padding[0] = Other.Padding[0];
		Padding[1] = Other.Padding[1];

		return *this;
	}

	// compares 2 vertices to see if they have the same position, bones, and texture coordinates. vertices between subsets that pass this test are most likely duplicates.
	static bool CompareSimilar( CVertex& A, CVertex& B, bool CompareTextures, bool CompareBones, Float32 PositionalTolerance, Float32 AngularTolerance )
	{
		// compare position
		if ( PositionalTolerance > 0.0f )
		{
			Float32 Delta[3];
			Delta[0] = A.Position[0] - B.Position[0];
			Delta[1] = A.Position[1] - B.Position[1];
			Delta[2] = A.Position[2] - B.Position[2];
			Float32 Distance = sqrt( ( Delta[0] * Delta[0] ) + ( Delta[1] * Delta[1] ) + ( Delta[2] * Delta[2] ) );
			if ( Distance > PositionalTolerance )
			{
				return false;
			}
		}
		else
		{
			if ( ( A.Position[0] != B.Position[0] ) || ( A.Position[1] != B.Position[1] ) || ( A.Position[2] != B.Position[2] ) )
			{
				return false;
			}
		}

		// compare texture coordinates
		if ( CompareTextures )
		{
			if ( ( A.Texture[0] != B.Texture[0] ) || ( A.Texture[1] != B.Texture[1] ) )
			{
				return false;
			}
		}

		// compare bones
		if ( CompareBones )
		{
			// order independent comparison
			bool SameBones[4]; SameBones[0] = false; SameBones[1] = false; SameBones[2] = false; SameBones[3] = false;
			for ( UInt32 i = 0; i < 4; i++ )
			{
				bool HasSameBone = false;
				for ( UInt32 j = 0; j < 4; j++ )
				{
					if ( A.Bones[i] == B.Bones[j] && SameBones[j] == false)
					{
						SameBones[j] = true;
						break;
					}
				}
			}
			if ( !( SameBones[0] && SameBones[1] && SameBones[2] && SameBones[3] ) )
			{
				return false;
			}
		}

		// compare normals
		// one way to visualize this is that NormalA defines a plane and NormalB defines a point
		// we want to determine what side of the PlaneA that the PointB lies on
		// it's just as simple as getting the dot product of the two vectors and checking the sign of the result
		// arc cosine the dot product of the vectors to get the angle between them
		if ( AngularTolerance > 0.0f )
		{
			Float32 Dot = ( A.Normal[0] * B.Normal[0] ) + ( A.Normal[1] * B.Normal[1] ) + ( A.Normal[2] * B.Normal[2] );
			if ( acos( Dot ) > AngularTolerance )	// units are radians
			{
				return false;
			}
		}
		else
		{
			if ( ( A.Normal[0] != B.Normal[0] ) || ( A.Normal[1] != B.Normal[1] ) || ( A.Normal[2] != B.Normal[2] ) )
			{
				return false;
			}
		}

		return true;
	}

};
#pragma pack(pop)


// unused.
// this is needlessly complicted and doesn't really solve anything.
// orignally intended to solve WMV issues involving alledged crossover of submeshes accessing vertices belonging to another submesh, but that problem turned out to be casued by a missing vertex list length assignment.
// so actually, this is nothing more than just a std::vector< CVertex >
class CVertexListBuilder
	: public std::vector< CVertex >
{
private:
	// index of first vertex in cache.
	UInt32 m_CacheStart;
	// list of vertices in current subset. this list will keep us from adding the same vertex more than once to a single subset. also, by clearing it between subsets, we keep duplicate vertices from crossing over between subsets.
	std::vector< CVertex > m_VertexCache;


public:
	CVertexListBuilder()
	{
		m_CacheStart = 0;
	}

	// adds a vertex to this list and returns its index, or returns index to existing similar vertex.
	UInt32 Add( CVertex& Vertex )
	{
		UInt32 Count = this->size();
		UInt32 CountCache = m_VertexCache.size();
		for ( UInt32 i = 0; i != CountCache; i++ )
		{
			CVertex& CurrentVertex = m_VertexCache[i];
			if ( CVertex::CompareSimilar( Vertex, CurrentVertex, true, true, 0.0f, 0.0f ) )
			{
				// add normal to existing, to be normalized later
				CurrentVertex.Normal[0] += Vertex.Normal[0];
				CurrentVertex.Normal[1] += Vertex.Normal[1];
				CurrentVertex.Normal[2] += Vertex.Normal[2];
				return m_CacheStart + i;
			}
		}

		assert( Count < 0xFFFF );
		this->push_back( Vertex );
		m_VertexCache.push_back( Vertex );
		return Count;
	}

	// normalizes normals
	void Normalize()
	{
		UInt32 Count = this->size();
		for ( UInt32 i = 0; i != Count; i++ )
		{
			CVertex& CurrentVertex = operator[]( i );
			Float32 Length = sqrt( ( CurrentVertex.Normal[0] * CurrentVertex.Normal[0] ) + ( CurrentVertex.Normal[1] * CurrentVertex.Normal[1] ) + ( CurrentVertex.Normal[2] * CurrentVertex.Normal[2] ) );
			CurrentVertex.Normal[0] /= Length;
			CurrentVertex.Normal[1] /= Length;
			CurrentVertex.Normal[2] /= Length;
		}
	}

	void ClearVertexCache()
	{
		m_CacheStart = this->size();
		m_VertexCache.clear();
	}

	void ResetVertexCache()
	{
		m_CacheStart = 0;
		m_VertexCache.clear();
	}
};


// unused.
// a list of bones that can be sorted by most used bones first.
class CSortedBoneListBuilder
	: public  std::list< std::pair< UInt32, UInt32 > >
{
public:
	UInt32 Add( UInt32 Bone )
	{
		if ( Bone == 0 )
			return 0;

		UInt32 i = 0;
		for ( std::list< std::pair< UInt32, UInt32 > >::iterator Iterator = this->begin(); Iterator != this->end(); Iterator++ )
		{
			if ( Bone == Iterator->first )
			{
				Iterator->second++;
				return i;
			}
			i++;
		}

		std::pair< UInt32, UInt32 > New;
		New.first = Bone;
		New.second = 1;
		UInt32 Count = this->size();
		this->push_back( New );
		return Count;
	}

	void Sort()
	{
		this->sort( &CSortedBoneListBuilder::compare_usage );
	}

	static bool compare_usage( std::pair< UInt32, UInt32 > first, std::pair< UInt32, UInt32 > second )
	{
		return first.second > second.second;
	}

	void AsVector( std::vector< UInt16 >& Result )
	{
		for ( std::list< std::pair< UInt32, UInt32 > >::iterator Iterator = this->begin(); Iterator != this->end(); Iterator++ )
		{
			Result.push_back( Iterator->first );
		}
	}

};




}
