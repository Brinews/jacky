
#include<stdio.h>
#include<string.h>
#include <stdlib.h>   
#include <time.h>  
#include <math.h>
#include <malloc.h>

#define MAX 100
#define LEN sizeof(struct slink)

void sub(int a[MAX],int b[MAX] ,int c[MAX] );

//结构体数组slink,加密前:每个结点(数组)用来存放一个字符对应的ASCII码的整数值,以便逐个加密字符.
//解密时,每个结点(数组)用来存放一个字符加密后整数形式的密文,以便逐个解密字符.
struct slink
{			
	int  bignum[MAX];/*bignum[98]用来标记正负号，1正，0负;bignum[99]来标记实际长度*/
	struct slink *next;
};
void  print(  int a[MAX] )//输出数组a[]
{
	int i;
	for(i=0;i<a[99];i++)
		printf("%d",a[a[99]-i-1]);
	printf("\n\n");
	return;
}
int  cmp(int a1[MAX],int a2[MAX])//比较数组a1,a2的大小,返回值有0,1,-1
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
void mov(int a[MAX],int *b)//把数组a赋给数组b
{
	int j;
	for(j=0;j<MAX;j++)
		b[j]=a[j];

	return ;
}
void mul(int a1[MAX],int a2[MAX],int *c)//返回c =a*b
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
		if(c[l1-1]==0)/*使得数组C中的前面所以0字符不显示了，如1000-20=0980--->显示为980了*/
		{
			len=l1-1;
			i=2;
			while (c[l1-i]==0)/*111456-111450=00006，消除0后变成了6；*/
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

void  divt(int t[MAX],int b[MAX],int  *c ,int *w)/*//试商法//调用以后w为a mod b, C为a  div b;*/
{

	int a1,b1,i,j,m;/*w用于暂时保存数据*/
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
			mov(f,a);/*f复制给g*/
		}

		for(j=i;j<MAX;j++)/*高位清零*/
			e[j]=0;		
	}
	mov(a,w);
	if (c[m]==0) c[MAX-1]=m;
	else c[MAX-1]=m+1;

	return;
}

void mulmod(int a[MAX] ,int b[MAX] ,int n[MAX],int *m)//解决 了 m=a*b mod n;/
{
	int c[MAX],d[MAX];
	int i;
	for(i=0;i<MAX;i++)
		d[i]=c[i]=0;
	mul(a,b,c);
	divt(c,n, d,m);
}

//实现了 m=a^p  mod n的函数问题。*/

void expmod(int a[MAX] ,int p[MAX] ,int n[MAX],int *m)
{
	int t[MAX],l[MAX],temp[MAX]; /*/t放入2，l放入1；*/
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
		if(cmp(c,l)==0) /*/余数c==1*/
		{
			for(i=0;i<MAX;i++)
				w[i]=0;

			mul(temp,m,w);			
			mov(w,temp);			
			for(i=0;i<MAX;i++)
				w[i]=c[i]=0;

			divt(temp,n,w,c);/* /c为余c=temp % n，w为商w=temp/n */			
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
	mov (c,m);/*余数c给m*/

	m[MAX-2]=a[MAX-2];/*为后面的汉字显示需要，用第99位做为标记*/

	return;	
}
int is_prime_san(int p[MAX] )//判断p是否为素数,返回1时为素数
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

int re3(int *p)//判断p是否满足p=3mod4
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

void prime_random(int *p,int *q)//随机生成素数p,q;且p=q=3mod4
{
	int i,k;
	time_t t; 
	p[0]=1;
	q[0]=3;

	p[MAX-1]=5;//这里可以动态控制产生素数的位数.
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
	}while( re3(p)!=0 );//使p=3mod4

	printf("素数 p 为  : ");
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
	}while( re3(q)!=0 );//使q=3mod4

	printf("素数 q 为 : ");
	for(i=0;i<q[MAX-1];i++)
	{
		printf("%d",q[q[MAX-1]-i-1]);
	}

	printf("\n\n");
	return;
}	

void rabinab(int *p,int *q,int *b)//由于算法效率问题,当p,q得位数到8,9位数以上时,a,b的计算时间相当长.可能要30分钟~
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
		//余数re清零
		for(i=0;i<MAX;i++)
			bb[i]=re[i]=0;
		divt(qq,p,bb,re);//当余数re=0时,bb*p即为所求.
	}
	mul(bb,p,b);
}

