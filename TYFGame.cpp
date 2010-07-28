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

#include "TYFGame.h"
#include "TYFPlayer.h"
#include "TYFUITemplate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <iostream>
#include <iomanip>

/**
	Dear future me. Please forgive me. 
	I can't even begin to express how sorry I am for this code.
**/

using namespace std;

//#define DEBUG 1

/*
 * Univeral Random Number Getter.
 * Awesome. Pure Awesome.
 * */
int random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

/*
 * constructor
 * */
TYFGame::TYFGame(TYFUITemplate *UI)
{
	// CAN I HAZ RANDOM?
	srand((int)time(NULL));

	this->Ball.Possession = 0;
	this->Ball.Position = 20;
	this->Ball.ToGo = 10;
	this->Ball.Down = 1;
	this->Time.Time = 15*60;
	this->Time.Quarter = 0;
	this->clockStopped = false;
	
	// init teams
	this->Teams[0] = new TYFTeam("CHI");
	this->Teams[1] = new TYFTeam("GB");
	
	this->UI = UI;
}

/**
 * play the next play
 * */
PlayReturn TYFGame::nextPlay()
{
	this->UI->beginPlay();
	// game not started yet? Kickoff firsts
	if (this->Time.Quarter == 0)
	{
		this->doKickOff();
		this->Time.Quarter = 1;
		this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	else
		if (this->isStillRunning())
		{
			this->doAction();
			
			// touchdown check
			if (this->Ball.Position > 100)
			{
				this->getThisTeam()->scorePoints(7);
				this->doKickOff();
				this->UI->endPlay(PLAY_TOUCHDOWN);
			}
			
			// safety check
			else if (this->Ball.Position < 0)
			{
				this->UI->endPlay(PLAY_SAFETY);
				this->getOtherTeam()->scorePoints(2);
				this->doKickOff();
			}
			
			// first down check
			else if (this->Ball.ToGo < 0)
			{
				this->UI->endPlay(PLAY_FIRST_DOWN);
				this->Ball.Down = 1;
				this->Ball.ToGo = 10;
			}
			
			// turnover on downs check
			if (this->Ball.Down == 5)
			{
				this->UI->endPlay(PLAY_TURNOVER_ON_DOWNS);
				this->changeBallPossession();
			}
			cout << endl;
			return PL_OK;
		}
		else
			// game is not running anymore
			return PL_GAME_OVER;
}

/**
 * get some game info out
 * */
GameInfo TYFGame::getGameInfo()
{
	GameInfo info;
	info.Ball = this->Ball;
	info.Time = this->Time;
	info.Scores[0] = TeamScore(this->Teams[0]->getName(), this->Teams[0]->getPoints());
	info.Scores[1] = TeamScore(this->Teams[1]->getName(), this->Teams[1]->getPoints());
	return info;
}

/*
 * some play action here
 * */
void TYFGame::doAction()
{
	if (this->Ball.Down == 4)
	{
		if (this->getDistanceToEndzone() < 33)
			this->doFieldGoal();
		else
			this->doPunt();
	}
	else
	{
		int r = random(1, 10);
		if (r < 6)
		{
			// various Pass Length
			this->doPass(PASS_NORMAL);
			if (this->clockStopped)
				this->advanceTime(random(10, 25));
			else
				this->advanceTime(random(20, 35));
		}
		else
		{
			this->doRun();
			
			if (this->clockStopped)
				this->advanceTime(random(20, 35));
			else
				this->advanceTime(random(30, 45));
		}
		this->Ball.Down++;
	}
}

/**
	advance the ball by n yards
**/
void TYFGame::advanceBall(int n)
{
	#ifdef DEBUG
		cout <<	"[DEBUG] Ball +" << n << endl;
	#endif
	this->Ball.Position += n;
	this->Ball.ToGo -= n;
}

/*
 * Puts the Ball n yards from the current teams end zone
 * */
int TYFGame::setBallPosition(int n)
{
	#ifdef DEBUG
		cout <<	"[DEBUG] Ball =" << n << endl;
	#endif
	this->Ball.Position = n;
	this->Ball.ToGo = 10;
}

/*
 * advance the time by n seconds
 * */
void TYFGame::advanceTime(int n)
{
	this->Time.Time -= n;
	if (this->Time.Time <= 0)
	{
		if (this->Time.Quarter < 4)
		{
			this->Time.Quarter += 1;
			this->Time.Time = 15*60;
		}
		else
		{
			this->Time.Time = 0;
		}
	}
}

/*
 * stop the clock
 * */
void TYFGame::stopClock()
{
	this->clockStopped = true;
	#ifdef DEBUG
		cout <<	"[DEBUG] Stopped the clock" << endl;
	#endif
}

/*
 * do a KickOff
 * */
void TYFGame::doKickOff()
{
	this->setBallPosition(20);
	
	int r = this->getThisTeam()->getKicker()->getKickRating();
	
	int mod = 0;
	switch(r)
	{
		case 1:
			mod = random(-14, 10);
			break;
		case 2:
			mod = random(-13, 10);
			break;
		case 3:
			mod = random(-9, 12);
			break;
		case 4:
			mod = random(-7, 14);
			break;
		case 5:
			mod = random(-5, 15);
			break;
	}
	
	int kick = 65 + mod;
	this->advanceBall(kick);
	this->UI->playKickOff(kick);
	this->changeBallPossession();
}

/*
 * the distance from the line of scimmage to the end zone
 * */
int TYFGame::getDistanceToEndzone()
{
	return 100 - this->Ball.Position;
}

/*
 * punt the Ball away
 * */
void TYFGame::doPunt()
{
	int r = this->getThisTeam()->getKicker()->getKickRating();
	
	int mod = 0;
	switch(r)
	{
		case 1:
			mod = random(-14, 14);
			break;
		case 2:
			mod = random(-13, 13);
			break;
		case 3:
			mod = random(-9, 9);
			break;
		case 4:
			mod = random(-7, 7);
			break;
		case 5:
			mod = random(-5, 5);
			break;
	}
	
	int kick = this->getDistanceToEndzone() * 0.9 + mod;
	if (kick > 50)
		kick = 50 + random(0, 5 + r);
	this->advanceBall(kick);
	
	bool touchback = (this->Ball.Position > 100);
	if (touchback)
		this->setBallPosition(80);
	this->UI->playPunt(kick, touchback);
	this->changeBallPossession();
}

/*
 * a try for a field foal
 * */
void TYFGame::doFieldGoal()
{
	int r = this->getThisTeam()->getKicker()->getKickRating();
	int dist = this->getDistanceToEndzone() + 17;
	
	int x = random(0, 100);
	bool good = false;
	if (dist < 20)
		good = (x < 98);
	else if (dist < 30)
		good = (x < 92);
	else if (dist < 40)
		good = (x < 80 + r/2);
	else if (dist < 50)
		good = (x < 60 + r);
	else if (dist < 55)
		good = (x < 45 + r*2);
	else if (dist < 60)
		good = (x < 20 + r*2);
	else if (dist < 70)
		good = (x < 10 + r*2);
	
	this->UI->playFieldGoal(dist, good);
	if (good)
	{
		this->getThisTeam()->scorePoints(3);
		this->doKickOff();
	}
	else
		this->changeBallPossession();
}

/*
 * Is Game Still Running?
 * */
bool TYFGame::isStillRunning()
{
	return (this->Time.Time > 0);
}

/*
 * change the teams
 * */
void TYFGame::changeBallPossession()
{
	#ifdef DEBUG
		cout <<	"[DEBUG] Change Ball Possession" << endl;
	#endif
	if (this->Ball.Possession == 0)
		this->Ball.Possession = 1;
	else
		this->Ball.Possession = 0;
	this->Ball.Position = 100 - this->Ball.Position;
	this->Ball.Down = 1;
	this->Ball.ToGo = 10;
	this->stopClock();
}

/*
 * Get a pointer to the team which has the ball
 * */
TYFTeam* TYFGame::getThisTeam()
{
	return this->Teams[this->Ball.Possession];
}

/*
 * Same as getThisTeam(), but the other team.
 * */
TYFTeam* TYFGame::getOtherTeam()
{
	if (this->Ball.Possession == 0)
		return this->Teams[1];
	else
		return this->Teams[0];
}

/*
 * Pass that Ball
 * */
void TYFGame::doPass(PassType type)
{
	int pass = this->pass(type);
	if (this->isSacked(type))
	{
		int sack = this->sacked(type);
		this->UI->playSack(-sack);
		this->advanceBall(sack);
		this->stopClock();
	}
	else if (this->isIncomplete(type))
	{
		this->UI->playPass(pass, PASS_INCOMPLETE);
		this->stopClock();
	}
	else
	{
		this->advanceBall(pass);
		bool intercepted = this->isIntercepted(pass, type);
		
		if (intercepted)
		{
			this->UI->playPass(pass, PASS_INTERCEPTED);
			this->changeBallPossession();
		}
		else
			this->UI->playPass(pass, PASS_OK);
	}
}

/*
 * this here handles all the pass action
 * */
int TYFGame::pass(PassType type)
{
	int pass = 0;
	int rand = random(0, 9);

	// Short Passes
	if (type == PASS_SHORT)
	{
		if ((0 <= rand) && (rand <= 2))
			pass = random(0, 2);
		else if ((3 <= rand) && (rand <= 5))
			pass = random(3, 4);
		else if ((6 <= rand) && (rand <= 8))
			pass = random(5, 9);
		else if (9 == rand)
			pass = random(10, 15);
	}
	// Normal Range Passes
	else if (type == PASS_NORMAL)
	{
		if ((0 <= rand) && (rand <= 1))
			pass = random(0, 2);
		else if ((2 <= rand) && (rand <= 3))
			pass = random(3, 4);
		else if ((4 <= rand) && (rand <= 7))
			pass = random(5, 9);
		else if (8 == rand)
			pass = random(10, 19);
		else if (9 == rand)
			pass = random(20, 25);
	}
	// Long Passes
	else if (type == PASS_LONG)
	{
		if (0 == rand)
			pass = random(0, 4);
		else if ((1 <= rand) && (rand <= 2))
			pass = random(5, 11);
		else if ((3 <= rand) && (rand <= 7))
			pass = random(12, 19);
		else if ((8 <= rand) && (rand <= 9))
			pass = random(20, 30);
	}

	return pass;
}

/*
 * Did that dude just steal my ball?!
 * */
bool TYFGame::isIntercepted(int pass, PassType type)
{
	int diff = this->getOffDefDifferences();
	int rand = random(0, 100);

	switch (type)
	{
		case PASS_SHORT:
			return (rand < 4 - diff / 2);
		case PASS_NORMAL:
			return (rand < 6 - diff / 2);
		case PASS_LONG:
			return (rand < 8 - diff / 2);
	}
}

/*
 * Was a Pass Completed?
 * */
bool TYFGame::isIncomplete(PassType type)
{
	int diff = this->getOffDefDifferences();
	int rand = random(0, 100);
	
	switch (type)
	{
		case PASS_SHORT:
			return (rand > 38 + diff * 2);
		case PASS_NORMAL:
			return (rand > 44 + diff * 2);
		case PASS_LONG:
			return (rand > 55 + diff * 2);
	}
}

/*
 * QB Sacked?
 * */
bool TYFGame::isSacked(PassType type)
{
	int diff = this->getOffDefDifferences();
	int rand = random(0, 100);
	
	switch (type)
	{
		case PASS_SHORT:
			return (rand < 7 - diff);
		case PASS_NORMAL:
			return (rand < 9 - diff);
		case PASS_LONG:
			return (rand < 13 - diff);
	}
}

/*
 * How far back was he?
 * */
int TYFGame::sacked(PassType type)
{
	int diff = this->getOffDefDifferences();
	int min = 0;

	switch (type)
	{
		case PASS_SHORT:
			min = -2 - diff;
		case PASS_NORMAL:
			min = -4 - diff;
		case PASS_LONG:
			min = -6 - diff;
	}
	if (min > -1)
		min = -1;
	return random(min, -1);
}

/*
 * offense / defense differences
 * more than 0 means better offense than defense
 * less than 0 means, well you can figure that one out yourself
 * */
int TYFGame::getOffDefDifferences()
{
	return this->getThisTeam()->getOffenseRating() - this->getOtherTeam()->getDefenseRating();
}

/*
 * was a ball fumbled?
 * */
bool TYFGame::isFumble()
{
	int diff = this->getOffDefDifferences();
	int rand = random(0, 100);
	return (rand < 5 - diff);
}

/*
 * fumbled ball recovered?
 * */
bool TYFGame::isRecovered(int run)
{
	int rand = random(1, 10);
	if (run <= 0)
		return (rand < 8);
	else if (run <= 4)
		return (rand < 5);
	else if (run <= 10)
		return (rand < 4);
	else if (run <= 15)
		return (rand < 3);
	else if (run <= 10)
		return (rand < 2);
	else
		return (rand < 1);
}

/*
 * Let's run the ball
 * */
void TYFGame::doRun()
{
	int run = this->run();
	this->UI->playRun(run);
	
	this->advanceBall(run);
	
	if (this->isFumble())
	{
		this->stopClock();
		bool recovered = this->isRecovered(run);
		this->UI->playFumble(recovered);
		
		// not recovered
		if (!recovered)
			this->changeBallPossession();
	}
}

/*
 * This function is responsible for run actions
 * */
int TYFGame::run()
{
	int per = random(1, 10);
	int run = 0;

	// offense / defense differences
	int diff = this->getOffDefDifferences();

	switch(per)
	{
		/* 50% runs under 4 yards */
		case 1: // 10% under 0 yards
			if (diff <= -2) // even worse for bad defenses
				run = random(-4 + diff, -1);
			else
				run = random(-4, -1);
			break;
		case 2: // 10% 0 yard runs
			run = 0;
			break;
		case 3: // 10% 1 yard runs
			run = 1;
			break;
		case 4: // 10% 2 yard runs
			run = 2;
			break;
		case 5: // 10% 3 yard runs
			run = 3;
			break;
		
		/* 50% runs equal or more than 4 yards */
		case 6: // 20% 4-8 yard runs
		case 7:
			if (diff >= 2) // better runs for better offenses
				run = random(4, 8 + diff * 2);
			else
				run = random(4, 8);
			break;
		case 8: // 10% 9-13 yard runs
			if (diff >= 3) // better runs for better offenses
				run = random(9, 13 + diff * 2);
			else
				run = random(9, 13);
			break;
		case 9: // 10% 14-19 yard runs
			if (diff >= 4) // better runs for better offenses
				run = random(14, 19 + diff * 2);
			else
				run = random(14, 19);
			break;
		case 10: // 10% 20-25 yard runs
			if (diff >= 4) // better runs for better offenses
				run = random(20, 25 + diff * 2);
			else
				run = random(20, 25);
			break;
	}

	return run;
}
