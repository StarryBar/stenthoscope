/*************************************************************
 * һ��ʵ���������ֵ�ֽ⣬�μ���c �����㷨���򼯡�������P169
 *************************************************************/
#ifndef _SVD_H
#define _SVD_H
#define MAX_ITERA 60
#define MIN_DOUBLE (1e-30)

#include "stdlib.h"
#include "math.h"

int extern dluav(double a[],int m,int n,double u[],double v[],double eps,int ka);
void extern damul(double a[],double b[],int m,int n,int k,double c[]);
void extern ppp(double a[],double e[],double s[],double v[],int m,int n);
void extern sss(double fg[2],double cs[2]);

#endif