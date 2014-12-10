package edu;

class LinePoints {
	public double x;
	public double y;
	
	public LinePoints()
	{
		x = y = 0;
	}
}

public class GaussPoints {

	public static int GPNUM = 10;
	public LinePoints[] lpts = new LinePoints[GPNUM];
	
	public GaussPoints(){
		for (int i = 0; i < GPNUM; i++) 
			lpts[i] = new LinePoints();
	}
	
	/* reset points */
	public void resetPoints()
	{
		for (int i = 0; i < GPNUM; i++) {
			lpts[i].x = lpts[i].y = 0;
		}
	}
	
	/* random points yield */
	public void generateRandPoints(double max)
	{
		for (int i = 0; i < GPNUM; i++) {
			lpts[i].x = (int)(Math.random() * max - max/2.0);
			//lpts[i].y = (int)(lpts[i].x * lpts[i].x*0.5 + 2 * lpts[i].x);
			lpts[i].y = (int)(lpts[i].x + Math.random()*90);
		}
	}
	
	public LinePoints[] getAllPoints() {
		return lpts;
	}
	
	/* set points value by index */
	public void setPointXAt(double x, int i)
	{
		if (i < 0 || i > GPNUM) return;
		
		lpts[i].x = x;
	}
	
	public void setPointYAt(double y, int i)
	{
		if (i < 0 || y > GPNUM) return;
		
		lpts[i].y = y;
	}
	
	/* get points number */
	public int getSize()
	{
		return GPNUM;
	}
	
	/* calculate X means */
	public double getXpointMeans()
	{
		double means = 0.0;
		
		for (int i = 0; i < GPNUM; i++) {
			means += lpts[i].x;
		}
		
		return means / GPNUM;
	}
	
	/* calculate Y means */
	public double getYpointMeans()
	{
		double means = 0.0;
		
		for (int i = 0; i < GPNUM; i++) {
			means += lpts[i].y;
		}
		
		return means / GPNUM;
	}
	
	/* calculate M */
	public double getLineSlope()
	{
		double xmean, ymean;
		xmean = getXpointMeans();
		ymean = getYpointMeans();
		
		double fenzi = 0.0, fenmu = 0.0;
		
		for (int i = 0; i < GPNUM; i++) {
			fenzi += (lpts[i].x-xmean)*(lpts[i].y-ymean);
			fenmu += (lpts[i].x-xmean)*(lpts[i].x-xmean);
		}
		
		if (fenmu == 0) {
			//System.out.println("Divide by Zero error.");
			return 0.0;
		}
		
		return fenzi / fenmu;
	}
	
	/* calculate y-intercept */
	public double getLineOffsetY()
	{
		return getYpointMeans() - getLineSlope()*getXpointMeans();
	}
	
	/* calculate x-intercept */
	public double getLineOffsetX()
	{
		if (getLineSlope() == 0.0) return 0.0;
		
		return -getLineOffsetY()/getLineSlope();
	}
	
	/* calculate new Y by x */
	public double getNewLineY(double x)
	{
		return getLineSlope()*x + getLineOffsetY();
	}
	
	/* get points (x,y) */
	public double getPointXAt(int i) {
		return lpts[i].x;
	}
	
	public double getPointYAt(int i) {
		return lpts[i].y;
	}
}
