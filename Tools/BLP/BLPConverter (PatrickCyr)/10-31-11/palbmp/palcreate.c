/**********

createPaletteFast :
	trivial kludge:
		gather colors in an octree
		sorts colors on popularity
		adds them to the palette, trying to avoiding adding extremely similar colors
		has some speed-ups (like croppping low-count leaves)

createPaletteSlow :
	this is the "optimal" octree color quantizer. (see below, note on non-optimality)
	it is *VERY FAST* !
	gather all colors in an octree
	prune isolated strands so all nodes have > 1 kids
	the primary action is a "collapse" move:
		a leaf is cut, so that its color will be mapped to a sibling
		if it has no siblings, the color gets mapped to its parent
	each leaf keeps track of the "cost" (= increase in MSE) of cutting it
	each node has color which is the weighted average of its children
	the "cost" of a node, is the cost of all its children, plus the cost to
		move its new centroid.  This is exact, it's kind of subtle. see later
	we keep removing the node which has the lowest cost to cut
		(we use a radix sort to sort on cutCost ; this gives us the speed win)

my fast (incremental) way to compute the true node cost :
	true_cost = Sum[kids] kid_count * (kid_color - new_color)^2
	my_cost = Sum[kids] kid_count * (kid_color - node_color)^2
					+ node_count * (node_color - new_color)^2

	true_cost = Sum[kids] kid_count * (kid_color - new_color)^2
			  = Sum[kids] kid_count * ((kid_color - node_color) + (node_color - new_color))^2
			  = Sum[kids] kid_count * ((kid_color - node_color)^2 + (node_color - new_color)^2
										+ 2 * (kid_color - node_color) * (node_color - new_color))
			= approx_cost + 2 *  (node_color - new_color) * { Sum[kids] kid_count * (kid_color - node_color) }

	the correction here is exactly zero! why :
		Sum[kids] kid_count * (kid_color - node_color) = (Sum[kids] kid_count * kid_color) - node_count * node_color = 0 !
	since that's the definition of node_color !

why this isn't exactly optimal:
	because octree children without the same parent are never grouped.
	the classic example	is in the binary tree, values 128 and 127 should have a cost of 1 to be
		merged together, but 128 will be merged with all values > 128 first.
	that is, the square boundaries of the tree are unnatural cuts in color space.
	this means that the "cutCost" is not accurate; there could be an actual lower MSE cost
		than our cutCost.
	furthermore, cutCost should be relative to all other leaves, not to their parent nodes,
		so that when I cut one leaf it changes the cutCosts of all other leaves.

***********/

#if 1
#include <crblib/inc.h>
#include <crblib/tsc.h>
#else
#define NEED_CRBLIB_FUNCS
#include "port.h"
#endif

#include "colorconv.h"
#include "paloptimize.h"

#define assertdebug(exp)	

void createPaletteFast(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);
void createPaletteMedium(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);
void createPaletteSlow(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries);

void createPalette(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries)
{
	createPaletteSlow(plane24bit,planeLen,palette,palEntries);

	palettizePlane24Optimize(plane24bit,planeLen,palette,palEntries,16384);
}

typedef struct octNode octNode;
struct octNode 
{
	octNode * kids[8];
	octNode * parent;
	int count,nKids;
	int R,G,B;

	// for the pruner:
	ulong cutCost;	// this could overflow in the upper root nodes
	octNode *prev,*next; // sorted linked list of leaves
};

#define square(x)	((x)*(x))

#define RGBbits(R,G,B,bits) (((((R)>>(bits))&1)<<2) + ((((G)>>(bits))&1)<<1) + (((B)>>((bits)))&1))

#define RADIX_SIZE	1024

static void freeOctNodes(octNode *node);
static void addOctNode(octNode *root,int R,int G,int B,int *nLeavesPtr);
static void gatherLeaves(octNode *node,octNode *** leavesPtrPtr,int minCount);
static void gatherLeavesCutting(octNode *node,octNode *** leavesPtrPtr);
static int leafCompareCount(const void *a,const void *b);
static int leafCompareCost(const void *a,const void *b);
int findClosest(int R,int G,int B,ubyte *palette,int palEntries,int *foundPalPtr);
void computeOctRGBs(octNode *node);
void computeCutCosts(octNode *node);
void readLeavesToPal(octNode **leaves,int gotLeaves,ubyte *palette,int palEntries);
void insertRadix(octNode * radix,octNode *leaf);

