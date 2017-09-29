package client.testPages;

import geometry.Vertex3D;
import line.LineRenderer;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

import polygon.PolygonRenderer;
import polygon.Polygon;

import java.util.*;

public class StarburstPolygonTest {
	private static final int NUM_RAYS = 90;
	private static final double FRACTION_OF_PANEL_FOR_DRAWING = 0.9;
	
	private final PolygonRenderer renderer;
	private final Drawable panel;
	Vertex3D center;
    private Random rand = new Random();

	public StarburstPolygonTest(Drawable panel, PolygonRenderer renderer) {
		this.panel = panel;
		this.renderer = renderer;
		
        makeCenter(randColor());
		//render();
		render();
	}

    private void render1()
    {
        rand.setSeed(13301);

        Vertex3D p1 = new Vertex3D(10, 10, 0, Color.WHITE);
        Vertex3D p2 = new Vertex3D(20, 50, 0, Color.WHITE);
        Vertex3D p3 = new Vertex3D(30, 40, 0, Color.WHITE);

        Polygon pg = Polygon.make(p1, p2, p3);
        renderer.drawPolygon(pg, panel, null);
    }
	
	private void render() {		
		double radius = computeRadius();
		double angleDifference = (2.0 * Math.PI) / NUM_RAYS;
		double angle = 0.0;

        Color c = randColor();
        Vertex3D radialPoint1 = radialPoint(radius, angle, c);
		
		for(int ray = 1; ray <= NUM_RAYS+1; ray++) {
			Vertex3D radialPoint2 = radialPoint(radius, angle, c);
            makeCenter(c);

            Polygon pg = Polygon.make(center, radialPoint1, radialPoint2);
			renderer.drawPolygon(pg, panel, null);

            radialPoint1 = radialPoint2;
			
            c = randColor();
			angle = angle + angleDifference;
		}
	}

    private Color randColor() {
        return new Color(rand.nextDouble(), rand.nextDouble(),
                rand.nextDouble());
    }

	private void makeCenter(Color c) {
		int centerX = panel.getWidth() / 2;
		int centerY = panel.getHeight() / 2;
		center = new Vertex3D(centerX, centerY, 0, c);
	}

	private Vertex3D radialPoint(double radius, double angle, Color c) {
		double x = center.getX() + radius * Math.cos(angle);
		double y = center.getY() + radius * Math.sin(angle);
		return new Vertex3D(x, y, 0, c);
	}
	
	private double computeRadius() {
		int width = panel.getWidth();
		int height = panel.getHeight();
		
		int minDimension = width < height ? width : height;
		
		return (minDimension / 2.0) * FRACTION_OF_PANEL_FOR_DRAWING;
	}
}
