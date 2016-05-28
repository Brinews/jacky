#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include "GraphX.hpp"

using namespace std;

enum Location {LAND, SEA};

Location lastTerrain = SEA;

class Edge;
void drawWorld(int tx, int ty);
void drawLand(int traderx, int tradery);
void drawPolygon(int xs[], int ys[], int size, int traderx, int tradery);
void sort(int arr[], int bot, int top);
int partition(int list[], int bot, int top);
void sort(Edge arr[], int bot, int top, int type);
int partition(Edge list[], int bot, int top, int type);


const int XSIZE = 1330;
const int YSIZE = 880;
const int CARGO_HOLD = 10;

// added code for cloud
const double CLOUD_SPEED = 7.3;
const int CLOUDS_NUM = 2;

const int NUM_CITIES = 13;
const char NO_KEY = '\0';

typedef int Price;
enum TradeGood { EMPTY, GOLD, IRON, TEXTILES, SILK, WOOD, PORCELIN, SPICE, TRADEGOOD_COUNT };
string goodNames[] = {"Empty", "(G)old", "(I)ron", "(T)extiles", "Sil(k)", "(W)ood", "(P)orcelin", "(S)pice", "COUNT"};

GraphX window = GraphX(1330,880); // yes... I used a global...

class MapObject {
protected:
	string ID;
	int xpos;
	int ypos;
	int size;
public:
	MapObject() {xpos = 0; ypos = 0; ID = "Unknown"; size = 0;}
	MapObject(string id, int x, int y, int s) { ID = id; xpos = x; ypos = y; size = s;} 
	virtual void draw() {}
	int getX() { return xpos; }
	int getY() { return ypos; }
	string getID(){ return ID; }
	int getSize() { return size; }
};

class City : public MapObject {
protected:
	Price goodsHigh[TRADEGOOD_COUNT];
	Price goodsLow[TRADEGOOD_COUNT];
	Price goodsCurrent[TRADEGOOD_COUNT];
public:
	City();
	City(string id, int x, int y, int s);
	void setPrice(TradeGood item, Price low, Price max) { goodsHigh[item] = max; goodsLow[item] = low; }
	Price getPrice(TradeGood item) { return goodsCurrent[item]; }
	void regeneratePrice();
	bool nearCity(int x, int y);
	void draw();
};

class MoveableObject : public MapObject {
protected:
	double speed;
	int destinationX;
	int destinationY;
public:
	MoveableObject() : MapObject() {speed = 0; destinationX = xpos; destinationY = ypos;}
	MoveableObject(string id, int x, int y, int s, double spd) : MapObject(id,x,y,s) { speed = spd;  destinationX = xpos; destinationY = ypos;} 
	void setSpeed(double s) {speed = s;}
	double getSpeed() {return speed;}
	virtual void draw() {}
	virtual void updatePosition();
	virtual void setDestination(int x, int y) {destinationX = x; destinationY = y;}
	int getDestinationX() { return destinationX; }
	int getDestinationY() { return destinationY; }
};

class Ship : public MoveableObject {
protected:
	TradeGood cargo[CARGO_HOLD];
	Price money;
	int findGood(TradeGood tg);
public:
	Ship();	
	Ship(string name, int x, int y, int sz, double spd, int cash);
	bool addGood(TradeGood tg);
	bool removeGood(TradeGood tg);
	int getGoodCount(TradeGood tg);
	Price getMoney() { return money; }
	void adjustMoney(Price amount) { money += amount; }
	void draw();
	void remove();
};

/*
 * cloud class
 */
class Cloud : public MoveableObject{
public:
	Cloud() : MoveableObject(){};

	Cloud(string name,int x,int y,int s,double spd) 
		: MoveableObject(name,x,y,s,spd)
	{
	};

	/* bound exceed judge: when out of bound, we need reshape */
	bool outOfBounds();

	/* override */
	void draw();
	void remove();
};

int randInRange(int min, int max); // random number in [min, max] (integers)
double randInRange(double min, double max); // random number in [min, max] (reals)
void makeCities(City cities[]); // make all of our cities here
void display(City c, Ship s); // show cargo and city prices
void trade(City c, Ship &s); // trade with a city
void clearTerminal(); // blanks out terminal

