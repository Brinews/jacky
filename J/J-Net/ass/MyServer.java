import java.net.*;
import java.io.*;

public class MyServer extends Thread{

    private ServerSocket serverSocket;
    public int i;
    public Socket [] server;
    public DataOutputStream [] out;
    static boolean haveClient;
    static int clients;

    public MyServer(int port) throws IOException{

        serverSocket = new ServerSocket(port);
        serverSocket.setSoTimeout(1000000);
        this.i=0;
        haveClient = false;
        out = new DataOutputStream[3];
        server = new Socket[3];
        clients = 0;
    }

    public void run(){

        while(true){
            try{
                System.out.println("Waiting for client on port " + serverSocket.getLocalPort() + "...");
                server[clients] = serverSocket.accept();
                haveClient = true;
                System.out.println("Just connected to " + server[clients].getRemoteSocketAddress());
                out[clients] = new DataOutputStream(server[clients].getOutputStream());
                clients=clients+1;
            }catch(SocketTimeoutException s){System.out.println("Socket timed out!");break;
            }catch(IOException e){e.printStackTrace();break;
            }catch(Exception e){e.printStackTrace();break;}

        }
    }

    public void printer() throws IOException{
        for(int j=0;j<clients;j++){
            out[j].writeInt(i);
            out[j].flush();
        }
    }

    public void incI(){
        i++;
    }

    public static void main(String [] args){

        try{
            MyServer t = new MyServer(10001);
            t.start();
            while(true){
                if(haveClient){t.printer();}
                t.incI();
                sleep(1000);
            }
        }catch(Exception e){e.printStackTrace();}
    }
}
