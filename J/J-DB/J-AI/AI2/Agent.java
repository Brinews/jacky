/*********************************************
 *  Agent.java 
 *  Sample Agent for Text-Based Adventure Game
 *  COMP3411 Artificial Intelligence
 *  UNSW Session 1, 2017
 */

import java.util.*;
import java.awt.Point;
import java.io.*;
import java.net.*;

/*
 * Stragety:
 * 1. Get the treasure if find it;
 * 2. Go home if got the treasure;
 * 3. Go to the unknown place in map as many as possible;
 * 4. Get the axe if find it;
 * 5. Get the key if find it;
 * 6. Get the dynamite if find it;
 * 7. Open the door if find it;
 * 8. Cut the tree if find it;
 * 9. Bomb the wall if no way can find;
 * 10. Go to water if no way in land;
 */

/**
 * mark symbol
 */
class Mark {
	public static final char HOME = 'H';
	public static final char TREASURE = '$';

	public static final char AXE = 'a';
	public static final char KEY = 'k';
	public static final char DYNAMITE = 'd';

	public static final char WALL = '*';
	public static final char DOOR = '-';
	public static final char TREE = 'T';

	public static final char WATER = '~';
	public static final char LAND = ' ';

	public static final char OUTSIDE = '.';
	public static final char UNEXPLORE = '?';

	public static final int UP = 0;
	public static final int RIGHT = 1;
	public static final int DOWN = 2;
	public static final int LEFT = 3;

	public static final char TURNLEFT = 'l';
	public static final char TURNRIGHT = 'r';
	public static final char FORWARD = 'f';
	public static final char BOMB = 'b';
	public static final char CUT = 'c';
	public static final char UNLOCK = 'u';
	public static final char ERROR = 'e';
};

public class Agent {

	Agent() {
		dynamiteNum = 0;
		faceDirection = Mark.UP;
		// start position
		pos = new Point(MapGrid.MAX_SIZE / 2, MapGrid.MAX_SIZE / 2);
		path = new LinkedList<Point>();

		gmap = new MapGrid(pos);
		astar = new AstarSearch(gmap);
		bfs = new BFSearch(gmap);
	}

	private void bfsSearch() {

		LinkedList<Character> nonpass = new LinkedList<Character>(Arrays
				.asList(Mark.OUTSIDE, Mark.WALL, Mark.UNEXPLORE));

		if (!gotAxe)
			nonpass.add(new Character(Mark.TREE));
		if (!gotKey)
			nonpass.add(new Character(Mark.DOOR));
		if (!gotRaft)
			nonpass.add(new Character(Mark.WATER));

		char[] targets;

		if (gotTreasure) {
			targets = new char[] { Mark.HOME };
			if (getPathByBFS(targets, nonpass, 0)) {
				return;
			}
		}

		if (gmap.getTreasurePos() != null) {
			targets = new char[] { Mark.TREASURE };
			if (getPathByBFS(targets, nonpass, 0)) {
				return;
			}
		}

		nonpass.remove(new Character(Mark.UNEXPLORE));

		if (anotherLand) {
			nonpass.add(new Character(Mark.TREE));
		}

		targets = new char[] { Mark.UNEXPLORE };
		if (getPathByBFS(targets, nonpass, 2)) {
			return;
		}

		targets = new char[] { Mark.AXE, Mark.DYNAMITE, Mark.KEY,
				Mark.UNEXPLORE };
		if (getPathByBFS(targets, nonpass, 0)) {
			return;
		}

	}

	private char astarSearch() {
		LinkedList<Character> nonpass = new LinkedList<Character>(Arrays
				.asList(Mark.OUTSIDE, Mark.TREE, Mark.UNEXPLORE));

		if (!gotKey)
			nonpass.add(new Character(Mark.DOOR));
		if (!gotRaft)
			nonpass.add(new Character(Mark.WATER));

		char[] targets;

		targets = new char[] { Mark.TREASURE, Mark.AXE, Mark.DYNAMITE,
				Mark.KEY, Mark.UNEXPLORE };
		char temp = getPathByAstar(targets, nonpass);

		if (temp != 0) {
			return temp;
		} else {
			if (!gotRaft) {
				nonpass.remove(new Character(Mark.TREE));
				targets = new char[] { Mark.TREASURE, Mark.AXE, Mark.DYNAMITE,
						Mark.TREE, Mark.KEY, Mark.UNEXPLORE };
				temp = getPathByAstar(targets, nonpass);
				if (temp != 0) {
					return temp;
				}
			} else {
				nonpass.remove(new Character(Mark.WATER));
				targets = new char[] { Mark.TREASURE, Mark.AXE, Mark.DYNAMITE,
						Mark.KEY, Mark.UNEXPLORE };
				temp = getPathByAstar(targets, nonpass);
				if (temp != 0) {
					return temp;
				}
			}
		}

		return Mark.ERROR;
	}