void makeClouds(Cloud clouds[],int n);


// *********************************************
// ***** CITY FUNCTIONS (longer)
// *********************************************

City::City() : MapObject() 
{
	for(int i=0; i < TRADEGOOD_COUNT; i++)
	{
		goodsHigh[i] = 0;
		goodsLow[i] = 0;
		goodsCurrent[i] = 0;
	}
}


City::City(string id, int x, int y, int s) : MapObject(id, x, y, s) 
{
	for(int i=0; i < TRADEGOOD_COUNT; i++)
	{
		goodsHigh[i] = 0;
		goodsLow[i] = 0;
		goodsCurrent[i] = 0;
	}
}

void City::regeneratePrice()
{
	for(int i=0; i < TRADEGOOD_COUNT; i++)
	{
		goodsCurrent[i] = randInRange(goodsLow[i], goodsHigh[i]);
	}
}


bool City::nearCity(int x, int y)
{

	//int maxX, maxY;
	//window.getscreensize(maxX, maxY);
	//window.origin(-size/2.5,maxY-size/2.5);

	//if(abs(x-xpos) < size && abs(y - (maxY+ypos)) < size)
	if(abs(x-xpos) < size*2 && abs(y - (ypos)) < 2*size)
	{
		return true;
	}
	return false;
	//window.origin(0,0);
}

void City::draw()
{	
	//int maxX, maxY;
	//window.getscreensize(maxX, maxY);
	window.origin(-size/2.5,-size/2.5);

	window.pencolor("orange");
	window.symbol("solidsquare");
	window.symbolsize(size/4);

	window.setpos(xpos, ypos);
	window.plot();

	window.setpos(xpos +3.0/4*size, ypos);
	window.plot();
	
	window.symbolsize(size/3);
	window.setpos(xpos + size/4.0, ypos+size/3.0);
	window.plot();

	window.setpos(xpos +2.0/3*size, ypos+1.0/2*size);
	window.plot();
	
	window.symbol("solidtriangle");
	window.pencolor("black");
	window.setpos(xpos, ypos+size/4);
	window.plot();

	window.setpos(xpos +3.0/4*size, ypos+size/4);
	window.plot();
	
	window.symbolsize(size/2);
	window.setpos(xpos + size/4.0, ypos+2*size/3.0);
	window.plot();

	window.setpos(xpos +2.0/3*size, ypos+5.0/6*size);
	window.plot();
}

// *********************************************
// ***** MOVEABLEOBJECT FUNCTIONS (longer)
// *********************************************


void MoveableObject::updatePosition()
{
	double distance = sqrt( (xpos-destinationX)*(xpos-destinationX) + (ypos-destinationY)*(ypos-destinationY));
	if(distance < 1)
	{
		return;
	}
	double dx = (destinationX-xpos)/distance;
	double dy = (destinationY-ypos)/distance;
	xpos = xpos + min(dx*speed,abs(1.0*destinationX-xpos));
	ypos = ypos + min(dy*speed,abs(1.0*destinationY-ypos));
}

// *********************************************
// ***** SHIP FUNCTIONS (longer)
// *********************************************

// private! MINE
int Ship::findGood(TradeGood tg)
{
	for(int i=0; i < CARGO_HOLD; i++)
	{
		if(cargo[i] == tg)
		{
			return i;
		}
	}
	return -1; // not found!
}


Ship::Ship() : MoveableObject()
{
	money = 0;
	for(int i=0; i < CARGO_HOLD; i++)
	{
		cargo[i] = EMPTY;
	}
}

Ship::Ship(string name, int x, int y, int sz, double spd, int cash) : MoveableObject(name, x, y, sz, spd)
{
	money = cash;
	for(int i=0; i < CARGO_HOLD; i++)
	{
		cargo[i] = EMPTY;
	}
}

bool Ship::addGood(TradeGood tg)
{
	int openIndex = findGood(EMPTY);
	if(openIndex != -1)
	{
		cargo[openIndex]=tg;
		return true;
	}
	return false;
}

bool Ship::removeGood(TradeGood tg)
{
	int goodSpot = findGood(tg);
	if(goodSpot != -1)
	{
		cargo[goodSpot]=EMPTY;
		return true;
	}
	return false;
}

