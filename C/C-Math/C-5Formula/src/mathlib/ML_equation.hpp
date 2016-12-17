#ifndef _ML_EQUATION_H
#define _ML_EQUATION_H

class CEquation 
{ 
private: 
    double solution;            //方程的近似解 
    double a, b;                //近似解的区间 

    double (*p_fx)(double x);   //p_fx是一个指向函数的指针,指向方程式求值函数 
    double (*p_solution)(double x, double y); //指向由近似解区间求近似解的函数的指针 
    double delta;               //求解精度 

public: 
    CEquation(double av, double bv, double (*p1)(double), double (*p2)(double,double), double dv); 

    double biSection();        //二分法求方程近似解 
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
