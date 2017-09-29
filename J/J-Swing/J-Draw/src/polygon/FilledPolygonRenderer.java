package polygon;

import polygon.Shader;
import windowing.drawable.Drawable;
import geometry.Vertex3D;
import windowing.graphics.Color;

public class FilledPolygonRenderer implements PolygonRenderer {
    public FilledPolygonRenderer() {
    }

    public void drawLine(Vertex3D p1, Vertex3D p2, Drawable drawable) {
		double deltaX = p2.getIntX() - p1.getIntX();
		double deltaY = p2.getIntY() - p1.getIntY();
		
		double slope = deltaY / deltaX;
		double intercept = p2.getIntY() - slope * p2.getIntX();
		int argbColor = p1.getColor().asARGB();
		
		for(int x = p1.getIntX(); x <= p2.getIntX(); x++) {
			int y = (int)(slope * x + intercept);
			drawable.setPixel(x, y, 0.0, argbColor);
		}
	}

    public double dotMult(Vertex3D p, Vertex3D q) {
        return p.getX()*q.getX()+p.getY()*q.getY()+p.getZ()*q.getZ();
    }

    boolean pointinTriangle(Vertex3D A, Vertex3D B, Vertex3D C, Vertex3D P)
    {
        Vertex3D v0 = C.subtract(A);
        Vertex3D v1 = B.subtract(A);
        Vertex3D v2 = P.subtract(A);

        double dot00 = dotMult(v0, v0);
        double dot01 = dotMult(v0, v1);
        double dot02 = dotMult(v0, v2);
        double dot11 = dotMult(v1, v1);
        double dot12 = dotMult(v1, v2);

        double inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

        double u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
        if (u < 0 || u > 1) // if u out of range, return directly
        {
            return false ;
        }

        double v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
        if (v < 0 || v > 1) // if v out of range, return directly
        {
            return false ;
        }

        return u + v <= 1 ;
    }

    @Override
    public void drawPolygon(Polygon polygon, Drawable drawable, Shader vertexShader) {
        double xMin, xMax, yMin, yMax;

        Vertex3D p1 = polygon.get(0);
        Vertex3D p2 = polygon.get(1);
        Vertex3D p3 = polygon.get(2);

        xMin = Math.min(Math.min(p1.getX(), p2.getX()), p3.getX());
        xMax = Math.max(Math.max(p1.getX(), p2.getX()), p3.getX());

        yMin = Math.min(Math.min(p1.getY(), p2.getY()), p3.getY());
        yMax = Math.max(Math.max(p1.getY(), p2.getY()), p3.getY());

        Color c = p1.getColor();

		int argbColor = p1.getColor().asARGB();

        for (double x1 = xMin ; x1 < xMax; x1++) {
            for (double y1 = yMin; y1 < yMax; y1++) {
                Vertex3D p = new Vertex3D(x1, y1, 0.0, c);

                if (pointinTriangle(p1, p2, p3, p)) {
                    drawable.setPixel((int)x1, (int)y1, 0.0, argbColor);
                }
            }
        }
    }

    //@Override
    public void drawPolygon1(Polygon polygon, Drawable drawable, Shader vertexShader) {
        Vertex3D p1 = polygon.get(1);
        Vertex3D p2 = polygon.get(2);

        double deltaX = p2.getIntX() - p1.getIntX();
		double deltaY = p2.getIntY() - p1.getIntY();
		
		double slope = deltaY / deltaX;
		double intercept = p2.getIntY() - slope * p2.getIntX();
		int argbColor = p1.getColor().asARGB();
		
		for(int x = p1.getIntX(); x <= p2.getIntX(); x++) {
			int y = (int)(slope * x + intercept);
			//drawable.setPixel(x, y, 0.0, argbColor);
            drawLine(polygon.get(0), new Vertex3D(x, y, 0.0, p1.getColor()), drawable);
		}
    }

    public static PolygonRenderer make() {
        return new FilledPolygonRenderer();
    }
}
