import java.io.*;
import java.net.*;
import java.util.*;

public class MessageClient {

    public static void main(String[] args) throws IOException {

        String serverHostname = new String ("127.0.0.1");
        int serverHostPort = 8100;

        if (args.length > 0)
            serverHostname = args[0];

        if (args.length > 1) {
            try {
                serverHostPort = Integer.parseInt(args[1]);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        /*
        System.out.println ("Attemping to connect to host " +
                serverHostname + " on port " + String.valueOf(serverHostPort));
        */

        Socket echoSocket = null;
        PrintWriter out = null;
        BufferedReader in = null;

        try {
            echoSocket = new Socket(serverHostname, serverHostPort);

            out = new PrintWriter(echoSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(
                        echoSocket.getInputStream()));

        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: " + serverHostname);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for "
                    + "the connection to: " + serverHostname);
            System.exit(1);
        }

        BufferedReader stdIn = new BufferedReader(
                new InputStreamReader(System.in));
        String userInput;

        System.out.printf("Type Message (\"BYE\" to quit)\n? ");

        while ((userInput = stdIn.readLine()) != null) 
        {
            if (userInput == "") continue;

            out.println(userInput);
            out.flush();

            // end loop
            if (userInput.equalsIgnoreCase("bye"))
                break;

            if (userInput.equalsIgnoreCase("list")) {
                String ret = in.readLine();
                if (!ret.equals("NULL")) {
                    String[] keys = ret.split("##");
                    Arrays.sort(keys);
                    for (int i = 0; i < keys.length; i++) {
                        System.out.println(keys[i]);
                    }
                }
            } else if (userInput.indexOf(":") > 0){
                String[] items = userInput.split(":");
                if (items[0].equalsIgnoreCase("get")) {
                    System.out.println(in.readLine());
                }
            }

            System.out.printf("? ");
        }

        out.close();
        in.close();
        stdIn.close();
        echoSocket.close();
    }
}

