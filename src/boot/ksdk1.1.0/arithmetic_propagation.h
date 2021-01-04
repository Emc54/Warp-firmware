#include <stdlib.h>
#include <math.h>
#include <complex.h>

typedef struct sts
{
  float mean;
  float variance;
  int count;
  float M2;
} statistics;

typedef struct uv
{
	float value;
	float variance;
} uncertain_value;

typedef struct cuv
{
	complex value;
	float real_variance;
	float imag_variance;
} complex_uncertain_value;


statistics twoPassAlgorithm(float arr[2][100]);
statistics WelfordAlgorithm(statistics data_stats, float new_value);

uncertain_value add_prop(uncertain_value data1, uncertain_value data2);
uncertain_value subtract_prop(uncertain_value data1, uncertain_value data2);
uncertain_value mult_prop(uncertain_value data1, uncertain_value data2);
uncertain_value div_prop(uncertain_value data1, uncertain_value data2);
uncertain_value to_power_prop(uncertain_value data1, float power);
uncertain_value exp_prop(uncertain_value data1);
uncertain_value as_power_prop(uncertain_value data1, float base);
uncertain_value log_prop(uncertain_value data1);
uncertain_value sin_prop(uncertain_value data1);
uncertain_value cos_prop(uncertain_value data1);

complex_uncertain_value cadd_prop(complex_uncertain_value data1, complex_uncertain_value data2);
complex_uncertain_value csub_prop(complex_uncertain_value data1, complex_uncertain_value data2);
complex_uncertain_value cmult_prop(complex_uncertain_value data1, complex_uncertain_value data2);
complex_uncertain_value cexp_prop(complex_uncertain_value data1);


#ifdef GENERALISED_FUNCTIONS

typedef struct
{
	float value;
	float mean;
	float variance;
	float std_div;
} uncertain_value;

uncertain_value add_prop(uncertain_value data1, uncertain_value data2, float a, float b);
uncertain_value mult_prop(uncertain_value data1, uncertain_value data2, float a);
uncertain_value div_prop(uncertain_value data1, uncertain_value data2, float a);
uncertain_value to_power_prop(uncertain_value data1, float power, float a);
uncertain_value exp_prop(uncertain_value data1, float a, float b);
uncertain_value as_power_prop(uncertain_value data1, float a, float b);
uncertain_value log_prop(uncertain_value data1, float a, float b);
uncertain_value sin_prop(uncertain_value data1, float a, float b);
uncertain_value cos_prop(uncertain_value data1, float a, float b);

#endif



