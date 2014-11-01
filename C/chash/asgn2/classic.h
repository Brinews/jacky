#ifndef __CLASSIC_H
#define __CLASSIC_H

#include "cd.h"

class Classic : public Cd
{
	private:
		char work[100];
	public:
		Classic();
		Classic(char *w, char *s1, char *s2, int n, double x);
		Classic(const Classic &c) : Cd(c)
		{
			strcpy(work, c.work);
		}

		virtual ~Classic(){}

		virtual void Report() const;
		Classic &operator=(const Classic &c);
};

#endif
