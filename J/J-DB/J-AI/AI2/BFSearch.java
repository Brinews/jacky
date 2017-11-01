import java.util.*;
import java.awt.Point;

public class BFSearch {
    public MapGrid gmap;

    public BFSearch(MapGrid mg) {
        gmap = mg;
    }

    public LinkedList<Point> searchObject(Point start, char in[], 
            int offset, LinkedList<Character> walls) 
    {

        LinkedList<Point> Q = new LinkedList<Point>();
        HashMap<Point, Point> fromWhich = new HashMap<Point, Point>();

        Point v = null;
        boolean foundTarget = false;

        Q.add(start);
        while (!Q.isEmpty()) {
            v = Q.poll();

            if (gmap.inRange(v, in, offset)) {
                foundTarget = true;
                break;
            }

            for (Point p : gmap.getNeighbours(v)) {
                if (walls.contains(gmap.getPos(p.y, p.x))) {
                    continue;
                }

                if (!fromWhich.containsKey(p)) {
                    Q.add(p);
                    fromWhich.put(p, v);
                }
            }
        }

        if (!foundTarget) {
            return new LinkedList<Point>();
        }

        LinkedList<Point> solpath = new LinkedList<Point>();
        solpath.add(v);

        while(!v.equals(start)) {
            solpath.addFirst(fromWhich.get(v));
            v = fromWhich.get(v);
        }

        return solpath;
    }


    public LinkedList<Point> searchPos(Point start, Point end, 
            int offset, LinkedList<Character> walls) 
    {

        LinkedList<Point> Q = new LinkedList<Point>();
        HashMap<Point, Point> fromWhich = new HashMap<Point, Point>();

        Point v = null;
        boolean foundTarget = false;

        Q.add(start); 
        while (!Q.isEmpty()) {
            v = Q.poll();

            if (v.x == end.x && v.y == end.y) {
                foundTarget = true;
                break;
            }

            for (Point p : gmap.getNeighbours(v)) {
                if (walls.contains(gmap.getPos(p.y, p.x))) {
                    continue;
                }

                if (!fromWhich.containsKey(p)) {
                    Q.add(p);
                    fromWhich.put(p, v);
                }
            }
        }

        if (!foundTarget) {
            return new LinkedList<Point>();
        }

        LinkedList<Point> solpath = new LinkedList<Point>();
        solpath.add(v);

        while(!v.equals(start)) {
            solpath.addFirst(fromWhich.get(v));
            v = fromWhich.get(v);
        }

        return solpath;
    }

}
