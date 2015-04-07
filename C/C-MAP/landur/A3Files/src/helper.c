#include"lander.h"

/****************pipe operations **************************/
FILE* open_pipe(const char* exec_name){

	FILE *sketchpad = popen(exec_name, "w");

	if (sketchpad == NULL){
		fprintf(stderr, "open pipe %s failed.", exec_name);
	}

	return sketchpad;
}

void close_pipe(FILE* sketchpad){
	pclose(sketchpad);
}

/****************color operations **************************/
void set_color(int r, int g, int b) 
{
	fprintf(sketchpad, "setColor %d %d %d\n", r, g, b);
}

static void set_black(){
	set_color(0, 0, 0);
}

static void set_blue(){
	set_color(0, 0, 255);
}

static void set_purple(){
	set_color(200, 0, 255);
}

/****************line operations **************************/
static void draw_rect(MapPoint *R)
{
	draw_line(R[0], R[1]);
	draw_line(R[0], R[2]);
	draw_line(R[1], R[3]);
	draw_line(R[2], R[3]);
}

void draw_line(MapPoint A, MapPoint B)
{
	fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
			lround(A.x),lround(A.y),lround(B.x),lround(B.y));
}

void erase_line(MapPoint A,MapPoint B)
{
	fprintf(sketchpad, "eraseSegment %ld %ld %ld %ld\n",
			lround(A.x),lround(A.y),lround(B.x),lround(B.y));
}

/****************rotate operations **************************/
double angle_to_radian(long angle)
{
	double radian;
	radian = angle*PI/180.0;

	return radian;
}

void rotate_by_degree(MapPoint* P, int clockwise)
{
	double rotated_x,rotated_y;

	long angle = 10;
	if (clockwise == 0) angle = 10;
	else angle = 350;

	rotated_x = (P->x*cos(angle_to_radian(angle)))
					-(P->y*sin(angle_to_radian(angle)));
	rotated_y = (P->x*sin(angle_to_radian(angle))) 
					+ (P->y*cos(angle_to_radian(angle)));

	P->x=rotated_x;
	P->y=rotated_y;
}

/****************draw operations **************************/
void draw_ship()
{
	set_blue();
	draw_rect(ship);
}

void draw_thrust()
{
	set_purple();
	draw_line(ship[2],ship[4]);
	draw_line(ship[3],ship[4]);
}

void erase_ship()
{
	erase_line(ship[0],ship[1]);
	erase_line(ship[0],ship[2]);
	erase_line(ship[1],ship[3]);
	erase_line(ship[2],ship[3]);
}

void erase_thrust()
{
	erase_line(ship[2],ship[4]);
	erase_line(ship[3],ship[4]);
}

void draw_landscape(FILE* landscape){
	set_black();

	memset(LANDSCAPE,0,25*sizeof(MapPoint));

	int idx=0;
	long x_value,y_value;

	char linebuf[256];
	memset(linebuf,0,256);

	while (fgets(linebuf, 256,landscape) != NULL){

		sscanf(linebuf,"%ld%ld", &x_value, &y_value);

		LANDSCAPE[idx].x   = x_value;
		LANDSCAPE[idx++].y = y_value;

		memset(linebuf, 0, 256);
	}

	int i=0;
	land_point_cnt=idx;

	for(i=0; i < idx-1; i++){
		draw_line(LANDSCAPE[i],LANDSCAPE[i+1]);
	}
}

