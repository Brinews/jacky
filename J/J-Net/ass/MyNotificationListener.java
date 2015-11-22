import java.net.*;
import java.io.*;

public class MyNotificationListener implements Runnable{

    Socket notifier;
    InputStream inFromServer;
    DataInputStream in;

    public MyNotificationListener(String serverName, int port){
        try{
            this.notifier = new Socket(serverName, port);
            System.out.println("Just connected to " + notifier.getRemoteSocketAddress());

        }catch(IOException e){e.printStackTrace();}
    }

    public void run(){

        try{

            inFromServer = notifier.getInputStream();
            in =new DataInputStream(inFromServer);

            while(true){
                    System.out.println("Server says " + in.readInt());
            }
        }catch(IOException e){e.printStackTrace();}
    }
}
