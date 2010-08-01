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
#include <sstream>
#include <string>
#include <fstream> 

using namespace std;

TYFUIConsole::TYFUIConsole(void)
{
	srand((int)time(NULL));
	this->Game = new TYFGame(this);
}

/*
 * this clears the screen on the console
 * */
void TYFUIConsole::cls()
{
	// I know, I know ...
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
	cout << this->header;
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
	else if (100 - info.Ball.Position < info.Ball.ToGo)
		ss << "goal";
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
	//fflush (stdin);
	cin.get();
}

/**
 * this is called when the UI officially takes over as the main thread
 * Basically it just runs until the game finishes here
 * */
void TYFUIConsole::run()
{
	while (this->Game->nextPlay() == PL_OK) {}
	
	// game is over, -> overview table
	GameInfo info = this->Game->getGameInfo();
	cout << endl << "Game Over" << endl;
	cout << info.Scores[0].Name << ": " << info.Scores[0].Points << endl;
	cout << info.Scores[1].Name << ": " << info.Scores[1].Points << endl;
	while (cin.get()) {}
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
	cout << "Ran out of bounds at " << this->getBallPosition() << endl;
}

void TYFUIConsole::playReturn(TYFPlayer* returner, int distance, bool faircatch)
{
	GameInfo info = this->Game->getGameInfo();
	if (faircatch)
		cout << "Fair Catch at " << this->getBallPosition() << " by " << returner->getFullName() << endl;
	else
		cout << "A return of " << distance << " yards by " << returner->getFullName() << endl;
}

/*
 * used for naming offensive formations
 * */
string printTimes(string name, int count, int max)
{
	stringstream ss;
	for (int j = 0; j < count; j++)
		ss << name << (j+1) << " " ;
	for (int j = 0; j < max-count; j++)
		ss << "    ";
	return ss.str();
}

/*
 * Displays a Menu and returns the selected item
 * back item is 0 if it is enabled
 * */
int TYFUIConsole::displayMenu(string title, vector<string> menuItems, bool back)
{
	int ret;
	do
	{
		this->cls();
		cout << title << endl;
		if (back)
			cout << " 0) Back" << endl;
		for (unsigned int i = 0; i < menuItems.size(); i++)
			cout << setw(2) << (i + 1) << ") " << menuItems[i] << endl;
		
		cout << "# ";
		cin >> ret;
		
		if (!cin.good())
		{
			ret = -1;
			cin.clear();
			cin.sync();
		}
		
		int ch;
		while ((ch = cin.get()) != '\n' && ch != EOF) { cin.clear(); };
		if ((int)menuItems.size() < ret)
			ret = -1;
		if ((back && ret < 0) || (!back && ret < 1))
			ret = -1;
	} while (ret == -1);
	return ret;
}

/*
 * Let's the player decide on an offensive call
 * */
