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
	
	this->UI = UI;
	
	//                                             name,pass,run,blz,  CB,LB,SA,DE,DT
	DefensiveFormations.push_back(new DefFormation("3-2",  2, -2, 2,   3, 2, 3, 2, 1));
	DefensiveFormations.push_back(new DefFormation("3-3",  1,  0, 0,   2, 3, 3, 2, 1));
	DefensiveFormations.push_back(new DefFormation("3-4",  0,  1, 2,   2, 4, 2, 2, 1));
	DefensiveFormations.push_back(new DefFormation("4-1",  3, -2, 2,   4, 1, 2, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-2",  2, -2, 1,   3, 2, 2, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-3",  0,  1, 0,   1, 3, 3, 2, 2));
	DefensiveFormations.push_back(new DefFormation("4-4", -3,  3, 0,   2, 4, 1, 2, 2));
	
	DefensiveFormations.push_back(new DefFormation("Punt Return",      -3,  -3, -3,   2, 4, 1, 2, 2));
	DefensiveFormations.push_back(new DefFormation("Field Goal Block", -3,  -3, -3,   2, 4, 1, 2, 2));
	
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

void TYFGame::init()
{
	// init teams
	this->Teams[0] = new TYFTeam(this->UI->setTeam());
	this->Teams[1] = new TYFTeam(this->UI->setTeam());
	
	this->Teams[0]->setController(this->UI->setPlayerControl(this->Teams[0]));
	this->Teams[1]->setController(this->UI->setPlayerControl(this->Teams[1]));
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
		this->getThisTeam()->setupOffFormation(this->OffensiveFormation);
	}
	else if (type == PLAY_RUN)
	{
		do
		{
			this->OffensiveFormation = this->OffensiveFormations[random(0, this->OffensiveFormations.size() - 1)];
		} while ((*this->OffensiveFormation).Run <= 0);
		this->getThisTeam()->setupOffFormation(this->OffensiveFormation);
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
	this->getOtherTeam()->setupDefFormation(this->DefensiveFormation);
	int r = random(0, 10);
	if (r == 0)
		this->DefensivePlay = DPLAY_BLITZ;
	else if (r <= 5)
		this->DefensivePlay = DPLAY_PASSBLOCK;
	else
		this->DefensivePlay = DPLAY_RUNBLOCK;
	if (this->Ball.Down == 4)
		this->DefensivePlay = DPLAY_PUNTRETURN;
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
			this->getThisTeam()->scorePoints(6, this->Time.Quarter);
			this->needKickoff = true;
			this->Ball.Down = 1;
			this->Ball.ToGo = 10;
			if (random(0, 100) > 2)
			{
				this->getThisTeam()->scorePoints(1, this->Time.Quarter);
				this->UI->endPlay(PLAY_TOUCHDOWN_XP);
			}
			else
				this->UI->endPlay(PLAY_TOUCHDOWN);
		}
		
		// safety check
		else if (this->Ball.Position < 0)
		{
			this->getOtherTeam()->scorePoints(2, this->Time.Quarter);
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
	for (int i = 0; i <= 1; i++)
	{
		info.Scores[i] = TeamScore(this->Teams[i]->getName(), this->Teams[i]->getPoints());
		for (int j = 0; j <= 4; j++)
			info.Scores[i].PointsQuarter[j] = this->Teams[i]->getPoints(j+1);
	}
	
	return info;
}

/*
 * some play action here
 * this is basically the AI of TYF
 * */
