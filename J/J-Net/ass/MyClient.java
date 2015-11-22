import java.net.*;
import java.io.*;

public class MyClient {

        public static void main(String[] args) {

            String serverName = "localhost";
            int port1 = 10000;
            int port2 = 10001;

            MyNotificationListener not = new MyNotificationListener(serverName,port2);
            Thread notifications = new Thread(not);
            notifications.start();

        }
}
