// Project3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES
#include <cmath>

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <random>
#include <time.h>


void	InitBarrier(int);
void	WaitBarrier();
int		Ranf( int, int);
float	Ranf( float, float);
float	SQR(float);

void TimeOfDaySeed();

void	GrainDeer();
void	Grain();
void	Nomad();
void	Watcher();


int	NowYear;		// 2020 - 2025
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int		NowNumDeer;		// number of deer in the current population
int		NowNumNomad;


const float GRAIN_GROWS_PER_MONTH = 10.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;

const float ONE_NOMAD_GRAIN_EATS_PER_MONTH = 1.0;
const float ONE_NOMAD_DEER_EATS_PER_MONTH = 1;


const float AVG_PRECIP_PER_MONTH = 7.0;	// average
const float AMP_PRECIP_PER_MONTH = 6.0;	// plus or minus
const float RANDOM_PRECIP = 2.0;	// plus or minus noise

const float AVG_TEMP = 55.0;	// average
const float AMP_TEMP = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

//unsigned int seed = 0;


omp_lock_t	Lock;
int		NumInThreadTeam;
int		NumAtBarrier;
int		NumGone;


int main()
{
	NowMonth = 0;
	NowYear = 2020;
	NowNumDeer = 1;
	NowHeight = 1.;
	NowNumNomad = 0;

	TimeOfDaySeed();

	float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);

	float temp = AVG_TEMP - AMP_TEMP * cos(ang);
	NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
	NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
	if (NowPrecip < 0.)
		NowPrecip = 0.;

	InitBarrier(4);

	omp_set_num_threads(4);	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer();
		}

		#pragma omp section
		{
			Grain();
		}

		#pragma omp section
		{
			Watcher();
		}
		#pragma omp section
		{
			Nomad();
		}
	}
}


void GrainDeer()
{
	while (NowYear < 2026)
	{

		int tempNextNumDeer;

		if ((double)NowNumDeer < NowHeight)
		{
			tempNextNumDeer = NowNumDeer + 1;
		}
		else if ((double)NowNumDeer > NowHeight && NowNumDeer!=0)
		{
			tempNextNumDeer = NowNumDeer - 1;
		}
		else
		{
			tempNextNumDeer = NowNumDeer;
		}

		if (NowMonth%2 == 0)
		{
			tempNextNumDeer = tempNextNumDeer - (NowNumNomad *  ONE_NOMAD_DEER_EATS_PER_MONTH);
		}
		
		

		if (tempNextNumDeer < 0)
		{
			tempNextNumDeer = 0;
		}

		WaitBarrier();

		NowNumDeer = tempNextNumDeer;

		WaitBarrier();
		WaitBarrier();
	}
}

void Nomad()
{
	while (NowYear < 2026)
	{

		int tempNextNumNomad;

		if (((double)NowNumNomad * 2) < NowHeight && (NowNumNomad * 2) < NowNumDeer)
		{
			tempNextNumNomad = NowNumNomad + 1;
		}
		else if ((((double)NowNumNomad * 2) > NowHeight || (NowNumNomad * 2) > NowNumDeer))// && NowNumNomad != 0)
		{
			//tempNextNumNomad = NowNumNomad - 1;
			tempNextNumNomad = 0;
		}
		else
		{
			tempNextNumNomad = NowNumNomad;
		}

		WaitBarrier();

		NowNumNomad = tempNextNumNomad;

		WaitBarrier();
		WaitBarrier();
	}
}

void Grain()
{
	while (NowYear < 2026)
	{
		float nextHeight = NowHeight;
		float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
		float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));

		nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
		nextHeight -= (float)NowNumNomad * ONE_NOMAD_GRAIN_EATS_PER_MONTH;

		if (NowMonth % 2 == 1)
		{
			nextHeight -= (float)NowNumNomad * ONE_NOMAD_GRAIN_EATS_PER_MONTH;
		}


		if (nextHeight < 0)
		{
			nextHeight = 0;
		}

		WaitBarrier();

		NowHeight = nextHeight;

		WaitBarrier();
		WaitBarrier();
	}
}

void Watcher()
{

	TimeOfDaySeed();
	int runningCount = 1;

	while (NowYear < 2026)
	{
		WaitBarrier();
		WaitBarrier();

		NowMonth++;

		printf("%d\t%lf\t%lf\t%lf\t%d\t%d\t\n", runningCount, ((5./9.)*(NowTemp-32)), NowPrecip, (NowHeight*2.54), NowNumDeer, NowNumNomad);

		runningCount++;

		float ang = (30.*(float)NowMonth + 15.) * (M_PI / 180.);

		float temp = AVG_TEMP - AMP_TEMP * cos(ang);
		NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
		NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
		if (NowPrecip < 0.)
			NowPrecip = 0.;


		if (NowMonth == 12)
		{
			NowMonth = 0;
			NowYear++;
		}

		WaitBarrier();
	}
}



float
SQR(float x)
{
	return x * x;
}

void
InitBarrier(int n)
{
	NumInThreadTeam = n;
	NumAtBarrier = 0;
	omp_init_lock(&Lock);
}

void
WaitBarrier()
{
	omp_set_lock(&Lock);
	{
		NumAtBarrier++;
		if (NumAtBarrier == NumInThreadTeam)
		{
			NumGone = 0;
			NumAtBarrier = 0;
			// let all other threads get back to what they were doing
			// before this one unlocks, knowing that they might immediately
			// call WaitBarrier( ) again:
			while (NumGone != NumInThreadTeam - 1);
			omp_unset_lock(&Lock);
			return;
		}
	}
	omp_unset_lock(&Lock);

	while (NumAtBarrier != 0);	// this waits for the nth thread to arrive

#pragma omp atomic
	NumGone++;			// this flags how many threads have returned
}


float
Ranf(float low, float high)
{
	float r = (float)rand();               // 0 - RAND_MAX
	float t = r / (float)RAND_MAX;       // 0. - 1.

	return   low + t * (high - low);
}

int
Ranf(int ilow, int ihigh)
{
	float low = (float)ilow;
	float high = ceil((float)ihigh);

	return (int)Ranf(low, high);
}

void
TimeOfDaySeed()
{
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time(&timer);
	double seconds = difftime(timer, mktime(&y2k));
	unsigned int seed = (unsigned int)(1000.*seconds);    // milliseconds
	srand(seed);
}