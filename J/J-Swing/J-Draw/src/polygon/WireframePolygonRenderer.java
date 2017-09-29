package polygon;

import polygon.Shader;
import windowing.drawable.Drawable;

public class WireframePolygonRenderer implements PolygonRenderer {
    public WireframePolygonRenderer () {
    }

	// assumes polygon is ccw.
    @Override
	public void drawPolygon(Polygon polygon, Drawable drawable, Shader vertexShader) {
    }

    public static PolygonRenderer make() {
		return new WireframePolygonRenderer();
    }
}
