#include "ML_vector.hpp"
#include "ML_equation.hpp"
#include "ML_poly.hpp"
#include "ML_talor.hpp"
#include "ML_integral.hpp"

#define PI 3.1415926

double  fx( double  x)   // ����Ϊ: e^x+4^x3-6^x2+3x-2=0 
{
    return exp(x)+4.0*x*x*x-6.0*x*x+3.0*x-2.0;
} 

double f(double x)
{
    return (1+1/(x*x));
}

double  middle( double  x,  double  y)   // ��ֵ 
{
    return 0.5*(x+y);
} 

void menu() {
    cout << "��������ʾ" << endl;
    cout << "==========\n";
    cout << "1. �����нǼ���\n";
    cout << "2. ���ַ��ⷽ��\n";
    cout << "3. ����ʽ����΢����\n";
    cout << "4. ̩�չ�ʽ����ֵ\n";
    cout << "5. ���ַ������\n";
    cout << "0. �˳�\n >> ";
}

int main(int argc, char **argv)
{
    int a1[100], a2[100];

    int op;
    while (true) {
        menu();
        cin >> op;
        if (op == 1) {

            /* test 1: ����н� */
            int a[3] = {1, 2, 3};
            int b[3] = {4, 7, 3};

            Vector v1(a, 3);
            Vector v2(b, 3);

            cout << "��ʾ��\n����<1,2,3>��<4,7,3>�ļн�:" << v1.getAngle(v2) << endl;

            cout << "����������ά��:";
            int n;
            cin >> n;

            int i;
            cout << "����������һ:";
            for (i = 0; i < n; i++) {
                cin >> a1[i];
            }
            cout << "������������:";
            for (i = 0; i < n; i++) {
                cin >> a2[i];
            }

            Vector v3(a1, n);
            Vector v4(a2, n);
            cout << "����н�Ϊ:" << v3.getAngle(v4) << endl;
        }

        if (op == 2) {
            /* test 2: ���ַ��󷽳̽� */
            cout << "��ⷽ��e^x+4^x3-6^x2+3x-2=0\n";
            cout << "������[0.0, 1.0]\n";
            CEquation ceq(0.0, 1.0, fx, middle, 1e-6);

            ceq.biSection();
            ceq.printSolution();
        }

        if (op == 3) {

            /* test 3: ����ʽ������ֵ */
            double d[4] = {1, 2, 3, 4};
            CPolyexp cp1(d, 4);
            //cout << cp1.eval(2.0) << endl;

            CPolyexp cp1d = cp1.integrate();

            cout << "ʾ��:�󷽳�"<< cp1.toString() << " �Ļ���[1,2]:";
            cout << cp1d.eval(2.0) - cp1d.eval(1.0) << endl;

            cout << "���������ʽ�������n:";
            int n;
            cin >> n;

            double d1[100];
            cout << "������n+1��ϵ��:";
            for (int i = 0; i < n+1; i++) {
                cin >> d1[i];
            }

            int A, B;
            cout << "��������ֵa:";
            cin >> A;
            cout << "��������ֵb:";
            cin >> B;

            CPolyexp cp2(d1, n+1);
            CPolyexp cp2d = cp2.integrate();

            cout << "�󷽳�"<< cp2.toString() << " �Ļ���"<<"["<<A<<","<<B<<"] :";
            cout << cp2d.eval(B) - cp2d.eval(A) << endl;


        }

        if (op == 4) {
            /* test 4: ̩�չ�ʽ */
            CTalor ct;
            cout << sin(PI*56/180.0) << "," << cos(PI*56/180.0) 
                << "," << exp(1.0) << endl;
            cout << "ʾ��������sinֵ��" << ct.getSin(PI*56/180.0) << endl;
            cout << "ʾ��������cosֵ��" << ct.getCos(PI*56/180.0) << endl;
            cout << "ʾ��������e^xֵ��" << ct.getExp(1.0) << endl;

            double x;
            cout << "����Ƕ�xֵ:";
            cin >> x;

            cout << "����sin(x)=" << ct.getSin(PI*x/180.0) << endl;
            cout << "����cos(x)=" << ct.getSin(PI*x/180.0) << endl;
        }

        if (op == 5) {
            /* test 5: ���η������ */
            CIntegral ci;
            cout << "���ι�ʽ����"<< "1+1/(x^2)"
                << "�������:" << ci.rectIntegral(f, 0.5, 1.0)
                << endl;
        }

        if (op == 0) break;

    }

    return 0;
}
