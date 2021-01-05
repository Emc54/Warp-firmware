#include <stdio.h>
#include <math.h>
#include "statistics.h"


Statistics 
twoPassAlgorithm(double arr[2][100])
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
	    	stats.M2 = stats.M2 + (arr[0][i] - stats.mean) * (arr[0][i] - stats.mean);    		
      		}
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