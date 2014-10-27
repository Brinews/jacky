#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const int MAXSIZ = 40960;

class SortingSet
{
private:
	int dataset[MAXSIZ];/* to sorting data */
	int datasiz;

	int mydata[MAXSIZ];	/* data copy */

	int cmpcnt;			/* compare count */
	int swpcnt;			/* swap count */

public:
	SortingSet()
	{
		for (int i = 0; i < MAXSIZ; i++) dataset[i] = mydata[i] = 0;
		datasiz = 0;
		cmpcnt = swpcnt = 0;
	}

	int ReadFile(string filename)
	{
		ifstream fin;
		fin.open(filename.c_str());

		int idx = 0;

		while (fin >> dataset[idx])
		{
			idx++;
		}

		datasiz = idx;

		fin.close();

		return 0;
	}

	void CopyData()
	{
		for (int i = 0; i < datasiz; i++)
			mydata[i] = dataset[i];

		cmpcnt = swpcnt = 0;	/* reset */
	}

	void PrintResult(string sortName)
	{
		cout << sortName << "\t"<<cmpcnt << "\t"<<swpcnt << endl;
	}

	void BubbleSort()
	{
		CopyData();

		int flag = 0;

		for (int i = datasiz-1; i > 0; i--)
		{
			flag = 0;
			for (int j = 0; j < i; j++)
			{
				cmpcnt++;
				if (mydata[j] > mydata[j+1])
				{
					int tmp = mydata[j];
					mydata[j] = mydata[j+1];
					mydata[j+1] = tmp;

					swpcnt++;
					flag = 1;
				}
			}
			if (flag == 0) break;
		}

		dbg(mydata);
		
		PrintResult("BubbleSort");
	}

	void SelectSort()
	{
		CopyData();

		for (int i = 0; i < datasiz; i++)
		{
			int min = mydata[i];
			int k = i;

			/* find the min */
			for (int j = i+1; j < datasiz; j++)
			{
				/* accumulate compare times */
				cmpcnt++;
				if (mydata[j] < min)
				{
					k = j;
					min = mydata[j];

					swpcnt++;
				}
			}

			if (k != i)
			{
				int tmp = mydata[i];
				mydata[i] = mydata[k];
				mydata[k] = tmp;

				swpcnt++;
			}
		}

		dbg(mydata);
		
		PrintResult("SelectSort");
	}

	void InsertSort()
	{
		CopyData();

		for (int i = 1; i < datasiz; i++)
		{
			for (int j = i; j >= 1; j--)
			{
				cmpcnt++;

				if (mydata[j] < mydata[j-1])
				{
					int tmp = mydata[j];
					mydata[j] = mydata[j-1];
					mydata[j-1] = tmp;

					swpcnt++;
				}
				else
					break;
			}
		}

		dbg(mydata);
		PrintResult("InsertSort");
	}

	int mdata[MAXSIZ];
	void merge_sort(int *data, int low, int high)
	{
		if (low >= high) 
		{
			mdata[low] = data[low];
			return;
		}

		int mid = (low+high)/2;

		merge_sort(data, low, mid);
		merge_sort(data, mid+1, high);

		//back up
		for (int i = low; i <= high; i++)
		{
			mdata[i] = data[i];
			swpcnt++;
		}

		//merge
		int i = low, j = mid+1;
		int k = low;

		while (i <= mid && j <= high)
		{
			cmpcnt ++;
			swpcnt ++;

			if (mdata[i] < mdata[j])
			{
				data[k] = mdata[i];
				i++;
			}
			else
			{
				data[k] = mdata[j];
				j++;
			}

			k++;
		}

		while (j <= high) 
		{
			data[k] = mdata[j];
			k++;
			j++;
			swpcnt++;
		}

		while (i <= mid)
		{
			data[k] = mdata[i];
			k++;
			i++;
			swpcnt++;
		}

		return;
	}

	void MergeSort()
	{
		CopyData();

		merge_sort(mydata, 0, datasiz-1);

		dbg(mydata);
		PrintResult("MergeSort");
	}

	int h_size;
	void trick_down(int idx)
	{
		int n = idx*2 + 1;

		if (n > datasiz) return;

		cmpcnt++;
		if (n+1 < datasiz && mydata[n+1] < mydata[n])
		{
			swpcnt++;
			n = n + 1;
		}

		cmpcnt++;
		if (mydata[idx] > mydata[n])
		{
			swpcnt ++;

			int tmp = mydata[idx];
			mydata[idx] = mydata[n];
			mydata[n] = tmp;

			trick_down(n);
		}
	}

	int extract_min()
	{
		int tmp = mydata[0];
		mydata[0] = mydata[--h_size];
		swpcnt++;

		trick_down(0);

		return tmp;
	}

	void make_heap()
	{
		for (int i = (datasiz/2-1); i>=0; i--)
			trick_down(i);
	}

	void heap_sort()
	{
		int i = 0;
		h_size = datasiz;

		while (h_size > 0)
		{
			mdata[i] = extract_min();
			i++;
		}
	}

	void HeapSort()
	{
		CopyData();

		make_heap();
		heap_sort();

		dbg(mdata);
		PrintResult("HeapSort");
	}

	void quick_sort(int *data, int low, int high)
	{
		if (low >= high) return;
		
		int beg = low, end = high;

		int privot = data[low];

		while (low < high)
		{
			while (low < high && data[high] >= privot)
			{
				cmpcnt++;
				high--;
			}
			data[low] = data[high];
			swpcnt++;

			while (low < high && data[low] <= privot)
			{
				cmpcnt++;
				low++;
			}
			data[high] = data[low];
			swpcnt++;
		}
		data[low] = privot;

		quick_sort(data, beg, low-1);
		quick_sort(data, low+1, end);

		return;
	}

	void QuickSort()
	{
		CopyData();

		quick_sort(mydata, 0, datasiz-1);

		dbg(mydata);
		PrintResult("QuickSort");
	}

	void dbg(int *data)
	{
		for (int i = 0; i < datasiz-1; i++)
			if (data[i] > data[i+1])
				cout << i << " sort error.\n";
	}

	~SortingSet()
	{
	}

};

void test()
{
	srand(0);

	ofstream fout("Inverse100.txt");

	for (int i = 0; i < 100; i++)
	{
		fout << rand()%80+10 << endl;
	}

	fout.close();
}

int main(int argc, char **argv)
{
	SortingSet *ss = new SortingSet();

	string filenames[6] = {"Inverse100.txt", "Random100.txt", "Almost100.txt", 
		"Inverse1000.txt", "Random1000.txt", "Almost1000.txt"};

	test();

	for (int i = 0; i < 1; i++)
	{
		ss->ReadFile(filenames[i]);

		cout << "--- Staticstics By [" << filenames[i]<< " ] ---" << endl;
		cout << "          compare-count\tswap-count\n";

		ss->SelectSort();
		ss->InsertSort();
		ss->HeapSort();
		ss->MergeSort();
		ss->QuickSort();
		ss->BubbleSort();
	}

	delete ss;
	system("pause");

	return 0;
}
