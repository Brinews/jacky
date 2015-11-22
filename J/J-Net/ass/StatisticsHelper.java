import java.net.*;
import java.io.*;

public class StatisticsHelper implements Runnable{

    Socket notifier;

	// read from server
    InputStream inFromServer;
    DataInputStream in;

    public StatisticsHelper(String serverName, int port) {
        try{
            this.notifier = new Socket(serverName, port);
            System.out.println("Connected to aggregator: " + notifier.getRemoteSocketAddress());

        }catch(IOException e){e.printStackTrace();}
    }

    public void run(){

        try {

            inFromServer = notifier.getInputStream();
            in =new DataInputStream(inFromServer);

			int length = in.readInt();
			int i = 0;
			int total = 0;

            while(i < length/4) {
				int val = in.readInt();
				i++;
				System.out.println("Read Data :" + val);

				total += val;
            }

			System.out.println("Average: " + total*4.0/length);

        }catch(IOException e){
			e.printStackTrace();
		}
    }
}
