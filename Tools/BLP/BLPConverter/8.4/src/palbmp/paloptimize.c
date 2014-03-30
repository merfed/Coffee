/*********

our colors are referred to as "RGB" triples, but are actually typically YUV

--------

The basics :

	find the average color to which each palette entry is mapped.
	set the palette entry to that color.
	repeat.

The problems :

	1. this does NOT converge ; there's some wierd histerysis;
		it also does NOT monotonically improve MSE !
		we simply stop when MSE hits a local min.

		<*> I think this is because our "ClosestPal" algorithm is imperfect
			since it works within an oct-tree structure

	2. we can fall into "unstable local minimum" traps, like :
		(this is a plot in color space)

				X
			   + +
			  X + X

		here X indicates a blob of colors used, and + indicates a palette color;
		this configuration is stable under our algorithm, though of
		course the optimal is

				*
		
			  *   *

		(where * = X and +)
		(note that these configurations cannot occur in 1d finite graphs; the
		1d infinite graph for an unstable minimum is ...X+X+X+X+X+... )

	<>	this could be solved with like a Monte-Carlo walk,
			adding some rare random component.
		it doesn't work to just add a random wiggle whenever we stall out;
			perhaps a random componenet in the direction of the deltas, like

			diffR = Guassian_Rand( center = diffR , variance = abs(diffR) /2 );

		seems to help sometimes, but isn't a clear win

**********/
#if 1
#include <crblib/inc.h>
#include <crblib/tsc.h>
#else
#define NEED_CRBLIB_FUNCS
#include "port.h"
#endif

#include "colorconv.h"
#include "palettize.h"
#include <stdio.h>

/*******/

typedef struct 
{
	int totR,totG,totB,count;
} palOptInfo;

extern int tune_param;

int GaussianRand(int mean,int variance);

int stepTable[] = { 1009 , 757, 499, 401, 307, 239, 197, 157, 131, 103, 67, 41, 29, 17, 13, 7, 4, 1 };

void palettizePlane24Optimize(ubyte *rgbPlane,int planeLen,ubyte *palette,int palEntries,int maxSamples)
{
palInfo *palInfo;
palOptInfo *optInfo;
int pal,R,G,B;
ulong mse,last_mse;
ubyte *rgbPtr,*palPtr,*rgbPlaneDone;
ubyte savePalette[768];
int extraStepIndex,extraStepSize,samples,totSamples;

	// pushTSC();

	// rgbPlane is (planeLen*3) bytes
	// palette is 768 bytes

	R = palette[(palEntries-1)*3 + 0];
	G = palette[(palEntries-1)*3 + 1];
	B = palette[(palEntries-1)*3 + 2];
	for(pal=palEntries;pal<256;pal++)
	{
		palette[pal*3 + 0] = R;
		palette[pal*3 + 1] = G;
		palette[pal*3 + 2] = B;
	}

	optInfo = malloc(sizeof(palOptInfo)*palEntries);
	if ( ! optInfo ) return;	// just give up

	mse = ~0;
	extraStepIndex = 0;
	extraStepSize = -1;
	totSamples = 0;
	if ( maxSamples <= 0 ) maxSamples = 0x0FFFFFFF;
	for(;;)
	{
		if ( extraStepSize != 0 )
		{
			extraStepSize = ( stepTable[ extraStepIndex ] - 1 ) * 3;
			extraStepIndex++;
		}

		last_mse = mse;

		palInfo = closestPalInit(palette);
		if ( ! palInfo ) return;

		memclear(optInfo,sizeof(palOptInfo)*palEntries);

		mse = 0;
		rgbPlaneDone = rgbPlane + (planeLen*3) - 2;
		samples =0;

		for( rgbPtr = rgbPlane ; rgbPtr < rgbPlaneDone ; )
		{
		int x;

			R = *rgbPtr++;	G = *rgbPtr++;	B = *rgbPtr++;

			// <> this 'closestPal' is not great for this application
			//		it's approximate & has a large initialization overhead
			//	 we should use the methods from the "Local K-Means" paper

			pal = closestPal(R,G,B,palInfo);

			if ( pal >= palEntries ) pal = palEntries-1;			
		
			palPtr = palette + pal+pal+pal;
			x = R - (*palPtr++);	mse += x*x;
			x = G - (*palPtr++);	mse += x*x;
			x = B - (*palPtr  );	mse += x*x;

			optInfo[pal].totR += R;
			optInfo[pal].totG += G;
			optInfo[pal].totB += B;
			optInfo[pal].count ++;

			rgbPtr += extraStepSize;
			samples ++;
		}

		closestPalFree(palInfo);

		if ( samples == 0 ) continue;
		mse = (mse<<8)/samples;
		totSamples += samples;

		if ( mse >= last_mse && extraStepSize == 0 )
		{
			memcpy(palette,savePalette,768);
			break;
		}
		else if ( mse > last_mse )
		{
#if 0
			// seems to slow convergence (!?)
			memcpy(palette,savePalette,768);
#endif
		}
		else
		{
			memcpy(savePalette,palette,768);
		}
	
		// fprintf(stderr,"mse*256 = %d , extrastep = %d, samples = %d\n",mse,extraStepSize,samples);

		if ( totSamples >= maxSamples )
			break;

		for(pal=0,palPtr = palette; pal<palEntries ; pal++,palPtr += 3)
		{
		double fd;
		int diffR,diffG,diffB;

			if ( optInfo[pal].count == 0 ) continue;

			fd = 1.0 / optInfo[pal].count;

			diffR = (int)(optInfo[pal].totR * fd) - palPtr[0];
			diffG = (int)(optInfo[pal].totG * fd) - palPtr[1];
			diffB = (int)(optInfo[pal].totB * fd) - palPtr[2];

#if 1
			#define DIV(diff)	if ( diff < 0 ) diff = - ((-diff + 1)>>1); else if ( diff > 0 ) diff = ((diff + 1)>>1)
			DIV(diffR);
			DIV(diffG);
			DIV(diffB);
#endif

#if 0
			// this helps sometimes, hurts sometimes
			diffR = GaussianRand(diffR,abs(diffR));
			diffG = GaussianRand(diffG,abs(diffG));
			diffB = GaussianRand(diffB,abs(diffB));
#endif

			palPtr[0] = minmax( palPtr[0]+diffR , 0,255);
			palPtr[1] = minmax( palPtr[1]+diffG , 0,255);
			palPtr[2] = minmax( palPtr[2]+diffB , 0,255);
		}
	}

	destroy(optInfo);
	
	// showPopTSC("palOptimize",stderr);
}

#include <stdlib.h>

int GaussianRand(int val,int step)
{
int r;

	r = 0;

restart:

	r >>= 1;	if ( ! r ) r = rand() >> 1;
	if ( r&1 ) step = - step;

	for(;;)
	{
		r >>= 1;	if ( ! r ) r = rand() >> 1;
		if ( r & 1 )
			val += step;
		else
		{
			val += (rand()*step)/RAND_MAX;
			step /= 2;
			if ( step == 0 ) return val;
			goto restart;
		}
	}
}

