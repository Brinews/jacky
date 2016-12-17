#ifndef _ML_INTEGRAL_H
#define _ML_INTEGRAL_H

#define N 1000
#define COUNT 100
#define epsilon 0.00001

/* 积分求解 */
class CIntegral {

public:

    double rectIntegral(double (*fun)(double), double a, double b) {
        double s,h;  //h为积分步长，s为梯形法上下底边和

        int i;
        //梯形法公式：sum=h[(f(a)+f(b))/2+f(a+h)+f(a+2h)+...+f(a+(n-1)h)]
        s=((* fun)(a)+(* fun)(b))/2.0; 

        h=(b-a)/N;

        for(i=1; i <= N; i++) {
            s+=(*fun)(a+i*h);
        }

        return(s*h);
    }

    // romberg方法求积分，效率更高
    double Romberg(double (*fun)(double), double a,double b)
    {
        int m ,n;
        double h,x,s,q,ep;
        double p,*R =new double[COUNT];

        h=b-a;
        R[0]= h*(fun(a)+ fun(b))/2.0;
        m=1;
        n=1;
        ep=epsilon+1.0;
        while ((ep >= epsilon)&& (m <COUNT))
        {
            p = 0.0;
            {
                for(int i=0;i<n;i++)
                {
                    x = a+ (i+0.5)*h ;
                    p= p + fun(x);
                }
                p= (R[0]+ h*p)/2.0;
                s = 1.0;
                for(int k=1;k<=m;k++)
                {
                    s = 4.0*s;
                    q= (s*p-R[k-1])/(s-1.0);
                    R[k-1]= p;
                    p =q;
                }
                p=fabs(q -R[m-1]);
                m =m + 1;
                R[m-1]= q;
                n = n + n;
                h = h/2.0;
            }
            return (q);
        }
    }
};

#endif
