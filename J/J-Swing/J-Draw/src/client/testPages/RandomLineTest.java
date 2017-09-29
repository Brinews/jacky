package client.testPages;

import geometry.Vertex3D;
import line.LineRenderer;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

import java.util.*;

public class RandomLineTest {
	private static final int COUNT = 30;
	private static final int END = 299;

    private static int seed = 13;
	
	private final LineRenderer renderer;
	private final Drawable panel;
    private Random rand = new Random();

	public RandomLineTest(Drawable panel, LineRenderer renderer) {
		this.panel = panel;
		this.renderer = renderer;
		
		render();
	}
	
	private void render() {		
        rand.setSeed(13301+seed);
        for (int i = 0; i < COUNT; i++) {
            Color c = randColor();

            Vertex3D start = randPoint(c);
            Vertex3D end = randPoint(c);

            //System.out.println(c.toString());

            while (end.getX() == start.getX() 
                    && end.getY() == start.getY()) {
                end = randPoint(c);
            }

            //System.out.println(start.toString());
            //System.out.println(end.toString());

            renderer.drawLine(start, end, panel);
        }
	}

    private Color randColor() {
        return new Color(rand.nextDouble(), rand.nextDouble(),
                rand.nextDouble());
    }

	private Vertex3D randPoint(Color c) {
        int x = rand.nextInt(END+1);
        int y = rand.nextInt(END+1);
		return new Vertex3D(x, y, 0, c);
	}
}
