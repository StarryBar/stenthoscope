#include<stdio.h>
#include<math.h>
int WD=10;//////1

typedef struct TYPE
{
	float wd[10];//////2
} TYPE;

float distance(TYPE a,TYPE b)
{
	float d=0;
	for (int i=0;i<WD;i++)
	{
		d+=(a.wd[i]-b.wd[i])*(a.wd[i]-b.wd[i]);
	}
	d=sqrt(d);
	return d;
}

int MIN(float D[],int n)
{
	int i;
	int p=0;
	float fmin=D[0];
	for(i=1;i<n;i++)
	{
		if(D[i]<fmin) { fmin=D[i]; p=i; }
	}
	return p;
}

int MAX(TYPE c)
{
	float max=c.wd[0];
	int p=0;
	for(int i=1;i<WD;i++)
	{
		if(max < c.wd[i]) {max=c.wd[i];p=i;}
	}
	return p;
}

void main()
{
	int K=3,Nl=1,S=200,C=800,L=1,I=6; //预设参数，
		int N=10;//样本数目/////////////////////////////////////////////////////////////////////////////3

	TYPE X[10]={{4550,1550,1290,950,820,510,400,370,330,220},
				{4360,1540,1300,1070,970,500,380,330,320,200},
				{5470,2400,1310,440,390,290,290,160,130,120},
				{4290,1800,910,740,520,410,370,260,240,190},
				{4470,1490,1270,910,810,660,410,390,370,290},
				{4520,1700,1250,890,780,430,400,290,280,210},
				{4360,1750,860,710,530,460,330,280,230,200},
				{5400,2350,1180,460,370,330,340,190,150,130},
				{4310,1810,840,700,480,470,350,310,190,180},
				{4740,1330,1330,1020,880,470,320,340,340,190}}; //模式样本!!1151313531!!!!!!!!/////////////////4
	TYPE *center= new TYPE[K]; //聚类中心
	TYPE *sd= new TYPE[K]; //standard deviation标准差向量
	float *sdm= new float[K]; //标准差向量中的最大分量
	int *flag= new int[N]; //每个样本所属类别
	int *count= new int[K]; //各类别样本数
	float *D= new float[K]; //样本到每个聚类中心的距离
	float *Da= new float[K]; //各聚类域中诸样本与聚类中心间的平均距离
	float Dl; //全部模式样本对其相应聚类中心的总平均距离
	float *Dc= new float[K]; //全部聚类中心的距离
	float a=0.5;
	int t,t1,t2;
	int i,j,jj;
	int n=0; //迭代次数
	short TAG_tmp=0;

	int Nc=1;
	center[0]=X[0];
	for(i=1;i<K;i++)
	{
		for(j=0;j<WD;j++)
		center[i].wd[j]=-1;
	}

	while(1)
	{
		n++;
		//按近邻原则聚类
		for(i=0;i<N;i++)
		{
			for(j=0;j<Nc;j++)
				D[j]=distance(X[i],center[j]);
			flag[i]=MIN(D,Nc);
		}

		//赋初值
		for(j=0;j<Nc;j++)
		{
			count[j]=0;
			for(jj=0;jj<WD;jj++)
				center[j].wd[jj]=0;
			Da[j]=0;
			Dl=0;
		}
		//修正聚类中心
		for(i=0;i<N;i++)
		{//h5
			j=flag[i];//样本i离中心j最近
			for(jj=0;jj<WD;jj++)
				center[j].wd[jj] +=X[i].wd[jj];
			count[j]++;
		}
		for(j=0;j<Nc;j++)
		{
			for(jj=0;jj<WD;jj++)
				center[j].wd[jj] /=count[j];//离中心j最近的求平均值，离j+1最近的求平均值!!!!!!

		}
		//计算各聚类域中诸样本与聚类中心间的平均距离
		//和全部模式样本对其相应聚类中心的总平均距离
		for(i=0;i<N;i++)
		{
			j=flag[i];
			Da[j]+=distance(X[i],center[j]);
		}
		for(j=0;j<Nc;j++)
		{
			Dl+=Da[j];
			Da[j]=Da[j]/count[j];
		}
		Dl=Dl/N;
		
		//进行分裂处理
		if(n!=I&&(Nc<=K/2||(Nc>K/2&&((fmod(n,2)!=0)&&Nc<2*K))))//Nc中心个数，K类数，n迭代次数，I总共需要迭代六次
		{
			for(j=0;j<Nc;j++)
			{
				for(jj=0;jj<WD;jj++)
					sd[j].wd[jj] =0;
			}
			for(i=0;i<N;i++)
			{
				j=flag[i];
				for(jj=0;jj<WD;jj++)
					sd[j].wd[jj]+=(X[i].wd[jj]-center[j].wd[jj])*(X[i].wd[jj]-center[j].wd[jj]);//平方距离
			}
			t=Nc;
			for(j=0;j<t;j++)
			{
				for(jj=0;jj<WD;jj++)
					sd[j].wd[jj]=sqrt(sd[j].wd[jj]/count[j]);
				
				int p_tmp=MAX(sd[j]);
				sdm[j]=sd[j].wd[p_tmp];//较大的平方距离若为x
					if(sdm[j]>S&&((Da[j]>Dl&&count[j]>2*(Nl+1))||Nc<=K/2))// Nl
					{
						for(jj=0;jj<WD;jj++)
							center[Nc].wd[jj]=center[j].wd[jj];
						center[Nc].wd[p_tmp]=center[j].wd[p_tmp]-a*sdm[j];
						center[j].wd[p_tmp]=center[j].wd[p_tmp]+a*sdm[j];
						Nc++;
					}
				
			}
			if(Nc!=t) //进行了分裂处理
				continue;
		}

		if(n==I) C=0; //最后一次迭代

		//进行合并处理
		t=0;
		t1=-1;
		t2=-1;
		for(i=1;i<Nc;i++)
		{
			for(j=0;j<i;j++)
			{
				Dc[t]=distance(center[i],center[j]);
				if(t>0&&Dc[t]<C&&Dc[t-1]>Dc[t])
					Dc[t]=Dc[t-1];
				else if(Dc[t]<C) { t1=j; t2=i; }
				t++;
			}
		}
		if(t1!=-1&&t2!=-1)
		{
			for(jj=0;jj<WD;jj++)
			{
				center[t1].wd[jj]=(count[t1]*center[t1].wd[jj]+count[t2]*center[t2].wd[jj])/(count[t1]+count[t2]);
				center[t2].wd[jj]=-1;
			}
			for(int jj_tmp=0;jj_tmp<WD;jj_tmp++)
			{
				if(center[Nc-1].wd[jj]=-1)
					TAG_tmp=1;
			}
			if(!TAG_tmp)//center[Nc-1].x!=-1&&center[Nc-1].y!=-1)
			{
				TAG_tmp=0;
				for(jj=0;jj<WD;jj++)
					center[t2].wd[jj]=center[Nc-1].wd[jj];
			}
			Nc--;
		}

		if(n==I) break;
	}


	//////////////////打印环节~///////////////////
	printf("=======================ISODATA聚类分析法=======================================\n");
	for(int print_tmp=0;print_tmp<K;print_tmp++)
	{
		printf("第%d类:\n",print_tmp+1);
		for(i=0;i<N;i++)
		{
			if(flag[i]==print_tmp)
			{
				printf("X[%d]=(",i);
				for (jj=0;jj<WD;jj++)
				{
					printf("%.0f",X[i].wd[jj]);
					if(jj<WD-1) printf(",");else printf(")\n");
				}
			}
		}
		printf("聚类中心\n (");
		for (jj=0;jj<WD;jj++)
			{
				printf("%.2f",center[print_tmp].wd[jj]);
				if(jj<WD-1) printf(",");else printf(")\n");	//printf("聚类中心 (%f,%f)\n",center[print_tmp].x,center[print_tmp].y);
			}

		printf("\n");
	}
	printf("===============================================================================\n");
	printf("                                                             edit by WANG LIFAN\n");
}




/*

	printf("第二类:\n");
	for(i=0;i<10;i++)
	{
		if(flag[i]==1)
			printf("X[%d]=(%f,%f)\n",i,X[i].x,X[i].y);
	}
	printf("聚类中心 (%f,%f)\n",center[1].x,center[1].y);
	printf("\n");
	printf("第三类:\n");
	for(i=0;i<10;i++)
	{
		if(flag[i]==2)
			printf("X[%d]=(%f,%f)\n",i,X[i].x,X[i].y);
	}
	printf("聚类中心为 (%f,%f)\n",center[2].x,center[2].y);*/
