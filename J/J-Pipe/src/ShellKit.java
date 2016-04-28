import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Update: 2016-03-25 14:23
 */
public class ShellKit {
    /**
     * Run java command starting with 'java'
     *
     * @param cmd command list to execute
     * @return  Any Output
     */
    public static String adb(String[] cmd, String input) {
        ArrayList<String> cmds = new ArrayList<>();

        cmds.add("java");
        for (int i = 0; i < cmd.length; i++) {
            cmds.add(cmd[i]) ;
        }

        return command(cmds, input);
    }

    /**
     * Run java command starting with 'java'
     *
     * @param cmd command to execute ,tokenize with whitespace
     * @return  Any Output
     */
    public static String adb(String cmd, String input) {
        String[] splits = cmd.split(" ");
        return adb(splits, input);
    }

    
    /**
     * Execute a java program and return its output
     *
     * @param command command to execute
     * @return process executed
     */
    public static String command(List<String> command, String input) {
        //set redirectErrorStream to be true to cross output streams
        ProcessBuilder pb = new ProcessBuilder(command).redirectErrorStream(true);
        String output = "";

        try {
            Process process = pb.start();

			if (input != null) {
				OutputStream out = process.getOutputStream();
				for (int i = 0; i < input.length(); i++) {
					out.write((int)input.charAt(i));
				}
				out.flush();
			}

            IOThreadHandler outputHandler = new IOThreadHandler(
                    process.getInputStream());


            outputHandler.start();

            //wait for the process to stop
            process.waitFor();

            //in case the process stopped before the thread
            outputHandler.join();

            output = outputHandler.getOutput();

        } catch (InterruptedException | IOException e) {
            e.printStackTrace();
        }

        return output;
    }


    /**
     * Thread to drain the output of cmd running
     */
    private static class IOThreadHandler extends Thread {
        private InputStream inputStream;
        private StringBuilder output = new StringBuilder();

        IOThreadHandler(InputStream inputStream) {
            this.inputStream = inputStream;
        }

        public void run() {
            try (Scanner br = new Scanner(new InputStreamReader(inputStream))) {
                String line = null;
                while (br.hasNextLine()) {
                    line = br.nextLine();
                    output.append(line).append(System.getProperty("line.separator"));
                }
            }
        }

        public String getOutput() {
            return output.toString();
        }
    }

	public static String readLine() throws java.io.IOException
    {
        StringBuffer s = new StringBuffer();
        char c;
        while ((c = (char)System.in.read())!= '\n')
        {
            if (c == '\b' && s.length() > 0)
                s.setLength(s.length() - 1);
            else
                s.append(c);
        }
        return s.toString();
    }


    //mininal unit test
    public static void main(String[] args) {

		String line = "";

		while (true) {
			System.out.print("$");

			try {
				line = ShellKit.readLine();
			} catch (Exception e) {
			}

			line = line.trim();

			if (line.equals("")) continue;
			if (line.equals("exit")) break;


			String[] commands = line.split("\\|");
			String output = null;

			for (int i = 0; i < commands.length; i++) {
				output = ShellKit.adb(commands[i], output);
			}

			System.out.println(output);
		}
	}
}

