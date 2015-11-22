import java.net.*;
import java.io.*;

public class Calculator {

	public static void main(String[] args) {

		String serverName = "localhost";
		int port1 = 10000;

		StatisticsHelper not = new StatisticsHelper(serverName,port1);

		Thread notifications = new Thread(not);
		notifications.start();
	}
}
