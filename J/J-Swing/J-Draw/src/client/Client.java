package client;

import client.testPages.StarburstLineTest;
import geometry.Point2D;
import line.AlternatingLineRenderer;
import line.ExpensiveLineRenderer;
import line.LineRenderer;

import windowing.drawable.ColoredDrawable;
import client.testPages.MeshPolygonTest;
import client.testPages.ParallelogramTest;
import client.testPages.RandomLineTest;
import client.testPages.RandomPolygonTest;
import client.testPages.StarburstPolygonTest;
import line.AntialiasingLineRenderer;
import line.BresenhamLineRenderer;
import line.DDALineRenderer;
import polygon.FilledPolygonRenderer;
import polygon.WireframePolygonRenderer;

/*
import notProvided.client.ColoredDrawable;
import notProvided.client.testpages.MeshPolygonTest;
import notProvided.client.testpages.ParallelogramTest;
import notProvided.client.testpages.RandomLineTest;
import notProvided.client.testpages.RandomPolygonTest;
import notProvided.client.testpages.StarburstPolygonTest;
import notProvided.line.AntialiasingLineRenderer;
import notProvided.line.BresenhamLineRenderer;
import notProvided.line.DDALineRenderer;
import notProvided.polygon.FilledPolygonRenderer;
import notProvided.polygon.WireframePolygonRenderer;
*/
import polygon.PolygonRenderer;
import windowing.PageTurner;
import windowing.drawable.Drawable;
import windowing.drawable.GhostWritingDrawable;
import windowing.drawable.InvertedYDrawable;
import windowing.drawable.TranslatingDrawable;
import windowing.graphics.Dimensions;

public class Client implements PageTurner {
	private static final int ARGB_WHITE = 0xff_ff_ff_ff;
	private static final int ARGB_GREEN = 0xff_00_ff_40;
	
	private static final int NUM_PAGES = 5;
	protected static final double GHOST_COVERAGE = 0.14;

	private static final int NUM_PANELS = 4;
	private static final Dimensions PANEL_SIZE = new Dimensions(300, 300);
	private static final Point2D[] lowCornersOfPanels = {
			new Point2D( 50, 400),
			new Point2D(400, 400),
			new Point2D( 50,  50),
			new Point2D(400,  50),
	};
	
	private final Drawable drawable;
	private int pageNumber = 0;
	
	private Drawable image;
	private Drawable[] panels;
	private Drawable[] ghostPanels;					// use transparency and write only white
	private Drawable largePanel;
	
	private LineRenderer lineRenderers[];
	private PolygonRenderer polygonRenderer;
	
	
	public Client(Drawable drawable) {
		this.drawable = drawable;	
		createDrawables();
		createRenderers();
	}

	public void createDrawables() {
		image = new InvertedYDrawable(drawable);
		image = new TranslatingDrawable(image, point(0, 0), dimensions(750, 750));
		image = new ColoredDrawable(image, ARGB_WHITE);
		
		largePanel = new TranslatingDrawable(image, point(  50, 50),  dimensions(650, 650));
		
		createPanels();
		createGhostPanels();
	}

	public void createPanels() {
		panels = new Drawable[NUM_PANELS];
		
		for(int index = 0; index < NUM_PANELS; index++) {
			panels[index] = new TranslatingDrawable(image, lowCornersOfPanels[index], PANEL_SIZE);
		}
	}

	private void createGhostPanels() {
		ghostPanels = new Drawable[NUM_PANELS];
		
		for(int index = 0; index < NUM_PANELS; index++) {
			Drawable drawable = panels[index];
			ghostPanels[index] = new GhostWritingDrawable(drawable, GHOST_COVERAGE);
		}
	}
	private Point2D point(int x, int y) {
		return new Point2D(x, y);
	}	
	private Dimensions dimensions(int x, int y) {
		return new Dimensions(x, y);
	}
	private void createRenderers() {
		
		lineRenderers = new LineRenderer[4];
//		lineRenderers[0] = BresenhamLineRenderer.make();
		lineRenderers[0] = ExpensiveLineRenderer.make();
		lineRenderers[1] = DDALineRenderer.make();
		lineRenderers[2] = AlternatingLineRenderer.make();
		lineRenderers[3] = AntialiasingLineRenderer.make();
		
		polygonRenderer = FilledPolygonRenderer.make();
	}
	
	@Override
	public void nextPage() {
		System.out.println("PageNumber " + (pageNumber + 1));
		pageNumber = (pageNumber + 1) % NUM_PAGES;
		
		image.clear();
		largePanel.clear();
		switch(pageNumber) {
		case 1:  lineDrawerPage((panel, renderer)->{ new StarburstLineTest(panel, renderer); });
				 break;
		case 2:  lineDrawerPage((panel, renderer)->{ new ParallelogramTest(panel, renderer); });
				 break;
		case 3:	 lineDrawerPage((panel, renderer)->{ new RandomLineTest(panel, renderer); });
				 break;
		case 4:  polygonDrawerPage(panels);
				 break;
		case 0:	 polygonDrawerPage(ghostPanels);		// will be fifth page.  5 == 0 (mod 5)
				 break;
		default: defaultPage();
				 break;
		}
	}

	@FunctionalInterface
	private interface TestPerformer {
		public void perform(Drawable drawable, LineRenderer renderer);
	}
	private void lineDrawerPage(TestPerformer test) {
		image.clear();

		for(int panelNumber = 0; panelNumber < panels.length; panelNumber++) {
			panels[panelNumber].clear();
			test.perform(panels[panelNumber], lineRenderers[panelNumber]);
		}
	}
	public void polygonDrawerPage(Drawable[] panelArray) {
		image.clear();
		for(Drawable panel: panels) {		// 'panels' necessary here.  Not panelArray, because clear() uses setPixel.
			panel.clear();
		}
        // TODO
		new StarburstPolygonTest(panelArray[0], polygonRenderer);
		new MeshPolygonTest(panelArray[1], polygonRenderer, MeshPolygonTest.NO_PERTURBATION);
		new MeshPolygonTest(panelArray[2], polygonRenderer, MeshPolygonTest.USE_PERTURBATION);
		new RandomPolygonTest(panelArray[3], polygonRenderer);
	}

	private void defaultPage() {
		image.clear();
		largePanel.fill(ARGB_GREEN, Double.MAX_VALUE);
	}
}
