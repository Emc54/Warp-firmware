#include <stdlib.h>
#include <math.h>
#include <complex.h>


typedef struct
{
	double value;
	double variance;
} uncertain_value;

typedef struct
{
	complex value;
	double real_variance;
	double imag_variance;
} complex_uncertain_value;

uncertain_value add_prop(uncertain_value data1, uncertain_value data2);
uncertain_value subtract_prop(uncertain_value data1, uncertain_value data2);
uncertain_value mult_prop(uncertain_value data1, uncertain_value data2);
uncertain_value div_prop(uncertain_value data1, uncertain_value data2);
uncertain_value to_power_prop(uncertain_value data1, double power);
uncertain_value exp_prop(uncertain_value data1);
uncertain_value as_power_prop(uncertain_value data1, double base);
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
	double value;
	double mean;
	double variance;
	double std_div;
} uncertain_value;

uncertain_value add_prop(uncertain_value data1, uncertain_value data2, double a, double b);
uncertain_value mult_prop(uncertain_value data1, uncertain_value data2, double a);
uncertain_value div_prop(uncertain_value data1, uncertain_value data2, double a);
uncertain_value to_power_prop(uncertain_value data1, double power, double a);
uncertain_value exp_prop(uncertain_value data1, double a, double b);
uncertain_value as_power_prop(uncertain_value data1, double a, double b);
uncertain_value log_prop(uncertain_value data1, double a, double b);
uncertain_value sin_prop(uncertain_value data1, double a, double b);
uncertain_value cos_prop(uncertain_value data1, double a, double b);

#endif



