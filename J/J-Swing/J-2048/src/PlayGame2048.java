import java.awt.*;

import javax.swing.*;
public class PlayGame2048 
{
    public static void main ( String args[])
    {
        {
            try {
                UIManager.setLookAndFeel( UIManager.getCrossPlatformLookAndFeelClassName() );
            } catch (Exception e) {
                e.printStackTrace();
            }
            JFrame frame = new JFrame("PlayGame2048");
            frame.setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);

            frame.getContentPane().add(new PlayGame2048Panel());
            frame.pack();
            frame.setVisible(true);
        }
    }
}



