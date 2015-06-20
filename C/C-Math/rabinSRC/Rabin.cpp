
#include<stdio.h>
#include<string.h>
#include <stdlib.h>   
#include <time.h>  
#include <math.h>
#include <malloc.h>

#define MAX 100
#define LEN sizeof(struct slink)

void sub(int a[MAX],int b[MAX] ,int c[MAX] );

//�ṹ������slink,����ǰ:ÿ�����(����)�������һ���ַ���Ӧ��ASCII�������ֵ,�Ա���������ַ�.
//����ʱ,ÿ�����(����)�������һ���ַ����ܺ�������ʽ������,�Ա���������ַ�.
struct slink
{			
	int  bignum[MAX];/*bignum[98]������������ţ�1����0��;bignum[99]�����ʵ�ʳ���*/
	struct slink *next;
};
void  print(  int a[MAX] )//�������a[]
{
	int i;
	for(i=0;i<a[99];i++)
		printf("%d",a[a[99]-i-1]);
	printf("\n\n");
	return;
}
int  cmp(int a1[MAX],int a2[MAX])//�Ƚ�����a1,a2�Ĵ�С,����ֵ��0,1,-1
{  
	int l1, l2,i;
	l1=a1[99];
	l2=a2[99];
	if (l1>l2)
		return 1;
	if (l1<l2)
		return -1;
	for(i=(l1-1);i>=0;i--)
	{
		if (a1[i]>a2[i])
			return 1 ;
		if (a1[i]<a2[i])
			return -1;
	}
	return 0;
}
void mov(int a[MAX],int *b)//������a��������b
{
	int j;
	for(j=0;j<MAX;j++)
		b[j]=a[j];

	return ;
}
void mul(int a1[MAX],int a2[MAX],int *c)//����c =a*b
{
	int i,j;
	int y;
	int x;
	int z;
	int w;
	int l1, l2;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if (a1[MAX-2]=='-'&& a2[MAX-2]=='-')
		c[MAX-2]=0;
	else if (a1[MAX-2]=='-')
		c[MAX-2]='-';
	else if (a2[MAX-2]=='-')
		c[MAX-2]='-';
	for(i=0;i<l1;i++)
	{
		for(j=0;j<l2;j++)
		{
			x=a1[i]*a2[j];
			y=x/10;
			z=x%10;
			w=i+j;
			c[w]=c[w]+z;
			c[w+1]=c[w+1]+y+c[w]/10;
			c[w]=c[w]%10;
		}
	}
	w=l1+l2;
	if(c[w-1]==0)w=w-1;
	c[MAX-1]=w;
	return;
} 