OffensePlay TYFUIConsole::pickOffensePlay(TYFTeam* team)
{
	vector<OffFormation* > formations = this->Game->getOffensiveFormations();
	vector<string> FormationMenu;
	stringstream ss;
	for (unsigned int j = 0; j < formations.size(); j++)
	{
		ss.str("");
		ss << printTimes("HB", formations[j]->HB, 2);
		ss << printTimes("FB", formations[j]->FB, 1);
		ss << printTimes("WR", formations[j]->WR, 4);
		ss << printTimes("TE", formations[j]->TE, 2);
		FormationMenu.push_back(ss.str());
	}
	FormationMenu.push_back("Punt");
	FormationMenu.push_back("Field Goal");
	
	int formation;
	int play;
	int pass;
	do
	{
		formation = this->displayMenu("Choose Offense Formation:", FormationMenu, false);
		this->cls();
		
		if ((formation == (int)formations.size() + 1) || (formation == (int)formations.size() + 2))
		{
			GameInfo info = this->Game->getGameInfo();
			vector<string> YesNoDialog;
			YesNoDialog.push_back("Yes");
			YesNoDialog.push_back("No");
			if ((info.Ball.Down == 4) || (this->displayMenu("Are you sure you want to Punt/Try for FG now?",
											YesNoDialog, false)) == 1)
			{
				if (formation == (int)formations.size() + 1)
					return OffensePlay(NULL, PLAY_PUNT, NULL);
				if (formation == (int)formations.size() + 2)
					return OffensePlay(NULL, PLAY_FIELDGOAL, NULL);
			}
			else
			{
				formation = 0;
			}
		}
		while (formation != 0)
		{
			vector<string> PlayMenu;
			team->setupOffFormation(formations[formation-1]);
			
			vector<TYFPlayer* > runners = team->getRunners();
			for (unsigned int i = 0; i < runners.size(); i++)
			{
				ss.str("");
				ss << "Run by " << runners[i]->getFullName();
				PlayMenu.push_back(ss.str());
			}
				
			vector<TYFPlayer* > receivers = team->getReceivers();
			for (unsigned int i = 0; i < receivers.size(); i++)
			{
				ss.str("");
				ss << "Pass to " << receivers[i]->getFullName();
				PlayMenu.push_back(ss.str());
			}
			play = this->displayMenu("Choose your play:", PlayMenu, true);
			if (play == 0)
				formation = 0;
			else
			{
				if (play <= (int)runners.size())
				{
					this->cls();
					return OffensePlay(formations[formation-1], PLAY_RUN, runners[play-1]);
				}
				while (play <= (int)runners.size() + (int)receivers.size() && play != 0)
				{
					vector<string> PassMenu;
					PassMenu.push_back("Short Pass");
					PassMenu.push_back("Medium Pass");
					PassMenu.push_back("Long Pass");
					
					pass = this->displayMenu("Choose your pass length:", PassMenu, true);
					
					this->cls();
					if (pass == 0)
						play = 0;
					else if (pass == 1)
						return OffensePlay(formations[formation-1], PLAY_PASS_SHORT, receivers[play-1-runners.size()]);
					else if (pass == 2)
						return OffensePlay(formations[formation-1], PLAY_PASS, receivers[play-1-runners.size()]);
					else if (pass == 3)
						return OffensePlay(formations[formation-1], PLAY_PASS_LONG, receivers[play-1-runners.size()]);
				}
			}
		}
		
	} while (true);
}

/*
 * Let's the player decide on an defensive call
 * */
DefensePlay TYFUIConsole::pickDefensePlay(TYFTeam* team)
{
	vector<DefFormation* > formations = this->Game->getDefensiveFormations();
	vector<string> FormationMenu;
	for (unsigned int j = 0; j < formations.size(); j++)
		FormationMenu.push_back(formations[j]->name);
	
	vector<string> PlayMenu;
	PlayMenu.push_back("Pass Block");
	PlayMenu.push_back("Run Block");
	PlayMenu.push_back("Blitz");
	
	int formation;
	int play;
	do
	{
		formation = this->displayMenu("Choose Defensive Formation:", FormationMenu, false);
		this->cls();
		while (formation != 0)
		{
			play = this->displayMenu("Choose Defensive Play:", PlayMenu, true);
			
			if (play == 0)
				formation = 0;
			else
			{
				this->cls();
				if (play == 1)
					return DefensePlay(formations[formation-1], DPLAY_PASSBLOCK);
				else if (play == 2)
					return DefensePlay(formations[formation-1], DPLAY_RUNBLOCK);
				else if (play == 3)
					return DefensePlay(formations[formation-1], DPLAY_BLITZ);
			}
		}
	} while (true);
}

/*
 * Ask the Player if and how much he wants to control the team
 * */
ControlFlag TYFUIConsole::setPlayerControl(TYFTeam* team)
{
	this->cls();
	vector<string> ControlMenu;
	ControlMenu.push_back("Computer Controlled");
	ControlMenu.push_back("Head Coach");
	ControlMenu.push_back("Offensive Coach");
	ControlMenu.push_back("Defensive Coach");
	stringstream ss;
	ss << "Control Setting for " << team->getName() << ":";
	switch (this->displayMenu(ss.str(), ControlMenu, false))
	{
		case 1:
			return CONTROL_COMPUTER;
		case 2:
			return CONTROL_PLAYER;
		case 3:
			return CONTROL_PLAYER_OFFENSE;
		case 4:
			return CONTROL_PLAYER_DEFENSE;
		default:
			return CONTROL_COMPUTER;
	}
}
