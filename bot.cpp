/*
Author: Muhtadi Munshi
Course: CSCI-135
Instructor: Gennadiy Maryash
Assignment: Project 3

Here, briefly, at least in one or a few sentences
describe what the program does.
*/



#include <cstdlib>
#include <iostream>
#include "bot.h"
#include <sstream>
#include <cctype>
using namespace std;

int closestDebris(int row, int col, Area &area, ostream &log, Loc loc, int id);
string minnum (int up, int down, int left, int right, ostream &log, Loc loc);
int checkcorners(int row, int col, Area &area);
void initvars();
int botbump(int id, Loc loc, Area &area);
bool isbothere(int row, int col, Area &area);

const int MAX_ROBOT_NUM = 50;
bool messup[50];
bool messupr[50];
bool messupl[50];
bool noturning[50];
bool turnu[50];
bool turnd[50];
bool turnl[50];
bool turnr[50];
int brokenum = 0;
int brokebotx = 0;
int brokeboty = 0;
int NUM;          // to remember number or robots
int ROWS, COLS;   // map dimensions
int repairman = 0;
/* Initialization procedure, called when the game starts: */
void onStart(int num, int rows, int cols, double mpr,
             Area &area, ostream &log)
{
	NUM = num;   // save the number of robots and the map dimensions
	ROWS = rows;
	COLS = cols;
	initvars();
	log << "Start!" << endl;
}

/* Deciding robot's next move */
Action onRobotAction(int id, Loc loc, Area &area, ostream &log) {
	int row = loc.r; // current row and column
	int col = loc.c;
	

	if (area.inspect(row, col) == DEBRIS) {
		::noturning[id]= false;
		return COLLECT; 
		
	}
	
    	if ((::brokenum != 0) and (id == ::repairman)) { // if broken robot, will fix robot before moving to debris
            if((row != ::brokeboty-1) or (row != ::brokeboty + 1)) {
                if (row > ::brokeboty + 1) {
                    return DOWN;
                }
                if (row < ::brokeboty - 1) {
                    return UP;
                }
            } 
    	    if (col != ::brokebotx) {
    	        if ((col + 1 == ::brokebotx) and (row == ::brokeboty)) {
    	            return REPAIR_RIGHT;
    	        }
    	        if ((col - 1 == ::brokebotx) and (row == ::brokeboty)) {
    	            return REPAIR_LEFT;
    	        }
                if (col > ::brokebotx) {
                    return LEFT;
                }
                if (col < ::brokebotx) {
                    return RIGHT;
                }
            }
            if (col == ::brokebotx) {
                if (row == ::brokeboty + 1) {
                    return REPAIR_DOWN;
                } 
                if (row == ::brokeboty - 1) {
                    return REPAIR_UP;
                }
            }
            
        }
    	
    	if (::noturning[id] == false) {
    		if (closestDebris(row, col, area, log, loc, id) == 0) {		
    			::turnu[id] = true;
    			::turnd[id] = false;
    			::turnl[id] = false;
    			::turnr[id] = false;
    			::noturning[id] = true;		
    			return UP;
    		
    		}
    		else if (closestDebris(row, col, area, log, loc, id) == 1) {	
    			::turnu[id] = false;
    			::turnd[id] = true;
    			::turnl[id] = false;
    			::turnr[id] = false;
    			::noturning[id] = true;		
    			return DOWN;
    		
    		}
    		else if (closestDebris(row, col, area, log, loc, id) == 2) {
    			::turnu[id] = false;
    			::turnd[id] = false;
    			::turnl[id] = true;
    			::turnr[id] = false;
    			::noturning[id] = true;
    			return LEFT;
    		}
    		else if (closestDebris(row, col, area, log, loc, id) == 3) {
    			::turnu[id] = false;	
    			::turnd[id] = false;
    			::turnl[id] = false;
    			::turnr[id] = true;
    			::noturning[id] = true;
    			return RIGHT;
    		}
    		
    		else if (checkcorners(row, col, area) == 1) {
    		    return UP;
    		}
    		
    		else if (checkcorners(row, col, area) == 2) {
    		    return DOWN;
    		}
    		
    		else {
    			::noturning[id] = false;
    			::messup[id] = true;		
    		}
    
    	}
    
    	if (::noturning[id] == true) {
    		//log << "True" << endl;
    		if (::turnu[id] == true) {
    		//	log << "up" << endl;
    		    if (botbump(id, loc, area) == 0) {
    		        return LEFT;
    		    }
    			return UP;
    		}
    		if (::turnd[id] == true) {
    		//	log << "down" << endl;
    		    if (botbump(id, loc, area) == 1) {
    		        return RIGHT;
    		    }
    			return DOWN;
    		}
    		if (::turnl[id] == true) {
    		    if (botbump(id, loc, area) == 2) {
    		        return UP;
    		    }
    			return LEFT;
    		}
    		if (::turnr[id] == true) {
    		    if (botbump(id, loc, area) == 3) {
    		        return DOWN;
    		    }
    			return RIGHT;
    		}
    
    		
    	}
    	
    	if (::messup[id] == true) {
    	//	log << "Engaging Messup Protocol" << endl;
    	//	log << "Engaging Messup Protocol" << endl;
    	//	log << "Engaging Messup Protocol" << endl;
    		if (::messupr[id] == true) {
    		//	log << "Messup Protocol: Moving Right" << endl;
    			if (col == ::COLS-1) {
    				::messupr[id] = false;
    				::messupl[id] = true;
    			//	log << "Messup Protocol: Switching Left" << endl;
    				::messup[id] = false;				
    				return LEFT;			
    			}
    			::messup[id] = false;
    			return RIGHT;		
    		}
    		if (::messupl[id] == true) {
    			//log << "Messup Protocol: Moving Left" << endl;
    			if (col == 0) {
    				::messupr[id] = true;
    				//log << "Messup Protocol: Switching Right" << endl;
    				::messupl[id] = false;
    				::messup[id] = false;
    				return RIGHT;			
    			}
    			::messup[id] = false;
    			return LEFT;		
    		}
    	}	
    	
	

	return UP;
}

