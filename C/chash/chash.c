#include <stdio.h>

#define SIZ	1000
int heap[SIZ];		// heap structure
int hpos[SIZ];		// heap additional value
int hcnt = 0;		// heap size

int hashtable[SIZ];	// hash structure
int offset[SIZ];	// flags for status

void trim_down(int idx)
{
	// if out-of-range
	if (idx*2+1 >= hcnt) return;

	// compare with brother node
	int n = idx * 2 + 1;
	if (n+1 < hcnt && heap[n+1] < heap[n])
		n = n + 1;

	if (heap[idx] > heap[n]) {
		//swap the heap element
		int tmp = heap[idx];
		heap[idx] = heap[n];
		heap[n] = tmp;

		tmp = hpos[idx];
		hpos[idx] = hpos[n];
		hpos[n] = tmp;

		// recursive adjustment
		trim_down(n);
	}
}

/* for debug */
void print_heap()
{
	int i;
	for (i = 0; i < hcnt; i++)
		printf("%d ", heap[i]);
	printf(" : heap\n");
}

/* for debug */
void print_hash(int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", hashtable[i]);

	printf(": hash\n");
}

/* get min from heap */
int extract_min()
{
	/* default min is on heap head */
	int min = heap[0];
	int pos = hpos[0];

	/* change state : already selected */
	offset[pos] = -1;

	heap[0] = heap[--hcnt];
	hpos[0] = hpos[hcnt];

	/* adjust heap: keep min-heap still */
	trim_down(0);

	return min;
}

/* for test */
void make_heap()
{
	int i;
	for (i = hcnt/2-1; i >= 0; i--)
		trim_down(i);
}

/* for test */
void heap_sort()
{
	make_heap();

	while (hcnt > 0) {
		int min = extract_min();
		printf("[%d] ", min);
	}
}

int main()
{
	/*test heap
	int a[10] = {2, 3, 6, 8, 9, 0, 1, 3, 4, 7};

	int i;
	for (i = 0; i < 10; i++)
		heap[hcnt++] = a[i];
	*/
	int hashsize = 0;

	/* begin process */
	while (scanf("%d", &hashsize) && hashsize)
	{
		int i = 0, vcnt = 0, min = 0x7f, minpos;

		hcnt = 0;

		/* initial state */
		for (i = 0; i < hashsize; i++)
		{
			scanf("%d", &hashtable[i]);

			if (hashtable[i] != -1) {
				offset[i] = hashtable[i] % hashsize;
				vcnt ++;

				if (offset[i] == i) {
					hpos[hcnt] = i;
					heap[hcnt++] = hashtable[i];

					offset[i] = -2; // add to heap already state
				}
			}
			else
				offset[i] = -1;
		}

		//print_hash(hashsize);
		//print_heap();
		make_heap();
		//print_heap();

		while (vcnt-- > 0)
		{
			// select next first input element
			min = heap[0];
			minpos = hpos[0];
			offset[minpos] = -1;	// pop from heap 

			//printf("[%d, %d]", min, minpos);
			printf("%d", min);
			if (vcnt >= 0) printf(" ");

			/* when an element is selected, then release another element if possible */
			int npos = minpos + 1;
			while (offset[(npos-1)%hashsize] == -1 
					&& hashtable[npos%hashsize]%hashsize <= npos%hashsize)
			{
				if (offset[npos%hashsize] != -1) break;
				else npos = (npos+1)%hashsize;
			}

			if (offset[npos%hashsize] != -1
					&& offset[hashtable[npos%hashsize]%hashsize] == -1
					&& offset[npos%hashsize] != -2)
			{
				/* the next release element push into heap */
				heap[0] = hashtable[npos%hashsize];
				hpos[0] = npos%hashsize;

				offset[npos%hashsize] = -2;
			}
			else
			{
				/* select the already element in heap */
				heap[0] = heap[--hcnt];
				hpos[0] = hpos[hcnt];
			}

			//print_heap();
			trim_down(0);		/* adjust dynamicly */
			//print_heap();
		}
		printf("\n");
	}
	//heap_sort();

	return 0;
}
