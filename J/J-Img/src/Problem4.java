import java.math.*;

interface Func {
	public double funcX(double x);
	public double dfuncX(double x);
}

class FX implements Func {
	public double funcX(double x) {
		return 2*x*x*x - 15*x*x + 36*x - 23;
	}

	public double dfuncX(double x) {
		return 6*x*x - 30*x + 36;
	}
}

class GX implements Func {

	public double funcX(double x) {
		return Math.exp(0.1*x) - Math.exp(-0.4*x) - 1;
	}

	public double dfuncX(double x) {
		return 0.1*Math.exp(0.1*x) + 0.4*Math.exp(-0.4*x);
	}
}

class TX implements Func {

	public double funcX(double x) {
		return Math.log(x)-1+Math.exp(-1*x);
	}

	public double dfuncX(double x) {
		return 1.0/x - Math.exp(-1*x);
	}
}

class NewtonSolve {
	public Func func = null;

	public NewtonSolve() {}

	public double RootFind(double initValue) {
		double x1 = 0, x2 = initValue;
		int i = 0;

		System.out.println("Step i      xi      f(xi)      df(x)/dx     x(i+1)");
		System.out.println("--------------------------------------------------");

		do {

			x1 = x2;

			double fx = func.funcX(x1);
			double dx = func.dfuncX(x1);

			x2 = x1 - fx/dx;

			System.out.printf("%4d|%11.6f|%11.6f|%11.6f|%11.6f\n",
					i++, x1, fx, dx, x2);

		} while (Math.abs(x1-x2) > 0.0001);

		return x1;
	}
}

public class Problem4 {
	public static void main(String[] args) {
		NewtonSolve ns = new NewtonSolve();

		// Qa
		ns.func = new FX();
		double fx = ns.RootFind(0.0);

		ns.func = new GX();
		double gx = ns.RootFind(0.0);

		// Qb
		ns.func = new TX();
		double tx = ns.RootFind(1.0);
	}
}
