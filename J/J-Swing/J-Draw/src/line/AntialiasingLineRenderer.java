package line;

import geometry.Vertex;
import geometry.Vertex3D;
import windowing.drawable.Drawable;
import windowing.graphics.Color;

public class AntialiasingLineRenderer implements LineRenderer {
	// use the static factory make() instead of constructor.
	private AntialiasingLineRenderer() {}

    private Drawable dw;

    // integer part of x
    int ipart(double x){
        return (int)Math.floor(x);
    }

    int round(double x) {
        return ipart(x + 0.5);
    }

    // fractional part of x
    double fpart(double x) {
        return x - Math.floor(x);
    }

    double rfpart(double x) {
        return 1 - fpart(x);
    }

    void plot(double x, double y, double c) {
        dw.setPixel((int) x, (int) y, 0, (int)c);
    }
    
	/*
	 * (non-Javadoc)
	 * @see client.LineRenderer#drawLine(client.Vertex2D, client.Vertex2D, windowing.Drawable)
	 * 
	 * @pre: p2.x >= p1.x && p2.y >= p1.y
	 */
	@Override
	public void drawLine(Vertex3D p1, Vertex3D p2, Drawable drawable) {

        dw = drawable;

        double x0 = p1.getX();
        double y0 = p1.getY();
        double x1 = p2.getX();
        double y1 = p2.getY();

        boolean steep = Math.abs(y1 - y0) > Math.abs(x1 - x0);
    
        if (steep) {
            //swap(x0, y0);
            double temp = x0;
            x0 = y0;
            y0 = temp;
            //swap(x1, y1);
            temp = x1;
            x1 = y1;
            y1 = temp;
        }

        if (x0 > x1) {
            //swap(x0, x1);
            double temp = x0;
            x0 = x1;
            x1 = temp;
            //swap(y0, y1);
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
    
        double dx = x1 - x0;
        double dy = y1 - y0;
        double gradient = dy / dx;
        if (dx == 0.0) { 
            gradient = 1.0;
        }

        // handle first endpoint
        double xend = round(x0);
        double yend = y0 + gradient * (xend - x0);
        double xgap = rfpart(x0 + 0.5);
        double xpxl1 = xend; // this will be used in the main loop
        double ypxl1 = ipart(yend);

        if (steep) { 
            plot(ypxl1,   xpxl1, rfpart(yend) * xgap);
            plot(ypxl1+1, xpxl1,  fpart(yend) * xgap);
        }
        else {
            plot(xpxl1, ypxl1  , rfpart(yend) * xgap);
            plot(xpxl1, ypxl1+1,  fpart(yend) * xgap);
        }

        double intery = yend + gradient; // first y-intersection for the main loop
    
        // handle second endpoint
        xend = round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = fpart(x1 + 0.5);
        double xpxl2 = xend; //this will be used in the main loop
        double ypxl2 = ipart(yend);
        if (steep) {
            plot(ypxl2  , xpxl2, rfpart(yend) * xgap);
            plot(ypxl2+1, xpxl2,  fpart(yend) * xgap);
        }
        else {
            plot(xpxl2, ypxl2,  rfpart(yend) * xgap);
            plot(xpxl2, ypxl2+1, fpart(yend) * xgap);
        }

        // main loop
        if (steep) {
            for (double x = xpxl1 + 1; x <=  xpxl2 - 1; x++) {
                plot(ipart(intery)  , x, rfpart(intery));
                plot(ipart(intery)+1, x,  fpart(intery));
                intery = intery + gradient;
            }
        }
        else {
            for (double x = xpxl1 + 1; x <= xpxl2 - 1; x++) { 
                plot(x, ipart(intery),  rfpart(intery));
                plot(x, ipart(intery)+1, fpart(intery));
                intery = intery + gradient;
            }
        }
	}

	public static LineRenderer make() {
		return new AnyOctantLineRenderer(new AntialiasingLineRenderer());
	}
}
