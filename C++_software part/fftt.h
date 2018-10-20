#include "math.h"
#define pi 3.1415926
typedef struct         //�����ʾ�����Ľṹ��
{
   float real;
   float imag;
}COMPX;
COMPX EE(COMPX b1,COMPX b2) //������˺���
{
    COMPX b3;
    b3.real=b1.real*b2.real-b1.imag*b2.imag;
    b3.imag=b1.real*b2.imag+b1.imag*b2.real;
    return(b3);
}

void FFT(COMPX *xin,int N)   //FFT����
{
    int m,LH,nm,k,M,l,I,J,K;
    float p,ps ;
    int B,N1;
    COMPX w,T;
    M=(int)(log(N)/log(2));                     //��ӦNֵΪ2048
	
	int bit[20]={0};
	int* code= new int[N];
	int mytemp=0;
	for (I=0;I<N;I++)
		code[I]=0;

	for(J=0;J<N;J++)
	{
		mytemp=J;
		for(I=0;I<M;I++)
		{
			bit[I]=mytemp&1;
			mytemp=mytemp>>1;
		}
		for(K=0;K<(M/2);K++)
		{
			mytemp=bit[K];
			bit[K]=bit[M-1-K];
			bit[M-1-K]=mytemp;
		}
			

		for(I=0;I<M;I++)
		{
			code[J]+=(int)pow(2,I)*bit[I];
		}
		T=xin[J];
		xin[J]=xin[code[J]];
		xin[code[J]]=T;

	}


 	/*    int m,LH,nm,I,k,J,M,K,l;
    float p,ps ;
    int B,N1;
    COMPX w,T;
    M=(int)(log(N)/log(2));                     //��ӦNֵΪ2048

//�����ǵ���ĳ���    
    LH=N/2;
    J=LH;
    N1=N-2;    
    //��ַ����
    for(I=1;I<=N-2;I++)
    {
        if(I<J)
        {
            T=xin[I];xin[I]=xin[J];xin[J]=T;
         }
        K=LH;
        while(J>=K)
        {
            J=J-K;
            K=K/2;
         }
        J=J+K;
    }
    //////////////////

double t;
for(I=1;I<N-2;I++)
{
	K=I;
	J=0;
	t=M;
	while((t--)>0)
	{
		J=J<<1;
		J|=(K&1);
		K=K>>1;
	}
	if(J>I)
	{
    T=xin[I];xin[I]=xin[J];xin[J]=T;
	}
}
*/	///////////
//������DIT-FFT�������

    for(m=1;m<=M;m++)//��ǰ�����m��
    { 
     B=pow(2,m-1);  //���ھ��롢���ξ��� ������  
      for(J=0;J<=B-1;J++)
       {
          p=pow(2,M-m)*J;//ȷ������ ��ȷ��W_N_K 
            ps=2*pi/N*p;
            w.real=cos(ps);
            w.imag=-sin(ps);
              for(k=J;k<=N-1;k=k+pow(2,m))//������(����*������=������=����*2��������)
             { 
                    T=EE(xin[k+B],w);
                    xin[k+B].real=xin[k].real-T.real;
                    xin[k+B].imag=xin[k].imag-T.imag;
                    xin[k].real=xin[k].real+T.real;
                    xin[k].imag=xin[k].imag+T.imag;
             }
         }
    }
}

//2��IFFT


void IFFT(COMPX *xin,int N)   //IFFT����
{
    int m,LH,nm,k,M,l,I,J,K;
    float p,ps ;
    int B,N1;
    COMPX w,T;
    M=(int)(log(N)/log(2));                     //��ӦNֵΪ2048
	
	int bit[20]={0};
	int* code= new int[N];
	int mytemp=0;
	for(I=0;I<N;I++)
	{
		xin[I].imag=-xin[I].imag;
	}
	for (I=0;I<N;I++)
		code[I]=0;

	for(J=0;J<N;J++)
	{
		mytemp=J;
		for(I=0;I<M;I++)
		{
			bit[I]=mytemp&1;
			mytemp=mytemp>>1;
		}
		for(K=0;K<(M/2);K++)
		{
			mytemp=bit[K];
			bit[K]=bit[M-1-K];
			bit[M-1-K]=mytemp;
		}
			

		for(I=0;I<M;I++)
		{
			code[J]+=(int)pow(2,I)*bit[I];
		}
		T=xin[J];
		xin[J]=xin[code[J]];
		xin[code[J]]=T;

	}
 
    
//������DIT-IFFT�������

    for(m=1;m<=M;m++)
    { 
     B=pow(2,m-1);     
      for(J=0;J<=B-1;J++)
       {
          p=pow(2,M-m)*J;
            ps=2*pi/N*p;
            w.real=cos(ps);
            w.imag=-sin(ps);
              for(k=J;k<=N-1;k=k+pow(2,m))
             { 
                    T=EE(xin[k+B],w);
                    xin[k+B].real=xin[k].real-T.real;
                    xin[k+B].imag=xin[k].imag-T.imag;
                    xin[k].real=xin[k].real+T.real;
                    xin[k].imag=xin[k].imag+T.imag;
             }
         }
    }
for(I=0;I<N;I++)
	{
		xin[I].imag=-xin[I].imag;
	}
for(k=0;k<N;k++)
    {
    xin[k].real=xin[k].real/N;
    xin[k].imag=xin[k].imag/N;
    }
}