int Ship::getGoodCount(TradeGood tg)
{
	int total=0;
	for(int i=0; i < CARGO_HOLD; i++)
	{
		if(cargo[i] == tg)
		{
			total++;
		}
	}
	return total;
}

void Ship::draw()
{
	window.origin(-size/2.5,-size/2.5);
	window.pencolor("red");
	window.symbol("solidsquare");
	window.symbolsize(size/4);

	window.setpos(xpos, ypos+1.0/20*size);
	window.plot();

	window.setpos(xpos +1.0/5*size, ypos);
	window.plot();

	window.setpos(xpos +2.0/5*size, ypos);
	window.plot();

	window.setpos(xpos +3.0/5*size, ypos+1.0/20*size);
	window.plot();

	
	window.symbolsize(3/12.0*size);
	window.setpos(xpos + 1.0/2*size, ypos+3.0/12*size);
	window.plot();

	window.setpos(xpos + 1.0/2*size, ypos+6.0/12*size);
	window.plot();
	
	window.symbol("solidtriangle");
	window.pencolor("white");
	window.symbolsize(3.0/4*size);
	window.setpos(xpos + 1.0/2*size, ypos+3.0/4*size);
	window.plot();

	window.origin(0,0);	
}

void Ship::remove()
{
	window.origin(-size/2.5,-size/2.5);
	window.pencolor("red");
	window.symbol("solidsquare");
	window.symbolsize(size/4);

	window.setpos(xpos, ypos+1.0/20*size);
	window.erase();

	window.setpos(xpos +1.0/5*size, ypos);
	window.erase();

	window.setpos(xpos +2.0/5*size, ypos);
	window.erase();

	window.setpos(xpos +3.0/5*size, ypos+1.0/20*size);
	window.erase();

	
	window.symbolsize(3/12.0*size);
	window.setpos(xpos + 1.0/2*size, ypos+3.0/12*size);
	window.erase();

	window.setpos(xpos + 1.0/2*size, ypos+6.0/12*size);
	window.erase();
	
	window.symbol("solidtriangle");
	window.pencolor("white");
	window.symbolsize(3.0/4*size);
	window.setpos(xpos + 1.0/2*size, ypos+3.0/4*size);
	window.erase();

	window.origin(0,0);	
}

// *********************************************
// ***** CLOUD FUNCTIONS
// *********************************************

bool Cloud::outOfBounds()
{
	// top, bottom, left, right bound
	return xpos == 0 || ypos == 0 
			|| xpos == XSIZE || ypos == YSIZE;
}

/*
 * draw the cloud 
 */
void Cloud::draw()
{
	window.origin(-size/2.5,-size/2.5);
	window.pencolor("white");
	window.symbol("solidcircle");

	// first circle
	window.symbolsize(size/2);
	window.setpos(xpos-1.0/6*size, ypos+1.0/6*size);
	window.plot();

	// second circle
	window.symbolsize(size/2);
	window.setpos(xpos+1.0/6*size, ypos+1.0/6*size);
	window.plot();

	// third circle
	window.symbolsize(size/2);
	window.setpos(xpos, ypos);
	window.plot();
}

void Cloud::remove()
{
	window.origin(-size/2.5,-size/2.5);
	window.pencolor("white");
	window.symbol("solidcircle");

	// first cirle
	window.symbolsize(size/2);
	window.setpos(xpos-1.0/6*size, ypos+1.0/6*size);
	window.erase();

	// second cirle
	window.symbolsize(size/2);
	window.setpos(xpos+1.0/6*size, ypos+1.0/6*size);
	window.erase();

	// third cirle
	window.symbolsize(size/2);
	window.setpos(xpos, ypos);
	window.erase();
}

// *********************************************
// ***** END CLASS FUNCTIONS
// *********************************************


int randInRange(int min, int max)
{
	return rand()%(max-min+1) + min;
}

double randInRange(double min, double max)
{
	double zeroToOne = (1.0*rand()/RAND_MAX);
	return zeroToOne*(max-min) + min;
}

/*
 * single cloud with random postion and size 
 */