void TYFGame::doAction()
{
	if (this->getOtherTeam()->isPlayerControlled(CONTROL_PLAYER_DEFENSE))
	{
		DefensePlay defplay = this->UI->pickDefensePlay(this->getOtherTeam());
		this->DefensiveFormation = defplay.Formation;
		this->DefensivePlay = defplay.Type;
		if (this->DefensiveFormation != NULL)
			this->getOtherTeam()->setupDefFormation(this->DefensiveFormation);
	}
	else
		this->chooseDefFormation();
		
	if (this->getThisTeam()->isPlayerControlled(CONTROL_PLAYER_OFFENSE))
	{
		OffensePlay offplay = this->UI->pickOffensePlay(this->getThisTeam());
		this->OffensiveFormation = offplay.Formation;
		if (this->OffensiveFormation != NULL)
			this->getThisTeam()->setupOffFormation(this->OffensiveFormation);
		
		switch (offplay.Type)
		{
			case PLAY_PASS_SHORT:
			case PLAY_PASS:
			case PLAY_PASS_LONG:
				this->doPass(this->getThisTeam()->getQuarterback(), offplay.Player, offplay.Type);
				break;
			case PLAY_RUN:
				this->doRun(offplay.Player);
				break;
			case PLAY_PUNT:
				this->doPunt();
				break;
			case PLAY_FIELDGOAL:
				this->doFieldGoal();
				break;
			default:
				cout << "If you see this the UI fracked up" << endl;
		}
	}
	else
	{
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
				int maxDistanceForFG = 35;			
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
				this->chooseOffFormation(PLAY_PASS);
				
				TYFPlayer* receiver = this->getThisTeam()->getRandomReceiver();
				TYFPlayer* sender = this->getThisTeam()->getQuarterback();
				
				if (this->getDistanceToFirstDown() >= 8)
				{
					// long yardage 8-10 yards
					if (this->Ball.Down == 1)
					{
						r = random(1, 10);
						if (r <= 4)
							this->doPass(sender, receiver, PLAY_PASS_SHORT);
						else if (r <= 8)
							this->doPass(sender, receiver, PLAY_PASS);
						else
							this->doPass(sender, receiver, PLAY_PASS_LONG);
					}
					else
					{
						if (random(0, 1) == 0)
							this->doPass(sender, receiver, PLAY_PASS_LONG);
						else
							this->doPass(sender, receiver, PLAY_PASS);
					}
				}
				else if (this->getDistanceToFirstDown() >= 4)
				{
					// middle yardage 4-7 yards
					r = random(1, 10);
					if (r <= 3)
						this->doPass(sender, receiver, PLAY_PASS_LONG);
					else if (r <= 8)
						this->doPass(sender, receiver, PLAY_PASS);
					else
						this->doPass(sender, receiver, PLAY_PASS_SHORT);
				}
				else
				{
					// short yardage 0 - 3 yards
					r = random(1, 10);
					if (r <= 6)
						this->doPass(sender, receiver, PLAY_PASS_SHORT);
					else if (r <= 9)
						this->doPass(sender, receiver, PLAY_PASS);
					else
						this->doPass(sender, receiver, PLAY_PASS_LONG);
				}
			}
			else
			{
				this->chooseOffFormation(PLAY_RUN);
				this->doRun(this->getThisTeam()->getRandomRunner());
			}
		}
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
void TYFGame::setBallPosition(int n)
{
	this->Ball.Position = n;
	this->Ball.ToGo = 10;
}

/*
 * gets the Ball Position
 * */
int TYFGame::getBallPosition()
{
	return this->Ball.Position;
}

/*
 * advance the time by n seconds
 * */
