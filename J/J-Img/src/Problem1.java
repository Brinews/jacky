import Jama.Matrix;
import Jama.SingularValueDecomposition;

class SvdOps {
	private String srcImgName = "baboon.pgm";
	private String imgU = "./1Qa/U.pgm"; //for output image result
	private String imgD = "./1Qa/D.pgm";
	private String imgV = "./1Qa/V.pgm";

	private int srcImgHeight, srcImgWidth;
	private int srcImgId;

	private Matrix A;
	private Matrix U, D, V;

	/* parameters */
	int rank;
	double[] ratio;
	double[] maxerror;
	double[] meanerror;

	public SvdOps() {}
	public SvdOps(String fileName) { srcImgName = fileName; }

	/* for debug */
	private int pi = 6, pf = 2;

	public Matrix SvdReadImg() {
		int[] id = new int[1];
		int[] row = new int[1];
		int[] col = new int[1];

		char[] imageData = PGM_PPM_Handler.
							readFilePGM_PPM(srcImgName, id, row, col);

		srcImgWidth = col[0];
		srcImgHeight = row[0];
		srcImgId = id[0];

		double[][] pixelData = new double[srcImgHeight][srcImgWidth];
		for (int i = 0; i < srcImgHeight; i++)
			for (int j = 0; j < srcImgWidth; j++)
				pixelData[i][j] = imageData[i*srcImgWidth+j];

		A = new Matrix(pixelData);

		return A;
	}

	public void SvdDecomposition() {
		//A.print(pi, pf); //for debug

		SingularValueDecomposition s = A.svd();

		U = s.getU();
		//System.out.print("U = ");
		//U.print(pi, pf); //for debug

		D = s.getS();
		//System.out.print("S = ");
		//D.print(pi, pf); //for debug

		V = s.getV();
		//System.out.print("V = ");
		//V.print(pi, pf); //for debug
	}

	public Matrix SvdComposition(int K) {

		Matrix subU = U.getMatrix(0, U.getRowDimension()-1, 0, K);
		Matrix subD = D.getMatrix(0, K, 0, K);
		Matrix subV = V.getMatrix(0, V.getRowDimension()-1, 0, K);

		//System.out.print("SubD = ");
		//subD.print(6, 2); //for debug

		return subU.times(subD).times(subV.transpose());
	}

	public Matrix SvdNormalize(Matrix m) {
		double[][] dataArray = m.getArray();

		int H = m.getRowDimension();
		int W = m.getColumnDimension();

		double Max = -10000, Min = 100000000;

		// 1. get Max and Min
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++) {
				if (dataArray[i][j] > Max)
					Max = dataArray[i][j];
				if (dataArray[i][j] < Min)
					Min = dataArray[i][j];
			}

		//System.out.print("Max="+ Max+",Min="+Min+"\n");

