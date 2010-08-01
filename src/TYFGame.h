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

#pragma once

#include "TYFTeam.h"
#include <vector>

#define TEAM1 0
#define TEAM2 1

enum PlayReturn { PL_GAME_OVER, PL_OK };
enum MatchupType { MATCH_PASS, MATCH_RUN, MATCH_BLITZ };

struct GameTime
{
	int Time;
	int Quarter;
};

struct BallInfo
{
	int Possession;
	int Down;
	int Position;
	int ToGo;
};

struct TeamScore
{
	string Name;
	int Points;
	
	TeamScore()
	{
		this->Name = "";
		this->Points = 0;
	}
	
	TeamScore(string Name, int Points)
	{
		this->Name = Name;
		this->Points = Points;
	}
};

struct GameInfo
{
	GameTime Time;
	BallInfo Ball;
	TeamScore Scores[2];
};

struct OffensePlay
{
	PlayType Type;
	TYFPlayer* Player;
	OffFormation* Formation;
	
	OffensePlay(OffFormation* fo, PlayType ty, TYFPlayer* pl)
	{
		this->Formation = fo;
		this->Type = ty;
		this->Player = pl;
	}
};

struct DefensePlay
{
	DefFormation* Formation;
	
	DefensePlay(DefFormation* fo)
	{
		this->Formation = fo;
	}
};

// forward declaration
class TYFUITemplate;

class TYFGame
{
	private:
		TYFTeam *Teams[2];
		GameTime Time;
		BallInfo Ball;
		vector<DefFormation* > DefensiveFormations;
		vector<OffFormation* > OffensiveFormations;
		
		DefFormation* DefensiveFormation;
		OffFormation* OffensiveFormation;
		
		bool clockStopped;
		bool TwoMinuteWarning;
		int firstKickoff;
		TYFUITemplate *UI;
		bool needKickoff;
		bool needPunt;
		
		bool isFumble();
		bool isIncomplete(TYFPlayer* sender, TYFPlayer* receiver, PlayType type);
		bool isIntercepted(TYFPlayer* sender, TYFPlayer* receiver, int pass, PlayType type);
		bool isRecovered(int run);
		bool isSacked(TYFPlayer* sender, TYFPlayer* tackler, PlayType type);
		bool isStillRunning();
		bool isPlayOutOfBounds(PlayType type);
		int getDistanceToEndzone();
		int getDistanceToFirstDown();
		int getOffDefDifferences(PlayType type);
		int pass(TYFPlayer* sender, TYFPlayer* receiver, PlayType type);
		int run(TYFPlayer* runner);
		int sacked(PlayType type);
		void setBallPosition(int n);
		void advanceBall(int n);
		void advanceTime(int n);
		void changeBallPossession();
		void doAction();
		void doFieldGoal();
		void doKickOff();
		void doPass(TYFPlayer* sender, TYFPlayer* receiver, PlayType type);
		void doPunt();
		void doReturn(PlayType type);
		void doRun(TYFPlayer* runner);
		void getBallPosition(int n);
		int getBallPosition();
		void stopClock();
		
		void chooseOffFormation(PlayType type);
		void chooseDefFormation();
		int matchupFormations(MatchupType type);
		
		TYFTeam* getThisTeam();
		TYFTeam* getOtherTeam();
	public:
		TYFGame(TYFUITemplate *UI);
		~TYFGame(void);
		PlayReturn nextPlay();
		GameInfo getGameInfo();
		vector<OffFormation* > getOffensiveFormations();
		vector<DefFormation* > getDefensiveFormations();
};

