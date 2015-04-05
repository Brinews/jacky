import Jama.Matrix;
import Jama.SingularValueDecomposition;

class PinvOps {
	String srcImgName = "baboon.pgm"; //input image file
	Matrix sI;

	public PinvOps() {}
	public PinvOps(String srcFile) { srcImgName = srcFile; }

	/*
	 * Get peusdoinverse of Matrix A, which is from srcImgFile
	 */
	public Matrix PinvInverse(String outFile) {
		SvdOps so = new SvdOps(srcImgName);

		Matrix A = so.SvdReadImg();
		Matrix pinv = A.inverse();	/* do pseudoinverse */

		sI = pinv.times(A); /* get the identity Matrix sI */

		// for debug
		//pinv.print(6,2);
		//System.out.println("Si=");
		//sI.print(6,2);

		so.SvdNormalize(pinv);

		//for debug
		//System.out.println("Pinv=");
		//pinv.print(6,2);

		so.SvdWriteImg(pinv, outFile);
		so.SvdWriteImg(so.SvdNormalize(sI), "./2Qa/I.pgm");

		return pinv;
	}

	/*
	 * E = I - sI
	 */
	public void PinvErrors() {
		SvdOps so = new SvdOps(srcImgName);

		Matrix A = so.SvdReadImg();

		Matrix pinv = A.inverse();	/* do pseudoinverse */
		Matrix Si = pinv.times(A); /* get the identity Matrix sI */

		int m = Si.getRowDimension();
		int n = Si.getColumnDimension();

		/* Real Identity Matrix */
		Matrix I = new Matrix(m, n, 0.0);
		double[][] dataI = I.getArray();
		for (int i = 0; i < m; i++)
			dataI[i][i] = 1.0;

		Matrix E = I.minus(Si); /* 0 matrix ? */

		for (int i = 0; i < m; i++)
			for (int j = 0; j < m; j++) {
				if (E.get(i, j) > 0.001) {
					System.out.printf("[%d, %d]=%f\n", 
											i, j, E.get(i,j));
				}
			}
	}

	public Matrix PinvGetB(String outFile) {

		SvdOps so = new SvdOps(srcImgName);
		Matrix A = so.SvdReadImg();

		Matrix pinv = A.inverse();	/* do pseudoinverse */

		Matrix B = A.times(pinv);
		B.print(4, 1);

		so.SvdNormalize(B);
		so.SvdWriteImg(B, outFile);

		return B;
	}
}

public class Problem2 {
	public static void main(String[] args) {
		PinvOps po = new PinvOps();

		// Qa : Pinv.pgm
		po.PinvInverse("./2Qa/Pinv.pgm");

		// Qb
		po.PinvErrors();

		// Qc
		po.PinvGetB("./2Qa/B.pgm");
	}
}
