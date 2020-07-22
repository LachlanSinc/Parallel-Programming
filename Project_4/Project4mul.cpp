// Project4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include <cstdio>
#include <xmmintrin.h>
#define SSE_WIDTH		4

float SimdMulSum(float *a, float *b, int len);
float PlainMulSum(float *a, float *b, int len);

int
main(int argc, char *argv[])
{
#ifndef _OPENMP
	//printf(stderr, "No OpenMP support!\n");
	printf("No OpenMP support!\n");
	return 1;
#endif

	int length = atoi(argv[1]);
	int numTrials = 10; // atoi(argv[2]);
	int numt = atoi(argv[2]);

	//float *a = new float[length];
	//float *b = new float[length];

	unsigned char *p = (unsigned char *)malloc(64 + (length) * sizeof(float));
	int offset = (long int)p & 0x3f; // 0x3f = bottom 6 bits are all 1’s
	float *a = (float *)&p[64 - offset];

	unsigned char *v = (unsigned char *)malloc(64 + (length) * sizeof(float));
	offset = (long int)v & 0x3f; // 0x3f = bottom 6 bits are all 1’s
	float *b = (float*)&p[64 - offset];

	double maxSimPerformance = 0, simSum, maxPlainPerformance = 0, plainSum, maxMulPerformance = 0;

	for (int i = 0; i < length; i++)
	{
		a[i] = i;// + 0.45;
		b[i] = i; // + .67;
	}
	/*
	for (int i = 0; i < numTrials; i++)
	{
		double time0 = omp_get_wtime();

		simSum = SimdMulSum(&a[0], &b[0], length);

		double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double)length / (time1 - time0) / 1000000.;
		//double megaTrialsPerSecond = (time1 - time0);
		if (megaTrialsPerSecond > maxSimPerformance)
			maxSimPerformance = megaTrialsPerSecond;
	}
	*/
	for (int i = 0; i < numTrials; i++)
	{
		double time0 = omp_get_wtime();

		plainSum = PlainMulSum(&a[0], &b[0], length);

		double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double)length / (time1 - time0) / 1000000.;
		//double megaTrialsPerSecond = (time1 - time0);
		if (megaTrialsPerSecond > maxPlainPerformance)
			maxPlainPerformance = megaTrialsPerSecond;
	}

	
	omp_set_num_threads(numt);
	int elementsPerCore = length / numt;
	float mulSum = 0;
	for (int i = 0; i < numTrials; i++)
	{
		double time0 = omp_get_wtime();
		#pragma omp parallel default(none) shared(a, b, elementsPerCore) reduction(+:mulSum)
		{
			int first = omp_get_thread_num() * elementsPerCore;
			mulSum = SimdMulSum(&a[first], &b[first], elementsPerCore);
		}
		double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double)length / (time1 - time0) / 1000000.;
		//double megaTrialsPerSecond = (time1 - time0);
		if (megaTrialsPerSecond > maxMulPerformance)
			maxMulPerformance = megaTrialsPerSecond;
	}
	


	printf("%d\t%d\t%lf\t%lf\n", length, numt, maxPlainPerformance,  maxMulPerformance);

	free(p);
	free(v);
}


float PlainMulSum(float *a, float *b, int len)
{
	float sum = 0;

	for (int i = 0; i < len; i++)
	{
		sum += a[i] * b[i];
	}
	return sum;
}


float SimdMulSum(float *a, float *b, int len)
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = (len / SSE_WIDTH) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps(&sum[0]);
	for (int i = 0; i < limit; i += SSE_WIDTH)
	{
		ss = _mm_add_ps(ss, _mm_mul_ps(_mm_loadu_ps(pa), _mm_loadu_ps(pb)));
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps(&sum[0], ss);

	for (int i = limit; i < len; i++)
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}
/*

float
SimdMulSum(float *a, float *b, int len)
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = (len / SSE_WIDTH) * SSE_WIDTH;
	__asm
	(
		".att_syntax\n\t"
		"movq -40(%rbp), %r8\n\t" // a
		"movq -48(%rbp), %rcx\n\t" // b
		"leaq -32(%rbp), %rdx\n\t" // &sum[0]
		"movups (%rdx), %xmm2\n\t" // 4 copies of 0. in xmm2		);
		);

		for (int i = 0; i < limit; i += SSE_WIDTH)
		{
			__asm
			(
				".att_syntax\n\t"
				"movups (%r8), %xmm0\n\t" // load the first sse register
				"movups (%rcx), %xmm1\n\t" // load the second sse register
				"mulps %xmm1, %xmm0\n\t" // do the multiply
				"addps %xmm0, %xmm2\n\t" // do the add
				"addq $16, %r8\n\t"
				"addq $16, %rcx\n\t"
				);
		}
		__asm
		(
		".att_syntax\n\t"
		"movups %xmm2, (%rdx)\n\t" // copy the sums back to sum[ ]
		);
		for (int i = limit; i < len; i++)
		{
			sum[0] += a[i] * b[i];
		}
		return sum[0] + sum[1] + sum[2] + sum[3];
}
*/








