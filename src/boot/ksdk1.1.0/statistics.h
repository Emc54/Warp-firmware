#include <stdio.h>
#include <math.h>


typedef struct stats
{
	double mean;
	double variance;
	int count;
	double M2;
} Statistics;


Statistics twoPassAlgorithm(double arr[2][100]);

Statistics WelfordAlgorithm(Statistics data_stats, double new_value);