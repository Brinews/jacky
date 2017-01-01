import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;

public class MediaPlayer {
	private Player player;

	public MediaPlayer() {
		player = new Player();
	}

	public boolean getPause() {
		return player.isPause();
	}

	public boolean isStop() {
		return player.IsStop();
	}

	public void Play(String string) {
        player.play(new File(string));
	}

	public void Play() {
		player.replay();
	}

	public void Pause() {
		player.pause();
	}

	public void Stop() {
		try {
			player.stop();
		} catch (UnsupportedAudioFileException | IOException
				| LineUnavailableException e) {
			e.printStackTrace();
		}
	}
}