int closestDebris(int row, int col, Area &area, ostream &log, Loc loc, int id) {
	int staticR = row;
	int staticC = col;
	int curR = row;
	int curC = col;
	bool enterwhile = false;
	int upcheck = 0; // amount of spaces between debris and robot from above
	int downcheck = 0; // amount of spaces between debris and robot from below
	int leftcheck = 0; // amount of spaces between debris and robot from the left
	int rightcheck = 0; // amount of spaces between debris and robot from right

	// checks distance  between bot and blue square above it
	while ((area.inspect(curR,curC) != DEBRIS ) and (curR > 0)) { 
		enterwhile = true;		
		upcheck += 1;
		curR -= 1;
		//log << "Upcheck=" << upcheck << endl;
		if ((area.inspect(curR,curC) != DEBRIS ) and (curR == 0)) {
			upcheck = 1000;	
		}
		
	}
	if (enterwhile != true) {
		upcheck = 1000;	
	}
	enterwhile = false;	
	curR = staticR;
	curC = staticC;
	// checks distance  between bot and blue square beneath it
	while ((area.inspect(curR,curC) != DEBRIS ) and (curR < ::ROWS)) {
		enterwhile = true;
		downcheck += 1;
		curR += 1;
		//log << "Downcheck=" << downcheck << endl;
		if ((area.inspect(curR,curC) != DEBRIS ) and (curR == ::ROWS)) {
			downcheck = 1000;	
		}
		
	}
	if (enterwhile != true) { //if the while loop was not entered, no debris
		downcheck = 1000;	
	}	
	enterwhile = false;
	curR = staticR;
	curC = staticC;
	// checks distance  between bot and blue square left of it
	while ((area.inspect(curR,curC) != DEBRIS ) and (curC > 0)) {	
		enterwhile = true;	
		leftcheck += 1;
		curC -= 1;
		//log << "Leftcheck=" << leftcheck << endl;
		if ((area.inspect(curR,curC) != DEBRIS ) and (curC == 0)) {
			leftcheck = 1000;	
		}	
		
	}
	if (enterwhile != true) {
		leftcheck = 1000;	
	}
	enterwhile = false;
	curR = staticR;
	curC = staticC;
	// checks distance  between bot and blue square right of it
	while ((area.inspect(curR,curC) != DEBRIS) and (curC < ::COLS)) {
		enterwhile = true;		
		rightcheck += 1;
		curC += 1;
		//log << "Rightcheck=" << rightcheck << endl;
		if ((area.inspect(curR,curC) != DEBRIS) and (curC == ::COLS)) {
			rightcheck = 1000;	
		}
		
	}
	if (enterwhile != true) {
		rightcheck = 1000;	
	}
	enterwhile = false;
	curR = staticR;
	curC = staticC;
	

	string best = minnum(upcheck, downcheck, leftcheck, rightcheck, log, loc);
	//log << "Bot " << id << ": " << "Best is " << best << "|up=" << upcheck << "|" << "right=" << rightcheck << "|" << "down=" << downcheck << "|" << "left=" << leftcheck << endl;
	if (best == "up") {
		return 0;
	}

	else if (best == "down") {
		return 1;
	}

	else if (best == "left") {
		return 2;
	}

	else if (best == "right") {
		return 3;
	}
	return 4;
}