		// 2. Normalize
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++) {
				dataArray[i][j] = 255.0 * (dataArray[i][j] - Min) /
										(Max - Min);

				dataArray[i][j] = (int) (dataArray[i][j] + 0.5);
			}

		//m.print(pi, pf);
		return m;
	}

	public void SvdWriteImg(Matrix x, String fileName) {
		int H = x.getRowDimension();
		int W = x.getColumnDimension();

		char[] pixelData = new char[H*W];

		//System.out.println(fileName);
		//x.print(pi, pf);

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				double xv = x.get(i,j);

				if (xv < 0) xv = 0;
				if (xv > 255) xv = 255;

				pixelData[i*W+j] = (char) xv;
			}
		}

		PGM_PPM_Handler.saveFilePGM_PPM(fileName, pixelData, srcImgId,
				H, W);
	}

	// A=UDV'
	public void SvdDecompo() {
		//1. read image
		A = SvdReadImg();

		//2. decomposition and normalize
		SvdDecomposition();
		SvdNormalize(U);
		SvdNormalize(D);
		SvdNormalize(V);

		//3. write image
		SvdWriteImg(U, imgU);
		SvdWriteImg(D, imgD);
		SvdWriteImg(V, imgV);
	}

	public double SvdMaxMatrixError(Matrix A, Matrix B) {
		int R = A.getRowDimension();
		int C = A.getColumnDimension();

		double max = -10000;

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				double bij = B.get(i,j);
				if (bij < 0) bij = 0;
				if (bij > 255) bij = 255;

				if (Math.abs(bij - A.get(i,j)) > max)
					max = Math.abs(bij - A.get(i, j));
			}
		}

		return max;
	}

	public double SvdMeanMatrixError(Matrix A, Matrix B) {
		int R = A.getRowDimension();
		int C = A.getColumnDimension();

		double mean = 0.0;

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				double bij = B.get(i,j);
				if (bij < 0) bij = 0;
				if (bij > 255) bij = 255;

				mean += (bij - A.get(i, j));
			}
		}

		return mean/(R*C);
	}

	// UDV'=A^
	public void SvdCompo() {
		//1. read image
		A = SvdReadImg();

		//2. decomposition
		SvdDecomposition();

		//3. get rank and singular values
		rank = A.rank();
		System.out.println("rank=" + rank);

		int[] sv = new int[rank];

		ratio = new double[rank];
		maxerror = new double[rank];
		meanerror = new double[rank];

		for (int i = 0; i < rank; i++) {
			sv[i] = i;

			Matrix likeA = SvdComposition(i);

			maxerror[i] = SvdMaxMatrixError(A, likeA);
			meanerror[i] = SvdMeanMatrixError(A, likeA);

			int K = i+1;
			String num = new Integer(K).toString();
			// write result image
			SvdWriteImg(likeA, "./1Qb/"+num+".pgm");

			double total = srcImgWidth*srcImgHeight;
			double compress = srcImgHeight * K + K*K + K * srcImgWidth;

			ratio[i] = (1-compress/total)*100;
		}
	}

	// Get approximation errors and compression ratio
	public void SvdParameters() {

		System.out.print("p            |");
		for (int i = 0; i < rank; i++) {
			System.out.printf("%6d", i);
		}
		System.out.println();

		System.out.print("Max error    |");
		for (int i = 0; i < rank; i++) {
			System.out.printf("%6.1f", maxerror[i]);
		}
		System.out.println();

		System.out.print("Mean error   |");
		for (int i = 0; i < rank; i++) {
			System.out.printf("%6.1f", meanerror[i]);
		}
		System.out.println();

		System.out.print("Compression,%|");
		for (int i = 0; i < rank; i++) {
			if (ratio[i] > 0)
				System.out.printf("%6.1f", ratio[i]);
			else
				System.out.printf("  n/a ");
		}
		System.out.println();
	}
}

public class Problem1 {

	public static void main(String[] args) {
		SvdOps so = new SvdOps();

		//Question A:
		so.SvdDecompo();
		
		//Question B:
		so.SvdCompo();

		//Question C:
		so.SvdParameters();

		//test();
	}

	/* just for test */
    public static void test() {
         // create M-by-N matrix that doesn't have full rank
          int M = 8, N = 5;
          Matrix B = Matrix.random(5, 3);
          Matrix A = Matrix.random(M, N).times(B).times(B.transpose());
          System.out.print("A = ");
          A.print(9, 6);

          // compute the singular vallue decomposition
          System.out.println("A = U S V^T");
          System.out.println();
          SingularValueDecomposition s = A.svd();
          System.out.print("U = ");
          Matrix U = s.getU();
          U.print(9, 6);
          System.out.print("Sigma = ");
          Matrix S = s.getS();
          S.print(9, 6);
          System.out.print("V = ");
          Matrix V = s.getV();
          V.print(9, 6);
          System.out.println("rank = " + s.rank());
          System.out.println("condition number = " + s.cond());
          System.out.println("2-norm = " + s.norm2());

          // print out singular values
          System.out.print("singular values = ");
          Matrix svalues = new Matrix(s.getSingularValues(), 1);
          svalues.print(9, 6);

		  Matrix C = U.times(S).times(V.transpose());
		  C.print(9, 6);

		  Matrix D = A.transpose();
		  D.print(9, 6);

		  Matrix E = D.times(A);
		  E.print(9, 6);
    }
} 
