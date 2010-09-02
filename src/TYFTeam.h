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

#include <string>
#include <vector>
#include "TYFPlayer.h"
#include "helper.h"

using namespace std;

struct DefFormation
{
	string name;
	int Pass, Run, Blitz;
	int CB, LB, SA, DE, DT;
	DefFormation(string name, int Pass, int Run, int Blitz, int CB, int LB, int SA, int DE, int DT)
	{
		this->name = name;
		this->Pass = Pass;
		this->Run = Run;
		this->Blitz = Blitz;
		this->CB = CB;
		this->LB = LB;
		this->SA = SA;
		this->DE = DE;
		this->DT = DT;
	}
};

struct OffFormation
{
	int Pass, Run, Blitz;
	int HB, FB, WR, TE, OG, OT, QB, CE;
	OffFormation(int Pass, int Run, int Blitz, int HB, int FB, int WR, int TE, int OG, int OT, int QB, int CE)
	{
		this->Pass = Pass;
		this->Run = Run;
		this->Blitz = Blitz;
		this->HB = HB;
		this->FB = FB;
		this->WR = WR;
		this->TE = TE;
		this->OG = OG;
		this->OT = OT;
		this->QB = QB;
		this->CE = CE;
	}
};

enum ControlFlag { CONTROL_COMPUTER, CONTROL_PLAYER, CONTROL_PLAYER_OFFENSE, CONTROL_PLAYER_DEFENSE };

class TYFTeam
{
	private:
		int score;
		int scoreQuarter[5];
		string name;
		string shortname;
		ControlFlag controlled;
	public:
		TYFTeam(string name);
		void setController(ControlFlag controlled);
		~TYFTeam(void);
		vector<TYFPlayer* > Players;
		TYFPlayer *OnField[11];
		double getDefenseRating(PlayType type);
		double getOffenseRating(PlayType type);
		void scorePoints(int n, int q);
		int getPoints();
		int getPoints(int q);
		bool isPlayerControlled(ControlFlag controlled);
		void loadFromFile(string filename);
		void setupDefFormation(DefFormation *form);
		void setupOffFormation(OffFormation *form);
		void setupPuntFormation();
		void setupFieldGoalFormation();
		void setupKickoffFormation();
		void addPlayer(TYFPlayer *p, string pos, int* formvar);
		string getName();
		TYFPlayer* getKicker();
		TYFPlayer* getQuarterback();
		TYFPlayer* getRandomPlayer();
		TYFPlayer* getRandomRunner();
		TYFPlayer* getRandomReceiver();
		TYFPlayer* getRandomTackle();
		TYFPlayer* getRandomReturner(PlayType type);
		vector<TYFPlayer* > getTackles();
		vector<TYFPlayer* > getRunners();
		vector<TYFPlayer* > getReceivers();
};
