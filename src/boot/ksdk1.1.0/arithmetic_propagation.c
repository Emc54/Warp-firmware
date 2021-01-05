#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "arithmetic_propagation.h"

//#define GENERALISED_FUNCTIONS

//Minimal Cases (Do not require covariance or constants a and b from the Bevington and Robinson Book)

// typedef struct
// {
//   double value;
//   double variance;
// } uncertain_value;

// typedef struct
// {
// 	complex value;
// 	double real_variance;
// 	double imag_variance;
// } complex_uncertain_value;

//Deal with divisions by 0, they break uncertainty calculations and give NAN

uncertain_value
add_prop (uncertain_value data1, uncertain_value data2)
{
  uncertain_value result;
  result.value = data1.value + data2.value;
  result.variance = data1.variance + data2.variance;
  return result;
}

uncertain_value
subtract_prop (uncertain_value data1, uncertain_value data2)
{
    data2.value = -data2.value;
    return add_prop(data1,data2);
}

uncertain_value
mult_prop (uncertain_value data1, uncertain_value data2)
{
    uncertain_value result;
    result.value = data1.value * data2.value;
    
    double svar1 = data1.variance;
    double svar2 = data2.variance;
    
    if (data1.value != 0)
    {
     svar1 = data1.variance / (data1.value * data1.value);
    }
    
    if (data2.value != 0)
    {
        svar2 = data2.variance / (data2.value * data2.value);
    }
    
    // Relative Variance
    result.variance = svar1 + svar2;
    //Absolute variance
    result.variance = result.variance * result.value * result.value;
    
    return result;
}

uncertain_value
div_prop (uncertain_value data1, uncertain_value data2)
{
    uncertain_value result;
    result.value = data1.value / data2.value;
    
    double svar1 = data1.variance;
    double svar2 = data2.variance;
    
    if (data1.value != 0)
    {
     svar1 = data1.variance / (data1.value * data1.value);
    }
    
    if (data2.value != 0)
    {
        svar2 = data2.variance / (data2.value * data2.value);
    }
    
    // Relative Variance
    result.variance = svar1 + svar2;
    //Absolute variance
    result.variance = result.variance * result.value * result.value;
    
    return result;
}

uncertain_value
to_power_prop (uncertain_value data1, double power)
{
    double std_div;
    uncertain_value result;
    result.value = pow(data1.value,power);
    std_div = sqrt(abs(data1.variance));
    std_div = result.value * power * std_div / data1.value;
    result.variance = std_div*std_div;
    return result;
}

uncertain_value
exp_prop (uncertain_value data1)
{
    uncertain_value result;
    result.value = exp(data1.value);
    result.variance = result.value*result.value*data1.variance;
    return result;
}

uncertain_value
as_power_prop (uncertain_value data1, double base)
{
    uncertain_value result;
    result.value = pow(base,data1.value);
    result.variance = result.value * result.value * data1.variance * log(base) * log(base);
    return result;
}    

uncertain_value
log_prop (uncertain_value data1)
{
    uncertain_value result;
    result.value = log(data1.value);
    if (data1.value != 0)
    {
        result.variance = data1.variance / (data1.value * data1.value);
    }
    else
    {
        result.variance = data1.variance;
    }
    return result;
}

uncertain_value
sin_prop (uncertain_value data1)
{
    uncertain_value result;
    result.value = sin(data1.value);
    
    if (cos(data1.value) != 0)
    {
        result.variance = data1.variance * cos(data1.value) * cos(data1.value);
    }
    else
    {
        result.variance = data1.variance;
    }
    return result;
}

uncertain_value
cos_prop (uncertain_value data1)
{
    uncertain_value result;
    result.value = cos(data1.value);
    
    if(sin(data1.value) != 0)
    {
        result.variance = data1.variance * sin(data1.value) * sin(data1.value);
    }
    else
    {
        result.variance = data1.variance;
    }
    return result;
}

// Probably need to implement complex addition, complex multiplication and complex exponentiation. Complex division optional extra

complex_uncertain_value
cadd_prop(complex_uncertain_value data1, complex_uncertain_value data2)
{
    complex_uncertain_value result;
    result.value = data1.value + data2.value;
    result.real_variance = data1.real_variance + data2.real_variance;
    result.imag_variance = data1.imag_variance + data2.imag_variance;
    return result;
}

complex_uncertain_value
csub_prop(complex_uncertain_value data1, complex_uncertain_value data2)
{
    data2.value = -data2.value;
    return cadd_prop(data1,data2);
}

