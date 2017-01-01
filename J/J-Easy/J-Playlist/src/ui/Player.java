import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.concurrent.locks.LockSupport;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

public class Player {

    private boolean isStop = true;
    private boolean isPause = false;

    AudioInputStream audioInputStream;
    AudioFormat audioFormat;
    SourceDataLine sourceDataLine;

    Thread playthread;
    Thread replaythread;

    public boolean isPause() {
        return isPause;
    }
    public void setPause(boolean isPause) {
        this.isPause = isPause;
    }
    public boolean IsStop(){
        return isStop ;
    }

    public void init(InputStream input) {
        try {

            audioInputStream = AudioSystem.getAudioInputStream(input);
            audioFormat = audioInputStream.getFormat();
            // change to mp3 format
            if (audioFormat.getEncoding() != AudioFormat.Encoding.PCM_SIGNED) {
                audioFormat = new AudioFormat(AudioFormat.Encoding.PCM_SIGNED,
                        audioFormat.getSampleRate(), 16,
                        audioFormat.getChannels(),
                        audioFormat.getChannels() * 2,
                        audioFormat.getSampleRate(), false);
                audioInputStream = AudioSystem.getAudioInputStream(audioFormat,
                        audioInputStream);
            }

            // open out device
            DataLine.Info dataLineInfo = new DataLine.Info(
                    SourceDataLine.class, audioFormat,
                    AudioSystem.NOT_SPECIFIED);
            sourceDataLine = (SourceDataLine) AudioSystem.getLine(dataLineInfo);
            sourceDataLine.open(audioFormat);
            sourceDataLine.start();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public class PlayThread extends Thread {
        byte tempBuffer[] = new byte[2048];

        public void run() {
            try {
                isStop = false ;
                int cnt;

                // read mp3 data
                while ((cnt = audioInputStream.read(tempBuffer, 0,
                                tempBuffer.length)) != -1) {

                    if (cnt > 0) {
                        // write to device
                        sourceDataLine.write(tempBuffer, 0, cnt);
                    }

                    if (isPause == true) {
                        LockSupport.park();
                    }
                }

                System.out.println("Over");

                // Block for data over
                sourceDataLine.drain();
                sourceDataLine.close();

                isStop = true ;
            } catch (Exception e) {
                e.printStackTrace();
                System.exit(0);
            }
        }
    }

    public void play(InputStream in) {
        init(in);
        play() ;
    }

    public void play(File file) {
        try {
            play(new FileInputStream(file));
        } catch (FileNotFoundException e) {
            System.out.println("File open error\n");
        }
    }

    public void play() {

        if (isPause == false) {
            if (playthread != null) {
                playthread.stop();
                playthread = new PlayThread();
                playthread.start();
                System.out.println("Thread exist, recreate");
            } else {
                playthread = new PlayThread();
                playthread.start();
                System.out.println("Create new thread.");
            }
        }
    }

    public void replay(){
        replaythread = new RePlayThread() ;
        replaythread.start();
    }

    public void stop() throws UnsupportedAudioFileException, 
           IOException,
           LineUnavailableException {
        if(playthread!=null){
            playthread.stop();
            playthread = new PlayThread() ;
        }

        if(!(replaythread==null)){
            replaythread.stop();
        }
    }

    public void pause() {
        if (!(replaythread == null)) {
            replaythread.stop();
        }
        isPause = true;
    }

    class RePlayThread extends Thread {
        @Override
        public void run() {
            while (true) {
                LockSupport.unpark(playthread);
            }
        }
    }
}
