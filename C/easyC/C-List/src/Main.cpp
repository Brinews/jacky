#include "LinkedList.h"
#include "Process.h"

#include <iostream>

using namespace std;

LinkedList<Process>* compactList(LinkedList<Process> *root)
{
    for (int i = 0; i < root->size()-1; i++) {

        Process &p1 = root->infoAt(i);
        Process &p2 = root->infoAt(i+1);

        if (p1.infoHole() && p2.infoHole()) {
            p1.setEndAddress(p2.infoEndAddress());
            root->removeAt(i+1);
            i--;
            if (i < 0) break;
        }
    }
	return root;
}

int getPosition(LinkedList<Process> *root, int p)
{
    int pos = -1;
    int size = root->size();

    for (int i = 0; i < size; i++) {
        Process ps = root->infoAt(i);
        if (ps.infoProcessID() == p) {
            pos = i;
            break;
        }
    }

    return pos;
}

int findAvailable(LinkedList<Process> *root, int space, Process &p)
{
    int pos = -1;
    int size = root->size();

    for (int i = 0; i < size; i++) {
        p = root->infoAt(i);
        if (p.infoHole() 
             && space <= (p.infoEndAddress()-p.infoBeginAddress())) {
            pos = i;
            break;
        }
    }

    return pos;
}

int main()
{
    int p, q;
    LinkedList<Process>* root = new LinkedList<Process>();

    /*
    Process a(1);
    Process b(2);

    root->add(a);
    root->add(b);

    cout << (*root);

    delete root;
    */

    int total = 128;
    Process full(-1, 0, total, true);
    root->add(full);

    while (cin >> p >> q) {
        if (p == 0 && q == 0) {
            //print memory content
            cout << (*root);
        }

        if (p == -1 && q == 0) {
            //print free memory
            cout << "Left Free Space: " << total << " KB\n";
        }

        if (p > 0) {

            Process temp(p, 0, 0, true);
            int pos;

            try {
                temp = root->find(temp);
            } catch (Exception e) {
                temp.setProcessID(-1);
            }


            if (q > 0) {
                // allocate
                if (temp.infoProcessID() != -1) {
                    // re-allocate
                    Process np;
                    pos = findAvailable(root, q, np);
                    if (pos >= 0) {
                        Process toins;

                        toins.setBeginAddress(np.infoBeginAddress());
                        toins.setEndAddress(np.infoBeginAddress()+q);
                        toins.setProcessID(p);
                        toins.setHole(false);

                        Process &rp = root->infoAt(pos);
                        rp.setBeginAddress(rp.infoBeginAddress()+q+1);

                        root->insertAt(toins, pos);

                        // free origin
                        Process &toupd = root->find(temp);
                        toupd.setHole(true);
                        toupd.setProcessID(-1);

                        total += (np.infoEndAddress()-np.infoBeginAddress()+1);
                        total -= q;
                    } else {
                        cout << "No fit space.\n";
                    }
                } else {
                    // new-allocate
                    Process np;
                    pos = findAvailable(root, q, np);
                    if (pos >= 0) {
                        temp.setBeginAddress(np.infoBeginAddress());
                        temp.setEndAddress(np.infoBeginAddress()+q);
                        temp.setProcessID(p);
                        temp.setHole(false);

                        Process &rp = root->infoAt(pos);
                        rp.setBeginAddress(rp.infoBeginAddress()+q+1);

                        root->insertAt(temp, pos);

                        total -= q;
                    } else {
                        cout << "No fit space.\n";
                    }
                }
            } else {
                if (temp.infoProcessID() == -1) {
                    cout << "No exist process\n";
                } else {
                    if (q < 0) {
                        // memory adjust 
                        if (temp.infoEndAddress()-temp.infoBeginAddress()+q < 0) {
                            cout << "Not enough space to adjust.\n";
                        }  else if (temp.infoEndAddress()+q==temp.infoBeginAddress()) {
                            Process &rp = root->find(temp);
                            rp.setProcessID(-1);
                            rp.setHole(true);

                            compactList(root);

                            total += (-q);
                        } else {
                            Process toins;

                            toins.setBeginAddress(temp.infoEndAddress()+q+1);
                            toins.setEndAddress(temp.infoEndAddress());
                            toins.setProcessID(-1);
                            toins.setHole(true);

                            Process &rp = root->find(temp);
                            rp.setEndAddress(rp.infoEndAddress()+q);

                            root->insertAt(toins, getPosition(root, p)+1);

                            compactList(root);

                            total += (-q);
                        }
                    } else {
                        // free space
                        Process &toupd = root->find(temp);
                        toupd.setHole(true);
                        toupd.setProcessID(-1);

                        total += (toupd.infoEndAddress()-toupd.infoBeginAddress()+1);

                        // merge with neighbour free space
                        compactList(root);
                    }
                }
            }
        }
    }

    delete root;

    return 0;
}
