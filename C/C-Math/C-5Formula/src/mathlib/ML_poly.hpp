#ifndef _ML_POLY_H
#define _ML_POLY_H

#include <vector>
#include <string>

using namespace std;

/* ����ʽ������΢���� */

class CPolyexp {
private:
    vector<double> coe; /* ����ʽ����ϵ��(1,0,3) = 1+0*x+3*x^2 */

public:
    CPolyexp(double a[], int n) {
        for (int i = 0; i < n; i++)
            coe.push_back(a[i]);
    }

    CPolyexp(vector<double> a, int n) {
        for (int i = 0; i < n; i++)
            coe.push_back(a[i]);
    }

    string termString(double e, int c) {
        char buf[10];
        sprintf(buf, "%.2lf", e); 
        string cs(buf);

        if (c == 0) return cs;
        if (c == 1) {
            return cs + "*X + ";
        }

        sprintf(buf, "%d", c);
        cs += "*X^";
        cs += buf;

        if (c > 0) {
            cs += " + ";
        }

        return cs;
    }

    /* ��ӡ */
    string toString() {
        string ret = "";
        if (coe.size() == 0) return "0";

        for (int i = coe.size()-1; i >= 0; i--) {
            ret += termString(coe[i], i);
        }

        return ret;
    }

    /* ��ֵ */
    double eval(double x) {
        double val = 0.0;

        for (int i = 0; i < coe.size(); i++) {
            val += coe[i]*pow(x, i);
        }

        return val;
    }

    /* ��΢�� */
    CPolyexp differentiate() {
        vector<double> p;

        for (int i = 1; i < coe.size(); i++) {
            p.push_back(coe[i]*i);
        }

        return CPolyexp(p, p.size());
    }

    /* ����� */
    CPolyexp integrate() {
        vector<double> p;

        p.push_back(0);

        for (int i = 0; i < coe.size(); i++) {
            p.push_back(coe[i]/(i+1.0));
        }

        return CPolyexp(p, p.size());
    }

    ~CPolyexp() {}
};

#endif
