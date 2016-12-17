#ifndef _ML_VECTOR_H
#define _ML_VECTOR_H

#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

//-----------------------------------------------------------------------

// ����

class Vector
{
private:
    vector<double> v;
    int dim; // ά��

public:

    Vector() : dim(0) { v.clear(); }
    Vector(int _x[], int n) {
        dim = n;
        for (int i = 0; i < n; i++) {
            v.push_back(_x[i]);
        }
    }

    double getModule() {
        double mod = 0.0;

        for (int i = 0;i < dim; i++)
            mod += v[i]*v[i];

        return mod;
    }

    double getAngle(Vector vec) {
        double angle = 0.0f; // �н�

        if (dim != vec.dim) {
            std::cout << "ά�Ȳ�ͬ,���ܼ���н�\n";
            return 0;
        }

        double productValue = 0.0;  // �����ĳ˻�
        for (int i = 0; i < dim; i++) {
            productValue += v[i]*vec.v[i];
        }

        double va_val = sqrt(getModule());  // ����a��ģ
        double vb_val = sqrt(vec.getModule());  // ����b��ģ
        double cosValue = productValue / (va_val * vb_val);      // ���ҹ�ʽ

        // acos�����������Χ������[-1, 1]֮�䣬�����"domain error"
        // �����������У��ʹ���
        if(cosValue < -1 && cosValue > -2)
            cosValue = -1;
        else if(cosValue > 1 && cosValue < 2)
            cosValue = 1;

        // acos���ص��ǻ���ֵ��ת��Ϊ�Ƕ�ֵ
        angle = acos(cosValue) * 180 / M_PI; 

        return angle;
    }
};


#endif
