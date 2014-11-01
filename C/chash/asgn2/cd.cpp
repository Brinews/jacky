#include "cd.h"

#include <stdlib.h>

Cd::Cd()
{
	memset(performers, 0, sizeof(performers));
	memset(label, 0, sizeof(label));

	selections = 0;
	playtime = 0.0;
}

Cd::Cd(char *s1, char *s2, int n, double x)
{
	int i = 0;
	memset(performers, 0, sizeof(performers));

	while (s1 != NULL && *s1 != '\0')
		performers[i++] = *s1++;

	i = 0;
	memset(label, 0, sizeof(label));
	while (s2 != NULL && *s2 != '\0')
		label[i++] = *s2++;

	selections = n;
	playtime = x;
}

Cd::Cd(const Cd& d)
{
	selections = d.selections;
	playtime = d.playtime;

	strcpy(performers, d.performers);
	strcpy(label, d.label);
}

void Cd::Report() const
{
	cout << performers << ", " << label << ", "
		<< selections << ", " << playtime << endl;
}

Cd& Cd::operator=(const Cd &d)
{
	if (&d != this)
	{
		selections = d.selections;
		playtime = d.playtime;

		strcpy(label, d.label);
		strcpy(performers, d.performers);
	}
	else
		cout << "self assignment." << endl;

	return *this;
}
