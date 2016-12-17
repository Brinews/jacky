#ifndef _ML_EQUATION_H
#define _ML_EQUATION_H

class CEquation 
{ 
private: 
    double solution;            //���̵Ľ��ƽ� 
    double a, b;                //���ƽ������ 

    double (*p_fx)(double x);   //p_fx��һ��ָ������ָ��,ָ�򷽳�ʽ��ֵ���� 
    double (*p_solution)(double x, double y); //ָ���ɽ��ƽ���������ƽ�ĺ�����ָ�� 
    double delta;               //��⾫�� 

public: 
    CEquation(double av, double bv, double (*p1)(double), double (*p2)(double,double), double dv); 

    double biSection();        //���ַ��󷽳̽��ƽ� 
    void printSolution() const; 
}; 

CEquation::CEquation( double  a_val,  double  b_val,  
        double  ( * p1)( double ),  
        double  ( * p2)( double ,  double ),  
        double  delta_val) {
    a = a_val;
    b = b_val;
    p_fx = p1;
    p_solution = p2;
    solution = p_solution(a, b);
    delta = delta_val;
} 

double  CEquation::biSection()
{
    double h;

    while (fabs(a-b) > delta)
    {
        h = p_solution(a, b);

        if (p_fx(h) == 0) return h;

        if (p_fx(a)*p_fx(h) > 0)
            a = h;
        else
            b = h;
    }
    solution = p_solution(a, b);

    return solution;
} 

void  CEquation::printSolution()  const 
{
    cout << "Solution is: " << solution << endl;
} 


#endif