void  tencrypto(int e[MAX], int n[MAX])/*//对有需要的文件进行加密*/
{
	FILE *fp;
	int i,k,count,temp,c;
	char filename[25],ch;
	struct slink  *p,*p1,*p2;
	struct slink  *h;
	h=p=p1=p2=(struct slink * )malloc(LEN);
	h=NULL;
	printf("\n输入需要加密的文件路径 : ");
	scanf("%s",filename);
	if((fp=fopen(filename,"r"))==NULL)
	{
		printf("Cannot open file !\n");
		exit(0);
	}
	printf("\n文件的原文内容：\n\n");
	count=0;
	while((ch=fgetc(fp))!=EOF)//将每个字符转化成整数,再把这个整数存放到数组链表中的一个结点上(数组).
	{  
		putchar(ch);
		c=ch;
		k=0;
		if(c<0)
		{
			c=abs(c);/*/把负数取正并且做一个标记*/
			p1->bignum[MAX-2]='0';//用0标记,说明这个是汉字
		}
		else
		{
			p1->bignum[MAX-2]='1';
		}

		while(c/10!=0)//当c/10的商不为0时,即-10<c<10,这样就把单个字符的ASCII码的整数形式一位一位的存放到数组中.
		{
			temp=c%10;
			c=c/10;
			p1->bignum[k]=temp;
			k++;
		}
		p1->bignum[k]=c;
		p1->bignum[MAX-1]=k+1;//记录该字符占据该数组的有效位
		count=count+1;
		if(count==1)
			h=p1;//使h指向字符串中第一个字符对应数组的第一位.
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
	printf("\n加密后文件中所形成密文：\n\n");
	if(h!=NULL)
		do //先对第一个字符加密,然后第二个, 第三个...
		{ 
			expmod( p->bignum , e ,n ,p1->bignum);//p1->bignum指向加密后的密文(是一数组).
			ch=p1->bignum[MAX-2];
			printf("%c",ch);
			fputc(ch,fp);
			if ((p1->bignum[MAX-1]/10) ==0)//如果p1->bignum[99]<10
			{
				ch=0+48;
				printf("%c",ch);//打印出来
				fputc(ch,fp);//写入文件
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
	//先求出m1,m2,m3,m4
	int  pp[MAX],qq[MAX],s1[MAX],s2[MAX],m1[MAX],m2[MAX],m3[MAX],m4[MAX],re1[MAX],re2[MAX];
	int e[MAX],four[MAX];
	int i;
	for( i=0;i<MAX;i++)
		pp[i]=qq[i]=s1[i]=s2[i]=m1[i]=m2[i]=m3[i]=m4[i]=e[i]=four[i]=re1[i]=re2[i]=0;/*/简单初始化一下*/
	e[MAX-1]=1;e[0]=1;
	four[MAX-1]=1;four[0]=4;

	add(p,e,pp);add(q,e,qq);
	divt(pp,four,s1,re1);divt(qq,four,s2,re2);

	expmod(c,s1,p,m1);	//求得m1
	sub(p,m1,m2);		//m2
	expmod(c,s2,q,m3);	//m3
	sub(q,m3,m4);		//m4

	//然后求出M1,M2,M3,M4
	int t1[MAX],t2[MAX],t3[MAX],t4[MAX],am1[MAX],am2[MAX],bm3[MAX],bm4[MAX];

	for( i=0;i<MAX;i++)
		M1[i]=M2[i]=M3[i]=M4[i]=t1[i]=t2[i]=t3[i]=t4[i]=am1[i]=am2[i]=bm3[i]=bm4[i]=0;/*/简单初始化一下*/

	mul(a,m1,am1);mul(b,m3,bm3);mul(b,m4,bm4);mul(a,m2,am2);
	add(am1,bm3,t1);add(am1,bm4,t2);add(am2,bm3,t3);add(am2,bm4,t4);
	mulmod(t1,e,n,M1); M1[MAX-2]=c[MAX-2];//求得M1
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
	printf("\n输入加密过的文件路径 : ");
	scanf("%s",encryfile);
	if((fp=fopen(encryfile,"r"))==NULL)
	{
		printf("此文件不存在!\n");
		exit(0);
	}
	printf("\n文件中密文内容：\n\n");
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
			p1->bignum[MAX-1]=temp*10+c-48;//前面三步,是为了读取出原来明文的第一个字符在密文占多少位
			j--;
		}
		else if (j==0)
		{
			p1->bignum[i]=c-48;//这里真正开始读取密文的第一个字符,这里会循环读取,直到i==p1->bignum[MAX-1]成立.
			i++;
			if(i==p1->bignum[MAX-1])//当i=该字符密文中的长度时,还原,再去读取下一个字符
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
	printf("\n解密密文后文件中的明文:\n\n");
	p2=(struct slink * )malloc(LEN);
	p1=h;//使p1指向密文的数组链表,以便一个一个字符的密文解密
	k=0;
	if(h!=NULL)//temp为暂存ASIIC码的int值/
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
				if (( p2->bignum[MAX-2])=='0')//成立,说明这个字符对应的是汉字.
				{
					temp=0-temp;
				}//转化为正确的ASIIC码，如-78-96形成汉字	/	 
				ch=temp;//  str[k]--->ch 
				printf("%3c ",ch);//  str[k]--->ch ///当输出汉字时,比如"我"的ASCII码是: -57-46
				fputc(ch,fp);//写入文件str[k]--->ch///第一次输出-57系统不识别,所以不显示;要等到-46也输出时,才会合并显示出"我".
				//然而,实际上由于Rabin算法会产生4个明文,在没有冗余判定哪个是正确解时,
				//-57和-46很难刚好连接着一起输出,也就得不到汉字'我'			   
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
		m[i]=p[i]=q[i]=n[i]=e[i]=a[i]=b[i]=two[i]=0;/*/简单初始化一下*/
	two[MAX-1]=1;two[0]=2;

	printf("\n\n随机密钥对产生如下：  ( p,q满足: p=q=3mod4 )\n\n");
	//	printf("\n其中p,q满足:p=q=3mod4 ");
	prime_random(p,q);//随机产生两个大素数//且p=q=3mod4
	mul(p,q,n);	//计算n=p*q
	printf("由 p、q 得出 n :");
	print(n);//通过函数print(n),输出n

	rabinab(q,p,a);//求得a;//a是q与q的逆元之积
	printf("        得出 a (q与q的逆元之积) : ");	
	print(a); 
	rabinab(p,q,b);//求得b;//b是p与p的逆元之积
	printf("        得出 b (p与p的逆元之积) : ");
	print(b);

	//对文件加密
	printf("密钥对产生完成，现在可以直接进行加解密文件!\n");
	mov(two,e);//把RSA中的加密指数变为Rabin中的2.
	tencrypto( e,  n); //对有需要的文件进行加密
	printf("\n加密文件操作完成!\n");

	//对加密过的文件解密
	findM(p,q,n,a,b);
	printf("\n解密文件操作完成!\n");	   

	return 0;
}
