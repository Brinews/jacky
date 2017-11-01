import java.util.*;
import java.awt.Point;

public class AstarSearch {
    public MapGrid gdmap;

    public AstarSearch(MapGrid mg) {
        gdmap = mg;
    }

    public LinkedList<Point> aStarSearch(Point from, char to, 
            int myDynamiteNum, LinkedList<Character> walls) {

        Queue<WeightState> openSet = new PriorityQueue<WeightState>(1,
                new Comparator<WeightState>() {

            @Override
            public int compare(WeightState cost1, WeightState cost2) {
                return cost1.getWeight() - cost2.getWeight();
            }
        });

        ArrayList<WeightState> closedSet = new ArrayList<WeightState>();
        LinkedList<Point> solpath = new LinkedList<Point>();
        HashMap<Point, Point> prevLink = new HashMap<Point, Point>();

        WeightState first = new WeightState(from, 0);
        openSet.add(first);

        while (!openSet.isEmpty()) {
            WeightState current = new WeightState(new Point(0, 0), 0);
            current.setNode(openSet.peek().getNode());
            current.setWeight(openSet.peek().getWeight());
            openSet.remove();

            closedSet.add(current);

            char array[] = {to};
            // found it
            if (gdmap.inRange(current.getNode(), array, 0)) {
                solpath.add(current.getNode());
                while (!current.getNode().equals(from)) {
                    solpath.addFirst(prevLink.get(current.getNode()));
                    current.setNode(prevLink.get(current.getNode()));
                }
                break;
            }

            for (Point p : gdmap.getNeighbours(current.getNode())) {
                if (walls.contains(gdmap.getPos(p.y, p.x))) {
                    continue;
                }

                WeightState pnode = new WeightState(p, current.getWeight());
                // walls
                if (gdmap.getPos(p.y, p.x) == Mark.WALL) {
                    pnode.setWeight(current.getWeight() + 100);
                }

                // land or water
                if (gdmap.getPos(p.y, p.x) == Mark.LAND
                        || gdmap.getPos(p.y, p.x) == Mark.WATER) {
                    pnode.setWeight(current.getWeight() + 1);
                }

                if (!checkClosedSet(closedSet, pnode.getNode()) && 
                		!checkOpenSet(openSet, pnode.getNode(), pnode.getWeight())) {
                    if (!prevLink.containsKey(p)) {
                        openSet.add(pnode);
                        prevLink.put(p, current.getNode());
                    }
                }
            }
        }

        int bombsCnt = 0;

        for (int j = 0; j < solpath.size(); j++) {
            if (gdmap.getPos(solpath.get(j).y, solpath.get(j).x) == Mark.WALL) {
                bombsCnt++;
            }
        }
        
        if (bombsCnt > myDynamiteNum) {
        	solpath.clear();
        }

        return solpath;
    }


    private boolean checkOpenSet(Queue<WeightState> set, Point name, 
            int cost) 
    {
        ArrayList<WeightState> list = new ArrayList<WeightState>();
        WeightState pnode = new WeightState(new Point(0, 0), 0);
        int i = 0;

        while (!set.isEmpty()) {
            list.add(set.remove());
        }
        set.addAll(list);

        for(i = 0; i < list.size(); i++) {
            pnode = list.get(i);

            if(pnode.getNode().x == name.x 
                    && pnode.getNode().y == name.y 
                    && pnode.getWeight() > cost) {
                set.remove(pnode);
                pnode.setWeight(cost);
                set.add(pnode);

                return true;
            }
        }

        return false;
    }


    private boolean checkClosedSet(ArrayList<WeightState> set, Point name) 
    {
        int i = 0;

        for (i = 0; i < set.size(); i++) {
            if (set.get(i).getNode().x == name.x 
                    && set.get(i).getNode().y == name.y) {
                return true;
            }
        }
        return false;
    }

    class WeightState {

        public WeightState(Point newNode, int cost) {
            node = newNode;
            weight = cost;
        }

        public Point getNode() {
            return node;
        }

        public int getWeight() {
            return weight;
        }

        public void setNode(Point newVal) {
            node = newVal;
        }

        public void setWeight(int newVal) {
            weight = newVal;
        }

        private Point node;
        private int weight;
    }

}