	private boolean getPathByBFS(char targets[], LinkedList<Character> avoid,
			int offset) {

		int waterIndex = 0;
		LinkedList<Character> copy = new LinkedList<Character>(avoid);

		LinkedList<Point> trail = bfs.searchObject(pos, targets, offset, copy);

		waterIndex = getWaterPos(trail);
		if (trail.size() > 0) {
			Point p = trail.get(1);

			trail = waterPath(waterIndex, trail, p);

			trail.removeFirst();

			path = trail;
			return true;
		}

		return false;
	}

	private LinkedList<Point> waterPath(int waterIndex,
			LinkedList<Point> trail, Point p) {
		if (usingRaft && waterIndex != 0) {
			if (gmap.getPos(p.y, p.x) == Mark.LAND
					|| gmap.getPos(p.y, p.x) == Mark.TREE) {
				if (hasPath(pos, trail.get(waterIndex))) {
					LinkedList<Character> av = new LinkedList<Character>(
							Arrays.asList(Mark.OUTSIDE, Mark.LAND,
									Mark.WALL, Mark.TREE));
					trail = bfs
							.searchPos(pos, trail.get(waterIndex), 0, av);
				}
			}
		}
		return trail;
	}

	private char getPathByAstar(char targets[], LinkedList<Character> walls) {

		LinkedList<Character> copy = null;
		int waterStart = 0;

		for (int i = 0; i < targets.length; i++) {
			copy = new LinkedList<Character>(walls);

			LinkedList<Point> solpath = astar.aStarSearch(pos, targets[i],
					dynamiteNum, copy);

			waterStart = getWaterPos(solpath);

			if (solpath.size() > 0) {
				Point p = solpath.get(1);

				p = getinWater(waterStart, solpath, p);

				if (getNextPoint().equals(p)) {
					if (gmap.getPos(p.y, p.x) == Mark.WALL) {
						return Mark.BOMB;
					}
					return Mark.FORWARD;
				} else {
					return Mark.TURNLEFT;
				}
			}
		}

		return 0;
	}

	private Point getinWater(int waterStart, LinkedList<Point> solpath,
			Point p) {
		if (usingRaft && waterStart != 0) {
			if (gmap.getPos(p.y, p.x) == Mark.LAND
					|| gmap.getPos(p.y, p.x) == Mark.TREE) {
				if (hasPath(pos, solpath.get(waterStart))) {
					LinkedList<Character> newnonpass = new LinkedList<Character>(
							Arrays.asList(Mark.OUTSIDE, Mark.LAND,
									Mark.WALL, Mark.TREE, Mark.DOOR));
					solpath = bfs.searchPos(pos, solpath
							.get(waterStart), 0, newnonpass);
					p = solpath.get(1);
				}
			}
		}
		return p;
	}

	private int getWaterPos(LinkedList<Point> solpath) {
		for (int i = 1; i < solpath.size(); i++) {
			Point p = solpath.get(i);
			if (gmap.getPos(p.y, p.x) == Mark.WATER) {
				return i;
			}
		}
		return 0;
	}

	private Point findRaft(Point waterTile) {
		LinkedList<Character> walls = new LinkedList<Character>(Arrays.asList(
				Mark.OUTSIDE, Mark.LAND, Mark.WALL));

		char array[] = { Mark.TREE };

		LinkedList<Point> tempPath = bfs.searchObject(waterTile, array, 0,
				walls);

		Point p = null;
		if (tempPath.size() > 0) {
			p = tempPath.getLast();
		}
		return p;

	}

	private boolean hasPath(Point a, Point b) {
		LinkedList<Character> walls = new LinkedList<Character>(Arrays.asList(
				Mark.OUTSIDE, Mark.LAND, Mark.WALL, Mark.DOOR, Mark.TREE));

		LinkedList<Point> tempPath = bfs.searchPos(a, b, 0, walls);

		if (tempPath.size() > 0) {
			return true;
		}

		return false;
	}

	private Point getNextPoint() {
		switch (faceDirection) {
		case Mark.UP:
			return new Point(pos.x, pos.y - 1);
		case Mark.RIGHT:
			return new Point(pos.x + 1, pos.y);
		case Mark.DOWN:
			return new Point(pos.x, pos.y + 1);
		case Mark.LEFT:
			return new Point(pos.x - 1, pos.y);
		}
		return null;
	}

	void print_view(char view[][]) {
		int i, j;

		System.out.println("\n+-----+");
		for (i = 0; i < 5; i++) {
			System.out.print("|");
			for (j = 0; j < 5; j++) {
				if ((i == 2) && (j == 2)) {
					System.out.print('^');
				} else {
					System.out.print(view[i][j]);
				}
			}
			System.out.println("|");
		}
		System.out.println("+-----+");
	}

	public char get_action(char view[][]) {
		updateMap(view);
		char c = Mark.TURNLEFT;

		c = findNextPath();

		c = moveAction(c);
		
		return c;
	}

