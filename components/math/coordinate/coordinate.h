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


void abc_clark_amp(float a, float b,float c, float *alpha, float *beta);
void ab_clark_amp(float a, float b, float *alpha, float *beta);
void inv_clark_amp(float alpha, float beta, float *a, float *b,float *c);
void abc2dq_amp(float a, float b,float c, float sin,float cos,float *d, float *q);
void dq2abc_amp(float d, float q,float sin,float cos,float *a, float *b,float *c);


void park(float alpha, float beta,float sin,float cos,float *d, float *q);
void inv_park(float d, float q,float sin,float cos,float *alpha, float *beta);

void abc_clark_power(float a,float b,float c, float *alpha, float *beta);
void ab_clark_power(float a, float b, float *alpha, float *beta);
void inv_clark_power(float alpha, float beta, float *a, float *b,float *c);
void abc2dq_power(float a, float b,float c,float sin,float cos, float *d, float *q);
void dq2abc_power(float d, float q,float sin,float cos,float *a, float *b,float *c);


#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif // _COORDINATE_H
