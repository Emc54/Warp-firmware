/******************************************************************************

Online C Compiler.
Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <math.h>

struct stats
{
	double mean;
	double variance;
	int count;
	double M2;
};

typedef struct stats Struct;


Struct twoPassAlgorithm(double arr[2][100]);
Struct WelfordAlgorithm(Struct statistics, double newValue);

int
main()
{
	static double check[2][100];
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			check[j][i] = 0;
			//printf ("col %d row %d val %f\n", j + 1, i + 1, datapoints[j][i]);
		}
	}

	//check if int to double works

	int rawReading = 634;
	int first_decimal;
	int second_decimal;
	int intReading;
	double test;

	check[1][24] = rawReading;
	printf("\n%f\n", check[1][24]);

	second_decimal = rawReading % 10;	// Retrieves the second decimal (10^-2 value) of the raw reading
	first_decimal = ((rawReading % 100) - second_decimal) / 10;	//Retrieves the first decimal (10^-1 value) of the raw current reading
	intReading = (rawReading - rawReading % 100) / 100;	//Retrieves the floor of the integer current reading 
	test = intReading + first_decimal * 0.1 + second_decimal * 0.01;
	printf("\n%f\n", test);


	//Time to do statistics :( :(

	static double datapoints[2][100] = { 66000,
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
		63300
	};

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("col %d row %d val %f\n", j + 1, i + 1, datapoints[j][i]);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		datapoints[0][i] *= 0.0001;
		printf("new value %f\n", datapoints[0][i]);
	}

	Struct whatever = twoPassAlgorithm(datapoints);
	printf("variance hopefully %f\n", whatever.variance);

	printf("structure before Welford %d, mean %f,variance %f \n",
		whatever.count, whatever.mean, whatever.variance);
	whatever = WelfordAlgorithm(whatever, 6.66);
	printf("structure after Welford %d, mean %f,variance %f \n",
		whatever.count, whatever.mean, whatever.variance);

	//update variance in array

	for (int i = 0; i < 100; i++)
	{
		datapoints[1][i] = whatever.variance;
	}

	return 0;
}

//Time to do statistics :( :(


Struct
twoPassAlgorithm(double arr[2][100])
{
	double sum1;
	double sum2;
	double mean;
	double variance;
	Struct stats;

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
			stats.M2 =
				stats.M2 + (arr[0][i] - stats.mean) * (arr[0][i] - stats.mean);
			printf("weird %f \n", stats.M2);

		}
	}


	return stats;
}




Struct
WelfordAlgorithm(Struct statistics, double new_value)
{
	/*
	* For a new value new_value, compute the new count, new mean, the new M2.
	* mean accumulates the mean of the entire dataset
	* M2 aggregates the squared distance from the mean
	* count aggregates the number of samples seen so far
	*/

	Struct temp_stats = statistics;
	double delta1;
	double delta2;

	temp_stats.count += 1;

	if (temp_stats.count <= 0)
	{
		printf
		("Error with count of values, possible overflow, returning original input");
		return temp_stats;
	}

	delta1 = new_value - temp_stats.mean;

	temp_stats.mean += delta1 / temp_stats.count;

	delta2 = new_value - temp_stats.mean;	// this is using the updated mean

	temp_stats.M2 += delta1 * delta2;

	temp_stats.variance = temp_stats.M2 / (temp_stats.count - 1);	//sample variance with -1, variance with full count

	return temp_stats;
}