//finds smallest number
string minnum (int up, int down, int left, int right, ostream &log, Loc loc) {
	int bestnum = 100;
	ostringstream backup;
	backup << "";
	if (up < bestnum) {
	//	log << up << " is better than " << bestnum << endl;
		bestnum = up;
		backup.str("");		
		backup << "up";	
		
	}
	if (down < bestnum) {
	//	log << down << " is better than " << bestnum << endl;	
		bestnum = down;
		backup.str("");
		backup << "down";
		
	}
	if (left < bestnum) {
	//	log << left << " is better than " << bestnum << endl;
		bestnum = left;
		backup.str("");	
		backup << "left";
		
	}
	if (right < bestnum) {
	//	log << right << " is better than " << bestnum << endl;
		bestnum = right;
		backup.str("");	
		backup << "right";
			
	}

	
	return backup.str();
	
}

void onRobotMalfunction(int id, Loc loc, Area &area, ostream &log) {
	log << "Robot " << id << " is damaged." << endl;
	::brokenum = id; //wont pick up first bot
	// logs down the coordinates
	::brokebotx = loc.c;
	::brokeboty = loc.r;
	::repairman = ::NUM - (rand() % ::NUM); // assigns random bot to repair
	
}

void onClockTick(int time, ostream &log) {
	if (time % 100 == 0) log << time << " ";
}

int checkcorners(int row, int col, Area &area) {
    int x = col;
    int y = row;
    if ((area.inspect(x+1,y+1) == DEBRIS) or (area.inspect(x-1,y+1) == DEBRIS)) {
        return 1;
    }
    if ((area.inspect(x-1,y-1) == DEBRIS) or (area.inspect(x+1,y-1) == DEBRIS)) {
        return 2;
    }
    return 0;
}

void initvars() {
    int i = 0;
    for (i = 0; i < 50; i++) {
        ::messup[i] = false;
    }
    for (i = 0; i < 50; i++) {
        ::messupr[i] = true;
    }
    for (i = 0; i < 50; i++) {
        ::messupl[i] = false;
    }
    for (i = 0; i < 50; i++) {
        ::noturning[i] = false;
    }
    for (i = 0; i < 50; i++) {
        ::turnu[i] = true;
    }
    for (i = 0; i < 50; i++) {
        ::turnd[i] = true;
    }
    for (i = 0; i < 50; i++) {
        ::turnl[i] = true;
    }
    for (i = 0; i < 50; i++) {
        ::turnr[i] = true;
    }
}

int botbump(int id, Loc loc, Area &area) {
    int row = loc.r;
    int col = loc.c; 
    if (::turnu[id] == true) {
        row = row + 1;
        if (isbothere(row, col, area) == true) {
            return 0;
        }
    }
    if (::turnd[id] == true) {
        row = row - 1;
        if (isbothere(row, col, area) == true) {
            return 1;
        }
        
    }
    if (::turnl[id] == true) {
        col = col - 1;
        if (isbothere(row, col, area) == true) {
            return 2;
        }
    }
    if (::turnr[id] == true) {
        col = col + 1;
        if (isbothere(row, col, area) == true) {
            return 3;
        }
        
    }

    return 4;
}

bool isbothere(int row, int col, Area &area) {
    int botx;
    int boty;
    for (int i = 0; i < 50; i++) {
        botx = area.locate(i).r;
        boty = area.locate(i).c; 
        if ((botx == col) and (boty == row)) {
            return true;
        }
    }
    return false;
}


