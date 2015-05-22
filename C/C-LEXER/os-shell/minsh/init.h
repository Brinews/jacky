#ifndef _INIT_H_
#define _INIT_H_

#define PSIZ 256
extern char workpath[PSIZ];

void setup(void);
void init(void);
char *getpath();
void free_commands(void);

#endif /* _INIT_H_ */