void createPaletteFast(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries)
{
octNode * root;
ubyte *planePtr;
int R,G,B;
int nLeaves,i,minCount,gotLeaves;
octNode ** leaves,**leavesPtr;

	// pushTSC();

	root = new(octNode);
	assert(root);

	// read the whole image into an octree
	//	this is the only pass over the input plane

	nLeaves = 0;
	planePtr = plane24bit;
	for(i=planeLen;i--;)
	{
		R = *planePtr++;
		G = *planePtr++;
		B = *planePtr++;
		addOctNode(root,R,G,B,&nLeaves);
	}

	leaves = malloc(sizeof(octNode *)*nLeaves);
	assert(leaves);
	
	// gather leaves into a linear array
	//	for speed we ignore leaves with a count lower than [x]

	gotLeaves = 0;
	for( minCount = 3; minCount >= 0 && gotLeaves < palEntries ; minCount-- )
	{
		leavesPtr = leaves;
		gatherLeaves(root,&leavesPtr,minCount);
		gotLeaves = ((ulong)leavesPtr - (ulong)leaves)/sizeof(octNode *);
	}

	// sort the leaves by count

	qsort(leaves,gotLeaves,sizeof(octNode *),leafCompareCount);

	// read the sorted leaves in by count; we try to only read in leaves
	//	that are farther than 'minDistance' from nodes already in the palette

	readLeavesToPal(leaves,gotLeaves,palette,palEntries);

	free(leaves);
	freeOctNodes(root);

	// showPopTSC("createPalFast",stderr);
}

void createPaletteSlow(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries)
{
octNode * root;
ubyte *planePtr,*palPtr;
int R,G,B;
int nLeaves,i,gotLeaves,radixN;
octNode ** leaves,**leavesPtr;
octNode *leaf,*node;
octNode *radix;

	// pushTSC();

	root = new(octNode);
	assert(root);

	// read the whole image into an octree
	//	this is the only pass over the input plane

	nLeaves = 0;
	planePtr = plane24bit;
	for(i=planeLen;i--;)
	{
		R = *planePtr++;
		G = *planePtr++;
		B = *planePtr++;
		addOctNode(root,R,G,B,&nLeaves);
	}

	leaves = malloc(sizeof(octNode *)*nLeaves);
	assert(leaves);

	computeOctRGBs(root);
	root->parent = root;
	computeCutCosts(root);
	root->parent = NULL;
	
	// gather leaves into a linear array
	//	for speed we ignore leaves with a count lower than [x]

	leavesPtr = leaves;
	gatherLeavesCutting(root,&leavesPtr);
	gotLeaves = ((ulong)leavesPtr - (ulong)leaves)/sizeof(octNode *);

	// if gotLeaves < palEntries, just exit asap
	if ( gotLeaves < palEntries )
	{
		readLeavesToPal(leaves,gotLeaves,palette,palEntries);
		free(leaves);
		freeOctNodes(root);
		// showPopTSC("createPalSlow",stderr);
		return;
	}

	// sort the leaves by cutCost
	// radix sort instead of qsort

	radix = malloc(sizeof(octNode)*RADIX_SIZE);
	assert(radix);

	for(i=0;i<RADIX_SIZE;i++)
		radix[i].next = radix[i].prev = &radix[i];

	for(i=0;i<gotLeaves;i++)
		insertRadix(radix,leaves[i]);

	// keep cutting the tail
	radixN = 0;
	while(gotLeaves > palEntries)
	{
		while( (leaf = radix[radixN].next) == &(radix[radixN]) )
		{
			radixN++;
			assertdebug( radixN < RADIX_SIZE );
		}
		// cut it
		leaf->prev->next = leaf->next;
		leaf->next->prev = leaf->prev;

		node = leaf->parent;
		assertdebug(node);
		node->nKids --;

		// might turn its parent into a leaf;
		// if so, add it to the list
			// nKids no longer corresponds to the actual number of active kids

		if ( node->nKids == 0 )
			insertRadix(radix,node);
		else
			gotLeaves--;
	}

	// read the sorted leaves in by count; we try to only read in leaves
	//	that are farther than 'minDistance' from nodes already in the palette

	palPtr = palette;
	radixN = RADIX_SIZE-1;
	leaf = radix[radixN].prev;	
	for(i=0;i<palEntries && radixN>0;i++)
	{
		*palPtr++ = leaf->R;
		*palPtr++ = leaf->G;
		*palPtr++ = leaf->B;
		leaf = leaf->prev;
		while ( leaf == &(radix[radixN]) )
		{
			radixN --;
			if ( ! radixN )
				break;
			leaf = radix[radixN].prev;
		}
	}

	free(radix);
	free(leaves);
	freeOctNodes(root);

	// showPopTSC("createPalSlow",stderr);
}

