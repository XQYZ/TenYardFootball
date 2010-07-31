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

#include "TYFUIConsole.h"
#include "TYFUITemplate.h"
#include "TYFGame.h"
#include "TYFTeam.h"
#include "helper.h"
#include <iostream>
#include <iomanip>

using namespace std;

void TYFUIConsole::cls()
{
	cout << string( 100, '\n' ) << this->header;
}

/*
 * returns the position of the Ball in the "CHI 17" format
 * */
string TYFUIConsole::getBallPosition()
{
	GameInfo info = this->Game->getGameInfo();
	
	string str;
	
	if (info.Ball.Position == 50)
		return "50yd line";
	else if (info.Ball.Position < 50)
	{
		str = info.Scores[info.Ball.Possession].Name;
		str.append(" ");
		str.append(to_string(info.Ball.Position));
	}
	else
	{
		str = info.Scores[!info.Ball.Possession].Name;
		str.append(" ");
		str.append(to_string(100 - info.Ball.Position));
	}
	return str;
}

/**
 * this is called before each play
 * it gives us a nice header like this:
 * "Q4 1:01 - 2nd&10 - @CHI 22 GB 6 - CHI 42"
 * */
void TYFUIConsole::beginPlay()
{
	GameInfo info = this->Game->getGameInfo();
	
	string down[4] = {"1st", "2nd", "3rd", "4th"};
	
	stringstream ss;
	
	ss << setfill('0');
	ss << "Q" << info.Time.Quarter << " " << (int)info.Time.Time/60 << ":";
	ss << setw(2) << (int)info.Time.Time%60 << " - ";
	ss << down[info.Ball.Down-1] << "&";
	if (info.Ball.ToGo == 0)
		ss << "inches";
	else
		ss << info.Ball.ToGo;
	ss << " - ";
	for (int i = 0; i <= 1; i++)
	{
		if (i == info.Ball.Possession)
			ss << "@";
		ss << info.Scores[i].Name << " " << info.Scores[i].Points << " ";
	}
	ss << "- " << this->getBallPosition() << endl;
	
	this->header = ss.str();
	this->cls();
}

/**
 * this is called after each play and delivers after-play results to the UI
 * (e.g. a touchdown which was scored)
 * */
void TYFUIConsole::endPlay(PLAY_RESULT result)
{
	if (result == PLAY_FIRST_DOWN)
		cout << "First Down!" << endl;
	else if (result == PLAY_TOUCHDOWN)
		cout << "Touchdown!" << endl;
	else if (result == PLAY_SAFETY)
		cout << "Safety!" << endl;
	else if (result == PLAY_TURNOVER_ON_DOWNS)
		cout << "Turnover on Downs!" << endl;
	fflush (stdin);
	cin.get();
}

/**
 * this is called when the UI officially takes over as the main thread
 * Basically it just runs until the game finishes here
 * */
void TYFUIConsole::run()
{
	while (this->Game->nextPlay() == PL_OK) {}
}

void TYFUIConsole::playKickOff(TYFPlayer* kicker, int yards)
{
	cout << "Kickoff of " << yards << " yards by " << kicker->getFullName() << endl;
}

void TYFUIConsole::playPunt(TYFPlayer* kicker, int yards, bool touchback)
{
	cout << "A Punt of " << yards << " yards by " << kicker->getFullName() << endl;
	if (touchback)
		cout << "Touchback!" << endl;
}

void TYFUIConsole::playFieldGoal(TYFPlayer* kicker, int distance, bool good)
{
	cout << "A " << distance << " yard fieldgoal attempt by " << kicker->getFullName() << endl;
	if (good)
		cout << "Kick is good!" << endl;
	else
		cout << "Kick is no good!" << endl;
}

void TYFUIConsole::playPass(TYFPlayer* sender, TYFPlayer* receiver, TYFPlayer* intercepter, int distance,
							PASS_FLAG flag)
{
	if (flag == PASS_INCOMPLETE)
		cout << "Incomplete Pass intended for " << receiver->getFullName() << endl;
	else
		cout << "Pass of " << distance << " yards to " << receiver->getFullName() << endl;
	if (flag == PASS_INTERCEPTED)
		cout << "Ball intercepted by " << intercepter->getFullName() << "!" << endl;
}

void TYFUIConsole::playSack(TYFPlayer* quarterback, TYFPlayer* tackler, int loss)
{
	cout << quarterback->getFullName() << " sacked by " << tackler->getFullName();
	cout << "; a loss of " << loss << " yards" << endl;
}

