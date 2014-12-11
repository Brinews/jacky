#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <assert.h>
#include <time.h>

#include <sys/resource.h>

using namespace std;

#define NAMLEN	40
#define STALEN	4
#define PEOSIZ	1024

/***
 * Global Stucture
 */
typedef struct PeopleInfo 
{
	int SocialNo;
	char FirstName[NAMLEN];
	char LastName[NAMLEN];
	int BirthDay;
	char State[STALEN];
} PeopleInfo_t;

typedef struct PeopleInfo* PeopleInfo_p;

int pi_capacity = 0;
int pi_size = 0;

/****
 * Global Function
 */
PeopleInfo_p ps_readin(char *filename);
void ps_print(PeopleInfo_t p, ostream& out);
void ps_writeout(PeopleInfo_t *arr, int siz, char *filename);

PeopleInfo_p ps_namesearch(PeopleInfo_t *arr, int siz, char *name);
void ps_testsearch(PeopleInfo_t *arr, int siz);

int ps_findmax(PeopleInfo_t *arr, int s, int e);
void ps_swap(PeopleInfo_t *arr, int left, int right);
PeopleInfo_p ps_dosort(PeopleInfo_t *arr, int siz);

double ps_timecost(PeopleInfo_t *arr, int siz);
void ps_doclear(PeopleInfo_t *arr);

/***
 * IO: read in
 * Param@(filename)
 * Ret: PeopleInfo_p *p
 */
PeopleInfo_p ps_readin(char *filename)
{
	ifstream fin(filename);

	struct PeopleInfo* peo_arr = new PeopleInfo_t[PEOSIZ];
	//assert(NULL == peo_arr);

	pi_capacity = PEOSIZ;

	int sno, birth;
	char fname[NAMLEN], lname[NAMLEN], state[STALEN];

	int idx = 0;
	while (fin >> sno >> fname >> lname >> birth >> state)
	{
		if (idx >= pi_capacity)
		{
			pi_capacity <<= 1;
			PeopleInfo_t *new_arr = new PeopleInfo_t[pi_capacity];
			memcpy(new_arr, peo_arr, sizeof(PeopleInfo_t)*idx);

			delete []peo_arr;
			peo_arr = new_arr;
		}

		peo_arr[idx].SocialNo = sno;
		strcpy(peo_arr[idx].FirstName, fname);
		strcpy(peo_arr[idx].LastName, lname);
		peo_arr[idx].BirthDay = birth;
		strcpy(peo_arr[idx].State, state);

		idx++;
	}

	pi_size = idx;

	fin.close();

	return peo_arr;
}

void ps_print(PeopleInfo_t p, ostream& out)
{
	out << p.SocialNo << " " << p.FirstName << " " << p.LastName << " "
		<< p.BirthDay << " " << p.State << endl;
}

/***
 * Write out
 * Param@(PeopleInfo_t *arr, size, filename);
 * Ret: null
 */
void ps_writeout(PeopleInfo_t *arr, int siz, char *filename)
{
	ofstream out(filename);

	for (int i = 0; i < siz; i++) 
	{
		ps_print(arr[i], out);
	}

	out.close();
}

/***
 * Basic Research Function
 * Param@(PeopleInfo_t *arr, int siz, char *name)
 * Ret: PeopleInfo_t a
 */
PeopleInfo_p ps_namesearch(PeopleInfo_t *arr, int siz, char *name)
{
	PeopleInfo_t *ret = NULL;

	for (int i = 0; i < siz; i++)
	{
		if (strcmp(arr[i].FirstName, name) == 0
			|| strcmp(arr[i].LastName, name) == 0)
		{
			ret = &arr[i];
			ps_print(arr[i], cout);
		}
	}

	return NULL;
}

/***
 * Research Tester
 * Param@(PeopleInfo_t *arr, int siz)
 * Ret: null
 */
void ps_testsearch(PeopleInfo_t *arr, int siz)
{
	char buf[NAMLEN];
	cout << "Input key names :(null for end)" << endl;

	while (cin >> buf)
	{
		if (strcmp(buf, "null") == 0) break;

		PeopleInfo_p ret = ps_namesearch(arr, siz, buf);
		if (NULL == ret)
		{
			cout << "Not Found." << endl;
		}
	}
}

/***
 * Find max
 * Param@(PeopleInfo_t *arr, int s, int e)
 * Ret : index with oldest
 */
int ps_findmax(PeopleInfo_t *arr, int s, int e)
{
	int maxage = arr[s].BirthDay;
	int idx = s;

	for (int i = s+1; i < e; i++)
	{
		if (arr[i].BirthDay < maxage) 
		{
			maxage = arr[i].BirthDay;
			idx = i;
		}
	}

	return idx;
}

/***
 * Swap two element
 * Param@(PeopleInfo_t *arr, int left, int right)
 * Ret : null
 */
void ps_swap(PeopleInfo_t *arr, int left, int right)
{
	PeopleInfo_t tmp;

	memcpy(&tmp, &arr[left], sizeof(PeopleInfo_t));
	memcpy(&arr[left], &arr[right], sizeof(PeopleInfo_t));
	memcpy(&arr[right], &tmp, sizeof(PeopleInfo_t));

}

/***
 * Sort Function
 * Param@(PeopleInfo_t *arr, int siz)
 * Ret: arr
 */
PeopleInfo_p ps_dosort(PeopleInfo_t *arr, int siz)
{
	int max = 0;

	for (int i = 0; i < siz; i++)
	{
		// find the oldest
		max = ps_findmax(arr, i, siz);

		// swap the oldest with current
		if (max != i)
		{
			ps_swap(arr, i, max);
		}
	}

	return arr;
}

double get_cpu_time()
{
	struct rusage ruse;
	getrusage(RUSAGE_SELF, &ruse);

	return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0
		+ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0;

	/*
	return clock()/(double)CLOCKS_PER_SEC;
	*/

}
/***
 * Time Function
 * Param@(PeopleInfo_t *arr, int siz)
 * Ret: milliseconds
 */
double ps_timecost(PeopleInfo_t *arr, int siz)
{
	double start = get_cpu_time();

	ps_dosort(arr, siz);

	double end = get_cpu_time();

	cout << "Time Cost For Sorting : " << end - start << " sec." << endl;

	return end - start;
}

/***
 * Do Clear up
 * Param@(PeopleInfo_t *arr)
 * Ret : null
 */
void ps_doclear(PeopleInfo_t *arr)
{
	delete []arr;
	arr = NULL;
}

/***
 * Main Function
 */
int main(int argc, char **argv)
{
	char fname[NAMLEN];
	char pname[NAMLEN];

	cout << "Please Given the Input file Name: (none for end)" << endl;
	while (cin >> fname)
	{
		if (strcmp(fname, "none") == 0) break;

		PeopleInfo_t *arr = ps_readin(fname);

		//do test research function
		ps_testsearch(arr, pi_size);

		//calculate the sort cost time
		ps_timecost(arr, pi_size);

		strcpy(pname, "sort_");
		strcat(pname, fname);
		// write out sorted list
		cout << "Begin Write the sorted result into File : " << pname << endl;
		ps_writeout(arr, pi_size, pname);

		ps_doclear(arr);

		cout << "Please Given the Input file Name: (none for end)" << endl;
	}

	return 0;
}
