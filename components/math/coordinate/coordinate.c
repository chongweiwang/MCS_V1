/**
 * @copyright Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  coordinate transformation  
 * @changelog:
 * date         author          notes
 * 2021.10.17   wangchongwei    first version   
 **/

#include "coordinate.h"


/**
 * @brief:Constant amplitude clark transform,a b c -> alpha beta
 * @details:
 *  |alpha| =       |1    -1/2        -1/2   ||a|
 *            (2/3)*|                        ||b|
 *  | beta| =       |0  sqrt(3)/2  -sqrt(3)/2||c|
 * 
 *  sqrt(3)/2 = 0.866025, 2/3 = 0.666667
 */
void abc_clark_amp(double a, double b,double c, double *alpha, double *beta)
{
    *alpha = (a - 0.5*b - 0.5*c)*0.666667;
    *beta = (b-c)*0.866025*0.666667;
}

/**
 * @brief:Constant power clark transform, a b c ->alpha beta 
 * @details:
 * |alpha| =           |1    -1/2        -1/2   ||a|
 *           sqrt(2/3)*|                        ||b|
 * | beta| =           |0  sqrt(3)/2  -sqrt(3)/2||c|
 * 
 *  sqrt(2/3) = 0.816496, sqrt(3)/2 = 0.866025
 */
void abc_clark_power(double a,double b,double c, double *alpha, double *beta)
{
    *alpha = (a - 0.5*b - 0.5*c)*0.816496;
    *beta = (b-c)*0.866025*0.816496;
}

/**
 * @brief:Constant amplitude clark transform,a b -> alpha beta
 * @details:
 * |alpha| = sqrt(2/3)| 3/2          0   ||a|
 * |beta | = sqrt(2/3)| sqrt(3)/2 sqrt(3)||b|
 * 
 *  1/sqrt(3)= 0.577350
 */
void ab_clark_amp(double a, double b, double *alpha, double *beta)
{
    *alpha = a;
    *beta = (a+2*b)*0.57735;
}

/**
 * @brief:Constant power clark transform,a b -> alpha beta
 * @details:
 *  |alpha| = |sqrt(3/2)     0    ||a|
 *  |beta | = |1/sqrt(2) 2/sqrt(2)||b|
 * 
 *  sqrt(3/2) = 1.224744, 1/sqrt(2) = 0.707106  
 */
void ab_clark_power(double a, double b, double *alpha, double *beta)
{
    *alpha = a*1.224744; 
    *beta = (a+2*b)*0.707106;
}

/**
 * @brief:Constant amplitude inverse clark transform,alpha beta -> a b c
 * @details:
 * |a| =  | 1        0       ||alpha|
 * |b| =  |-1/2   sqrt(3)/2  |
 * |c| =  |-1/2   -sqrt(3)/2 ||beta |
 * 
 *  sqrt(3)/2 = 0.866025
 */
void inv_clark_amp(double alpha, double beta, double *a, double *b,double *c)
{
    *a = alpha;
    *b = -0.5*alpha + 0.866025*beta;
    *c = -0.5*alpha - 0.866025*beta;
}

/**
 * @brief: Constant power inverse clark transform,alpha beta -> a b c
 * @details:
 * |a| =            | 1        0       ||alpha|
 * |b| =  sqrt(2/3)*|-1/2  sqrt(3)/2   |
 * |c| =            |-1/2  -sqrt(3)/2  ||beta |
 * 
 * sqrt(3)/2 = 0.866025,sqrt(2/3)=0.816496 
 */
void inv_clark_power(double alpha, double beta, double *a, double *b,double *c)
{
    *a = alpha*0.816496;
    *b = (-0.5*alpha + 0.866025*beta)*0.816496;
    *c = (-0.5*alpha - 0.866025*beta)*0.816496;
}

/**
 * @brief: park transform,alpha beta -> d q
 * @details:
 * |d| = | cos sin||alpha|
 * |q| = |-sin cos|| beta|
 */
void park(double alpha, double beta,double sin,double cos,double *d, double *q)
{
    *d =  cos*alpha + sin*beta;
    *q = -sin*alpha + cos*beta ;
}

/**
 * @brief: Inverse park transform,dq -> alpha beta
 * @details:
 * |alpha|= |cos  -sin||d|
 * |beta |= |sin   cos||q|
 */
void inv_park(double d, double q,double sin,double cos,double *alpha, double *beta)
{
    *alpha = cos*d - sin*q;
    *beta  = sin*d + cos*q;
}


// sqrt(3)/2 = 0.866025
void abc2dq_amp(double a, double b,double c, double sin,double cos,double *d, double *q)
{ 
    *d = (cos*a + (0.866025*sin - 0.5*cos)*b - (0.5*cos + 0.866025*sin)*c)*0.666667;
    *q = (-sin*a + (0.5*sin + 0.866025*cos)*b + (0.5*sin - 0.866025*cos)*c )*0.666667;
}

// sqrt(2/3) = 0.816496
void abc2dq_power(double a, double b,double c,double sin,double cos, double *d, double *q)
{
    *d = (cos*a + (0.866025*sin - 0.5*cos)*b - (0.5*cos + 0.866025*sin)*c)*0.816496;
    *q = (-sin*a + (0.5*sin + 0.866025*cos)*b + (0.5*sin - 0.866025*cos)*c )*0.816496;
}

void dq2abc_amp(double d, double q,double sin,double cos,double *a, double *b,double *c)
{
    *a = cos*d - sin*q;
    *b = (0.866025*sin - 0.5*cos)*d + (0.5*sin + 0.866025*cos)*q;
    *c = -(0.5*cos + 0.866025*sin)*d + (0.5*sin - 0.866025*cos)*q;
}

void dq2abc_power(double d, double q,double sin,double cos,double *a, double *b,double *c)
{
    *a = (cos*d - sin*q)*0.816496;
    *b = ((0.866025*sin - 0.5*cos)*d + (0.5*sin + 0.866025*cos)*q)*0.816496;
    *c = (-(0.5*cos + 0.866025*sin)*d + (0.5*sin - 0.866025*cos)*q)*0.816496;
}


