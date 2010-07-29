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

#define TEAM1 0
#define TEAM2 1

enum PassType { PASS_LONG, PASS_NORMAL, PASS_SHORT };
enum PlayReturn { PL_GAME_OVER, PL_OK };
enum PlayType { PLAY_RUN, PLAY_PASS, PLAY_PUNT, PLAY_KICKOFF };

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

// forward declaration
class TYFUITemplate;

class TYFGame
{
	private:
		TYFTeam *Teams[2];
		GameTime Time;
		BallInfo Ball;
		bool clockStopped;
		bool TwoMinuteWarning;
		int firstKickoff;
		TYFUITemplate *UI;
		bool needKickoff;
		bool needPunt;
		
		bool isFumble();
		bool isIncomplete(PassType type);
		bool isIntercepted(int pass, PassType type);
		bool isRecovered(int run);
		bool isSacked(PassType type);
		bool isStillRunning();
		bool isPlayOutOfBounds(PlayType type);
		int getDistanceToEndzone();
		int getDistanceToFirstDown();
		int getOffDefDifferences();
		int pass(PassType type);
		int run();
		int sacked(PassType type);
		int setBallPosition(int n);
		void advanceBall(int n);
		void advanceTime(int n);
		void changeBallPossession();
		void doAction();
		void doFieldGoal();
		void doKickOff();
		void doPass(PassType type);
		void doPunt();
		void doReturn(PlayType type);
		void doRun();
		void getBallPosition(int n);
		void stopClock();
		TYFTeam* getThisTeam();
		TYFTeam* getOtherTeam();
	public:
		TYFGame(TYFUITemplate *UI);
		~TYFGame(void);
		PlayReturn nextPlay();
		GameInfo getGameInfo();
};

