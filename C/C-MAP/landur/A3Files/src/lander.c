/*
 * Date: 2015-04-01
 * Optimization:
 *  1. getopt used
 *  2. fuel display
 *  3. color the ship and thrust
 *  4. horizon range disposed
 */

#include"lander.h"

#include <unistd.h>


const char *prompt[] = {
	" Press any key to start.",
	" (Then press arrow keys to rotate, space for thust, 'q' to quit)",
	"left arrow key rotate counter-clockwise, right clockwise, space for thust, q to quit.",
	"Empty fuel tank !"
};

const char exec_name[] = "java -jar Sketchpad.jar";

FILE* sketchpad;	/* file pipe */
FILE* landscape;  	/* landscape file handler */

MapPoint ship[SHIPMAX]; 		/* shape of ship */
long ship_degree;				/* degree of ship when rotate */

MapPoint LANDSCAPE[LANDMAX];	/* landscape shape */
int land_point_cnt;				/* landscape points */

int thrust_on;			/* switch for control */
int is_over;			/* lander is_over or not */

int opt;
double fuel;			/* fuel optimaztion */

double xV,yV; 			/* velocity, acceleration */
double gravity,thrust;	/* speed control */

sigset_t block_mask_g;	/* signal block sets */

void parse_args(int argc, char **argv)
{
	int option;
	char **endptr = NULL;

	while ((option = getopt(argc, argv, ":f:g:t:i::")) != -1) {
		switch (option) {
			case 'f':
				landscape = fopen(optarg, "r");
				if (NULL == landscape) {
					fprintf(stderr, "open %s failed.\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'g':
				gravity = strtod(optarg, endptr);
				if (gravity < 0 || gravity > 20) {
					fprintf(stderr, "gravity must be in [0, 20]\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 't':
				thrust = strtod(optarg, endptr);
				if (thrust > 0 || thrust < -20) {
					fprintf(stderr, "thrust must be in [-20, 0]\n");
					exit(EXIT_FAILURE);
				}
				break;

			case 'i':
				opt = 1; /* enable optimization */
				break;
			case ':':
			case '?':
				fprintf(stderr,"bad '-%c',or option with no value.", optopt);
				exit(EXIT_FAILURE);
		}
	}
}

void ship_init()
{
	/* do init */
	opt=0; ship_degree=90;

	//left_up 
	ship[0].x=315; ship[0].y=1;
	//right_up
	ship[1].x=325; ship[1].y=1;
	//left_down
	ship[2].x=310; ship[2].y=19;
	//right_down
	ship[3].x=330; ship[3].y=19;
	//thrust
	ship[4].x=320; ship[4].y=23;

	/* state set */
	is_over=0; thrust_on=0;

	//velocity
	xV=0;yV=0;

	fuel=10; 
}

int main(int argc,char *argv[])
{
	ship_init();

	parse_args(argc, argv);

	setup_curses();

	sketchpad = open_pipe(exec_name);
	if (sketchpad == NULL) exit(EXIT_FAILURE);

	move(5, 10); printw(prompt[0]);
	move(6, 10); printw(prompt[1]);
	refresh();
	int c = getch();

	nodelay(stdscr, true);
	erase();

	move(5, 0); printw(prompt[2]);

	//draw landscape
	draw_landscape(landscape);

	if(opt==1){
		draw_fuel();
	}

	//timer
	// just initializes so it's empty (man 3 sigemptyset)
	sigemptyset(&block_mask_g);

	// block_mask_g will now indicate to signal functions that something
	// (i.e., blocking) must be to SIGALRM signal
	sigaddset(&block_mask_g, SIGALRM);

	// set up signal handler (man 2 sigaction)
	struct sigaction handler;
	handler.sa_handler = handle_timeout;
	sigemptyset(&handler.sa_mask);
	handler.sa_flags = 0;

	// when SIGALRM occurs (set up below), handle_timeout will now be called
	if (sigaction(SIGALRM, &handler, NULL) == -1){
		exit(EXIT_FAILURE);
	}

	// set up timer
	struct itimerval timer;

    // value for time between sending SIGALRM
	struct timeval time_delay;
	time_delay.tv_sec = 0;
	time_delay.tv_usec = 50000;

	timer.it_interval = time_delay;

    // value for initial time (before 1st alarm is triggered)
	struct timeval start;
	start.tv_sec = 0;
	start.tv_usec = 50000;

	timer.it_value = start;

	// will execute repeatedly, sending SIGALRM every timer.it_interval
	// microseconds, who initial value is timer.it_value.
	// should handle error better (and should use a macro).  should use
	// errno.  note it may save you some hassle if you do this, too
	if (setitimer(ITIMER_REAL, &timer, NULL) == -1){
		exit(EXIT_FAILURE);
	}

	for ( ; ; )
	{
		c = getch();
		if (c != ERR)
		{
			if (c == KEY_LEFT){
				if(is_over==0) ship_rotate(1); //rotate left 
			}
			else if(c == KEY_RIGHT){
				if(is_over==0) ship_rotate(0); //rotate right 
			}
			else if(c == ' '){ // thrust
				if(opt==0){
					thrust_on=1;
				}
				else if(opt==1){
					if(fuel <= 0) {
						move(8,10);
						refresh();

						printw(prompt[3]);
					}
					else if(fuel>0){
						thrust_on=1;
					}
				}
			}
			else if(c == 'q'){ //quit and block signals
				sigset_t old_mask;

				if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
					exit(EXIT_FAILURE);
				}

				if(sketchpad != NULL && is_over == 0) 
					fprintf(sketchpad,"end\n"); 

				break;
			}
		}
	}

	unset_curses();

	fclose(landscape);
	close_pipe(sketchpad);

	exit(EXIT_SUCCESS);
}

// functions for curses:
void setup_curses(){
	// use return values. see man pages. likely just useful for error
	// checking (NULL or non-NULL, at least for init_scr)
	initscr();
	cbreak();
	noecho();
	// needed for cursor keys (even though says keypad)
	keypad(stdscr, true);
}

void unset_curses(){
	keypad(stdscr, false);
	nodelay(stdscr, false);
	nocbreak();
	echo();
	endwin();
}
