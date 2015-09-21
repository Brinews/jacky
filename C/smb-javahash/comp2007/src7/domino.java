import java.util.*;

public class domino {
	private final int M = 10;
	private final int INF = 0x7fffffff;
	private int[][] board = null;
	private int Size;

	class Node {
		public int to;
		public int cap;
		public int rev;

		public Node(int t, int c, int r) {
			to = t;
			cap = c;
			rev = r;
		}
	}

	Vector<Vector<Node> > v = new Vector<Vector<Node> >();
	boolean[] used = new boolean[M*M+2];

	public domino() {
		board = new int[M][M];
		Size = 0;

		for (int i = 0; i < M*M; i++) {
			v.add(new Vector<Node>());
		}
	}

	public int getNodeNumber(int x, int y) {
		return (x-1)*Size + y;
	}

	public void addNode(int from, int to, int cap) {
		v.get(from).add(new Node(to, cap, v.get(to).size()));
		v.get(to).add(new Node(from, 0, v.get(from).size()-1));
	}

	public void readPartialBoard() {
		Scanner in = new Scanner(System.in);
		int vx, vy;

		while (in.hasNextLine()) {
			String line = in.nextLine();
			String[] item = line.split(",");

			if (item.length < 2) break;

			vx = Integer.parseInt(item[0]);
			vy = Integer.parseInt(item[1]);

			if (vx > Size) Size = vx;
			if (vy > Size) Size = vy;

			board[vx][vy] = 1;
		}

		int[][] dir = {{1,0},{0,1},{-1,0},{0,-1}};

		for (int i = 1; i <= Size; i++) {
			for (int j = 1; j <= Size; j++) {

				if (board[i][j] == 1) {
					for (int k = 0; k < 4; k++) {
						int ni = i + dir[k][0];
						int nj = j + dir[k][1];

						if (ni >= 1 && ni <= Size
								&& nj >= 1 && nj <= Size
								&& board[ni][nj] != 0) {
							/* add one edge */
							if (board[ni][nj] == 1)
								board[ni][nj] = 2;

							addNode(getNodeNumber(i, j),
									getNodeNumber(ni, nj),
									INF);

						}
					}
				}
			}
		}

		/* add start/end node */
		for (int i = 1; i <= Size; i++) {
			for (int j = 1; j <= Size; j++) {
				if (board[i][j] == 1) {
					addNode(0, getNodeNumber(i, j), 1);
				}
				if (board[i][j] == 2) {
					addNode(getNodeNumber(i, j), Size*Size+1, 1);
				}
			}
		}
	}

	public void printVector() {
		for (int i = 0; i < M*M; i++) {
			if (v.get(i).size() > 0) {
				System.out.println(i);

				for (int j = 0; j < v.get(i).size(); j++) {
					System.out.printf("->%d", v.get(i).get(j).to);
				}

				System.out.println();
			}
		}
	}

	public int dfs(int s, int t, int f) {
		if (s == t) return f;

		used[s] = true;

		for (int i = 0; i < v.get(s).size(); i++) {
			Node tmp = v.get(s).get(i);

			if (used[tmp.to] == false && tmp.cap > 0) {
				int d = dfs(tmp.to, t, Math.min(f, tmp.cap));
				if (d > 0) {
					v.get(s).get(i).cap -= d;
					int tpto = v.get(s).get(i).to;
					int tprev = v.get(s).get(i).rev;
					v.get(tpto).get(tprev).cap += d;

					return d;
				}
			}
		}

		return 0;
	}

	public int max_flow(int s, int t) {
		int flow = 0;

		for (;;) {
			for (int i = 0; i < M*M+2; i++) used[i] = false;

			int f = dfs(s, t, INF);

			if (f == 0) return flow;

			flow += f;
		}
	}

	public int maxFlow() {
		int flow = max_flow(0, Size*Size+1);

		System.out.println(flow*10);
		int start = 0, end = 0;

		Vector<Node> ret = new Vector<Node>();

		for (int i = 1; i <= Size; i++) {
			for (int j = 1; j <= Size; j++) {
				if (start == 0 && board[i][j] != 0) {
					start = getNodeNumber(i, j);
				}

				if (board[i][j] != 0) {
					end = getNodeNumber(i, j);
				}
			}
		}

		for (int i = 1; i < Size*Size+1; i++) {
			if (v.get(i).size() > 0) {

				for (int j = 0; j < v.get(i).size(); j++) {
					//System.out.printf("->%d", v.get(i).get(j).to);
					if (v.get(i).get(j).to > i) {
						ret.add(new Node(i-1, v.get(i).get(j).to-1,
									max_flow(i, v.get(i).get(j).to)));
					}
				}
			}
		}

		System.out.println(start);
		System.out.println(end);

		for (int i = 0; i < ret.size(); i++) {
			System.out.printf("%d %d %d\n", ret.get(i).to,
					ret.get(i).cap, ret.get(i).rev);
		}

		return flow;
	}

	/* just test */
	public void testVector() {
		Vector<Node> c = new Vector<Node>();
		c.add(new Node(12, 2, 33));
		c.add(new Node(1, 33, 44));

		System.out.println(c.get(0).to);
		c.get(0).to = 1;
		System.out.println(c.get(0).to);
	}

	public static void main(String[] args) {
		domino dom = new domino();

		//dom.testVector();

		dom.readPartialBoard();
		//dom.printVector();

		dom.maxFlow();

		//System.out.println(10*(flow+1));
	}
}
