import java.net.*;
import java.io.*;

public class Generator extends Thread{

    private ServerSocket serverSocket;
    public int i;
    public Socket [] server;
    public DataOutputStream [] out;
    static boolean haveClient;
    static int clients;

    public Generator(int port) throws IOException{

        serverSocket = new ServerSocket(port);
        serverSocket.setSoTimeout(1000000);
        this.i=0;
        haveClient = false;
        out = new DataOutputStream[10];
        server = new Socket[10];
        clients = 0;
    }

    public void run(){

        while(true){
            try{
                System.out.println("Waiting for collector on port " + serverSocket.getLocalPort() + "...");
                server[clients] = serverSocket.accept();
                haveClient = true;
                System.out.println("Just connected to collector: " + server[clients].getRemoteSocketAddress());
                out[clients] = new DataOutputStream(server[clients].getOutputStream());
                clients=clients+1;
            }catch(SocketTimeoutException s){System.out.println("Socket timed out!");break;
            }catch(IOException e){e.printStackTrace();break;
            }catch(Exception e){e.printStackTrace();break;}

        }
    }

    public void printer() throws IOException{
        for(int j=0;j<clients;j++){
			if (server[j] != null && !server[j].isClosed()) {
				try {
					out[j].writeInt(i);
					out[j].flush();
				} catch (Exception e) {
					server[j].close();
				}
			}
        }
    }

    public void incI(){
        i++;
    }

    public static void main(String [] args){

        try{
			//System.out.println(args[0]);

			if (args.length < 2) {
				System.out.println("Usage: Generator <port> <frequency>");
				return;
			}

			int port = Integer.parseInt(args[0]);
			int freq = Integer.parseInt(args[1]);

            Generator t = new Generator(port);

            t.start();
            while(true){
                if(haveClient){t.printer();}
                t.incI();
                sleep(freq);
            }
        }catch(Exception e){e.printStackTrace();}
    }
}
