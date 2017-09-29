package client.testPages;

import geometry.Vertex3D;
import line.LineRenderer;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

import polygon.PolygonRenderer;
import polygon.Polygon;

import java.util.*;

public class MeshPolygonTest {
	private static final int NUMS = 10;
	private static final double LEN = 30;
	
	private final PolygonRenderer renderer;
	private final Drawable panel;
    private Random rand = new Random();

    public static final int NO_PERTURBATION = 0;
    public static final int USE_PERTURBATION = 1;

    private int type;

	public MeshPolygonTest(Drawable panel, PolygonRenderer renderer, int p) {
		this.panel = panel;
		this.renderer = renderer;
        type = p;
		
        if (type == NO_PERTURBATION) {
            render();
        } else {
            render1();
        }
	}
	
	private void render() {		

        double startX = 10, startY = 10;
		
        for (int i = 0; i < NUMS; i++) {
            for (int j = 0; j < NUMS; j++) {
                Color c = randColor();
                Vertex3D p1 = new Vertex3D(startX+i*LEN, startY+j*LEN, 0.0, c);
                Vertex3D p2 = new Vertex3D(startX+i*LEN, startY+(1+j)*LEN, 0.0, c);
                Vertex3D p3 = new Vertex3D(startX+(i+1)*LEN, startY+j*LEN, 0.0, c);

                Polygon pg = Polygon.make(p1, p2, p3);
                renderer.drawPolygon(pg, panel, null);

                c = randColor();
                Vertex3D p4 = new Vertex3D(startX+(i+1)*LEN, startY+(1+j)*LEN, 0.0, c);
                pg = Polygon.make(p4, p2, p3);
                renderer.drawPolygon(pg, panel, null);
            }
        }
	}

    void copyPoints(double []x1, double []y1, double []x, double []y) {
        for (int i = 0; i < NUMS; i++) {
            x[i] = x1[i];
            y[i] = y1[i];
        }
    }

    int randLen() {
        if (rand.nextDouble() < 0.5) {
            return rand.nextInt(10);
        } else {
            return -1*rand.nextInt(10);
        }
    }

    void genPoints(double []x, double []y, int steps) {
        for (int i = 0; i < NUMS; i++) {
            x[i] = steps*LEN+randLen();
            y[i] = LEN*i+randLen();
        }
    }
	
	private void render1() {		

        double startX = 10, startY = 10;

        double[] x = new double[NUMS];
        double[] y = new double[NUMS];

        double[] x1 = new double[NUMS];
        double[] y1 = new double[NUMS];

        genPoints(x, y, 0);
		
        for (int i = 1; i < NUMS; i++) {
            genPoints(x1, y1, i);

            for (int j = 0; j < NUMS-1; j++) {
                Color c = randColor();
                Vertex3D p1 = new Vertex3D(x[j], y[j], 0.0, c);
                Vertex3D p2 = new Vertex3D(x[j+1], y[j+1], 0.0, c);
                Vertex3D p3 = new Vertex3D(x1[j], y1[j], 0.0, c);

                Polygon pg = Polygon.make(p1, p2, p3);
                renderer.drawPolygon(pg, panel, null);

                c = randColor();
                Vertex3D p4 = new Vertex3D(x1[j+1], y1[j+1], 0.0, c);
                pg = Polygon.make(p4, p2, p3);
                renderer.drawPolygon(pg, panel, null);
            }

            copyPoints(x1, y1, x, y);
        }
    }

    private Color randColor() {
        return new Color(rand.nextDouble(), rand.nextDouble(),
                rand.nextDouble());
    }

}
