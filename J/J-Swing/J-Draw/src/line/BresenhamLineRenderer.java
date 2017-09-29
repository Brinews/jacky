package line;

import geometry.Vertex;
import geometry.Vertex3D;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

public class BresenhamLineRenderer implements LineRenderer {
	// use the static factory make() instead of constructor.
	private BresenhamLineRenderer() {}

	
	/*
	 * (non-Javadoc)
	 * @see client.LineRenderer#drawLine(client.Vertex2D, client.Vertex2D, windowing.Drawable)
	 * 
	 * @pre: p2.x >= p1.x && p2.y >= p1.y
	 */
	@Override
	public void drawLine(Vertex3D p1, Vertex3D p2, Drawable drawable) {
        int x1 = p1.getIntX();
        int y1 = p1.getIntY();
        int x2 = p2.getIntX();
        int y2 = p2.getIntY();

        int argbColor = p1.getColor().asARGB();
        int dx = x2 - x1;
        int dy = y2 - y1;

        int ux = 1;

        if (dx > 0) ux = 1;
        else dx = -1;

        int uy = 1;

        if (dy > 0) uy = 1;
        else uy = -1;

        int x = x1, y = y1, eps;

        eps = 0;dx = Math.abs(dx); dy = Math.abs(dy); 
        if (dx > dy) 
        {
            for (x = x1; x != x2; x += ux)
            {
                drawable.setPixel(x, y, 0, argbColor);
                eps += dy;
                if ((eps << 1) >= dx)
                {
                    y += uy; eps -= dx;
                }
            }
        }
        else
        {
            for (y = y1; y != y2; y += uy)
            {
                drawable.setPixel(x, y, 0, argbColor);
                eps += dx;
                if ((eps << 1) >= dy)
                {
                    x += ux; eps -= dy;
                }
            }
        }       
    }

    public static LineRenderer make() {
        return new AnyOctantLineRenderer(new BresenhamLineRenderer());
    }
}
