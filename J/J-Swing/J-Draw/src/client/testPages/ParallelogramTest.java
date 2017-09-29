package client.testPages;

import geometry.Vertex3D;
import line.LineRenderer;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

public class ParallelogramTest {
	private static final int X1 = 20;
	private static final int Y1 = 80;
	private static final int X2 = 150;
	private static final int Y2 = 150;

	private static final int X3 = 160;
	private static final int Y3 = 270;
	private static final int X4 = 240;
	private static final int Y4 = 40;

	private static final int P1 = 0;
	private static final int P2 = 50;
	
	private final LineRenderer renderer;
	private final Drawable panel;

	public ParallelogramTest(Drawable panel, LineRenderer renderer) {
		this.panel = panel;
		this.renderer = renderer;
		
		render();
	}
	
	private void render() {		
        for (int p = P1; p <= P2; p++) {
            Vertex3D from = new Vertex3D(X1, Y1+p, 0, Color.WHITE);
            Vertex3D to = new Vertex3D(X2, Y2+p, 0, Color.WHITE);
            renderer.drawLine(from, to, panel);

            Vertex3D from1 = new Vertex3D(X3+p, Y3, 0, Color.WHITE);
            Vertex3D to1 = new Vertex3D(X4+p, Y4, 0, Color.WHITE);
            renderer.drawLine(from1, to1, panel);
        }
    }

}
