/*
 *  Copyright (c) 2010 Patrick Lerner
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 * */

#include "TYFUIBenchmark.h"
#include "TYFUITemplate.h"
#include "TYFGame.h"
#include "TYFTeam.h"
#include "helper.h"
#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

/**
 * this is called when the UI officially takes over as the main thread
 * Basically it just runs until the game finishes here
 * */
void TYFUIBenchmark::run()
{
	clock_t start, finish;
	start = clock();
	
	int Team1Sum = 0;
	int Team2Sum = 0;
	int Team1Win = 0;
	int Team2Win = 0;
	int Ties = 0;
	int count = 1000;
	
	for (int c = 1; c <= count; c++)
	{
		while (this->Game->nextPlay() == PL_OK) {}
		
		GameInfo info = this->Game->getGameInfo();
		
		cout << "Game " << c << " ";
		for (int i = 0; i <= 1; i++)
			cout << info.Scores[i].Name << " " << info.Scores[i].Points << " ";
		cout << endl;
		
		Team1Sum += info.Scores[0].Points;
		Team2Sum += info.Scores[1].Points;
		
		if (info.Scores[0].Points > info.Scores[1].Points)
			Team1Win += 1;
		else if (info.Scores[0].Points < info.Scores[1].Points)
			Team2Win += 1;
		else if (info.Scores[0].Points == info.Scores[1].Points)
			Ties += 1;
		
		this->Game = NULL;
		this->Game = new TYFGame(this);
	}
	finish = clock();

	cout << endl;
	cout << "Simulation Speed:..." << ( count / ((finish - start) / CLOCKS_PER_SEC )) << " games/sec" << endl;
	cout << "Team1 Average:..... " << Team1Sum / count << " pts/game" << endl;
	cout << "Team2 Average:..... " << Team2Sum / count << " pts/game"<< endl;
	cout << "Team1 Wins:........ " << Team1Win << endl;
	cout << "Team2 Wins:........ " << Team2Win << endl;
	cout << "Ties:.............. " << Ties << endl;
}