complex_uncertain_value
cmult_prop(complex_uncertain_value data1, complex_uncertain_value data2)
{
    complex_uncertain_value result;
    result.value = data1.value  * data2.value;
    
    //Check for zeroes
    double a = creal(data1.value);
    double b = cimag(data1.value);
    double c = creal(data2.value);
    double d = cimag(data2.value);
    
    //Sum components of variance
    double svar_a = data1.real_variance;
    double svar_b = data1.imag_variance;
    double svar_c = data2.real_variance;
    double svar_d = data2.imag_variance;
    
    // (a+bi)(c+di) = ac - bd for the real part
    // (a+bi)(c+di) = (ad + bc)i for the imag part
    // They turn out to have equal variance var_a/a^2 + var_b/b^2 + var_c/c^2 + var_d/d^2
    if (a != 0){
        svar_a = svar_a / (a * a);
    }    

    if (b != 0){
        svar_b = svar_b / (b * b);
    }    
    
    if (c != 0){
        svar_c = svar_c / (c * c);
    }    
    
    if (d != 0){
        svar_d = svar_d / (d * d);
    }    

    
    result.real_variance = svar_a + svar_b + svar_c + svar_d;
    result.imag_variance = result.real_variance;
    return result;
}

complex_uncertain_value
cexp_prop(complex_uncertain_value data1)
{
	complex_uncertain_value result;
	result.value = cexp(data1.value);
    
    //Variance Calculations: Using exp(a+ib) = cosa + i*sinb
    // solution can take the form of (cosa +- var_a*sin(a)^2) + i(sinb +- var_b*cos(b)^2)
    
    result.real_variance = data1.real_variance * sin(creal(data1.value)) * sin(creal(data1.value));
    result.imag_variance = data1.imag_variance * cos(cimag(data1.value)) * cos(cimag(data1.value));
    return result;
}







#ifdef GENERALISED_FUNCTIONS
// general cases including constants a and b

typedef struct
{
  double value;
  double mean;
  double variance;
  double std_div;
} uncertain_value;

uncertain_value
add_prop (uncertain_value data1, uncertain_value data2, double a, double b)
{
  uncertain_value result;
  result.value = a * data1.value + b * data2.value;
  result.variance = a * a * data1.variance + b * b * data2.variance;
  return result;
}

uncertain_value
subtract_prop (uncertain_value data1, uncertain_value data2)
{
    data2.value = -data2.value;
    return add_prop(data1,data2);
}

uncertain_value
mult_prop (uncertain_value data1, uncertain_value data2, double a)
{
    uncertain_value result;
    result.value = a * data1.value * data2.value;
    // Relative Variance
    result.variance =
    (data1.variance  / (data1.value * data1.value)) +
    (data2.variance / (data2.value * data2.value));
    //Absolute variance
    result.variance = result.variance * result.value * result.value;
    
    return result;
}

uncertain_value
div_prop (uncertain_value data1, uncertain_value data2, double a)
{
    uncertain_value result;
    result.value = a * data1.value / data2.value;
    result.variance =
    (data1.variance  / (data1.value * data1.value)) +
    (data2.variance / (data2.value * data2.value));
    //Absolute variance
    result.variance = result.variance * result.value * result.value;
    return result;
}

uncertain_value
to_power_prop (uncertain_value data1, double power, double a)
{
    uncertain_value result;
    result.value = a * pow(data1.value,power);
    result.std_div = result.value * power * data1.std_div / data1.value;
    result.variance = result.std_div * result.std_div;
    return result;
}

uncertain_value
exp_prop (uncertain_value data1, double a, double b)
{
    uncertain_value result;
    double temp = b * data1.value;
    result.value = a * exp(temp);
    result.std_div = result.value * b * data1.std_div;
    result.variance = result.std_div * result.std_div;
    return result;
}

uncertain_value
as_power_prop (uncertain_value data1, double a, double b)
{
    uncertain_value result;
    double temp = b * data1.value;
    result.value = pow(a, temp);
    result.std_div = result.value * b * data1.std_div * log(a);
    result.variance = result.std_div * result.std_div;
    return result;
}    

uncertain_value
log_prop (uncertain_value data1, double a, double b)
{
    uncertain_value result;
    double temp = b * data1.value;
    result.value = a * log(temp);
    result.std_div = a * b * data1.std_div / data1.value;
    result.variance = data1.std_div * data1.std_div;
    return result;
}

uncertain_value
sin_prop (uncertain_value data1, double a, double b)
{
    uncertain_value result;
    double temp = data1.value * b;
    result.value = a * sin(temp);
    result.std_div = data1.std_div * a * b * cos(temp);
    result.variance = result.std_div * result.std_div;
    return result;
}

uncertain_value
cos_prop (uncertain_value data1, double a, double b)
{
    uncertain_value result;
    double temp = data1.value * b;
    result.value = a * cos(temp);
    result.std_div = - data1.std_div * a * b * sin(temp);
    result.variance = result.std_div * result.std_div;
    return result;
}

#endif