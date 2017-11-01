import java.util.*;
import java.awt.Point;

/**
 * map of game
 */
public class MapGrid {
    public static final int MAX_SIZE = 180; 
    public char agentmap[][];
    public Point treasurePos;
    public Point startPos;

    public MapGrid(Point pos) {
        treasurePos = null;
        startPos = pos;
        agentmap = new char[MAX_SIZE][MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; i++) {
            for (int j = 0; j < MAX_SIZE; j++) {
                agentmap[i][j] = Mark.UNEXPLORE; 
            }
        }
        agentmap[pos.y][pos.x] = Mark.HOME;
    }

    public LinkedList<Point> getNeighbours(Point in) {
        LinkedList<Point> out = new LinkedList<Point>();

        if (in.x+1 < MAX_SIZE) out.add(new Point(in.x+1,in.y));
        if (in.x-1 >= 0) out.add(new Point(in.x-1,in.y));

        if (in.y+1 < MAX_SIZE) out.add(new Point(in.x,in.y+1));
        if (in.y-1 >= 0) out.add(new Point(in.x,in.y-1));

        return out;
    }

    public char getPos(int y, int x) {
        return agentmap[y][x];
    }

    public char getFrontObj(Point pos, int curOrient) {
        switch(curOrient) {
            case Mark.UP: return agentmap[pos.y-1][pos.x];
            case Mark.RIGHT: return agentmap[pos.y][pos.x+1];
            case Mark.DOWN: return agentmap[pos.y+1][pos.x];
            case Mark.LEFT: return agentmap[pos.y][pos.x-1];
        }

        return Mark.UNEXPLORE;
    }
    
    public char[][] getGridMap() {
    	return agentmap;
    }

    public void extendMap(char[][] view, int curOrient, Point pos) {
        int x = pos.x;
        int y = pos.y;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                agentmap[y+i-2][x+j-2] = view[i][j];
                if (agentmap[y+i-2][x+j-2] == Mark.TREASURE) {
                    treasurePos = new Point(x+j-2, y+i-2);
                }
            }
        }

        switch(curOrient) {
        case Mark.UP: agentmap[y][x] = '^'; break;
        case Mark.RIGHT: agentmap[y][x] = '>'; break;
        case Mark.DOWN: agentmap[y][x] = 'v'; break;
        case Mark.LEFT: agentmap[y][x] = '<'; break;
        }

        agentmap[MAX_SIZE/2][MAX_SIZE/2] = Mark.HOME;
    }

    public Point getTreasurePos() {
        return treasurePos;
    }

    public void setTreasurePos(Point p) {
        treasurePos = p;
    }

    public boolean inRange(Point p, char c[], int range) {
        for (int i = -range; i < range+1; i++) {
            for (int j = -range; j < range+1; j++) {
                for (int k = 0; k < c.length; k++) {
                    if (c[k] == agentmap[p.y+i][p.x+j]) {
                        return true;
                    }					
                }
            }
        }

        return false;
    }

    public char[][] clockwiseMatrix(char[][] mat) {
        int w = mat.length;
        int h = mat[0].length;
        char[][] ret = new char[h][w];
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                ret[i][j] = mat[w - j - 1][i];
            }
        }
        return ret;
    }

    public char[][] anticlockMatrix(char[][] mat) {
        int w = mat.length;
        int h = mat[0].length;   
        char[][] ret = new char[h][w];
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                ret[i][j] = mat[j][h - i - 1];
            }
        }
        return ret;
    }
}

