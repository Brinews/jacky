import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;

public class MyDlist {
	protected DList root;

	public MyDlist() {
		root = new DList();
	}

	public MyDlist(String f) {
		root = new DList();

		try { // read file
			BufferedReader br;

			if (!f.equals("stdin")) {
				br = new BufferedReader(new FileReader(f));
			}
			else
				br = new BufferedReader(
						new InputStreamReader(System.in));

			String line;

			while ((line = br.readLine()) != null) {
				if (f.equals("stdin") && line.equals("")) 
					break; // empty line means end
				root.addLast(new DNode(line, null, null));
			}

			br.close();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	/* travese DList from head to tail */
	public void printList() {
		if (!root.isEmpty()) {
			DNode p = root.getFirst();
			do {
				System.out.println(p.getElement());

				if (p == root.getLast()) break; //last node

				p = root.getNext(p); //next node

			} while (true);
		}
		else {
			System.out.println("Empty List.");
		}
	}

	/* deep copy : allocate node space for new one */
	public static MyDlist cloneList(MyDlist uDl) {

		DList u = uDl.root;

		MyDlist nlist = new MyDlist();

		if (!u.isEmpty()) {
			DNode p = u.getFirst();
			do {
				DNode n = new DNode(p.getElement(), null, null);
				nlist.root.addLast(n);

				if (p == u.getLast()) break;

				p = u.getNext(p);
			} while (true);
		}

		return nlist;
	}

	public static boolean find(MyDlist uDl, String kn) {

		DList u = uDl.root;

		if (!u.isEmpty()) {
			DNode p = u.getFirst();

			while (true) {
				if (p.getElement().equals(kn)) return true;

				if (p == u.getLast()) break;

				p = u.getNext(p);
			}
		}

		return false;
	}

	public static MyDlist addList(MyDlist ulist, MyDlist uDl) {

		DList u = uDl.root;

		if (!u.isEmpty()) {
			DNode p = u.getFirst();

			while (true) {
				String kn = p.getElement();

				if (!find(ulist, kn)) { // no find, can insert
					ulist.root.addLast(new DNode(kn, null, null));
				}

				if (p == u.getLast()) break;

				p = u.getNext(p);
			}
		}

		return ulist;
	}

	/* key algorithm:
	 * a. traverse each node(i) in u;
	 * b. is node(i) in union list?
	 * c. traverse each node(j) in v;
	 * d. is node(j) in union list?
	 *
	 * Time Complexity : O(M^2+N^2), M, N is the size of list.
	 */
	public static MyDlist union(MyDlist u, MyDlist v) {
		MyDlist ulist = new MyDlist();

		ulist = addList(ulist, u);
		ulist = addList(ulist, v);
		
		return ulist;
	}

	/* key algorithm:
	 * a. traverse each node(i) in u;
	 * b. is node(i) in v? 
	 *
	 * Time Complexity : O(M*N), M, N is the size of list.
	 */
	public static MyDlist intersection(MyDlist uDl, MyDlist vDl) {
		MyDlist ilist = new MyDlist();

		DList u = uDl.root;
		DList v = vDl.root;

		if (!u.isEmpty() && !v.isEmpty()) {
			DNode p = u.getFirst();

			while (true) {
				String kn = p.getElement();

				if (find(vDl, kn)) { // find in u&v, can insert
					ilist.root.addLast(new DNode(kn, null, null));
				}

				if (p == u.getLast()) break;

				p = u.getNext(p);
			}
		}

		return ilist;
	}

	/*
	 * test case
	 */
	public static void main(String[] args)
	{
		MyDlist L1 = new MyDlist("1.txt");
		MyDlist L2 = new MyDlist("stdin");

		System.out.println("Print List:");
		L1.printList();

		System.out.println("Print List:");
		L2.printList();

		MyDlist L3 = MyDlist.cloneList(L1);
		System.out.println("Print List:");
		L3.printList();

		MyDlist L4 = MyDlist.union(L1, L2);
		System.out.println("Print List:");
		L4.printList();

		MyDlist L5 = MyDlist.intersection(L1, L2);
		System.out.println("Print List:");
		L5.printList();
	}
}
