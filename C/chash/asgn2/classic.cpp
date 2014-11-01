#include "classic.h"

Classic::Classic() : Cd()
{
	memset(work, 0, sizeof(work));
}

Classic::Classic(char *w, char *s1, char *s2, int n, double x) : Cd(s1, s2, n, x)
{
	//Cd::Cd(s1, s2, n, x);

	memset(work, 0, sizeof(work));
	strcpy(work, w);
}

/*
Classic::Classic(const Classic& d)
{
	Cd::Cd(d);
	strcpy(work, d.work);
}
*/

void Classic::Report() const
{
	cout << work << ", ";
	Cd::Report();
}

Classic& Classic::operator=(const Classic &d)
{
	if (&d != this)
	{
		Cd::operator=(d);
		strcpy(work, d.work);
	}
	else
		cout << "self assignment." << endl;

	return *this;
}