void TYFGame::advanceTime(int n)
{
	// the huddle doesn't take time as the clock is stopped
	if (this->clockStopped)
		n -= 10;
	
	if ((this->Time.Quarter == 2) || (this->Time.Quarter == 4))
		if (this->Time.Time < 2*60)
		{
			if (this->getThisTeam()->getPoints() <= this->getOtherTeam()->getPoints())
				// hurry up offense when down in Q2/Q4
				n = n*0.9;
			else
				// run down the clock when up in Q2/Q4
				n = n*1.5;
		}
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
		else if (this->Time.Quarter == 4)
		{
			if (this->getThisTeam()->getPoints() != this->getOtherTeam()->getPoints())
				this->Time.Time = 0;
			else
			{
				// OVERTIME STARTS HERE, BABY
				this->Time.Quarter += 1;
				this->Time.Time = 15*60;
			}
		}
		else
			this->Time.Time = 0;
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
	this->UI->playKickOff(this->getThisTeam()->getKicker(), kick);
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
	this->UI->playPunt(this->getThisTeam()->getKicker(), kick, touchback);
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
	else if (dist < 35)
		good = (x < 80 + r/2);
	else if (dist < 40)
		good = (x < 60 + r);
	else if (dist < 45)
		good = (x < 45 + r*2);
	else if (dist < 50)
		good = (x < 18 + r*2);
	else if (dist < 60)
		good = (x < 8 + r*2);
	// anything longer is always failed. I mean seriously who would even try?
	
	this->UI->playFieldGoal(this->getThisTeam()->getKicker(), dist, good);
	if (good)
	{
		this->getThisTeam()->scorePoints(3, this->Time.Quarter);
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
	
	TYFPlayer *returner = this->getThisTeam()->getRandomReturner(type);
	int diff = returner->getReturnRating(type);
	
	int r = random(1 + diff, 100);
	
	if (type == PLAY_KICKOFF)
	{
		fairCatch = (random(0, 6 + diff) == 0);
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
		if (this->DefensivePlay == DPLAY_PUNTRETURN)
		{
			fairCatch = (random(0, min(3, diff)) == 0);
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
		else
		{
			fairCatch = (random(0, 1) == 0);
			distance = random(0, 5);
		}
	}
	if (fairCatch)
		distance = 0;
	this->advanceBall(distance);
	this->UI->playReturn(returner, distance, fairCatch);
	this->Ball.ToGo = 10;
}

/*
 * Is Game Still Running?
 * */
bool TYFGame::isStillRunning()
{
	if (this->Time.Quarter == 4)
	{
		if (this->getThisTeam()->getPoints() != this->getOtherTeam()->getPoints())
			return (this->Time.Time > 0);
	}
	if (this->Time.Quarter == 5)
		return ((this->getThisTeam()->getPoints() == this->getOtherTeam()->getPoints()) && (this->Time.Time != 0));
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
void TYFGame::doPass(TYFPlayer* sender, TYFPlayer* receiver, PlayType type)
{
	int pass = this->pass(sender, receiver, type);
	
	// end zone pass (trim if too long)
	if (this->getDistanceToEndzone() < pass)
		pass = this->getDistanceToEndzone() + random(1, 8);
	
	if (this->isSacked(sender, this->getOtherTeam()->getRandomTackle(), type))
	{
		int sack = this->sacked(type);
		this->UI->playSack(this->getThisTeam()->getQuarterback(), this->getOtherTeam()->getRandomPlayer(), -sack);
		this->advanceBall(sack);
		this->stopClock();
		this->advanceTime(random(20, 30));
		this->Ball.Down++;
	}
	else if (this->isIncomplete(sender, receiver, type))
	{
		this->UI->playPass(sender, receiver, NULL, pass, PASS_INCOMPLETE);
		this->stopClock();
		this->advanceTime(random(20, 36));
		this->Ball.Down++;
	}
	else
	{
		this->advanceBall(pass);
		bool intercepted = this->isIntercepted(sender, receiver, pass, type);
		
		this->advanceTime(random(20, 36));
		
		if (intercepted)
		{
			this->UI->playPass(sender, receiver, this->getOtherTeam()->getRandomReceiver(), pass, PASS_INTERCEPTED);
			this->changeBallPossession();
			if (this->getBallPosition() < 0)
				this->setBallPosition(0);
		}
		else
		{
			this->UI->playPass(sender, receiver, NULL, pass, PASS_OK);
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
int TYFGame::pass(TYFPlayer* sender, TYFPlayer* receiver, PlayType type)
{
	int pass = 0;
	int rand = random(0, 9);
	
	if (this->matchupFormations(MATCH_PASS) >= 4)
		rand += random(0, this->matchupFormations(MATCH_PASS) - 3);
		
	int average = (sender->getPassRating() + receiver->getCatchRating()) / 2;
	if (type != PLAY_PASS_SHORT)
	{
		if (average < 3)
			rand -= random(0, 3-average);
		if (average > 3)
			rand += random(0, average-3);
	}
	
	if (rand < 0)
		rand = 0;
	if (rand > 9)
		rand = 9;
	
	// Short Passes
	if (type == PLAY_PASS_SHORT)
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
	else if (type == PLAY_PASS)
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
	else if (type == PLAY_PASS_LONG)
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
bool TYFGame::isIntercepted(TYFPlayer* sender, TYFPlayer* receiver, int pass, PlayType type)
{
	int diff = this->getOffDefDifferences(PLAY_PASS);
	int average = (sender->getPassRating() + receiver->getCatchRating()) / 2;
	diff += (average+1/2);
	
	int rand = random(0, 100);

	switch (type)
	{
		case PLAY_PASS_SHORT:
			return (rand < 4 - diff / 2);
		case PLAY_PASS:
			return (rand < 6 - diff / 2);
		case PLAY_PASS_LONG:
			return (rand < 8 - diff / 2);
		default:
			return false;
	}
}

/*
 * Was a Pass Completed?
 * */
bool TYFGame::isIncomplete(TYFPlayer* sender, TYFPlayer* receiver, PlayType type)
{
	int diff = this->getOffDefDifferences(PLAY_PASS) + (this->matchupFormations(MATCH_PASS)/2);
	int average = (sender->getPassRating() + receiver->getCatchRating()) / 2;
	diff += (average+1/2)-1;

	int rand = random(0, 100);
	
	switch (type)
	{
		case PLAY_PASS_SHORT:
			return (rand > 38 + diff * 2);
		case PLAY_PASS:
			return (rand > 44 + diff * 2);
		case PLAY_PASS_LONG:
			return (rand > 55 + diff * 2);
		default:
			return false;
	}
}

/*
 * QB Sacked?
 * */
bool TYFGame::isSacked(TYFPlayer* sender, TYFPlayer* tackler, PlayType type)
{
	int diff = this->getOffDefDifferences(PLAY_PASS) + (this->matchupFormations(MATCH_BLITZ)/2);
	diff += sender->getSpeedRating() - tackler->getBlitzRating();
	int rand = random(0, 100);
	
	if (this->DefensivePlay == DPLAY_BLITZ)
		diff -= 15;
	else if (this->DefensivePlay == DPLAY_PASSBLOCK)
		diff -= 5;
	
	switch (type)
	{
		case PLAY_PASS_SHORT:
			return (rand < 3 - diff);
		case PLAY_PASS:
			return (rand < 5 - diff);
		case PLAY_PASS_LONG:
			return (rand < 10 - diff);
		default:
			return false;
	}
}

/*
 * How far back was he?
 * */
int TYFGame::sacked(PlayType type)
{
	int diff = this->getOffDefDifferences(PLAY_PASS);
	int min = 0;

	switch (type)
	{
		case PLAY_PASS_SHORT:
			min = -2 - diff;
			break;
		case PLAY_PASS:
			min = -4 - diff;
			break;
		case PLAY_PASS_LONG:
			min = -6 - diff;
			break;
		default:
			break;
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
int TYFGame::getOffDefDifferences(PlayType type)
{
	int diff = this->getThisTeam()->getOffenseRating(type)*3 - this->getOtherTeam()->getDefenseRating(type)*3;
	
	if ((type == PLAY_PASS) && (this->DefensivePlay == DPLAY_PASSBLOCK))
		diff -= 1;
	
	if ((type == PLAY_RUN) && (this->DefensivePlay == DPLAY_RUNBLOCK))
		diff -= 1;
		
	if (diff > 5)
		diff = 5;
	if (diff < -5)
		diff = -5;
	return diff;
}

/*
 * was a ball fumbled?
 * */
bool TYFGame::isFumble()
{
	int diff = this->getOffDefDifferences(PLAY_RUN);
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
void TYFGame::doRun(TYFPlayer* runner)
{
	int run = this->run(runner);
	
	// end zone run (trim if too long)
	if (this->getDistanceToEndzone() < run)
		run = this->getDistanceToEndzone() + random(1, 8);
	
	this->UI->playRun(runner, run);
	
	this->advanceBall(run);
	this->advanceTime(random(30, 45));
	
	if ((this->getBallPosition() >= 0) && (this->getBallPosition() <= 100) && (this->isFumble()))
	{
		this->advanceTime(random(5, 10));
		this->stopClock();
		bool recovered = this->isRecovered(run);
		
		TYFPlayer *player;
		if (recovered)
			player = this->getThisTeam()->getRandomPlayer();
		else
			player = this->getOtherTeam()->getRandomPlayer();
		
		this->UI->playFumble(player, recovered);
		
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
int TYFGame::run(TYFPlayer* runner)
{
	int per = random(1, 10);
	int run = 0;

	// offense / defense differences
	int diff = this->getOffDefDifferences(PLAY_RUN);
	diff += (runner->getRunRating()+1)/2 - 3;

	switch(per)
	{
		/* 50% runs under 4 yards */
		case 1: // 10% under 0 yards
			if (diff <= 0) // even worse for bad defenses
				run = random(-4 + diff/2, -1);
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
			if (diff >= 3) // better runs for better offenses
				run = random(4, 8 + diff * 2);
			else
				run = random(4, 8);
			break;
		case 8: // 10% 9-13 yard runs
			if (diff >= 4) // better runs for better offenses
				run = random(9, 13 + diff * 2);
			else
				run = random(9, 13);
			break;
		case 9: // 10% 14-19 yard runs
			if (diff >= 5) // better runs for better offenses
				run = random(14, 19 + diff * 2);
			else
				run = random(14, 19);
			break;
		case 10: // 10% 20-25 yard runs
			if (diff >= 5) // better runs for better offenses
				run = random(20, 25 + diff * 2);
			else
				run = random(20, 25);
			break;
	}
	
	if (this->matchupFormations(MATCH_RUN) >= 4)
		run += random(0, this->matchupFormations(MATCH_RUN));
	return run;
}

vector<OffFormation* > TYFGame::getOffensiveFormations()
{
	return this->OffensiveFormations;
}

vector<DefFormation* > TYFGame::getDefensiveFormations()
{
	return this->DefensiveFormations;
}
