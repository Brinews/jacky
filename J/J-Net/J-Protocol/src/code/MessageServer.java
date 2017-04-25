import java.net.*; 
import java.io.*; 

public class MessageServer extends Thread
{ 
    protected Socket clientSocket;
    protected MessageBoard mesgBoard;
    protected char label;
    int id;

    public static void main(String[] args) throws IOException 
    { 
        ServerSocket serverSocket = null; 
        char startLabel = 'A';
        MessageBoard mb = new MessageBoard();
        int port = 8100;

        if (args.length > 0) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        try { 
            serverSocket = new ServerSocket(port); 
            /*
            System.out.println ("Connection Socket Created on port: "+
                    String.valueOf(port));
            */
            try { 
                while (true)
                {
                    //System.out.println ("Waiting for Connection");
                    new MessageServer(serverSocket.accept(),startLabel,mb); 
                    startLabel += 1;
                }
            } 
            catch (IOException e) 
            { 
                System.err.println("Accept failed."); 
                System.exit(1); 
            } 
        } 
        catch (IOException e) 
        { 
            System.err.println("Could not listen on port: "
                    + String.valueOf(port)); 
            System.exit(1); 
        } 
        finally
        {
            try {
                serverSocket.close(); 
            }
            catch (IOException e)
            { 
                System.err.println("Could not close port: "
                        +String.valueOf(port)); 
                System.exit(1); 
            } 
        }
    }

    private MessageServer (Socket clientSoc, char lbl, MessageBoard mboard)
    {
        clientSocket = clientSoc;
        mesgBoard = mboard;
        label = lbl;
        id = 0;

        start();
    }

    public void run()
    {
        //System.out.println ("New Communication Thread Started");

        try { 
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), 
                    true); 
            BufferedReader in = new BufferedReader( 
                    new InputStreamReader( clientSocket.getInputStream())); 

            String inputLine; 

            while ((inputLine = in.readLine()) != null) 
            { 
                System.out.println (inputLine); 

                if (inputLine.equalsIgnoreCase("list")) { // list
                    out.println(mesgBoard.listMessages());

                } else if (inputLine.indexOf(":")>0) {
                    String[] items = inputLine.split(":");

                    if (items[0].equalsIgnoreCase("send")) { // send
                        MessageHeader mh = new MessageHeader(label, ++id);
                        mesgBoard.addMessage(mh, items[1]);

                    } else if (items[0].equalsIgnoreCase("get")) { // get
                        if (items[1].charAt(1) == '+') {
                            char c = items[1].charAt(0);
                            int m = 0;
                            try {
                                m = Integer.parseInt(items[1].substring(2));
                            } catch(Exception e) {
                                e.printStackTrace();
                            }

                            MessageHeader mh = new MessageHeader(c, m);
                            String value = mesgBoard.getMessage(mh);

                            if (value != null) {
                                out.println(value);
                                System.out.println(items[1]+"="+value+"\nOK");
                            } else {
                                out.println("ERR");
                                System.out.println(items[1]+"=null\nERR");
                            }
                        }
                    } else {
                        out.println("ERR");
                        System.out.println(items[1]+"=null\nERR");
                    }
                }

                out.flush();
                //out.println(inputLine); 

                if (inputLine.equalsIgnoreCase("bye")) {
                    System.out.println("BYE");
                    break; 
                }
            } 

            out.close(); 
            in.close(); 
            clientSocket.close(); 
        } 
        catch (IOException e) 
        { 
            System.err.println("Problem with Communication Server");
            System.exit(1); 
        } 
    }
} 
