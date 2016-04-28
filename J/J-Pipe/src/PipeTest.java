import java.util.*;
import java.io.*;

/**
  This program demonstrates how multiple threads communicate
  through pipes.
 */
public class PipeTest
{ 
	public static void main(String args[])
	{ 
		try
		{ 
			/* set up pipes */
			PipedOutputStream pout1 = new PipedOutputStream();
			PipedInputStream pin1 = new PipedInputStream(pout1);

			PipedOutputStream pout2 = new PipedOutputStream();
			PipedInputStream pin2 = new PipedInputStream(pout2);

			/* construct threads */

			Producer prod = new Producer(pout1);
			Filter filt = new Filter(pin1, pout2);
			Consumer cons = new Consumer(pin2);

			/* start threads */

			prod.start();
			filt.start();
			cons.start();
		}
		catch (IOException e){}
	}
}

/**
  A thread that writes random numbers to an output stream.
 */
class Producer extends Thread
{ 
	/**
	  Constructs a producer thread.
	  @param os the output stream
	 */
	public Producer(OutputStream os)
	{ 
		out = new DataOutputStream(os);
	}

	public void run()
	{ 
		while (true)
		{ 
			try
			{ 
				double num = rand.nextDouble();
				out.writeDouble(num);
				out.flush();
				sleep(Math.abs(rand.nextInt() % 1000));
			}
			catch(Exception e)
			{ 
				System.out.println("Error: " + e);
			}
		}
	}

	private DataOutputStream out;
	private Random rand = new Random();
}

/**
  A thread that reads numbers from a stream and writes their
  average to an output stream.
 */
class Filter extends Thread
{ 
	/**
	  Constructs a filter thread.
	  @param is the output stream
	  @param os the output stream
	 */
	public Filter(InputStream is, OutputStream os)
	{ 
		in = new DataInputStream(is);
		out = new DataOutputStream(os);
	}

	public void run()
	{ 
		for (;;)
		{ 
			try
			{ 
				double x = in.readDouble();
				total += x;
				count++;
				if (count != 0) out.writeDouble(total / count);
			}
			catch(IOException e)
			{ 
				System.out.println("Error: " + e);
			}
		}
	}

	private DataInputStream in;
	private DataOutputStream out;
	private double total = 0;
	private int count = 0;
}

/**
  A thread that reads numbers from a stream and 
  prints out those that deviate from previous inputs
  by a threshold value. 
 */
class Consumer extends Thread
{ 
	/**
	  Constructs a consumer thread.
	  @param is the input stream
	 */  
	public Consumer(InputStream is)
	{  
		in = new DataInputStream(is);
	}

	public void run()
	{ 
		for(;;)
		{ 
			try
			{ 
				double x = in.readDouble();
				if (Math.abs(x - oldx) > THRESHOLD)
				{ 
					System.out.println(x);
					oldx = x;
				}
			}
			catch(IOException e)
			{ 
				System.out.println("Error: " + e);
			}
		}
	}

	private double oldx = 0;
	private DataInputStream in;
	private static final double THRESHOLD = 0.01;
}

