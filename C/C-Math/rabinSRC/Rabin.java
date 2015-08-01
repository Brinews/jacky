import java.util.*;
import java.io.*;

/**
 * 设计Rabin算法
 * 1. 加密模块：Encrypt
 * 2. 解密模块：Decrypt
 * 3. 筛选模块: Solution
 * 4. 如何保证筛选结果：规定加密内容格式，按照两个字节重复原则
 */

public class Rabin {

	public static int isPrimeNumber(int number){ //判断是否为素数
		double num=(double)number;
		int n=(int)Math.sqrt(num);

		for(int i=2;i<=n;i++){
			if(number%i==0) return 0;
		}

		return 1;
	}

	public static int isMode(int number){    //判断结果是否模4余3
		if(number%4==3)
			return 1;
		else
			return 0;
	}

	public static int PrimeNumberTest(int max,int min){   //在max与min范围内产生随机数
		Random rand=new Random();
		int m = 4;
		while (isPrimeNumber(m) == 0) {
			m=rand.nextInt(max)%(max-min+1)+min;
		}

		int judge2=isMode(m);//判断是否模4余3
		if(judge2==0){
			int p=max;
			int q=min;
			return PrimeNumberTest(p,q);
		}
		return m;
	}

	public static int opposite(int a,int b){     //求a相对于b的逆
		int i=0;
		while(a*(++i)%b!=1);
		return i;
	}

	/** 
	 * (a^b)%c 
	 * @param a 基数
	 * @param b 指数
	 * @param c 模数
	 * @return int 结果
	 */
	public static int powermod(int a, int b, int c)
	{
		int t;
		if (b == 0) return 1%c;
		if (b == 1) return a%c;

		t = powermod(a, b/2, c);
		t = t * t % c;
		if ((b&1) == 1) t = t * a % c;

		return t;
	}

	public static int Deciphering(int b1,int b2,int m1,int m2){  //中国剩余定理求解
		int m=m1*m2;
		int M1=m2;
		int M2=m1;

		int M11=opposite(M1,m1);
		int M22=opposite(M2,m2);

		int result=(M11*M1*b1+M22*M2*b2)%m;

		return result;
	}

	/**
	 * 判断双字节数：前后字节相同
	 * @return boolean true or false
	 */
	public static boolean ReduntantBits(int val) {
		if (val <= 0xffff) {
			int low  = (val & 0xff);
			int high = ((val >> 8) & 0xff);

			if (high == low && low >= 0 && low <= 0xff)
				return true;
		}

		return false;
	}

	/**
	 * 8bit - 8bit (前后8个字节重复)
	 * @param encrytext 密文
	 * @param p 密钥
	 * @param q 密钥
	 * @return int 明文
	 */
	public static int Solution(int encrytext, int p, int q) {
		int c1 = 0, c2 = 0;
		int finaltext = 0;

		/* 第一个解 */
		c1 = powermod(encrytext, (p+1)/4, p);
		c2 = powermod(encrytext, (q+1)/4, q);

		finaltext = Deciphering(c1, c2, p, q);
		if (ReduntantBits(finaltext)) return finaltext;

		/* 第二个解 */
		c1 = p - powermod(encrytext, (p+1)/4, p);
		c2 = powermod(encrytext, (q+1)/4, q);

		finaltext = Deciphering(c1, c2, p, q);
		if (ReduntantBits(finaltext)) return finaltext;

		/* 第三个解 */
		c1 = powermod(encrytext, (p+1)/4, p);
		c2 = q - powermod(encrytext, (q+1)/4, q);

		finaltext = Deciphering(c1, c2, p, q);
		if (ReduntantBits(finaltext)) return finaltext;

		/* 第四个解 */
		c1 = p - powermod(encrytext, (p+1)/4, p);
		c2 = q - powermod(encrytext, (q+1)/4, q);

		finaltext = Deciphering(c1, c2, p, q);
		if (ReduntantBits(finaltext)) return finaltext;

		return -1;
	}

