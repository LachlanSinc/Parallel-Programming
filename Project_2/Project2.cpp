/*
Author: Lachlan Sinclair
Date: 4/22/2020
Program: Numeric Integration with OpenMP Reduction
*/
#include "pch.h"
#include <iostream>
#include <omp.h>


#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

//#define NUMNODES 400.
#define N 4.

float Height(int, int, int);

int main(int argc, char *argv[])
{

		int numThreads = atoi(argv[1]);
		int numNodes = atoi(argv[2]);
		int numTries =10;
		float maxPerformance =0;
		float volume = 0.; //wont change in any of the repeat loops when using the same number of subdivisons 

		// the area of a single full-sized tile:

		float fullTileArea = (((XMAX - XMIN) / (float)(numNodes - 1))  *
			((YMAX - YMIN) / (float)(numNodes - 1)));


		omp_set_num_threads(numThreads);

		// sum up the weighted heights into the variable "volume"
		// using an OpenMP for loop and a reduction:

		int loops = numNodes * numNodes;

		for (int j = 0; j < numTries; j++)
		{
			double time0 = omp_get_wtime();
			volume = 0.;

			#pragma omp parallel for default(none) reduction(+:volume)
			for (int i = 0; i < loops; i++)
			{
				int iu = i % (int)numNodes;
				int iv = i / numNodes;
				if (iu != 0 && iu != numNodes - 1 && iv != 0 && iv != numNodes - 1)
				{
					float z = Height(iu, iv, numNodes);
					volume += (z * fullTileArea);
				}
				/*if (iu == 0 && iu == numNodes - 1)
				{
					if (iv == 0 && iv == numNodes - 1) 
					{
						float z = Height(iu, iv, numNodes);
						volume += (z * fullTileArea)/4.;
					}
					else
					{
						float z = Height(iu, iv, numNodes);
						volume += (z * fullTileArea) / 2.;
					}


				}
				else if (iv == 0 && iv == numNodes - 1)
				{
					float z = Height(iu, iv, numNodes);
					volume += (z * fullTileArea) / 2.;
				}
				else
				{
					float z = Height(iu, iv, numNodes);
					volume += (z * fullTileArea);
				}*/

			}

			double time1 = omp_get_wtime();
			double numNodesqrPerSecond = (double)(numNodes*numNodes) / (time1 - time0) / 1000000.;
			if (numNodesqrPerSecond > maxPerformance)
			{
				maxPerformance = numNodesqrPerSecond;
			}
			volume = volume * 2;
			//printf("volume: %lf\n", volume*2);
		}

		printf("%d\t%d\t%lf\t%lf\t\n", numThreads, numNodes, maxPerformance, volume);

		//printf("volume: %lf\n", avgVolume);
		//printf("numNodesqrPerSecond: %lf", maxPerformance);

}

float
Height(int iu, int iv, int numNodes)	// iu,iv = 0 .. numNodes-1
{
	float x = -1. + 2.*(float)iu / (float)(numNodes - 1);	// -1. to +1.
	float y = -1. + 2.*(float)iv / (float)(numNodes - 1);	// -1. to +1.

	float xn = pow(fabs(x), (double)N);
	float yn = pow(fabs(y), (double)N);
	float r = 1. - xn - yn;
	if (r < 0.)
		return 0.;
	float height = pow(1. - xn - yn, 1. / (float)N);
	return height;
}




