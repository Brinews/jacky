import java.net.*;
import java.io.*;

public class DataHelper implements Runnable{

    Socket notifier;
	int myport;
	DataOutputStream myoutput;

	// read from server
    InputStream inFromServer;
    DataInputStream in;

	// write to server(aggregator)
	OutputStream outToServer;
	DataOutputStream out;

    public DataHelper(String serverName, int port, DataOutputStream dos) {
		myport = port;
		myoutput = dos;

        try{
            this.notifier = new Socket(serverName, port);
            System.out.println("Connected to generator: " + notifier.getRemoteSocketAddress());

        }catch(IOException e){e.printStackTrace();}
    }

    public void run(){

        try {

            inFromServer = notifier.getInputStream();
            in =new DataInputStream(inFromServer);

			outToServer = notifier.getOutputStream();
			out = new DataOutputStream(outToServer);

            while(true){
				int val = in.readInt();
				System.out.println("Collect Data from port <"+myport+"> :" + val);
				// write to destination file
				myoutput.writeInt(val);
				myoutput.flush();
            }

        }catch(IOException e){
			e.printStackTrace();
		}
    }

	public Socket getSocket() {
		return notifier;
	}
}
