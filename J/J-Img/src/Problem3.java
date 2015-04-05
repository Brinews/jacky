import Jama.Matrix;
import Jama.SingularValueDecomposition;

import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileWriter;

class PrincipalComp {

	final int WIDTH = 2734;
	final int HEIGHT = 100;

	String fileName = "snps.txt";
	Matrix Snp;

	public PrincipalComp() { }
	public PrincipalComp(String name) { fileName = name; }

	public Matrix PrcpReadSnps() {

		int row = 0, col = 0;
		double[][] data = new double[HEIGHT][WIDTH];

		try {
			BufferedReader br = new BufferedReader(new FileReader(fileName));
			String line;
			while ((line = br.readLine()) != null) {
				String[] array = line.split(" ");

				if (row >= HEIGHT) {
					System.out.println("input file has too much rows.");
					break;
				}

				int len = array.length;
				if (len != WIDTH) {
					System.out.println("input file has too much cols.");
				}

				if (len > WIDTH) len = WIDTH;

				for (int i = 0; i < len; i++) {
					data[row][i] = Integer.parseInt(array[i]);
				}

				row++;
			}

			br.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		Snp = new Matrix(data);

		return Snp;
	}

	public double[] mean(double[][] array) {
		int R = array.length;
		int C = array[0].length;

		double[] m = new double[C];

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				m[j] += array[i][j];
			}
		}

		for (int i = 0; i < C; i++)
			m[i] /= R;

		return m;
	}

	public double[][] repmat(double[] array, int R) {
		int C = array.length;

		double[][] m = new double[R][C];

		for (int i = 0; i < R; i++)
			for (int j = 0; j < C; j++) {
				m[i][j] = array[j];
			}

		return m;
	}

	/*
	 * Use SVD to get PCA
	 */
	public Matrix PrcpAnalysis(Matrix src, int K, double[] eig) {

		double[][] ar = src.getArray();
		double[] avg = mean(ar);
		double[][] nar = repmat(avg, src.getRowDimension());

		Matrix meanM = new Matrix(nar);

		Matrix A = src.minus(meanM);

		//A.print(6,2);

		// 1. get input data
		A = A.times(1.0/Math.sqrt(src.getRowDimension()-1));

		// 2. use svd 
		SingularValueDecomposition s = A.svd();

		Matrix V = s.getV();
		Matrix pcaV = V.getMatrix(0, V.getRowDimension()-1, 0, K-1);

		Matrix dst = src.times(pcaV);

		// 3. get singular values
		double[] sv = s.getSingularValues();
		for (int i = 0; i < eig.length; i++)
			eig[i] = sv[i];

		return dst;
	}

	public void PrcpWriteFile(Matrix L, String outFileName)
	{
		try {
			FileWriter fw = new FileWriter(outFileName);

			int R = L.getRowDimension();
			int C = L.getColumnDimension();

			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					String s = new Double(L.get(i, j)).toString();
					fw.write(s);
					if (j != C-1) fw.write(" ");
				}
				fw.write("\n");
			}

			fw.close();

		} catch(IOException e) {
			e.printStackTrace();
		}
	}
}

public class Problem3 {
	public static void main(String[] args) {
		PrincipalComp pc = new PrincipalComp();

		Matrix x = pc.PrcpReadSnps();
		//x.print(2,1);

		//Qa
		double[] eig = new double[100];

		Matrix five = pc.PrcpAnalysis(x, 5, eig);

		// for debug
		//five.print(6, 2);

		System.out.println("First 5 entries:");
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.printf("%10.6f ", five.get(i,j));
			}
			System.out.println("");
		}

		System.out.println("100 singular values:");
		Matrix hundred = pc.PrcpAnalysis(x, 100, eig);
		for (int i = 0; i < eig.length; i++)
			System.out.println(eig[i]);

		// Qb
		Matrix L = pc.PrcpAnalysis(x, 2, eig);

		pc.PrcpWriteFile(L, "./3Q/locations.txt");
	}
}