void insertRadix(octNode * radix,octNode *leaf)
{
octNode *insertAt;

	if ( leaf->cutCost >= RADIX_SIZE ) 
	{
		octNode * head;
		insertAt = head = & radix[RADIX_SIZE-1];
		while(insertAt->cutCost < leaf->cutCost && insertAt->next != head )
			insertAt = insertAt->next;
	}
	else
		insertAt = & radix[leaf->cutCost];

	leaf->next = insertAt->next;
	leaf->next->prev = leaf;
	insertAt->next = leaf;
	insertAt->next->prev = insertAt;
}

void createPaletteMedium(ubyte *plane24bit,int planeLen,ubyte *palette,int palEntries)
{
octNode * root;
ubyte *planePtr;
int R,G,B;
int nLeaves,i,gotLeaves;
octNode ** leaves,**leavesPtr;

	// pushTSC();

	root = new(octNode);
	assert(root);

	// read the whole image into an octree
	//	this is the only pass over the input plane

	nLeaves = 0;
	planePtr = plane24bit;
	for(i=planeLen;i--;)
	{
		R = *planePtr++;
		G = *planePtr++;
		B = *planePtr++;
		addOctNode(root,R,G,B,&nLeaves);
	}

	leaves = malloc(sizeof(octNode *)*nLeaves);
	assert(leaves);

	computeOctRGBs(root);
	root->parent = root;
	computeCutCosts(root);
	root->parent = NULL;
	
	// gather leaves into a linear array
	//	for speed we ignore leaves with a count lower than [x]

	leavesPtr = leaves;
	gatherLeavesCutting(root,&leavesPtr);
	gotLeaves = ((ulong)leavesPtr - (ulong)leaves)/sizeof(octNode *);

	// if gotLeaves < palEntries, just exit asap
	if ( gotLeaves < palEntries )
	{
		readLeavesToPal(leaves,gotLeaves,palette,palEntries);
		free(leaves);
		freeOctNodes(root);
		// showPopTSC("createPalMedium",stderr);
		return;
	}

	// sort the leaves by cutCost

	// this is actually slower tha "palSlow" because qsort() is much slower
	//	than a radix sort

	qsort(leaves,gotLeaves,sizeof(octNode *),leafCompareCost);

	readLeavesToPal(leaves,gotLeaves,palette,palEntries);
	free(leaves);
	freeOctNodes(root);

	// showPopTSC("createPalMedium",stderr);
}

int findClosest(int R,int G,int B,ubyte *palette,int palEntries,int *foundPalPtr)
{
int i,d,bestD,bestP;
	bestD = 99999999; bestP = -1;
	for(i=palEntries;i--;)
	{
		d = square(R - palette[0]) + square(G - palette[1]) + square(B - palette[2]);
		palette += 3;
		if ( d < bestD ) 
		{
			bestD = d;
			bestP = i;
		}
	}
	if ( foundPalPtr ) *foundPalPtr = bestP;
return bestD;
}

static void addOctNode(octNode *root,int R,int G,int B,int *nLeavesPtr)
{
int idx;
int bits;
octNode *node;

	node = root;
	for(bits=7;bits>=0;bits--) 
	{
		idx = RGBbits(R,G,B,bits);
		if ( ! node->kids[idx] ) 
		{
			node->nKids ++;
			node->kids[idx] = new(octNode);
			node->kids[idx]->parent = node;
		}
		node->count ++;
		node = node->kids[idx];
	}
	if ( node->count == 0 ) (*nLeavesPtr)++;
	node->count ++;
	node->R = R;
	node->G = G;
	node->B = B;
}

