#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <assert.h>
#include <time.h>

//#define UNIX_ENV

#ifdef UNIX_ENV
#include <sys/resource.h>
#endif

using namespace std;

#define NAMLEN	40
#define STALEN	4
#define PEOSIZ	1024
#define TELLEN	3
#define TELSTR	13

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
	unsigned int TelNo[TELLEN]; /* added */
} PeopleInfo_t;

typedef struct PeopleInfo* PeopleInfo_p;

int pi_capacity = 0;
int pi_size = 0;

/***
 * helper function
 */
unsigned int ps_random_in_range(int x, int y); /* added */
void ps_telno2string(unsigned int *telno, char *str); /* added */

/****
 * Global Function
 */
PeopleInfo_p ps_readin(char *filename);
void ps_print(PeopleInfo_p p, ostream& out);
void ps_writeout(PeopleInfo_t *arr, int siz, char *filename);

/* search functions */
PeopleInfo_p ps_telsearch(PeopleInfo_t *arr, int siz, char *telno); /* added */
PeopleInfo_p ps_namesearch(PeopleInfo_t *arr, int siz, char *name);
void ps_testsearch(PeopleInfo_t *arr, int siz);

/* sort functions */
int ps_findmax(PeopleInfo_t *arr, int s, int e);
int ps_nameorder(PeopleInfo_t *arr, int s, int e);
void ps_swap(PeopleInfo_t *arr, int left, int right);
PeopleInfo_p ps_dosort(PeopleInfo_t *arr, int siz);

/* added sort */
void ps_selectionsort(PeopleInfo_t *arr, int siz);
void ps_insertionsort(PeopleInfo_t *arr, int siz);
void ps_impinsertsort(PeopleInfo_t *arr, int siz);
void ps_bubblesort(PeopleInfo_t *arr, int siz);
void ps_twosidebubblesort(PeopleInfo_t *arr, int siz);

double ps_timecost(PeopleInfo_t *arr, int siz, char* fname);
void ps_doclear(PeopleInfo_t *arr);

/***
 * Random function for telenumber
 * Param@(x, y)
 * Return@(r)
 */
unsigned int ps_random_in_range(int x, int y)
{
	int rd = rand()%(y-x+1);

	return rd + x;
}

/***
 * IO: read in
 * Param@(filename)
 * Ret: PeopleInfo_p *p
 */
PeopleInfo_p ps_readin(char *filename)
{
	char outfile[NAMLEN];
	ifstream fin(filename);

	strcpy(outfile, "my");
	strcat(outfile, filename);
	ofstream fout(outfile);

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

		/* added */
		peo_arr[idx].TelNo[0] = ps_random_in_range(201, 999);
		peo_arr[idx].TelNo[1] = ps_random_in_range(1, 999);
		peo_arr[idx].TelNo[2] = ps_random_in_range(0, 9999);

		ps_print(&peo_arr[idx], fout);

		idx++;
	}

	pi_size = idx;

	fin.close();
	fout.close();

	return peo_arr;
}

void ps_print(PeopleInfo_p p, ostream& out)
{
	char telstr[TELSTR];
	memset(telstr, 0, sizeof(telstr));
	ps_telno2string(p->TelNo, telstr);

	out << p->SocialNo << " " 
		<< p->FirstName << " " 
		<< p->LastName << " "
		<< p->BirthDay << " " 
		<< p->State  << " "
		<< telstr << " "
		<< endl;
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
		ps_print(&arr[i], out);
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
			ps_print(&arr[i], cout);
		}
	}

	return NULL;
}

/***
 * telno translation: int -> char 
 * Param@(int[3], char *)
 */
void ps_telno2string(unsigned int *telno, char *str)
{
	memset(str, 0, sizeof(str));
	sprintf(str, "%03d-%03d-%04d", telno[0], telno[1], telno[2]);
}

/***
 * Telephone Number Research Function
 * Param@(PeopleInfo_t *arr, char *tel)
 * Return: PeopleInfo_t
 */
