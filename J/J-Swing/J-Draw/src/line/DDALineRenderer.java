package line;

import geometry.Vertex;
import geometry.Vertex3D;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

public class DDALineRenderer implements LineRenderer {
	// use the static factory make() instead of constructor.
	private DDALineRenderer() {}

	
	/*
	 * (non-Javadoc)
	 * @see client.LineRenderer#drawLine(client.Vertex2D, client.Vertex2D, windowing.Drawable)
	 * 
	 * @pre: p2.x >= p1.x && p2.y >= p1.y
	 */
	@Override
    public void drawLine(Vertex3D p1, Vertex3D p2, Drawable drawable) {

        int color = p1.getColor().asARGB();

        double x1 = p1.getX();
        double y1 = p1.getY();
        double x2 = p2.getX();
        double y2 = p2.getY();

        double dx=Math.abs(x2-x1);
        double dy=Math.abs(y2-y1);

        double step, x, y;
        int i;

        if(dx>=dy)
            step=dx;
        else
            step=dy;

        dx=dx/step;
        dy=dy/step;

        x=x1;
        y=y1;

        i=1;
        while(i<=step)
        {
            drawable.setPixel((int)x, (int)y, 0, color);
            x=x+dx;
            y=y+dy;
            i=i+1;
        }
    }

    public static LineRenderer make() {
        return new AnyOctantLineRenderer(new DDALineRenderer());
    }
}