void add(int a1[MAX],int a2[MAX],int *c)//c=a+b
{

	int i,l1,l2;
	int len,temp[MAX];
	int k=0;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if((a1[MAX-2]=='-')&&(a2[MAX-2]=='-'))
	{
		c[MAX-2]='-';
	}
	else if (a1[MAX-2]=='-')
	{
		mov(a1,temp);
		temp[MAX-2]=0;
		sub(a2,temp,c);
		return;
	}
	else if (a2[MAX-2]=='-')
	{
		mov(a2,temp);
		temp[98]=0;
		sub(a1,temp,c);
		return;
	}
	if(l1<l2)len=l1;
	else len=l2;
	for(i=0;i<len;i++)
	{
		c[i]=(a1[i]+a2[i]+k)%10;
		k=(a1[i]+a2[i]+k)/10;
	}
	if(l1>len)
	{
		for(i=len;i<l1;i++)
		{
			c[i]=(a1[i]+k)%10;
			k=(a1[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l1]=k;
			len=l1+1;
		}
		else len=l1;
	}
	else
	{
		for(i=len;i<l2;i++)
		{
			c[i]=(a2[i]+k)%10;
			k=(a2[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l2]=k;
			len=l2+1;
		}
		else len=l2;
	}
	c[99]=len;
	return;
} 

void sub(int a1[MAX],int a2[MAX],int *c)//c=a-b
{
	int i,l1,l2;
	int len,t1[MAX],t2[MAX];
	int k=0;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if ((a1[MAX-2]=='-') && (a2[MAX-2]=='-'))
	{
		mov(a1,t1);
		mov(a2,t2);
		t1[MAX-2]=0;
		t2[MAX-2]=0;
		sub(t2,t1,c);
		return;
	}
	else if( a2[MAX-2]=='-')
	{
		mov(a2,t2);
		t2[MAX-2]=0;
		add(a1,t2,c);
		return;
	}
	else if (a1[MAX-2]=='-')
	{
		mov(a2,t2);
		t2[MAX-2]='-';
		add(a1,t2,c);
		return;
	}

	if(cmp(a1,a2)==1)
	{

		len=l2;
		for(i=0;i<len;i++)
		{
			if ((a1[i]-k-a2[i])<0)
			{
				c[i]=(a1[i]-a2[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-a2[i]-k)%10;
				k=0;
			}
		}

		for(i=len;i<l1;i++)
		{
			if ((a1[i]-k)<0)
			{
				c[i]=(a1[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-k)%10;
				k=0;
			} 
		}
		if(c[l1-1]==0)/*ʹ������C�е�ǰ������0�ַ�����ʾ�ˣ���1000-20=0980--->��ʾΪ980��*/
		{
			len=l1-1;
			i=2;
			while (c[l1-i]==0)/*111456-111450=00006������0������6��*/
			{
				len=l1-i;
				i++;
			}
		}		
		else 
		{
			len=l1;
		}
	}
	else
		if(cmp(a1,a2)==(-1))
		{
			c[MAX-2]='-';
			len=l1;
			for(i=0;i<len;i++)
			{
				if ((a2[i]-k-a1[i])<0)
				{
					c[i]=(a2[i]-a1[i]-k+10)%10;
					k=1;
				}
				else 
				{
					c[i]=(a2[i]-a1[i]-k)%10;
					k=0;
				}
			}
			for(i=len;i<l2;i++)
			{
				if ((a2[i]-k)<0)
				{
					c[i]=(a2[i]-k+10)%10;
					k=1;
				}
				else 
				{
					c[i]=(a2[i]-k)%10;
					k=0;
				}   
			}
			if(c[l2-1]==0)
			{  
				len=l2-1;
				i=2;
				while (c[l1-i]==0)
				{
					len=l1-i;
					i++;
				}				
			}

			else len=l2;			
		}		
		else if(cmp(a1,a2)==0)
		{
			len=1;
			c[len-1]=0;
		}
	c[MAX-1]=len;
	return;
}

void  divt(int t[MAX],int b[MAX],int  *c ,int *w)/*//���̷�//�����Ժ�wΪa mod b, CΪa  div b;*/
{

	int a1,b1,i,j,m;/*w������ʱ��������*/
	int d[MAX],e[MAX],f[MAX],g[MAX],a[MAX];

	mov(t,a);
	for(i=0;i<MAX;i++)
		e[i]=0;
	for(i=0;i<MAX;i++)
		d[i]=0;
	for(i=0;i<MAX;i++) g[i]=0;
	a1=a[MAX-1];
	b1=b[MAX-1];
	if (cmp(a,b)==(-1))
	{
		c[0]=0;
		c[MAX-1]=1;
		mov(t,w);
		return;
	}
	else if (cmp(a,b)==0)
	{
		c[0]=1;
		c[MAX-1]=1;
		w[0]=0;
		w[MAX-1]=1;
		return;
	}
	m=(a1-b1);
	for(i=m;i>=0;i--)/*341245/3=341245-300000*1--->41245-30000*1--->11245-3000*3--->2245-300*7--->145-30*4=25--->25-3*8=1*/
	{
		for(j=0;j<MAX;j++)
			d[j]=0;
		d[i]=1;
		d[MAX-1]=i+1;
		mov(b,g);
		mul(g,d,e);


		while (cmp(a,e)!=(-1))
		{
			c[i]++;
			sub(a,e,f);
			mov(f,a);/*f���Ƹ�g*/
		}

		for(j=i;j<MAX;j++)/*��λ����*/
			e[j]=0;		
	}
	mov(a,w);
	if (c[m]==0) c[MAX-1]=m;
	else c[MAX-1]=m+1;

	return;
}

void mulmod(int a[MAX] ,int b[MAX] ,int n[MAX],int *m)//��� �� m=a*b mod n;/
{
	int c[MAX],d[MAX];
	int i;
	for(i=0;i<MAX;i++)
		d[i]=c[i]=0;
	mul(a,b,c);
	divt(c,n, d,m);
}

//ʵ���� m=a^p  mod n�ĺ������⡣*/

void expmod(int a[MAX] ,int p[MAX] ,int n[MAX],int *m)
{
	int t[MAX],l[MAX],temp[MAX]; /*/t����2��l����1��*/
	int w[MAX],s[MAX],c[MAX],b[MAX],i;
	for(i=0;i<MAX-1;i++)
		b[i]=l[i]=t[i]=w[i]=0;
	t[0]=2;t[MAX-1]=1;
	l[0]=1;l[MAX-1]=1;
	mov(l,temp);
	mov(a,m);
	mov(p,b);

	while(cmp(b,l)!=0)
	{
		for(i=0;i<MAX;i++)
			w[i]=c[i]=0;

		divt(b,t,w,c);/*// c=p mod 2  w= p /2*/		
		mov(w,b);/*//p=p/2*/		
		if(cmp(c,l)==0) /*/����c==1*/
		{
			for(i=0;i<MAX;i++)
				w[i]=0;

			mul(temp,m,w);			
			mov(w,temp);			
			for(i=0;i<MAX;i++)
				w[i]=c[i]=0;

			divt(temp,n,w,c);/* /cΪ��c=temp % n��wΪ��w=temp/n */			
			mov(c,temp);
		}

		for(i=0;i<MAX;i++)
			s[i]=0;		
		mul(m,m,s);//s=a*a		
		for(i=0;i<MAX;i++)
			c[i]=0;		
		divt(s,n,w,c);/*/w=s/n;c=s mod n*/		
		mov (c,m);		
	}

	for(i=0;i<MAX;i++)
		s[i]=0;	
	mul(m,temp,s);

	for(i=0;i<MAX;i++)
		c[i]=0;	
	divt(s,n,w,c);	
	mov (c,m);/*����c��m*/

	m[MAX-2]=a[MAX-2];/*Ϊ����ĺ�����ʾ��Ҫ���õ�99λ��Ϊ���*/

	return;	
}
int is_prime_san(int p[MAX] )//�ж�p�Ƿ�Ϊ����,����1ʱΪ����
{	
	int i,a[MAX],t[MAX],s[MAX],o[MAX]; 
	for(i=0;i<MAX;i++)
		s[i]=o[i]=a[i]=t[i]=0;
	t[0]=1;
	t[MAX-1]=1;
	a[0]=2;// { 2,3,5,7 }
	a[MAX-1]=1;

	sub(p,t,s);

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{
		return 0;
	}

	a[0]=3;
	for(i=0;i<MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 	   
	{
		return 0;
	}

	a[0]=5;
	for(i=0;i<MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{	   
		return 0;
	}

	a[0]=7;
	for(i=0;i<MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);

	if ( cmp(o,t) != 0 ) 
	{

		return 0;
	}

	return 1;
}

int re3(int *p)//�ж�p�Ƿ�����p=3mod4
{
	int three[MAX],four[MAX],zero[MAX],temp[MAX],s[MAX],re[MAX];
	int i;
	for(i=0;i<MAX;i++)
		three[i]=four[i]=zero[i]=temp[i]=s[i]=re[i]=0;
	three[0]=3;three[MAX-1]=1;
	four[0]=4;four[MAX-1]=1;
	zero[0]=0;zero[MAX-1]=1;
	sub(p,three,temp);
	divt(temp,four,s,re);
	if( cmp(re,zero)!=0)
		return 1;
	else 
		return 0;
}

void prime_random(int *p,int *q)//�����������p,q;��p=q=3mod4
{
	int i,k;
	time_t t; 
	p[0]=1;
	q[0]=3;

	p[MAX-1]=5;//������Զ�̬���Ʋ���������λ��.
	q[MAX-1]=6;

	do
	{
		do
		{
			t=time(NULL);
			srand((unsigned long)t);
			for(i=1;i<p[MAX-1]-1;i++)
			{
				k=rand()%10;
				p[i]=k;
			}
			k=rand()%10;
			while (k==0)
			{
				k=rand()%10;
			}
			p[p[MAX-1]-1]=k;

		}while((is_prime_san(p))!=1);
	}while( re3(p)!=0 );//ʹp=3mod4

	printf("���� p Ϊ  : ");
	for(i=0;i<p[MAX-1];i++)
	{
		printf("%d",p[p[MAX-1]-i-1]);
	}
	printf("\n\n");

	do
	{
		do
		{
			t=time(NULL);
			srand((unsigned long)t);
			for(i=1;i<q[MAX-1];i++)
			{
				k=rand()%10;
				q[i]=k;
			}

		}while((is_prime_san(q))!=1);
	}while( re3(q)!=0 );//ʹq=3mod4

	printf("���� q Ϊ : ");
	for(i=0;i<q[MAX-1];i++)
	{
		printf("%d",q[q[MAX-1]-i-1]);
	}

	printf("\n\n");
	return;
}	

void rabinab(int *p,int *q,int *b)//�����㷨Ч������,��p,q��λ����8,9λ������ʱ,a,b�ļ���ʱ���൱��.����Ҫ30����~
{
	int i;
	int   e[MAX],zero[MAX],qq[MAX],temp[MAX],re[MAX],bb[MAX];

	for(i=0;i<MAX;i++)
		e[i]=zero[i]=qq[i]=temp[i]=re[i]=bb[i]=0;
	e[MAX-1]=1;e[0]=1;//e=1;
	zero[MAX-1]=1;zero[0]=0;//zero=0;

	add(q,e,qq);
	divt(qq,p,bb,re);
	while( cmp(re,zero)!=0 )
	{
		add(qq,q,temp);
		mov(temp,qq);
		//����re����
		for(i=0;i<MAX;i++)
			bb[i]=re[i]=0;
		divt(qq,p,bb,re);//������re=0ʱ,bb*p��Ϊ����.
	}
	mul(bb,p,b);
}

void  tencrypto(int e[MAX], int n[MAX])/*//������Ҫ���ļ����м���*/
{
	FILE *fp;
	int i,k,count,temp,c;
	char filename[25],ch;
	struct slink  *p,*p1,*p2;
	struct slink  *h;
	h=p=p1=p2=(struct slink * )malloc(LEN);
	h=NULL;
	printf("\n������Ҫ���ܵ��ļ�·�� : ");
	scanf("%s",filename);
	if((fp=fopen(filename,"r"))==NULL)
	{
		printf("Cannot open file !\n");
		exit(0);
	}
	printf("\n�ļ���ԭ�����ݣ�\n\n");
	count=0;
	while((ch=fgetc(fp))!=EOF)//��ÿ���ַ�ת��������,�ٰ����������ŵ����������е�һ�������(����).
	{  
		putchar(ch);
		c=ch;
		k=0;
		if(c<0)
		{
			c=abs(c);/*/�Ѹ���ȡ��������һ�����*/
			p1->bignum[MAX-2]='0';//��0���,˵������Ǻ���
		}
		else
		{
			p1->bignum[MAX-2]='1';
		}

		while(c/10!=0)//��c/10���̲�Ϊ0ʱ,��-10<c<10,�����Ͱѵ����ַ���ASCII���������ʽһλһλ�Ĵ�ŵ�������.
		{
			temp=c%10;
			c=c/10;
			p1->bignum[k]=temp;
			k++;
		}
		p1->bignum[k]=c;
		p1->bignum[MAX-1]=k+1;//��¼���ַ�ռ�ݸ��������Чλ
		count=count+1;
		if(count==1)
			h=p1;//ʹhָ���ַ����е�һ���ַ���Ӧ����ĵ�һλ.
		else p2->next=p1;
		p2=p1;
		p1=(struct slink * )malloc(LEN);
	}
	p2->next=NULL; 
	printf("\n");
	fclose(fp);

	fp=fopen(filename,"w");
	p=p1=(struct slink * )malloc(LEN);
	p=h;
	printf("\n���ܺ��ļ������γ����ģ�\n\n");
	if(h!=NULL)
		do //�ȶԵ�һ���ַ�����,Ȼ��ڶ���, ������...
		{ 
			expmod( p->bignum , e ,n ,p1->bignum);//p1->bignumָ����ܺ������(��һ����).
			ch=p1->bignum[MAX-2];
			printf("%c",ch);
			fputc(ch,fp);
			if ((p1->bignum[MAX-1]/10) ==0)//���p1->bignum[99]<10
			{
				ch=0+48;
				printf("%c",ch);//��ӡ����
				fputc(ch,fp);//д���ļ�
				ch=p1->bignum[MAX-1]+48;
				printf("%c",ch);
				fputc(ch,fp);
			}
			else
			{
				ch=p1->bignum[MAX-1]/10+48;
				printf("%c",ch);
				fputc(ch,fp);
				ch=p1->bignum[MAX-1]%10+48;
				printf("%c",ch);
				fputc(ch,fp);
			}

			for(i=0;i<p1->bignum[MAX-1];i++)
			{
				printf("%d",p1->bignum[i]);
				ch=p1->bignum[i]+48;
				fputc(ch,fp);
			}
			p=p->next;
			p1=(struct slink * )malloc(LEN);
		}while(p!=NULL);
	printf("\n\n");
	fclose(fp);
	return;
}

void rabinM( int c[MAX], int p[MAX], int q[MAX], int n[MAX], int a[MAX], int b[MAX], int M1[MAX], int M2[MAX], int M3[MAX], int M4[MAX])
{
	//�����m1,m2,m3,m4
	int  pp[MAX],qq[MAX],s1[MAX],s2[MAX],m1[MAX],m2[MAX],m3[MAX],m4[MAX],re1[MAX],re2[MAX];
	int e[MAX],four[MAX];
	int i;
	for( i=0;i<MAX;i++)
		pp[i]=qq[i]=s1[i]=s2[i]=m1[i]=m2[i]=m3[i]=m4[i]=e[i]=four[i]=re1[i]=re2[i]=0;/*/�򵥳�ʼ��һ��*/
	e[MAX-1]=1;e[0]=1;
	four[MAX-1]=1;four[0]=4;

	add(p,e,pp);add(q,e,qq);
	divt(pp,four,s1,re1);divt(qq,four,s2,re2);

	expmod(c,s1,p,m1);	//���m1
	sub(p,m1,m2);		//m2
	expmod(c,s2,q,m3);	//m3
	sub(q,m3,m4);		//m4

	//Ȼ�����M1,M2,M3,M4
	int t1[MAX],t2[MAX],t3[MAX],t4[MAX],am1[MAX],am2[MAX],bm3[MAX],bm4[MAX];

	for( i=0;i<MAX;i++)
		M1[i]=M2[i]=M3[i]=M4[i]=t1[i]=t2[i]=t3[i]=t4[i]=am1[i]=am2[i]=bm3[i]=bm4[i]=0;/*/�򵥳�ʼ��һ��*/

	mul(a,m1,am1);mul(b,m3,bm3);mul(b,m4,bm4);mul(a,m2,am2);
	add(am1,bm3,t1);add(am1,bm4,t2);add(am2,bm3,t3);add(am2,bm4,t4);
	mulmod(t1,e,n,M1); M1[MAX-2]=c[MAX-2];//���M1
	mulmod(t2,e,n,M2); M2[MAX-2]=c[MAX-2];//M2
	mulmod(t3,e,n,M3); M3[MAX-2]=c[MAX-2];//M3
	mulmod(t4,e,n,M4); M4[MAX-2]=c[MAX-2];//M4

}

void  findM(int p[MAX], int q[MAX], int n[MAX], int a[MAX], int b[MAX])
{
	int  M1[MAX],M2[MAX],M3[MAX],M4[MAX];

	FILE *fp;   
	struct slink *h,*p1,*p2;
	char ch,encryfile[25];
	int i,j,k,c,count,temp;
	printf("\n������ܹ����ļ�·�� : ");
	scanf("%s",encryfile);
	if((fp=fopen(encryfile,"r"))==NULL)
	{
		printf("���ļ�������!\n");
		exit(0);
	}
	printf("\n�ļ����������ݣ�\n\n");
	i=0;
	j=3;
	count=0;
	h=p1=p2=(struct slink * )malloc(LEN);

	while((ch=fgetc(fp))!=EOF)
	{  
		putchar(ch);
		c=ch;	   
		if(j==3)
		{
			p1->bignum[MAX-2]=c;
			j--;
		}
		else if(j==2)
		{
			temp=c-48;
			j--;
		}
		else if(j==1)
		{
			p1->bignum[MAX-1]=temp*10+c-48;//ǰ������,��Ϊ�˶�ȡ��ԭ�����ĵĵ�һ���ַ�������ռ����λ
			j--;
		}
		else if (j==0)
		{
			p1->bignum[i]=c-48;//����������ʼ��ȡ���ĵĵ�һ���ַ�,�����ѭ����ȡ,ֱ��i==p1->bignum[MAX-1]����.
			i++;
			if(i==p1->bignum[MAX-1])//��i=���ַ������еĳ���ʱ,��ԭ,��ȥ��ȡ��һ���ַ�
			{ 
				i=0;
				j=3;
				count++;
				if (count==1)
					h=p1;
				else p2->next=p1;
				p2=p1;
				p1=(struct slink * )malloc(LEN);
			}
		}
	}
	p2->next=NULL; 
	printf("\n");
	fclose(fp);	

	fp=fopen(encryfile,"w");
	printf("\n�������ĺ��ļ��е�����:\n\n");
	p2=(struct slink * )malloc(LEN);
	p1=h;//ʹp1ָ�����ĵ���������,�Ա�һ��һ���ַ������Ľ���
	k=0;
	if(h!=NULL)//tempΪ�ݴ�ASIIC���intֵ/
		do
		{
			for(i=0;i<MAX;i++)
				p2->bignum[i]=0;					  
			rabinM(p1->bignum,p,q,n,a,b,M1,M2,M3,M4);//

			for(int z=1;z<5;z++)
			{
				for(i=0;i<MAX;i++)
					p2->bignum[i]=0;
				if(z==1) mov(M1,p2->bignum);//
				else if(z==2) mov(M2,p2->bignum);
				else if(z==3) mov(M3,p2->bignum);
				else mov(M4,p2->bignum);

				temp=p2->bignum[0]+p2->bignum[1]*10+p2->bignum[2]*100;
				if (( p2->bignum[MAX-2])=='0')//����,˵������ַ���Ӧ���Ǻ���.
				{
					temp=0-temp;
				}//ת��Ϊ��ȷ��ASIIC�룬��-78-96�γɺ���	/	 
				ch=temp;//  str[k]--->ch 
				printf("%3c ",ch);//  str[k]--->ch ///���������ʱ,����"��"��ASCII����: -57-46
				fputc(ch,fp);//д���ļ�str[k]--->ch///��һ�����-57ϵͳ��ʶ��,���Բ���ʾ;Ҫ�ȵ�-46Ҳ���ʱ,�Ż�ϲ���ʾ��"��".
				//Ȼ��,ʵ��������Rabin�㷨�����4������,��û�������ж��ĸ�����ȷ��ʱ,
				//-57��-46���Ѹպ�������һ�����,Ҳ�͵ò�������'��'			   
			} 
			printf("\n");
			k++;
			p1=p1->next;
			p2=(struct slink * )malloc(LEN);
		}while (p1!=NULL);
	printf("\n\n");
	fclose(fp);
	return;
}


int main()
{
	int  i;
	int  p[MAX],q[MAX],n[MAX],e[MAX],m[MAX];
	int a[MAX],b[MAX],two[MAX];
	for(i=0;i<MAX;i++)
		m[i]=p[i]=q[i]=n[i]=e[i]=a[i]=b[i]=two[i]=0;/*/�򵥳�ʼ��һ��*/
	two[MAX-1]=1;two[0]=2;

	printf("\n\n�����Կ�Բ������£�  ( p,q����: p=q=3mod4 )\n\n");
	//	printf("\n����p,q����:p=q=3mod4 ");
	prime_random(p,q);//�����������������//��p=q=3mod4
	mul(p,q,n);	//����n=p*q
	printf("�� p��q �ó� n :");
	print(n);//ͨ������print(n),���n

	rabinab(q,p,a);//���a;//a��q��q����Ԫ֮��
	printf("        �ó� a (q��q����Ԫ֮��) : ");	
	print(a); 
	rabinab(p,q,b);//���b;//b��p��p����Ԫ֮��
	printf("        �ó� b (p��p����Ԫ֮��) : ");
	print(b);

	//���ļ�����
	printf("��Կ�Բ�����ɣ����ڿ���ֱ�ӽ��мӽ����ļ�!\n");
	mov(two,e);//��RSA�еļ���ָ����ΪRabin�е�2.
	tencrypto( e,  n); //������Ҫ���ļ����м���
	printf("\n�����ļ��������!\n");

	//�Լ��ܹ����ļ�����
	findM(p,q,n,a,b);
	printf("\n�����ļ��������!\n");	   

	return 0;
}