PeopleInfo_p ps_telsearch(PeopleInfo_t *arr, int siz, char *telno)
{
	PeopleInfo_t *ret = NULL;
	char telstr[TELSTR];

	for (int i = 0; i < siz; i++)
	{
		ps_telno2string(arr[i].TelNo, telstr);

		if (strcmp(telstr, telno) == 0)
		{
			ret = &arr[i];
			ps_print(&arr[i], cout);
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
 * Ret : index with oldest in [s, e)
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
 * added: find max by name order
 * Param@(PeopleInfo_t *arr, int s, int e)
 * Return: index with asec name
 */
int ps_nameorder(PeopleInfo_t *arr, int s, int e)
{
	char fname[NAMLEN], lname[NAMLEN];
	int idx = s;

	int fcmp, lcmp;
	
	strcpy(fname, arr[s].FirstName);
	strcpy(lname, arr[s].LastName);

	for (int i = s+1; i < e; i++)
	{
		fcmp = strcmp(arr[i].FirstName, fname);
		lcmp = strcmp(arr[i].LastName, lname);

		if (lcmp < 0 || (lcmp == 0 && fcmp < 0))
		{
			strcpy(fname, arr[i].FirstName);
			strcpy(lname, arr[i].LastName);

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

/***
 * added sort 
 * selection sort
 */
void ps_selectionsort(PeopleInfo_t *arr, int siz)
{
	int max = 0;

	for (int i = 0; i < siz; i++)
	{
		max = ps_nameorder(arr, i, siz);

		if (max != i)
			ps_swap(arr, i, max);
	}
}

int ps_cmpbyname(PeopleInfo_t *p, PeopleInfo_t *q)
{
	int l, r;
	
	l = strcmp(p->LastName, q->LastName);
	r = strcmp(p->FirstName, q->FirstName);

	if (l < 0 || (l==0 && r < 0)) return -1;

	if (l == 0 && r == 0) return 0;

	return 1;
}

/***
 * insertion sort
 */
void ps_insertionsort(PeopleInfo_t *arr, int siz)
{
	int i, j;

	for (i = 1; i < siz; i++)
	{
		j = i;

		while (j > 0 && ps_cmpbyname(&arr[j], &arr[j-1]) < 0)
		{
			ps_swap(arr, j, j-1);
			j--;
		}
	}
}

/***
 * improved insertion sort
 */
void ps_impinsertsort(PeopleInfo_t *arr, int siz)
{
	int i, j;
	PeopleInfo_t tmp;

	for (i = 1; i < siz; i++)
	{
		j = i;

		memcpy(&tmp, &arr[i], sizeof(PeopleInfo_t));

		while (j > 0 && ps_cmpbyname(&tmp, &arr[j-1]) < 0)
		{
			memcpy(&arr[j], &arr[j-1], sizeof(PeopleInfo_t));
			j--;
		}

		if (i != j)
			memcpy(&arr[j], &tmp, sizeof(PeopleInfo_t));
	}
}

/***
 * Bubble sort 
 */
void ps_bubblesort(PeopleInfo_t *arr, int siz)
{
	int i, j;

	for (i = siz; i > 0; i--)
	{
		for (j = 0; j < i-1; j++)
		{
			if (ps_cmpbyname(&arr[j], &arr[j+1]) > 0)
				ps_swap(arr, j, j+1);
		}
	}
}

/***
 * Two-sided bubble sort
 */
void ps_twosidebubblesort(PeopleInfo_t *arr, int siz)
{
	int low = 0, high = siz-1;
	int exchange = 1;
	int low1, high1;

	while( exchange == 1 && (high - low) > 1)
	{
		exchange = 0;

		for (int i = low; i < high; i++)
		{
			if (ps_cmpbyname(&arr[i], &arr[i+1]) > 0)
			{
				ps_swap(arr, i, i+1);
				exchange = 1;
				high1 = i; /* record the last swap */
			}
		}

		// other side 
		if (exchange == 1)
		{
			high = high1;
			if (low < high)
			{
				exchange = 0;
				for (int i = high; i > low; i--)
				{
					if (ps_cmpbyname(&arr[i], &arr[i-1]) < 0)
					{
						ps_swap(arr, i, i-1);
						exchange = 1;
						low1 = i; /* record the last swap */
					}
				}

				if (exchange) low = low1;
			}
		}
	}
}

double get_cpu_time()
{
#ifdef UNIX_ENV
	struct rusage ruse;
	getrusage(RUSAGE_SELF, &ruse);

	return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0
		+ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0;

#else

	return clock()/(double)CLOCKS_PER_SEC;
#endif

}
/***
 * Time Function
 * Param@(PeopleInfo_t *arr, int siz)
 * Ret: milliseconds
 */
double ps_timecost(PeopleInfo_t *arr, int siz, char *fname)
{
	double start, end;
	char pname[NAMLEN];

	strcpy(pname, "old_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	PeopleInfo_t *old = new PeopleInfo_t[siz];
	memcpy(old, arr, sizeof(PeopleInfo_t) * siz);

	start = get_cpu_time();
	ps_selectionsort(arr, siz);
	end = get_cpu_time();
	cout << "Selection Sort : " << end - start << " sec." << endl;
	strcpy(pname, "ss_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	memcpy(arr, old, sizeof(PeopleInfo_t) * siz);
	start = get_cpu_time();
	ps_insertionsort(arr, siz);
	end = get_cpu_time();
	cout << "Insertion Sort : " << end - start << " sec." << endl;
	strcpy(pname, "is_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	memcpy(arr, old, sizeof(PeopleInfo_t) * siz);
	start = get_cpu_time();
	ps_impinsertsort(arr, siz);
	end = get_cpu_time();
	cout << "Improve Insertion Sort : " << end - start << " sec." << endl;
	strcpy(pname, "iis_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	memcpy(arr, old, sizeof(PeopleInfo_t) * siz);
	start = get_cpu_time();
	ps_bubblesort(arr, siz);
	end = get_cpu_time();
	cout << "Bubble Sort : " << end - start << " sec." << endl;
	strcpy(pname, "bs_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	memcpy(arr, old, sizeof(PeopleInfo_t) * siz);
	start = get_cpu_time();
	ps_twosidebubblesort(arr, siz);
	end = get_cpu_time();
	cout << "Two-Sided Bubble Sort : " << end - start << " sec." << endl;
	strcpy(pname, "tbs_");
	strcat(pname, fname);
	ps_writeout(arr, siz, pname);

	delete []old;

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
		ps_timecost(arr, pi_size, fname);

		ps_doclear(arr);

		cout << "Please Given the Input file Name: (none for end)" << endl;
	}

	return 0;
}