Cloud makeSingleCloud()
{
	int pos  = rand() % YSIZE ;
	int size = rand() % 30 + 60;

	Cloud cloud;
	
	static int rand_direction = 0;

	if (rand_direction){
		cloud = Cloud("cloud1", 0, pos+1, size, CLOUD_SPEED);
		cloud.setDestination(pos, 0);
	} else {
		cloud = Cloud("cloud2", pos-1, YSIZE, size, CLOUD_SPEED);
		cloud.setDestination(XSIZE, pos-1);
	}

	rand_direction = !rand_direction;
	
	return cloud; 
}

void makeClouds (Cloud clouds[],int n)
{
	for(int i = 0;i < n;i++){
		clouds[i] = makeSingleCloud();
	}
}


void makeCities(City tc[])
{
	int maxX, maxY;
	window.getscreensize(maxX, maxY);
	
	int citySize = 30;
	int i=0;

	tc[i] = City("Sofala", 85, maxY-796, citySize);
	tc[i].setPrice(GOLD,50,100);
	tc[i].setPrice(IRON,10,30);
	tc[i].setPrice(TEXTILES,200,420);
	tc[i].setPrice(SILK,400,800);
	tc[i].setPrice(WOOD,2,30);
	tc[i].setPrice(PORCELIN,190,420);
	tc[i].setPrice(SPICE,320,600);
	i++;
	
	tc[i] = City("Mombasa", 165, maxY-566, citySize);
	tc[i].setPrice(GOLD,80,140);
	tc[i].setPrice(IRON,20,40);
	tc[i].setPrice(TEXTILES,200,420);
	tc[i].setPrice(SILK,400,700);
	tc[i].setPrice(WOOD,10,30);
	tc[i].setPrice(PORCELIN,180,320);
	tc[i].setPrice(SPICE,320,600);
	i++;
	
	tc[i] = City("Aden", 251, maxY-344, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,300,800);
	tc[i].setPrice(WOOD,10,70);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Hormuz", 377, maxY-200, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,300,600);
	tc[i].setPrice(WOOD,20,70);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Surat", 546, maxY-292, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,70,100);
	tc[i].setPrice(SILK,300,500);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,320,500);
	i++;
	
	tc[i] = City("Calicut", 608, maxY-385, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,60,90);
	tc[i].setPrice(TEXTILES,80,100);
	tc[i].setPrice(SILK,300,500);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Masulipatam", 666, maxY-323, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,100);
	tc[i].setPrice(TEXTILES,70,120);
	tc[i].setPrice(SILK,300,500);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Chittagong", 824, maxY-273, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,300,500);
	tc[i].setPrice(WOOD,5,20);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Melaka", 925, maxY-489, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,300,500);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Da Nang", 986, maxY-354, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,140,170);
	tc[i].setPrice(SILK,300,400);
	tc[i].setPrice(WOOD,5,20);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,220,300);
	i++;
	
	tc[i] = City("Maluku Islands", 1261, maxY-580, citySize);
	tc[i].setPrice(GOLD,100,200);
	tc[i].setPrice(IRON,40,60);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,300,400);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,140,220);
	tc[i].setPrice(SPICE,120,250);
	i++;
	
	tc[i] = City("Guangzhou", 1063, maxY-249, citySize);
	tc[i].setPrice(GOLD,160,300);
	tc[i].setPrice(IRON,70,90);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,200,400);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,90,140);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
	tc[i] = City("Hangzhou", 1136, maxY-183, citySize);
	tc[i].setPrice(GOLD,100,300);
	tc[i].setPrice(IRON,100,200);
	tc[i].setPrice(TEXTILES,100,120);
	tc[i].setPrice(SILK,160,360);
	tc[i].setPrice(WOOD,10,40);
	tc[i].setPrice(PORCELIN,70,100);
	tc[i].setPrice(SPICE,220,400);
	i++;
	
}