void TYFUIConsole::playRun(TYFPlayer* runner, int distance)
{
	if (runner->getPosition() == "QB")
	{
		cout << runner->getFullName();
		if (distance > 0)
			cout << " running for a gain of " << distance << " yards" << endl;
		else if (distance == 0)
			cout << " running for no gain" << endl;
		else
			cout << " running for a loss of " << -distance << " yards" << endl;
	}
	else
	{
		cout << "Ball handed off to " << runner->getFullName();
		if (distance > 0)
			cout << " for a run of " << distance << " yards" << endl;
		else if (distance == 0)
			cout << " for no gain" << endl;
		else
			cout << " for a loss of " << -distance << " yards" << endl;
	}
}

void TYFUIConsole::playFumble(TYFPlayer* player, bool recovered)
{
	cout << "Ball fumbled!" << endl;
	if (recovered)
		cout << "Recovered by " << player->getFullName() << "!" << endl;
	else
		cout << "Recovered by " << player->getFullName() << " of the opposing team!" << endl;
}

void TYFUIConsole::callTwoMinuteWarning()
{
	cout << "Two Minute Warning!" << endl;
}

void TYFUIConsole::callOutOfBounds()
{
	GameInfo info = this->Game->getGameInfo();
	cout << "Ran out of bounds at " << this->getBallPosition() << endl;
}

void TYFUIConsole::playReturn(TYFPlayer* returner, int distance, bool faircatch)
{
	GameInfo info = this->Game->getGameInfo();
	if (faircatch)
		cout << "Faircatch at " << this->getBallPosition() << " by " << returner->getFullName() << endl;
	else
		cout << "A return of " << distance << " yards by " << returner->getFullName() << endl;
}

void printTimes(string name, int count, int max)
{
	for (int j = 0; j < count; j++)
		cout << " " << name << (j+1);
	for (int j = 0; j < max-count; j++)
		cout << "    ";
}

OffensePlay TYFUIConsole::pickOffensePlay(TYFTeam* team)
{
	unsigned int type;
	unsigned int play;
	vector<OffFormation* > formations = this->Game->getOffensiveFormations();
	while (true)
	{
		this->cls();
		cout << "Choose Offense Formation:" << endl;
		unsigned int j;
		for (j = 0; j < formations.size(); j++)
		{
			cout << setw(2) << (j + 1) << ")";
			printTimes("HB", formations[j]->HB, 2);
			printTimes("FB", formations[j]->FB, 1);
			printTimes("WR", formations[j]->WR, 4);
			printTimes("TE", formations[j]->TE, 2);
			cout << endl;
		}
		cout << setw(2) << formations.size() + 1 << ") Punt" << endl;
		cout << setw(2) << formations.size() + 2 << ") Field Goal" << endl;
		
		cout << "# ";
		(cin >> type).get();
		while ((0 < type) && (type <= formations.size()))
		{
			this->cls();
			cout << "Choose your play:" << endl;
			cout << " 0) Go back to Formation Menu" << endl;
			team->setupOffFormation(*formations[type-1]);
			vector<TYFPlayer* > runners = team->getRunners();
			for (unsigned int i = 0; i < runners.size(); i++)
			{
				cout << setw(2) << (i + 1) << ") ";
				cout << "Run by " << runners[i]->getFullName() << endl;
			}
				
			vector<TYFPlayer* > receivers = team->getReceivers();
			for (unsigned int i = 0; i < receivers.size(); i++)
			{
				cout << setw(2) << (runners.size() + i + 1) << ") ";
				cout << "Pass to " << receivers[i]->getFullName() << endl;
			}
			cout << "# ";
			(cin >> play).get();
			this->cls();
			if (play == 0)
				type = 0;
			else
			{
				play -= 1;
				if (play < runners.size())
					return OffensePlay(formations[type], PLAY_RUN, runners[play]);
				else if (play < runners.size() + receivers.size())
					return OffensePlay(formations[type], PLAY_PASS, receivers[play-runners.size()]);
			}
		}
		this->cls();
		if (type == formations.size() + 1)
			return OffensePlay(NULL, PLAY_PUNT, NULL);
		else if (type == formations.size() + 2)
			return OffensePlay(NULL, PLAY_FIELDGOAL, NULL);
	}
	return OffensePlay(NULL, PLAY_PASS, NULL);
}