/****************timer operations **************************/
void handle_timeout(int signal)
{
	static int called_times = 0;
	int idx;
	int land_index;
	int i;
	double maxX, minX;

	called_times++;

	// called because of SIGALRM
	if (signal == SIGALRM){
		// gets timer, puts it into timer (man 2 getitimer)
		struct itimerval timer;
		if (getitimer(ITIMER_REAL, &timer) == -1){
			exit(EXIT_FAILURE);
		}

		//block the signal 
		sigset_t old_mask;
		if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
			exit(EXIT_FAILURE);
		}

		//warp around the left_right side if necessary:
		maxX = ship[0].x;
		minX = ship[0].x;

		for(i=1;i<SHIPMAX;i++){
			if(ship[i].x>maxX) maxX=ship[i].x;
			if(ship[i].x<minX) minX=ship[i].x; 
		}

		if(maxX<=0){
			for(i=0;i<SHIPMAX;i++) ship[i].x+=640;
		} else if(minX>=640){
			for(i=0;i<SHIPMAX;i++) ship[i].x-=640;
		}

		//erase the ship and draw a new one

		erase_ship();
		erase_thrust();

		for(i=0;i<SHIPMAX;i++){

			if(thrust_on==0){
				ship[i].y+=(yV*0.05)+(0.5*gravity*0.05*0.05);
			}
			else if(thrust_on==1){
				ship[i].y+=(yV*0.05)+
					(0.5*(gravity+(thrust*sin(angle_to_radian(ship_degree))))*0.05*0.05);
			}

			if(xV!=0){
				ship[i].x+=(xV*0.05)+
					(0.5*(thrust*cos(angle_to_radian(ship_degree)))*0.05*0.05);
			}
		}

		draw_ship(); // new ship

		//velocity update
		if(thrust_on==0){
			yV+=gravity*0.05;

		} else if(thrust_on==1){

			draw_thrust();

			yV+=0.05*(gravity+(thrust*sin(angle_to_radian(ship_degree))));
			xV+=0.05*thrust*cos(angle_to_radian(ship_degree));
			thrust_on=0;

			if(opt==1){ /* update fuel */
				fuel-=0.05;
				erase_fuel(fuel);
			}
		}

		fflush(sketchpad);

		erase_thrust();

		//is_intersect

		for(idx=0;idx<land_point_cnt-1;idx++){
			if((is_intersect(LANDSCAPE[idx],LANDSCAPE[idx+1],ship[0],ship[1])==1)
					||(is_intersect(LANDSCAPE[idx],LANDSCAPE[idx+1],ship[0],ship[2])==1)
					||(is_intersect(LANDSCAPE[idx],LANDSCAPE[idx+1],ship[1],ship[3])==1)
					||(is_intersect(LANDSCAPE[idx],LANDSCAPE[idx+1],ship[2],ship[3])==1)){
				is_over=1;
				land_index=idx;
				break;
			}
		}

		//fuel display
		if(opt==1){

			move(7,12); if(fuel>0) printw("Fuel Left: %lf",fuel);
			move(8,12); printw("Time Passed: %lf",called_times/20.0);

			refresh();
		}

		// game is done (e.g., ship has crashed or landed)
		if (is_over==1) {

			if(opt==0) move(7,10);
			if(opt==1) move(9,10);

			if(yV<20
				&& ship_degree==90
				&& LANDSCAPE[land_index].y==LANDSCAPE[land_index+1].y){
				printw("LANDED ! ! !");
			}
			else{
				printw("CRASHED ! ! !");
			}

			timer.it_interval.tv_usec = 0;
			timer.it_value.tv_usec = 0;

			fprintf(sketchpad,"end\n");
		}

		//unblock the signal
		if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
			exit(EXIT_FAILURE);
		}

		if (setitimer(ITIMER_REAL, &timer, NULL) == -1){
			exit(EXIT_FAILURE);
		}
	}
}

/****************blocking signal operations **************************/
void ship_rotate(int direction)
{
	sigset_t old_mask;
	MapPoint trans_ship[SHIPMAX];

	// set blocked signal set to block_mask_g (man 2 sigprocmask).
	// so now block SIGALRM.
	if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0){
		exit(EXIT_FAILURE);
	}

	erase_ship();

	int idx=1;

	double min_x=ship[0].x;
	double max_x=ship[0].x;
	double min_y=ship[0].y;
	double max_y=ship[0].y;

	for(idx=1;idx<SHIPMAX-1;idx++){
		if(ship[idx].x<min_x) min_x=ship[idx].x; 
		if(ship[idx].x>max_x) max_x=ship[idx].x; 
		if(ship[idx].y<min_y) min_y=ship[idx].y; 
		if(ship[idx].y>max_y) max_y=ship[idx].y;
	}

	double center_x=(min_x + max_x) / 2.0;
	double center_y=(min_y + max_y) / 2.0;

	for(idx=0;idx<SHIPMAX;idx++){
		/* normalize */
		trans_ship[idx].x=ship[idx].x-center_x;
		trans_ship[idx].y=ship[idx].y-center_y;

		rotate_by_degree(&trans_ship[idx], direction);

		ship[idx].x=trans_ship[idx].x+center_x;
		ship[idx].y=trans_ship[idx].y+center_y;
	}

	if(direction==0){
		if(ship_degree==360) ship_degree=0;
		ship_degree+=10;
	}

	if(direction==1){
		if(ship_degree==0) ship_degree=360; 
		ship_degree-=10;
	}

	draw_ship();

	// unblock signal by setting mask to old value (which in our
	// case wasn't blocking anything)
	if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0){
		exit(EXIT_FAILURE);
	}
}

