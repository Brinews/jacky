package client.testPages;

import geometry.Vertex3D;
import line.LineRenderer;
import windowing.drawable.Drawable;
import windowing.graphics.Color;
import polygon.PolygonRenderer;

import polygon.Polygon;
import java.util.*;

public class RandomPolygonTest {
	private static final int NUMS = 20;
	private static final int MAX = 299;
	
	private final PolygonRenderer renderer;
	private final Drawable panel;
    private Random rand = new Random();

	public RandomPolygonTest(Drawable panel, PolygonRenderer pr) {
		this.panel = panel;
		this.renderer = pr;
		
		render();
	}
	
	private void render() {		
		
        for (int i = 0; i < NUMS; i++) {
            Color c = randColor();
            Vertex3D p1 = new Vertex3D(rand.nextInt(MAX),
                    rand.nextInt(MAX), 0.0, c);
            Vertex3D p2 = new Vertex3D(rand.nextInt(MAX),
                    rand.nextInt(MAX), 0.0, c);
            Vertex3D p3 = new Vertex3D(rand.nextInt(MAX),
                    rand.nextInt(MAX), 0.0, c);

            Polygon pg = Polygon.make(p1, p2, p3);
            renderer.drawPolygon(pg, panel, null);
        }
	}

    private Color randColor() {
        return new Color(rand.nextDouble(), rand.nextDouble(),
                rand.nextDouble());
    }


}
