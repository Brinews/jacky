#ifndef _ML_VECTOR_H
#define _ML_VECTOR_H

#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

//-----------------------------------------------------------------------

// 向量

class Vector
{
private:
    vector<double> v;
    int dim; // 维度

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
        double angle = 0.0f; // 夹角

        if (dim != vec.dim) {
            std::cout << "维度不同,不能计算夹角\n";
            return 0;
        }

        double productValue = 0.0;  // 向量的乘积
        for (int i = 0; i < dim; i++) {
            productValue += v[i]*vec.v[i];
        }

        double va_val = sqrt(getModule());  // 向量a的模
        double vb_val = sqrt(vec.getModule());  // 向量b的模
        double cosValue = productValue / (va_val * vb_val);      // 余弦公式

        // acos的输入参数范围必须在[-1, 1]之间，否则会"domain error"
        // 对输入参数作校验和处理
        if(cosValue < -1 && cosValue > -2)
            cosValue = -1;
        else if(cosValue > 1 && cosValue < 2)
            cosValue = 1;

        // acos返回的是弧度值，转换为角度值
        angle = acos(cosValue) * 180 / M_PI; 

        return angle;
    }
};


#endif
