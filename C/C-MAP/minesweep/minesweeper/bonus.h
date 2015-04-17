#include <string>
#include "minesweeper.h"

#ifndef bonus_H
#define	bonus_H

class bonus : public minesweeper {
    public:
		 bonus();
		 bonus(int col, int row, int numOfMines);
         int saveGame(std::string path);        //Save minesweeper object state to file
         int loadGame(std::string path);        //Load minesweeper object state from file
         int saveStatistics(std::string path);  //Save overall game statistics to file
         int loadStatistics(std::string path);  //Load overall game statistics from file
         int getWins();                         //Returns # of wins
         void setWins();                        //Modify # of wins
         int getLoss();                         //Returns # of losses
         void setLoss();                        //Modify # of losses
         double getRatio();                     //Returns Win/loss Ratio
         int getWinStreak();                    //Returns current Win Streak (consecutive wins)
         void setWinStreak();                   //Modifies current Win Streak # (consecutive wins)
         int getLongestWinStreak();             //Returns longest Win Streak (largest win streak seen)
         void setLongestWinStreak();            //Modifies longest Win Streak (largest win streak seen)
	 //void initialMineField(int fpX, int fpY);	//Initialize game board given first revealed tile.
							//Overrides minesweeper.initialMineField(fpX,fpY)
	 
    
    protected:
        int wins;                               // # of total wins
        int loss;                               // # of total losses
        double ratio;                           // Win/Loss ratio
        int winStreak;                          // Current win streak count
        int longestWinStreak;                   // Longest win streak count
        void calcRatio();                      //Calculates Win/loss Ratio
};

#endif	/* BONUS_H */
