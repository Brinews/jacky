#ifndef _ML_TALOR_H
#define _ML_TALOR_H

class CTalor {
private:
public:
    CTalor() {}

    double myabs(double x) {
        return x < 0 ? -x : x;
    }
    
    /* ����̩�չ�ʽ����sin(x) */
    double getSin(double x) {
        double sum = x, x_pow = x, item;
        //�������ʱ��ֵ���Ѿ�����һ��ǵ��ۼӺ�sum����
        int n = 1, fact = 1, sign = 1;

        do {
            fact = fact*(n + 1)*(n + 2);
            x_pow *= x*x; fact;
            sign = -sign; //  �ۼ���һ��ķ��ţ�
            item = x_pow / fact*sign; 
            sum += item;
            n += 2;

        } while (myabs(item) > 1e-5);

        return sum;
    }

    /* ����̩�չ�ʽ����e^x */
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

    /* ����̩�չ�ʽ����cos(x) */
    double getCos(double x) {
        double sum = 1,x_pow = 1, item;
        int n = 0, fact = 1, sign = -1;

        do {
            fact = fact*(n + 1)*(n + 2);
            item = x_pow / fact*sign;
            x_pow *= x*x; fact;
            sum += item;
            sign = -sign; //    �ۼ���һ��ķ��ţ�

            n += 2;
        } while (myabs(item) > 1e-5);

        return sum;

    }
};

#endif
