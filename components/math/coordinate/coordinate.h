/**
 * @copyright Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  coordinate transformation  
 * @changelog:
 * date         author          notes
 * 2021.10.17   wangchongwei    first version   
 **/

#ifndef _COORDINATE_H_
#define _COORDINATE_H_

#ifdef  __cplusplus
extern "C" {
#endif


void abc_clark_amp(double a, double b,double c, double *alpha, double *beta);
void ab_clark_amp(double a, double b, double *alpha, double *beta);
void inv_clark_amp(double alpha, double beta, double *a, double *b,double *c);
void abc2dq_amp(double a, double b,double c, double sin,double cos,double *d, double *q);
void dq2abc_amp(double d, double q,double sin,double cos,double *a, double *b,double *c);


void park(double alpha, double beta,double sin,double cos,double *d, double *q);
void inv_park(double d, double q,double sin,double cos,double *alpha, double *beta);

void abc_clark_power(double a,double b,double c, double *alpha, double *beta);
void ab_clark_power(double a, double b, double *alpha, double *beta);
void inv_clark_power(double alpha, double beta, double *a, double *b,double *c);
void abc2dq_power(double a, double b,double c,double sin,double cos, double *d, double *q);
void dq2abc_power(double d, double q,double sin,double cos,double *a, double *b,double *c);


#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif // _COORDINATE_H