void display(City c, Ship s)
{
	clearTerminal();
	cout << "The " << s.getID() << " sails into "<<c.getID()<<".\n";
	cout << "You have " << s.getMoney() << " currency.\n";
	cout << setw(10) << "Good" << setw(10) << "Price" << setw(10) << "On ship" << endl;
	for(int i =1 ; i < TRADEGOOD_COUNT; i++)
	{
		TradeGood g = static_cast<TradeGood>(i);
		cout << setw(10) << goodNames[g] << setw(10) << c.getPrice(g) << setw(10) << s.getGoodCount(g) << endl;
	}
}

void clearTerminal()
{
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void trade(City c, Ship &s)
{
	display(c, s);
	char key = NO_KEY;
	string options = "gitkwps";
	string message = "";
	
	while(tolower(key) != 'q' && key != ' ')
	{
		key = NO_KEY;		
		message = "";
		
		if(window.input() == "keypress")
		{
			key = window.eventkey();
		}
		
		
		TradeGood option = static_cast<TradeGood>(options.find(tolower(key))+1);
		int buy = options.find(key)+1;
		
		if(buy > 0 && option > 0)		// selling
		{
			bool success = s.removeGood(option);
			if(success)
			{
				s.adjustMoney(c.getPrice(option));
			}
			else
			{
				message = "You do not have any "; 
				message += goodNames[option]; 
				message +=" in your cargo.\n";
			}
		}
		else if (option > 0 )	// buying
		{
			if( s.getMoney() >= c.getPrice(option) )
			{
				bool success = s.addGood(option);
				if(success)
				{
					s.adjustMoney(-c.getPrice(option));
				}
				else
				{
					message = "You do not have any room left on your ship!\n";
				}
			}
			else
			{
				message = "You do not have enough money to buy ";
				message += goodNames[option];
				message +="!\n";
			}
		}
		
		if(key != NO_KEY)
		{
			display(c, s);
			cout << message;
		}			
	}
	cout << "You sail back onto the sea.\n";
}

int main()
{
	srand(time(0));
	string shipName;
	cout << "\n\nWelcome to trading in the Indian ocean.  Left click to move anywhere, but if you ram your ship into the land you will lose it.  When in a town you can buy a trade good by pressing the indicated letter in parenthesis.  Upper case will buy, lower case will sell.  Press 'q' to leave a town.\n\n";
//	cout << "What do you wish to name your ship?\n";
//	getline(cin,shipName);
	shipName = "Your ship";
	
	City tradeCenters[NUM_CITIES];
	makeCities(tradeCenters);
	
	window.backgroundcolor("blue");
		
	Ship trader = Ship(shipName, 500, 200, 50, 8, 200);

	Cloud clouds[CLOUDS_NUM];
	makeClouds(clouds,CLOUDS_NUM);

	int inCity = -2;
	string input="";
	int time = 200;
	bool inharbor = false;
	
	while(lastTerrain != LAND)
	{
		if(time == 200)
		{
			window.reset();
			time=0;
			for(int i=0; i < NUM_CITIES; i++)
			{			
				tradeCenters[i].regeneratePrice();
			}
		}

		drawWorld(trader.getX(), trader.getY());
		
		inCity = -1;
		
		for(int i=0; i < NUM_CITIES; i++)
		{			
			tradeCenters[i].draw();
			if(inCity == -1 && tradeCenters[i].nearCity(trader.getX(), trader.getY()))
			{
				inCity=i;
			}
			if(i==NUM_CITIES-1 && inCity == -1)
			{
				inharbor = false;
			}
		}
		trader.remove();
		trader.updatePosition();
		trader.draw();


		/* draw cloud and update its position */
		for(int i = 0;i < CLOUDS_NUM;i++){
			clouds[i].remove();

			clouds[i].updatePosition();

			if(clouds[i].outOfBounds()){
				clouds[i] = makeSingleCloud();
			}

			clouds[i].draw();
		}

		
		if(inCity > -1 && !inharbor)
		{
			inharbor = true;
			trade(tradeCenters[inCity], trader);
			trader.setDestination(trader.getX(),trader.getY());
		}


		input = window.input();
		if(input == "leftbutton")
		{
			int x, y;
			window.eventxy(x,y);
			trader.setDestination(x,y);
		}

		window.delay(0.05);
		time++;
	}
	
	cout << "You crash into the land and lose!\n\n";
	return 0;
}





/************************************************/
/**************Travel below here at**************/
/**************your own risk*********************/
/************************************************/
/**************(you do not need to***************/
/**************change anything below*************/
/**************here)*****************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/

class Edge {
private:
	int ymin;
	int ymax;
	int x_ymin;
	double slopeInv;
public:
	Edge() { ymin=0; ymax=0; x_ymin=0; slopeInv = 0; }
	Edge(int x1, int y1, int x2, int y2)
	{
        if(y1 < y2)
        {
                ymin = y1;
                ymax = y2;
                x_ymin = x1;
                slopeInv = 1.0*(x2-x1)/(y2-y1);
        }
        else
        {
                ymin = y2;
                ymax = y1;
                x_ymin = x2;
                slopeInv = 1.0*(x1-x2)/(y1-y2);
        }
	}
	int getYMin() { return ymin; }
	int getYMax() { return ymax; }
	int getX() { return x_ymin; }
	double getSlopeInv() { return slopeInv; }
	friend ostream& operator<<(ostream & out, Edge & e)
	{
        cout << "[(x_miny="<<e.x_ymin<<", ymin="<<e.ymin<<"), ymax ="<<e.ymax<<",slope^-1="<<e.slopeInv<<"]";

        return out;
	}	
};

// edge partition
int partition(Edge list[], int bot, int top, int type)
{
	int pivot= type == 0? list[top].getYMin() : list[top].getYMax();
	int i = bot;
	for(int j=bot; j < top; j++)
	{
        if( (type == 0 && list[j].getYMin() > pivot) || (type == 1 && list[j].getYMax() > pivot))
        {
                Edge temp = list[i];
                list[i] =  list[j];
                list[j] = temp;
                i++;
        }
	}
	Edge temp = list[i];
	list[i] =  list[top];
	list[top] = temp;

	return i;
}

// edge sort (largest to smallest)
void sort(Edge arr[], int bot, int top, int type)
{
	if(bot < top)
	{
			int mid = partition(arr, bot, top, type);
			sort(arr, bot, mid-1, type);
			sort(arr, mid+1, top, type);
	}
}

// int partition
int partition(int list[], int bot, int top)
{
	int pivot = list[top];
	int i = bot;
	for(int j=bot; j < top; j++)
	{
        if(list[j] < pivot)
        {
                int temp = list[i];
                list[i] =  list[j];
                list[j] = temp;
                i++;
        }
	}
	int temp = list[i];
	list[i] =  list[top];
	list[top] = temp;

	return i;
}

// int sort (smallest to largest)
void sort(int arr[], int bot, int top)
{
	if(bot < top)
	{
			int mid = partition(arr, bot, top);
			sort(arr, bot, mid-1);
			sort(arr, mid+1, top);
	}
}

void drawPolygon(int xs[], int ys[], int size, int traderx, int tradery)
{
	Edge global[size];
	Edge active[size];
	int numGlobal = size;
	int numActive = 0;

	int* switchPoint = new int[1]; // junk in my trunk


	int xmin=min(xs[0],xs[size-1]);
	int xmax=max(xs[0],xs[size-1]);
	int ymin=min(ys[0],ys[size-1]);
	int ymax=max(ys[0],ys[size-1]);

	int shift = 0;
	if(ys[0] == ys[size-1])
	{
			shift--;
	}
	else
	{
			global[0] = Edge(xs[size-1], ys[size-1], xs[0], ys[0]);
	}
	for(int i =1; i < size; i++)
	{
			if(ys[i] == ys[i-1])
			{
					shift--;
			}
			else
			{
					global[i+shift] = Edge(xs[i-1], ys[i-1], xs[i], ys[i]);
					if(xs[i] > xmax)
					{
							xmax = xs[i];
					}
					if(xs[i] < xmin)
					{
							xmin = xs[i];
					}
					if(ys[i] > ymax)
					{
							ymax = ys[i];
					}
					if(ys[i] < ymin)
					{
							ymin = ys[i];
					}
			}
	}

	size += shift;

	/*
	 *
	 * Zeros were from default constructor, from when array was created.
	 *
	 */
	numGlobal = size; //Gets rid of zeros

	sort(global, 0, size-1, 0);

	//for(int i=0;i<numGlobal;i++){cout << i << " ----" << global[i]<< "\n";}cout<<"\n\n";

	for(int y=ymin; y <= ymax; y++)
	{
        bool changed = false;
        // remove old ones
       
        for(int i=numActive-1; i >= 0; i--)
        {

                if(active[i].getYMax() < y)
                {
                        changed = true;
                        numActive--;
                }
                else
                {
        /*
         *
         * Need to be able to determine whether or not two consecutive x values is just a point or a glitch on conversion.
         *                 IE: Should 201, 201 be a point, or should we stop there
         *
         * So I removed edge vertices that didn't create a local y max or min. And that works (Remove this else block and you'll see all the problem spots)
         *
         */
                    for(int j= numGlobal-1; j  >= 0; j--)
                    {
                   
                        if(active[i].getYMax() == y && active[i].getYMax() == global[j].getYMin() && global[j].getX() == active[i].getX()+static_cast<int>(active[i].getSlopeInv()*(active[i].getYMax()-active[i].getYMin())))
                        {
                            changed = true;
                            numActive--;
                        }
                    }
                }
        }

        // add new ones
        for(int i= numGlobal-1; i >= 0; i--)
        {
                if(global[i].getYMin() <= y)
                {
                        numGlobal--;
                        active[numActive] = global[i];
                        numActive++;
                        changed = true;
                }
        }
       
       

        if(changed)
        {
                sort(active, 0, numActive-1, 1);

                delete [] switchPoint;
                switchPoint = new int[numActive];
        }

        for(int i=0; i < numActive; i++)
        {
                switchPoint[i] = (y-active[i].getYMin())*active[i].getSlopeInv() + active[i].getX();
        }

        sort(switchPoint, 0, numActive-1);

        if(numActive % 2)
        {
            window.pencolor("orange");//Color problem lines orange so I can see them
            cout << "y = "<<y<< "["<<switchPoint[0];
            for(int i=1; i < numActive; i++)
            {
                    cout <<","<<switchPoint[i];
            }
            cout << "]\n";
            for(int i=0;i<numActive;i++){cout << i << " ----" << active[i]<< "\n";}cout<<"\n\n";
           
        }
        else
        {
            window.pencolor("green");
        }

        window.setpos(0,y);
        window.penup();
        int inside = 0;
        int lastx = std::numeric_limits<int>::min();
        for(int i = 0; i < numActive; i++)
        {
                window.setpos(switchPoint[i], y);
                if(inside == 0)
                {
                        inside=1;
                        window.pendown();
                }
                else
                {
                        inside=0;
                        window.penup();
                }
                
                if(tradery == y && traderx > lastx && traderx <switchPoint[i])
                {
                        if(inside == 0)
                        {
                                lastTerrain = LAND;
                        }
                        else
                        {
                                lastTerrain = SEA;
                        }
                }

                lastx = switchPoint[i];
        }
        window.penup();
	}
	delete [] switchPoint;
}

void drawLand(int traderx, int tradery)
{
	//af-eur-asia
	int xmax = 1330, ymax = 880;
	window.getscreensize(xmax,ymax);
	window.origin(0,ymax);
	tradery -= ymax;
	int xs[] = {75, 148, 302, 210, 78, 250, 378, 264, 412, 484, 621, 657, 786, 868, 882, 942, 938, 894, 902, 969, 1016, 974, 1120, 1164, 1120, 0, 0};
	int ys[] = {-880, -614, -404, -405, -189, -374, -256, -182, -230, -225, -446, -350, -260, -345, -446, -525, -471, -434, -382, -430, -380, -297, -249, -170, 0, 0, -880};

	//int xs[] = {100, 200, 300, 400, 400, 100};
	//int ys[] = {0, 100, 0, 200, 400, 400};



	window.pencolor("green");
	window.pensize(1);
	drawPolygon(xs, ys, sizeof(xs)/sizeof(int), traderx, tradery);
	window.origin(0,0);
	
	// maluku islands
	window.symbol("solidcircle");
	window.setpos(1261, ymax-580);
	window.symbolsize(40);
	window.plot();

}

void drawWorld(int tx, int ty)
{
	//window.reset();
	drawLand(tx, ty); 
}