	/**
	 * @param n 公钥
	 * @param srcFile 待加密文件
	 * @param dstFile 加密后文件
	 */
	public static void Encrypt(int n, String srcFile, String dstFile) {
		try {
			/* 1. 打开文件，按字节进行读取 */
			File infile = new File(srcFile);
			InputStream in = null;

			FileOutputStream outfile = new FileOutputStream(dstFile);
			DataOutputStream out = null;

			out = new DataOutputStream(outfile);
			/* 读文件 */
			in = new FileInputStream(infile);
			int byteVal = 0;
			while ((byteVal = in.read()) != -1) {
				/* 2. 扩展字节：重复字节 */
				int extendVal = (byteVal << 8) + byteVal;
				//System.out.printf("bv=%x, ev=%x\n", byteVal, extendVal);

				/* 3. 按照扩展后的数进行加密, 并写文件 */
				int encrytext = (int)((1.0*extendVal*extendVal)%n);
				out.writeInt(encrytext);
			}

			out.flush();
			outfile.close();

			in.close();
			out.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 进行文件解密
	 * @param p 密钥
	 * @param q 密钥
	 * @param srcFile 加密文件
	 * @param dstFile 解密文件
	 */
	public static void Decrypt(int p, int q, String srcFile, String dstFile) {
		/* 1. 打开加密文件，按int四字节读取 */
		try {
			FileInputStream infile = new FileInputStream(srcFile);
			DataInputStream in = null;

			FileOutputStream outfile = new FileOutputStream(dstFile);
			DataOutputStream out = null;

			in = new DataInputStream(infile);
			out = new DataOutputStream(outfile);
			int readVal = 0;

			while (in.available() > 0) {

				readVal = in.readInt();

				int finalText = Solution(readVal, p, q);
				if (finalText != -1) {
					int byteVal = finalText & 0xff;
					out.writeByte((byte)byteVal);
				} else {
					System.out.println("未找到解。");
				}
			}

			out.flush();

			infile.close();
			outfile.close();

			in.close();
			out.close();

			/* 2. 进行解密，并写文件 */
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * ASCII加解密测试
	 * @param n 公钥
	 * @param p 私钥
	 * @param q 私钥
	 */
	public static void AsciiRabin(int n, int p, int q) {
		for (int i = 0; i < 256; i++) {

			int a = i;
			int b = i;

			int plaintext = (a<<8);
			plaintext += b;

			//System.out.printf("a=%d, b=%d, p=%x, n=%d,p*p=%f\n", a, b, 
			//			plaintext, n, 1.0*plaintext*plaintext);

			int encrytext=(int)((1.0*plaintext*plaintext)%n);
			System.out.println(plaintext+"加密以后的密文为："+encrytext);

			int finaltext = Solution(encrytext, p, q);
			if (finaltext != -1) {
				System.out.println("解为"+finaltext);
			} else {
				System.out.println("无解");
			}
		}
	}

	public static void main(String[] args) throws Exception{ 

		System.out.println("请输入需要素数的所在区间,建议(100,1000):");

		Scanner input=new Scanner(System.in);

		int min=input.nextInt();/* be sure 100<p,q<1000 */
		int max=input.nextInt();


		int p=PrimeNumberTest(max,min);
		//int p=7;
		int q=PrimeNumberTest(max,min);
		//int q=11;
		System.out.println("公钥:n="+p*q);
		System.out.println("私钥:p="+p+",q="+q);
		int n=p*q;

		System.out.println("请指定加密文件(如C:/test/test.in):");
		input.nextLine();
		String inFile = input.nextLine();

		input.close();

		String enFile = inFile + ".out";
		String deFile = inFile + ".old";

		System.out.println("加密文件"+inFile+"成功, 加密后文件为"+enFile);
		Encrypt(n, inFile, enFile);

		System.out.println("解密文件"+enFile+"成功, 解密后文件为"+deFile);
		Decrypt(p, q, enFile, deFile);

	}
}