/****************check operations **************************/
int is_intersect(MapPoint A,MapPoint B,MapPoint C,MapPoint D)
{

	double distAB, the_cos, the_sin, new_X, ABpos;

	/* fail if a segment has zero length */
	if ((A.x==B.x && A.y==B.y)||(C.x==D.x && C.y==D.y)){return 0;}
	if ((A.x==C.x && A.y==C.y)||(B.x==C.x && B.y==C.y)
			||(A.x==D.x && A.y==D.y)||(B.x==D.x && B.y==D.y)){return 0;}

	MapPoint cpyA=A;
	MapPoint cpyB=B;
	MapPoint cpyC=C;
	MapPoint cpyD=D;

	/* translate system so point A is in the origin */
	cpyB.x-=cpyA.x; cpyB.y-=cpyA.y;
	cpyC.x-=cpyA.x; cpyC.y-=cpyA.y;
	cpyD.x-=cpyA.x; cpyD.y-=cpyA.y;

	/* calculate distance between A and B */
	distAB=sqrt((cpyB.x*cpyB.x)+(cpyB.y*cpyB.y));

	/* rotate system so point B is on the positive x-axis */
	the_cos=cpyB.x/distAB;
	the_sin=cpyB.y/distAB;

	new_X=(cpyC.x*the_cos)+(cpyC.y*the_sin);
	cpyC.y=(cpyC.y*the_cos)-(cpyC.x*the_sin);
	cpyC.x=new_X;

	new_X=(cpyD.x*the_cos)+(cpyD.y*the_sin);
	cpyD.y=(cpyD.y*the_cos)-(cpyD.x*the_sin);
	cpyD.x=new_X;

	/* fail if landscape segment does not intersect ship */
	if ((cpyC.y<0. && cpyD.y<0.)
			||(cpyC.y>=0. && cpyD.y>=0.)){return 0;}

	/* calculate position of intersection along segment AB */
	ABpos=cpyD.x+(cpyC.x-cpyD.x)*cpyD.y/(cpyD.y-cpyC.y);

	/* fail if segment CD crosses AB outside line AB */
	if (ABpos<0.||ABpos>distAB){return 0;}

	return 1;
}

/****************fuel operations **************************/

int startY = 30, endY = 80;
int leftX = 570, rightX = 590;

void draw_fuel(){

	set_blue();

	MapPoint fuel_line[2];
	MapPoint fuel_level[4];
	
	fuel_line[0].x= leftX; fuel_line[1].x = rightX;
	fuel_line[0].y = fuel_line[1].y = startY;

	long fi=0;

	for(fi=startY;fi<endY;fi++){
		draw_line(fuel_line[0],fuel_line[1]);
		fuel_line[0].y++; fuel_line[1].y++;
	}

	fuel_level[0].x=leftX-1;
	fuel_level[0].y=startY-1;
	fuel_level[1].x=rightX+1;
	fuel_level[1].y=startY-1;
	fuel_level[2].x=leftX-1;
	fuel_level[2].y=endY;
	fuel_level[3].x=rightX+1;
	fuel_level[3].y=endY;

	draw_rect(fuel_level);
}

void erase_fuel(double fuel){

	MapPoint fuel_line[2];
	fuel_line[0].x= leftX;
	fuel_line[1].x= rightX;
	fuel_line[0].y=endY-(5*fuel);
	fuel_line[1].y=endY-(5*fuel);

	erase_line(fuel_line[0],fuel_line[1]);
}
