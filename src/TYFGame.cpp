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
#include "helper.h"
#include "TYFUITemplate.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>


/**
	Dear future me. Please forgive me. 
	I can't even begin to express how sorry I am for this code.
**/

using namespace std;

/*
 * constructor
 * */
TYFGame::TYFGame(TYFUITemplate *UI)
{
	// CAN I HAZ RANDOM?	
	this->firstKickoff = TEAM1;
	this->Ball.Position = 20;
	this->Ball.ToGo = 10;
	this->Ball.Down = 1;
	this->Time.Time = 15*60;
	this->Time.Quarter = 0;
	this->clockStopped = false;
	this->TwoMinuteWarning = false;
	this->needKickoff = false;
	this->needPunt = false;
	
	// init teams
	this->Teams[0] = new TYFTeam("Chicago Bears", "CHI");
	this->Teams[1] = new TYFTeam("Green Bay Packers", "GB");
	
	this->UI = UI;
	
	//                                             name,pass,run,blz,  CB,LB,SA,DE,DT
	DefensiveFormations.push_back(new DefFormation("3-2",  2, -2, 2,   3, 2, 3, 2, 1));
	DefensiveFormations.push_back(new DefFormation("3-3",  1,  0, 0,   2, 3, 3, 2, 1));
	DefensiveFormations.push_back(new DefFormation("3-4",  0,  1, 2,   2, 4, 2, 2, 1));
	DefensiveFormations.push_back(new DefFormation("4-1",  3, -2, 2,   4, 1, 2, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-2",  2, -2, 1,   3, 2, 2, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-3",  0,  1, 0,   1, 3, 3, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-4", -3,  3, 0,   2, 4, 1, 2, 2));
	
	//                                           pass,run,blz,  hb,fb,wr,te,OG,OT,QB,CE
	OffensiveFormations.push_back(new OffFormation( 3, -1,  1,   1, 0, 2, 2, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 3, -1, -1,   1, 0, 3, 1, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 2, -3, -2,   1, 0, 4, 0, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 2,  1,  1,   2, 0, 2, 1, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 1,  2,  2,   1, 1, 1, 2, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 2,  1,  1,   1, 1, 2, 1, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 3,  2, -2,   2, 1, 3, 0, 2, 1, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 1,  2,  1,   2, 1, 0, 2, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 0,  3,  2,   2, 1, 1, 1, 2, 2, 1, 1));
	OffensiveFormations.push_back(new OffFormation( 1,  3,  2,   2, 1, 2, 0, 2, 2, 1, 1));
}

/*
 * choose a Formation for the offense (badly, sadly)
 * */
void TYFGame::chooseOffFormation(PlayType type)
{
	if (type == PLAY_PASS)
	{
		do
		{
			this->OffensiveFormation = this->OffensiveFormations[random(0, this->OffensiveFormations.size() - 1)];
		} while ((*this->OffensiveFormation).Pass <= 0);
		this->getThisTeam()->setupOffFormation(*this->OffensiveFormation);
	}
	else if (type == PLAY_RUN)
	{
		do
		{
			this->OffensiveFormation = this->OffensiveFormations[random(0, this->OffensiveFormations.size() - 1)];
		} while ((*this->OffensiveFormation).Run <= 0);
		this->getThisTeam()->setupOffFormation(*this->OffensiveFormation);
	}
	
	if (type == PLAY_PUNT)
		this->getThisTeam()->setupPuntFormation();
	else if (type == PLAY_KICKOFF)
		this->getThisTeam()->setupKickoffFormation();
	else if (type == PLAY_FIELDGOAL)
		this->getThisTeam()->setupFieldGoalFormation();
}

/*
 * choose a Formation for the defense
 * */
void TYFGame::chooseDefFormation()
{
	this->DefensiveFormation = this->DefensiveFormations[random(0, this->DefensiveFormations.size() - 1)];
	this->getOtherTeam()->setupDefFormation(*this->DefensiveFormation);
}

/*
 * match up who "good" the Formations are for Running/Passing/Blitzing
 * */
int TYFGame::matchupFormations(MatchupType type)
{
	OffFormation Off = *this->OffensiveFormation;
	DefFormation Def = *this->DefensiveFormation;
	
	if (type == MATCH_PASS)
		return Off.Pass - Def.Pass;
	else if (type == MATCH_RUN)
		return Off.Run - Def.Run;
	else if (type == MATCH_BLITZ)
		return Off.Blitz - Def.Blitz;
	else
		return 0;
}

/**
 * play the next play
 * */
PlayReturn TYFGame::nextPlay()
{
	// 2nd half
	if ((this->Time.Quarter == 3) && (this->Time.Time == 15*60))
	{
		// the other team kicks now
		this->Ball.Possession = !this->firstKickoff;
		this->Ball.Down = 1;
		this->Ball.ToGo = 10;
		this->setBallPosition(20);
		this->UI->beginPlay();
		this->doKickOff();
		this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	// game not started yet? Kickoff firsts
	else if (this->Time.Quarter == 0)
	{
		this->Ball.Possession = this->firstKickoff;
		this->UI->beginPlay();
		this->setBallPosition(20);
		this->doKickOff();
		this->Time.Quarter = 1;
		this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	// need a kickoff after a touchdown?
	else if (this->needKickoff)
	{
		this->setBallPosition(20);
		this->UI->beginPlay();
		this->needKickoff = false;
		this->doKickOff();
		this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	// need a punt after a safety?
	else if (this->needPunt)
	{
		this->UI->beginPlay();
		this->needPunt = false;
		this->doPunt();
		this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	// if the game is still running, then do stuff
	else if (this->isStillRunning())
	{
		this->UI->beginPlay();
		this->doAction();
		
		// touchdown check
		if (this->Ball.Position > 100)
		{
			this->getThisTeam()->scorePoints(7);
			this->needKickoff = true;
			this->UI->endPlay(PLAY_TOUCHDOWN);
		}
		
		// safety check
		else if (this->Ball.Position < 0)
		{
			this->getOtherTeam()->scorePoints(2);
			this->setBallPosition(20);
			this->needPunt = true;
			this->UI->endPlay(PLAY_SAFETY);
		}
		
		// first down check
		else if (this->getDistanceToFirstDown() < 0)
		{
			this->Ball.Down = 1;
			this->Ball.ToGo = 10;
			this->UI->endPlay(PLAY_FIRST_DOWN);
		}
		
		// turnover on downs check
		else if (this->Ball.Down == 5)
		{
			this->changeBallPossession();
			this->UI->endPlay(PLAY_TURNOVER_ON_DOWNS);
		}
		else
			this->UI->endPlay(PLAY_NOTHING);
		return PL_OK;
	}
	// game is not running anymore
	else
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
 * this is basically the AI of TYF
 * */
void TYFGame::doAction()
{
	this->chooseDefFormation();
	
	bool action = true;
	if (this->Ball.Down == 4)
	{
		action = false;
		// consider going for it
		if ((this->getDistanceToFirstDown() <= 3) && (this->Ball.Position >= 50))
		{
			if (random(0, 3) == 0)
				action = true;
			if (this->getThisTeam()->getPoints() < this->getOtherTeam()->getPoints())
			{
				if (random(0, 2) == 0)
					action = true;
				if (this->Time.Quarter == 4)
					action = true;
			}
		}
		// not going for it after all
		if (!action)
		{
			int maxDistanceForFG = 33;			
			if (this->getThisTeam()->getPoints() < this->getOtherTeam()->getPoints())
			{
				// we REALLY need those points
				maxDistanceForFG = 50;
			}
			if (this->getDistanceToEndzone() + 10 <= maxDistanceForFG)
				this->doFieldGoal();
			else
				this->doPunt();
		}
	}
	
	if (action)
	{
		int r = random(1, 10);
		if (r <= 6)
		{
			if (this->getDistanceToFirstDown() >= 8)
			{
				// long yardage 8-10 yards
				if (this->Ball.Down == 1)
				{
					r = random(1, 10);
					if (r <= 4)
						this->doPass(PASS_SHORT);
					else if (r <= 8)
						this->doPass(PASS_NORMAL);
					else
						this->doPass(PASS_LONG);
				}
				else
				{
					if (random(0, 1) == 0)
						this->doPass(PASS_LONG);
					else
						this->doPass(PASS_NORMAL);
				}
			}
			else if (this->getDistanceToFirstDown() >= 4)
			{
				// middle yardage 4-7 yards
				r = random(1, 10);
				if (r <= 3)
					this->doPass(PASS_LONG);
				else if (r <= 8)
					this->doPass(PASS_NORMAL);
				else
					this->doPass(PASS_SHORT);
			}
			else
			{
				// short yardage 0 - 3 yards
				r = random(1, 10);
				if (r <= 6)
					this->doPass(PASS_SHORT);
				else if (r <= 9)
					this->doPass(PASS_NORMAL);
				else
					this->doPass(PASS_LONG);
			}
		}
		else
			this->doRun();
	}
}

/**
	advance the ball by n yards
**/
void TYFGame::advanceBall(int n)
{
	this->Ball.Position += n;
	this->Ball.ToGo -= n;
}

/*
 * Puts the Ball n yards from the current teams end zone
 * */
int TYFGame::setBallPosition(int n)
{
	this->Ball.Position = n;
	this->Ball.ToGo = 10;
}

/*
 * advance the time by n seconds
 * */
void TYFGame::advanceTime(int n)
{
	// the huddle doesn't take time as the clock is stopped
	if (this->clockStopped)
		n -= 10;
	if (n < 3)
		n = random(2, 4);
	
	// advance the time
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
	bool isTwoMin = (((this->Time.Quarter == 2) || (this->Time.Quarter == 4)) && (this->Time.Time <= 2*60));
	if (!isTwoMin)
		this->TwoMinuteWarning = false;
	else if (isTwoMin != this->TwoMinuteWarning)
	{
		this->UI->callTwoMinuteWarning();
		this->TwoMinuteWarning = true;
		this->Time.Time = 2*60;
		this->stopClock();
	}
}

/*
 * stop the clock
 * */
void TYFGame::stopClock()
{
	this->clockStopped = true;
}

/*
 * do a KickOff
 * */
void TYFGame::doKickOff()
{
	this->chooseOffFormation(PLAY_KICKOFF);
	
	this->setBallPosition(20);
	
	int r = this->getThisTeam()->getKicker()->getKickRating();
	
	int mod = 0;
	// accuracy for various skill levels
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
	this->advanceTime(random(15, 26));
	this->advanceBall(kick);
	this->UI->playKickOff(kick);
	this->changeBallPossession();
	this->doReturn(PLAY_KICKOFF);
}

/*
 * the distance from the line of scimmage to the end zone
 * */
int TYFGame::getDistanceToEndzone()
{
	return 100 - this->Ball.Position;
}

/*
 * the distance a team needs to advance to score a first down
 * this is really just for readability as it's stored in a private variable
 * */
int TYFGame::getDistanceToFirstDown()
{
	return this->Ball.ToGo;
}

/*
 * punt the Ball away
 * */
void TYFGame::doPunt()
{
	this->chooseOffFormation(PLAY_PUNT);
	
	int r = this->getThisTeam()->getKicker()->getKickRating();
	
	this->advanceTime(random(32, 56));
	int mod = 0;
	// accuracy for various skill levels
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
	if (!touchback)
		this->doReturn(PLAY_PUNT);
}

/*
 * a try for a field foal
 * */
void TYFGame::doFieldGoal()
{
	this->chooseOffFormation(PLAY_FIELDGOAL);
	
	int r = this->getThisTeam()->getKicker()->getKickRating();
	int dist = this->getDistanceToEndzone() + 10;
	this->advanceTime(random(17, 35));
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
		good = (x < 18 + r*2);
	else if (dist < 70)
		good = (x < 8 + r*2);
	// anything longer is always failed. I mean seriously who would even try?
	
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
 * return the Ball for a couple of yards
 * */
void TYFGame::doReturn(PlayType type)
{
	bool fairCatch = false;
	int distance = 0;
	int r = random(1, 100);
	if (type == PLAY_KICKOFF)
	{
		fairCatch = (random(0, 10) == 0);
		if (r < 10)
			distance = random(0, 10);
		else if (r < 50)
			distance = 10 + random(0, 10);
		else if (r < 70)
			distance = 20 + random(0, 10);
		else if (r < 90)
			distance = 30 + random(0, 10);
		else
		{
			int n = random(0, 9);
			if (n < 4)
				n = 4;
			distance = n*10+random(0, 10);
		}
	}
	else if (type == PLAY_PUNT)
	{
		fairCatch = (random(0, 1) == 0);
		if (r < 30)
			distance = random(0, 10);
		else if (r < 70)
			distance = 10 + random(0, 10);
		else if (r < 80)
			distance = 20 + random(0, 10);
		else if (r < 90)
			distance = 30 + random(0, 10);
		else
		{
			int n = random(0, 9);
			if (n < 4)
				n = 4;
			distance = n*10+random(0, 10);
		}
	}
	if (fairCatch)
		distance = 0;
	this->advanceBall(distance);
	this->UI->playReturn(distance, fairCatch);
	this->Ball.ToGo = 10;
}

/*
 * Is Game Still Running?
 * */
bool TYFGame::isStillRunning()
{
	if (this->Time.Quarter == 4)
		return (this->Time.Time > 0);
	else
		return true;
}

/*
 * change the teams
 * */
void TYFGame::changeBallPossession()
{
	this->Ball.Possession = !this->Ball.Possession;
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
 * determines if a player ran out of bounds on a play
 * */
bool TYFGame::isPlayOutOfBounds(PlayType type)
{
	// don't run out of bounds on own or opposing endzone
	if ((this->Ball.Position < 0) || (this->Ball.Position > 100))
		return false;
	if (type == PLAY_PASS)
	{
		if (this->TwoMinuteWarning)
			return (random(0, 4) == 0);
		else
			return (random(0, 8) == 0);
	}
	else if (type == PLAY_RUN)
	{
		if (this->TwoMinuteWarning)
			return (random(0, 3) == 0);
		else
			return (random(0, 6) == 0);
	}
	return false;
}

/*
 * Same as getThisTeam(), but the other team.
 * */
TYFTeam* TYFGame::getOtherTeam()
{
	return this->Teams[!this->Ball.Possession];
}

/*
 * Pass that Ball
 * */
void TYFGame::doPass(PassType type)
{
	this->chooseOffFormation(PLAY_PASS);
	
	int pass = this->pass(type);
	
	// end zone pass (trim if too long)
	if (this->getDistanceToEndzone() < pass)
		pass = this->getDistanceToEndzone() + random(1, 10);
	
	if (this->isSacked(type))
	{
		int sack = this->sacked(type);
		this->UI->playSack(-sack);
		this->advanceBall(sack);
		this->stopClock();
		this->advanceTime(random(20, 30));
		this->Ball.Down++;
	}
	else if (this->isIncomplete(type))
	{
		this->UI->playPass(pass, PASS_INCOMPLETE);
		this->stopClock();
		this->advanceTime(random(20, 36));
		this->Ball.Down++;
	}
	else
	{
		this->advanceBall(pass);
		bool intercepted = this->isIntercepted(pass, type);
		
		this->advanceTime(random(20, 36));
		
		if (intercepted)
		{
			this->UI->playPass(pass, PASS_INTERCEPTED);
			this->changeBallPossession();
		}
		else
		{
			this->UI->playPass(pass, PASS_OK);
			if (this->isPlayOutOfBounds(PLAY_PASS))
			{
				this->UI->callOutOfBounds();
				this->stopClock();
			}
			this->Ball.Down++;
		}
	}
}

/*
 * this here handles all the pass action
 * */
int TYFGame::pass(PassType type)
{
	int pass = 0;
	int rand = random(0, 9);
	
	if (this->matchupFormations(MATCH_PASS) >= 4)
		rand += random(0, this->matchupFormations(MATCH_PASS) - 3);
	if (rand > 9)
		rand = 9;
	
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
	int diff = this->getOffDefDifferences() + (this->matchupFormations(MATCH_PASS)/2);
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
	int diff = this->getOffDefDifferences() + (this->matchupFormations(MATCH_BLITZ)/2);
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
	return (rand < 3 - diff/2);
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
	this->chooseOffFormation(PLAY_RUN);
	int run = this->run();
	
	// end zone run (trim if too long)
	if (this->getDistanceToEndzone() < run)
		run = this->getDistanceToEndzone() + random(1, 10);
	
	this->UI->playRun(run);
	
	this->advanceBall(run);
	this->advanceTime(random(30, 45));
	
	if (this->isFumble())
	{
		this->advanceTime(random(5, 10));
		this->stopClock();
		bool recovered = this->isRecovered(run);
		this->UI->playFumble(recovered);
		
		// not recovered
		if (!recovered)
			this->changeBallPossession();
	}
	else
	{
		if (this->isPlayOutOfBounds(PLAY_RUN))
		{
			this->UI->callOutOfBounds();
			this->stopClock();
		}
		this->Ball.Down++;
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
	
	if (this->matchupFormations(MATCH_RUN) >= 4)
		run += random(0, this->matchupFormations(MATCH_RUN)*2);
	
	return run;
}
