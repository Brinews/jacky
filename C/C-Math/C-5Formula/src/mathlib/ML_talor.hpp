#ifndef _ML_TALOR_H
#define _ML_TALOR_H

class CTalor {
private:
public:
    CTalor() {}

    double myabs(double x) {
        return x < 0 ? -x : x;
    }
    
    /* 利用泰勒公式计算sin(x) */
    double getSin(double x) {
        double sum = x, x_pow = x, item;
        //定义变量时赋值，已经将第一项考虑到累加和sum中了
        int n = 1, fact = 1, sign = 1;

        do {
            fact = fact*(n + 1)*(n + 2);
            x_pow *= x*x; fact;
            sign = -sign; //  累加这一项的符号；
            item = x_pow / fact*sign; 
            sum += item;
            n += 2;

        } while (myabs(item) > 1e-5);

        return sum;
    }

    /* 利用泰勒公式计算e^x */
    double getExp(double x) {
        double e =1.0;
        int i;
        int n=30;
        double term = 1.0;

        for (i=1; i<=n; i++) {
            term *= (x/i);
            e +=term;
        }
        return e;
    }

    /* 利用泰勒公式计算cos(x) */
    double getCos(double x) {
        double sum = 1,x_pow = 1, item;
        int n = 0, fact = 1, sign = -1;

        do {
            fact = fact*(n + 1)*(n + 2);
            item = x_pow / fact*sign;
            x_pow *= x*x; fact;
            sum += item;
            sign = -sign; //    累加这一项的符号；

            n += 2;
        } while (myabs(item) > 1e-5);

        return sum;

    }
};

#endif
