#include <stdio.h>
#include <math.h>
#include "arithmetic_propagation.h"
#include "FFT.h"

typedef struct stats
{
	double mean;
	double variance;
	int count;
	double M2;
} Statistics;


//Struct twoPassAlgorithm(double arr[2][100]);
//Struct WelfordAlgorithm(Struct statistics, double newValue);


Statistics
twoPassAlgorithm (double arr[2][100])
{
	double sum1;
	double sum2;
	Statistics stats;

  for (int i = 0; i < 100; i++)
    {
      sum1 += arr[0][i];
    }
  stats.mean = sum1 / 100;

  for (int i = 0; i < 100; i++)
    {
      sum2 += (arr[0][i] - stats.mean) * (arr[0][i] - stats.mean);
    }

  	stats.variance = sum2 / 99;	// over N for finite population, over N-1 for Bessel correction

  	stats.count = 100;

  	for (int i = 0; i < 100; i++)
    	{
      	if (i < 2)
      		{
	    	stats.M2 = (arr[0][i] - stats.mean) * (arr[0][i] - stats.mean);
    		}

      	else
      		{
	    	stats.M2 = stats.M2 + (arr[0][i] - stats.mean) * (arr[0][i] - stats.mean);    		}
 	   		}

  return stats;

} 


Statistics
WelfordAlgorithm (Statistics data_stats, double new_value)
{
  /* 
   * For a new value new_value, compute the new count, new mean, the new M2.
   * mean accumulates the mean of the entire dataset
   * M2 aggregates the squared distance from the mean
   * count aggregates the number of samples seen so far
   */

  Statistics stats_results = data_stats;
  double delta1;
  double delta2;

  stats_results.count += 1;

  if (stats_results.count <= 0)
    {
      printf
	("Error with count of values, possible overflow, returning original input");
      return stats_results;
    }

  delta1 = new_value - stats_results.mean;

  stats_results.mean += delta1 / stats_results.count;

  delta2 = new_value - stats_results.mean;	// this is using the updated mean

  stats_results.M2 += delta1 * delta2;

  stats_results.variance = stats_results.M2 / (stats_results.count - 1);	//sample variance with -1, variance with full count

  return stats_results;
}




int
main_test()
{

const int n = 128;
const int rem = n-100;
const double PI = atan2(1, 1) * 4;

static double datapoints[2][128] = { 
	66000,
	63600,
	63800,
	66900,
	65700,
	64100,
	61800,
	62100,
	67000,
	65400,
	62500,
	62200,
	66900,
	64400,
	67100,
	62500,
	66300,
	66500,
	65600,
	65600,
	65600,
	66100,
	65700,
	65100,
	60900,
	64400,
	66200,
	64800,
	65000,
	61200,
	62300,
	66200,
	62900,
	63300,
	62300,
	61900,
	65900,
	62800,
	64200,
	65800,
	66100,
	64700,
	60100,
	62300,
	65300,
	65500,
	62700,
	63500,
	65300,
	62800,
	65600,
	59700,
	64600,
	64400,
	66900,
	65700,
	63400,
	63700,
	62900,
	65000,
	65000,
	62300,
	65000,
	65800,
	65900,
	61700,
	65100,
	65400,
	62400,
	59800,
	62100,
	61700,
	65600,
	65500,
	63800,
	60700,
	61500,
	66200,
	58700,
	61500,
	66100,
	64800,
	66800,
	65300,
	62100,
	66100,
	66400,
	61900,
	65200,
	65100,
	65000,
	66300,		
	63900,
	64100,
	65600,
	63800,
	64200,
	64500,
	64900,
	63300,
	63900,
   	64000,
   	63700,
   	65200,
   	61900,
   	62000,
   	62100,
   	63200,
   	62500,
   	62700,
   	63300,
   	59800,
   	62100,
   	61700,
   	65600,
   	60700,
   	61500,
   	66200,
   	58700,
   	61500,
   	66100,
   	64800,
   	66800,
   	65300,
   	62100,
   	66100,
   	66400,
    62100};


// 1) Scale everything to their millivolt values
	for (int i = 0; i < n; i++)
	{
		datapoints[0][i] *= 0.0001;
		printf("new value %f\n", datapoints[0][i]);
	}

// 2) Collect the first 100 Datapoints

double two_pass_points[2][100];

for (int i = 0; i < 100; i++)
	{
		two_pass_points[0][i]=datapoints[0][i];
	}

// 3) Calculate the variance for the first 100 points

	Statistics datapoint_stats = twoPassAlgorithm(two_pass_points);
	printf("variance hopefully %f\n", datapoint_stats.variance);

// 4) Set the value of variance for the first 100 points

	for (int i = 0; i < 100; i++)
	{
		datapoints[1][i] = datapoint_stats.variance;
	}

// 5) For the remainder of points, iterate through their variance

	for (int i = 0; i<rem; i++)
  {  
    datapoint_stats = WelfordAlgorithm (datapoint_stats, datapoints[0][99+i]);
    datapoints[1][100+i] = datapoint_stats.variance;  
  }
  
//Cast inputs into complex structure for FFT
complex_uncertain_value buf[128];

for (int i=0; i<n; i++)
{
    buf[i].value = datapoints[0][i]+0*I;
    buf[i].real_variance = datapoints[1][i];
    buf[i].imag_variance = 0.00;
}

fft(buf, 64);

  for (int i=0; i<n; i++)
  {
  printf ("structure after FFT %f+%fi,real variance %f, imag variance %f \n",
	  creal(buf[i].value),cimag(buf[i].value), buf[i].real_variance, buf[i].imag_variance);
  };

	return 0;
}