static void gatherLeaves(octNode *node,octNode *** leavesPtrPtr,int minCount)
{
	if ( node->count <= minCount ) return;
	if ( node->nKids == 0 ) 
	{
		*(*leavesPtrPtr)++ = node;	
	}
	else
	{
		int i;
		for(i=0;i<8;i++)
		{
			if ( node->kids[i] ) gatherLeaves(node->kids[i],leavesPtrPtr,minCount);
		}
	}
}

static void gatherLeavesCutting(octNode *node,octNode *** leavesPtrPtr)
{
	if ( node->nKids > 0 ) 
	{
		int i;
		for(i=0;i<8;i++)
		{
			if ( node->kids[i] )
			{
				if ( node->kids[i]->count <= 1 || node->kids[i]->cutCost <= 1 )
				{
					freeOctNodes(node->kids[i]);
					node->kids[i] = NULL;
					node->nKids--;
				}
				else
				{
					gatherLeavesCutting(node->kids[i],leavesPtrPtr);
					
					if ( node->kids[i]->nKids == 1 )
					{
						octNode *kid;
						int j;
						kid = node->kids[i];
						for(j=0;j<8;j++)
							if ( kid->kids[j] ) 
								node->kids[i] = kid->kids[j];
						assertdebug( node->kids[i] != kid );
						node->kids[i]->cutCost = kid->cutCost;
						destroy(kid);
						node->kids[i]->parent = node;
					}
				}
			}
		}
	}

	if ( node->nKids == 0 ) 
	{
		*(*leavesPtrPtr)++ = node;	
	}
}

static void freeOctNodes(octNode *node)
{
int i;

	for(i=0;i<8;i++) 
		if ( node->kids[i] )
			freeOctNodes(node->kids[i]);

	destroy(node);
}

static int leafCompareCount(const void *a,const void *b)
{
octNode *na,*nb;
	na = *((octNode **)a);
	nb = *((octNode **)b);
return (nb->count) - (na->count);
}
static int leafCompareCost(const void *a,const void *b)
{
octNode *na,*nb;
	na = *((octNode **)a);
	nb = *((octNode **)b);
return (nb->cutCost) - (na->cutCost);
}

void computeCutCosts(octNode *node)
{
	assertdebug(node->parent);
	node->cutCost = square(node->R - node->parent->R)
					+ square(node->G - node->parent->G)
					+ square(node->B - node->parent->B);
	node->cutCost *= node->count;
	
	if ( node->nKids > 0 )
	{
	int i;
		for(i=0;i<8;i++)
			if ( node->kids[i] )
			{
				computeCutCosts(node->kids[i]);
				node->cutCost += node->kids[i]->cutCost;
			}
	}
}

void computeOctRGBs(octNode *node)
{
	if ( node->nKids > 0 )
	{
	int R,G,B;
	int i;
	octNode *kid;
		R = G = B = 0;
		for(i=0;i<8;i++)
			if ( node->kids[i] )
				computeOctRGBs(node->kids[i]);
		for(i=0;i<8;i++)
		{
			if ( kid = node->kids[i] )
			{
				R += kid->count * kid->R;
				G += kid->count * kid->G;
				B += kid->count * kid->B;
			}
		}
		node->R = R / (node->count);
		node->G = G / (node->count);
		node->B = B / (node->count);
	}
}

void readLeavesToPal(octNode **leaves,int gotLeaves,ubyte *palette,int palEntries)
{
octNode **leavesPtr;
ubyte *palPtr;
int R,G,B;
int i,palGot;
int distance,minDistance;

	palPtr = palette; palGot = 0;
	for(minDistance=256;minDistance>=0 && palGot < palEntries;minDistance>>=1)
	{
		leavesPtr = leaves;
		for(i=0;i<gotLeaves;i++)
		{
			R = (*leavesPtr)->R;
			G = (*leavesPtr)->G;
			B = (*leavesPtr)->B;
			leavesPtr++;
			distance = findClosest(R,G,B,palette,palGot,NULL);
			if ( distance >= minDistance )
			{
				*palPtr++ = R;
				*palPtr++ = G;
				*palPtr++ = B;
				palGot ++;
				if ( palGot == palEntries )
					break;
			}
		}
	}
}
