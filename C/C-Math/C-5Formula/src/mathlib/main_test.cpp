#include "ML_vector.hpp"
#include "ML_equation.hpp"
#include "ML_poly.hpp"
#include "ML_talor.hpp"
#include "ML_integral.hpp"

#define PI 3.1415926

double  fx( double  x)   // 方程为: e^x+4^x3-6^x2+3x-2=0 
{
    return exp(x)+4.0*x*x*x-6.0*x*x+3.0*x-2.0;
} 

double f(double x)
{
    return (1+1/(x*x));
}

double  middle( double  x,  double  y)   // 中值 
{
    return 0.5*(x+y);
} 

void menu() {
    cout << "高数库演示" << endl;
    cout << "==========\n";
    cout << "1. 向量夹角计算\n";
    cout << "2. 二分法解方程\n";
    cout << "3. 多项式函数微积分\n";
    cout << "4. 泰勒公式求函数值\n";
    cout << "5. 积分法求面积\n";
    cout << "0. 退出\n >> ";
}

int main(int argc, char **argv)
{
    int a1[100], a2[100];

    int op;
    while (true) {
        menu();
        cin >> op;
        if (op == 1) {

            /* test 1: 计算夹角 */
            int a[3] = {1, 2, 3};
            int b[3] = {4, 7, 3};

            Vector v1(a, 3);
            Vector v2(b, 3);

            cout << "演示：\n向量<1,2,3>与<4,7,3>的夹角:" << v1.getAngle(v2) << endl;

            cout << "请输入向量维度:";
            int n;
            cin >> n;

            int i;
            cout << "请输入向量一:";
            for (i = 0; i < n; i++) {
                cin >> a1[i];
            }
            cout << "请输入向量二:";
            for (i = 0; i < n; i++) {
                cin >> a2[i];
            }

            Vector v3(a1, n);
            Vector v4(a2, n);
            cout << "计算夹角为:" << v3.getAngle(v4) << endl;
        }

        if (op == 2) {
            /* test 2: 二分法求方程解 */
            cout << "求解方程e^x+4^x3-6^x2+3x-2=0\n";
            cout << "根区间[0.0, 1.0]\n";
            CEquation ceq(0.0, 1.0, fx, middle, 1e-6);

            ceq.biSection();
            ceq.printSolution();
        }

        if (op == 3) {

            /* test 3: 多项式积分求值 */
            double d[4] = {1, 2, 3, 4};
            CPolyexp cp1(d, 4);
            //cout << cp1.eval(2.0) << endl;

            CPolyexp cp1d = cp1.integrate();

            cout << "示例:求方程"<< cp1.toString() << " 的积分[1,2]:";
            cout << cp1d.eval(2.0) - cp1d.eval(1.0) << endl;

            cout << "请输入多向式最高项数n:";
            int n;
            cin >> n;

            double d1[100];
            cout << "请输入n+1个系数:";
            for (int i = 0; i < n+1; i++) {
                cin >> d1[i];
            }

            int A, B;
            cout << "输入区间值a:";
            cin >> A;
            cout << "输入区间值b:";
            cin >> B;

            CPolyexp cp2(d1, n+1);
            CPolyexp cp2d = cp2.integrate();

            cout << "求方程"<< cp2.toString() << " 的积分"<<"["<<A<<","<<B<<"] :";
            cout << cp2d.eval(B) - cp2d.eval(A) << endl;


        }

        if (op == 4) {
            /* test 4: 泰勒公式 */
            CTalor ct;
            cout << sin(PI*56/180.0) << "," << cos(PI*56/180.0) 
                << "," << exp(1.0) << endl;
            cout << "示例：计算sin值：" << ct.getSin(PI*56/180.0) << endl;
            cout << "示例：计算cos值：" << ct.getCos(PI*56/180.0) << endl;
            cout << "示例：计算e^x值：" << ct.getExp(1.0) << endl;

            double x;
            cout << "输入角度x值:";
            cin >> x;

            cout << "计算sin(x)=" << ct.getSin(PI*x/180.0) << endl;
            cout << "计算cos(x)=" << ct.getSin(PI*x/180.0) << endl;
        }

        if (op == 5) {
            /* test 5: 梯形法求积分 */
            CIntegral ci;
            cout << "梯形公式法求"<< "1+1/(x^2)"
                << "面积积分:" << ci.rectIntegral(f, 0.5, 1.0)
                << endl;
        }

        if (op == 0) break;

    }

    return 0;
}