	private char moveAction(char c) {
		char in = gmap.getFrontObj(pos, faceDirection);

		if (in == Mark.TREE && gotAxe == true) {
			c = Mark.CUT;
			gotRaft = true;
		}

		if (in == Mark.DOOR && gotKey == true) {
			c = Mark.UNLOCK;
		}

		switch (c) {
		case Mark.FORWARD:
			if (in == Mark.TREE || in == Mark.WALL) {
				break;
			}
			if (in == Mark.WATER && !gotRaft) {
				break;
			}
			fetchTools(in);
			updateCurrentPos();
			break;
		case Mark.TURNLEFT:
			faceDirection--;
			break;
		case Mark.TURNRIGHT:
			faceDirection++;
			break;
		case Mark.BOMB:
			dynamiteNum--;
			break;
		}

		if (faceDirection < Mark.UP)
			faceDirection = Mark.LEFT;
		if (faceDirection > Mark.LEFT)
			faceDirection = Mark.UP;
		return c;
	}

	private char findNextPath() {
		char c;
		if (path.size() > 0) {
			Point p = path.getFirst();
			if (getNextPoint().equals(p)) {
				c = Mark.FORWARD;
				path.removeFirst();
			} else {
				c = Mark.TURNLEFT;
			}
		} else {
			bfsSearch();
			if (path.size() > 0) {
				Point p = path.getFirst();
				if (getNextPoint().equals(p)) {
					c = Mark.FORWARD;
					path.removeFirst();
				} else {
					c = Mark.TURNLEFT;
				}
			} else {
				c = astarSearch();
			}
			path.clear();
		}
		return c;
	}

	private void updateMap(char[][] view) {
		switch (faceDirection) { // change view of map
		case Mark.RIGHT:
			view = gmap.clockwiseMatrix(view);
			break;
		case Mark.DOWN:
			view = gmap.anticlockMatrix(view);
			view = gmap.anticlockMatrix(view);
			break;
		case Mark.LEFT:
			view = gmap.anticlockMatrix(view);
			break;
		}

		gmap.extendMap(view, faceDirection, pos);
	}

	private void fetchTools(char in) {
		if (in == Mark.TREASURE) {
			gotTreasure = true;
			gmap.setTreasurePos(null);
		}
		if (in == Mark.AXE)
			gotAxe = true;
		if (in == Mark.KEY)
			gotKey = true;
		if (in == Mark.DYNAMITE)
			dynamiteNum++;
		if (in == Mark.WATER)
			usingRaft = true;
		if ((in == Mark.LAND || in == Mark.TREASURE) && usingRaft) {
			usingRaft = false;
			gotRaft = false;
			anotherLand = true;
		}
	}

	private void updateCurrentPos() {
		switch (faceDirection) {
		case Mark.UP:
			pos.y--;
			break;
		case Mark.RIGHT:
			pos.x++;
			break;
		case Mark.DOWN:
			pos.y++;
			break;
		case Mark.LEFT:
			pos.x--;
			break;
		}
	}

	public boolean gotAxe;
	public boolean gotKey;
	public boolean gotRaft;
	public boolean gotTreasure;
	public boolean usingRaft;
	public boolean anotherLand;
	public int dynamiteNum;

	public int faceDirection;
	public Point pos;
	public LinkedList<Point> path;

	public MapGrid gmap;
	public AstarSearch astar;
	public BFSearch bfs;

	// please don't touch below
	public static void main(String[] args) {
		InputStream in = null;
		OutputStream out = null;
		Socket socket = null;
		Agent agent = new Agent();
		char view[][] = new char[5][5];
		char action = 'F';
		int port;
		int ch;
		int i, j;
		long time = System.currentTimeMillis();

		if (args.length < 2) {
			System.out.println("Usage: java Agent -p <port>\n");
			System.exit(-1);
		}

		port = Integer.parseInt(args[1]);

		try { // open socket to Game Engine
			socket = new Socket("localhost", port);
			in = socket.getInputStream();
			out = socket.getOutputStream();
		} catch (IOException e) {
			System.out.println("Could not bind to port: " + port);
			System.exit(-1);
		}

		try { // scan 5-by-5 wintow around current location
			while (true) {
				for (i = 0; i < 5; i++) {
					for (j = 0; j < 5; j++) {
						if (!((i == 2) && (j == 2))) {
							ch = in.read();
							if (ch == -1) {
								System.exit(-1);
							}
							view[i][j] = (char) ch;
						}
					}
				}
				// agent.print_view( view ); // COMMENT THIS OUT BEFORE
				// SUBMISSION
				action = agent.get_action(view);
				out.write(action);
			}
		} catch (IOException e) {
			System.out.println("Lost connection to port: " + port);
			System.exit(-1);
		} finally {
			try {
				socket.close();
			} catch (IOException e) {
			}
		}
	}
}
