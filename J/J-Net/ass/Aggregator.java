import java.net.*;
import java.io.*;
import java.util.Map;
import java.util.HashMap;
import java.util.Scanner;

public class Aggregator extends Thread{

    private ServerSocket serverSocket;
    public int i;
    public Socket server;

	public DataOutputStream out;
	public DataInputStream  in;

    static boolean haveClient;

	final int MAX = 10;

    public Aggregator(int port) throws IOException{
        serverSocket = new ServerSocket(port);
        serverSocket.setSoTimeout(1000000);
        haveClient = false;
	}

    public void run(){

        while(true){
            try{
                System.out.println("Waiting for Calculator on port " + serverSocket.getLocalPort() + "...");

                server = serverSocket.accept();
                haveClient = true;

                System.out.println("Send collect data to " + server.getRemoteSocketAddress());

                out = new DataOutputStream(server.getOutputStream());

            }catch(SocketTimeoutException s){
				System.out.println("Socket timed out!");break;
            }catch(IOException e){
				e.printStackTrace();break;
            }catch(Exception e){
				e.printStackTrace();break;
			}
        }
    }

    public void sendCollectDataFile(String fileName) throws IOException{
		// 1. read data-collect file to buf
		File file = new File(fileName);
		FileInputStream fis = new FileInputStream(file);

		int length = fis.available();

		DataInputStream dis = new DataInputStream(
				new BufferedInputStream(fis));

		// 2. send file-length to calculator
		out.writeInt(length);
		out.flush();
		
		// 3. read data and send to calculator
		byte[] data = new byte[8192];
		int readLen = 0;

		while (readLen < length) {

			int len = dis.read(data);

			if (len == -1) break;

			out.write(data, 0, len);

			readLen += len;
		}
    }

    public static void main(String [] args){

		Map<Integer, Socket> aggMap = new HashMap<Integer, Socket>();
		Map<Integer, Thread> runMap = new HashMap<Integer, Thread>();

        try{
			if (args.length < 1) {
				System.out.println("Usage: Aggregator <port> ... <port>");
				return;
			}
			// 1. collect data
			
			// data collect file
			String filename = System.currentTimeMillis() + ".txt";

			File fp = new File(filename);
			if (!fp.exists()) {
				try {
					fp.createNewFile();
				} catch(IOException e){
					e.printStackTrace();
				}
			}

			DataOutputStream outfile = new DataOutputStream(
					new BufferedOutputStream(
						new FileOutputStream(fp)));

			for (int i = 0; i < args.length; i++) {
				String serverName = "localhost";
				int port = Integer.parseInt(args[i]);

				DataHelper dh = new DataHelper(serverName, port, outfile);
				Thread notif = new Thread(dh);
				notif.start();

				aggMap.put(port, dh.getSocket());
				runMap.put(port, notif);
			}

			// controller thread
			Thread control = new Thread(new Controller(aggMap, runMap));
			control.start();
			
			// 2. send data to calculator
            Aggregator t = new Aggregator(10000);
            t.start();

            while (true){
                if(haveClient) {
					t.sendCollectDataFile(filename);
					haveClient = false;
				}
                sleep(1000);
            }
        }catch(Exception e){
			e.printStackTrace();
		}
    }
}

class Controller implements Runnable {
	public Map<Integer, Socket> genList;
	public Map<Integer, Thread> runList;

	public Controller(Map<Integer, Socket> l, Map<Integer, Thread> t) {
		genList = l;
		runList = t;
		System.out.println("Input port number to shutdown generator:");
	}

	public void run() {
		while (true) {
			try {
				Scanner in = new Scanner(System.in);
				int port = in.nextInt();

				if (genList.containsKey(port)) {
					runList.get(port).stop();
					genList.get(port).shutdownInput();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
