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

#ifdef GUI
#include "UFLUI.h"
#include "TYFUITemplate.h"
#include "TYFGame.h"
#include "TYFTeam.h"
#include "helper.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream> 
#include <time.h> 
#include <wx/wx.h>
#include <wx/image.h>
#include "UFLMainDialog.h"
#include "UFLChoiceDialog.h"
#include "UFLChoiceDialogPlay.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <cstring>

#include <libgen.h>
#include <stdlib.h> 

using namespace std;

class MyApp: public wxApp {
public:
    bool OnInit();
};

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MainDialog* UFL = new MainDialog(NULL, wxID_ANY, wxEmptyString);
    SetTopWindow(UFL);
    UFL->Show();
    return true;
}

wxAppConsole *wxCreateApp()
{
	wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "your program");
	return new MyApp;
}

wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp);
extern MyApp& wxGetApp();

MyApp& wxGetApp()
{
	return *wx_static_cast(MyApp*, wxApp::GetInstance());
}

TYFUIUFL::TYFUIUFL(void)
{
	srand((int)time(NULL));
	this->Game = new TYFGame(this);
	this->choice = false;
}

/**
 * this is called when the UI officially takes over as the main thread
 * Basically it just runs until the game finishes here
 * */
void TYFUIUFL::run()
{
	int argc = 0;
	char** argv= NULL;
     
	DIR *dp;
	struct dirent *ep;

	dp = opendir ("./teams/");
	if (dp != NULL)
	{
		ep = readdir (dp);
		do
		{
			stringstream ss;
			ss << ep->d_name;
			string::size_type idx = ss.str().find('.');
			if ((ss.str() != ".") && (ss.str() != ".."))
			{
				teamFiles.push_back(ss.str().substr(0, idx));
			}
			ep = readdir (dp);
		} while (ep);
		(void) closedir (dp);
	}
	else
		perror ("Couldn't open the directory");
	
	wxEntry(argc, argv);
}

/*
 * calls the next play
 * */
void TYFUIUFL::nextPlay(MainDialog* dialog)
{
	this->dialog = dialog;
	if (this->choice)
		this->Game->nextPlay();
	else
	{
		this->Game->init();
		this->choice = true;
		this->Game->nextPlay();
	}
}

/*
 * returns the position of the Ball in the "CHI 17" format
 * */
string TYFUIUFL::getBallPosition()
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
void TYFUIUFL::beginPlay()
{
	this->dialog->clearLog();
	GameInfo info = this->Game->getGameInfo();
	
	this->dialog->setScore(info.Scores[0], info.Scores[1], info.Ball.Possession);
	
	string down[4] = {"1st", "2nd", "3rd", "4th"};
	
	stringstream ss;
	
	ss << setfill('0');
	ss << "Q" << info.Time.Quarter << " " << (int)info.Time.Time/60 << ":";
	ss << setw(2) << (int)info.Time.Time%60 << " - ";
	ss << down[info.Ball.Down-1] << "&&";
	if (info.Ball.ToGo == 0)
		ss << "inches";
	else if (100 - info.Ball.Position < info.Ball.ToGo)
		ss << "goal";
	else
		ss << info.Ball.ToGo;
	ss << " - ";
	ss << this->getBallPosition() << endl;
	
	this->header = ss.str();
	
	this->dialog->setStatus(this->header);
}

/**
 * this is called after each play and delivers after-play results to the UI
 * (e.g. a touchdown which was scored)
 * */
void TYFUIUFL::endPlay(PLAY_RESULT result)
{
	if (result == PLAY_FIRST_DOWN)
		this->dialog->addLog("First Down!");
	else if (result == PLAY_TOUCHDOWN)
	{
		this->dialog->addLog("Touchdown!");
		this->dialog->addLog("Extra Point is NOT good!");
	}
	else if (result == PLAY_TOUCHDOWN_XP)
	{
		this->dialog->addLog("Touchdown!");
		this->dialog->addLog("Extra Point is good!");
	}
	else if (result == PLAY_SAFETY)
		this->dialog->addLog("Safety!");
	else if (result == PLAY_TURNOVER_ON_DOWNS)
		this->dialog->addLog("Turnover on Downs!");
}

void TYFUIUFL::playKickOff(TYFPlayer* kicker, int yards)
{
	stringstream ss;
	ss << "Kickoff of " << yards << " yards by " << kicker->getFullName();
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playPunt(TYFPlayer* kicker, int yards, bool touchback)
{
	stringstream ss;
	ss << "A Punt of " << yards << " yards by " << kicker->getFullName();
	if (touchback)
		ss << "Touchback!" << endl;
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playFieldGoal(TYFPlayer* kicker, int distance, bool good)
{
	stringstream ss;
	ss << "A " << distance << " yard fieldgoal attempt by " << kicker->getFullName();
	this->dialog->addLog(ss.str());
	ss.str("");
	if (good)
		ss << "Kick is good!";
	else
		ss << "Kick is no good!";
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playPass(TYFPlayer* sender, TYFPlayer* receiver, TYFPlayer* intercepter, int distance,
							PASS_FLAG flag)
{
	stringstream ss;
	if (flag == PASS_INCOMPLETE)
		ss << "Incomplete Pass intended for " << receiver->getFullName();
	else
		ss << "Pass of " << distance << " yards to " << receiver->getFullName();
	this->dialog->addLog(ss.str());
	ss.str("");
	if (flag == PASS_INTERCEPTED)
	{
		ss << "Ball intercepted by " << intercepter->getFullName() << "!";
		this->dialog->addLog(ss.str());
	}
}

void TYFUIUFL::playSack(TYFPlayer* quarterback, TYFPlayer* tackler, int loss)
{
	stringstream ss;
	ss << quarterback->getFullName() << " sacked by " << tackler->getFullName();
	ss << "; a loss of " << loss << " yards";
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playRun(TYFPlayer* runner, int distance)
{
	stringstream ss;
	if (runner->getPosition() == "QB")
	{
		ss << runner->getFullName();
		if (distance > 0)
			ss << " running for a gain of " << distance << " yards";
		else if (distance == 0)
			ss << " running for no gain";
		else
			ss << " running for a loss of " << -distance << " yards";
	}
	else
	{
		ss << "Ball handed off to " << runner->getFullName();
		if (distance > 0)
			ss << " for a run of " << distance << " yards";
		else if (distance == 0)
			ss << " for no gain";
		else
			ss << " for a loss of " << -distance << " yards";
	}
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playFumble(TYFPlayer* player, bool recovered)
{
	stringstream ss;
	this->dialog->addLog("Ball fumbled!");
	if (recovered)
		ss << "Recovered by " << player->getFullName() << "!";
	else
		ss << "Recovered by " << player->getFullName() << " of the opposing team!";
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::callTwoMinuteWarning()
{
	this->dialog->addLog("Two Minute Warning!");
}

void TYFUIUFL::callOutOfBounds()
{
	stringstream ss;
	ss << "Ran out of bounds at " << this->getBallPosition();
	this->dialog->addLog(ss.str());
}

void TYFUIUFL::playReturn(TYFPlayer* returner, int distance, bool faircatch)
{
	stringstream ss;
	GameInfo info = this->Game->getGameInfo();
	if (faircatch)
		ss << "Fair Catch at " << this->getBallPosition() << " by " << returner->getFullName();
	else
		ss << "A return of " << distance << " yards by " << returner->getFullName();
	this->dialog->addLog(ss.str());
}

/*
 * used for naming offensive formations
 * */
string TYFUIUFL::printTimes(string name, int count, int max)
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
int TYFUIUFL::displayMenu(string title, vector<string> menuItems, bool back, bool play)
{
	if (play)
	{
		ChoiceDialogPlay *dlg = new ChoiceDialogPlay(NULL, 0, wxString(title.c_str(), wxConvUTF8));
		dlg->setItems(menuItems);
		do
			dlg->ShowModal();
		while (dlg->getSelection() == -1);
		
		return dlg->getSelection() + 1;
	}
	else
	{
		ChoiceDialog *dlg = new ChoiceDialog(NULL, 0, wxString(title.c_str(), wxConvUTF8));
		dlg->setItems(menuItems);
		do
			dlg->ShowModal();
		while (dlg->getSelection() == -1);
		
		return dlg->getSelection() + 1;
	}
}

/*
 * Let's the player decide on an offensive call
 * */
OffensePlay TYFUIUFL::pickOffensePlay(TYFTeam* team)
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
		formation = this->displayMenu("Choose Offense Formation:", FormationMenu, false, true);
		
		if ((formation == (int)formations.size() + 1) || (formation == (int)formations.size() + 2))
		{
			GameInfo info = this->Game->getGameInfo();
			vector<string> YesNoDialog;
			YesNoDialog.push_back("Yes");
			YesNoDialog.push_back("No");
			if ((info.Ball.Down == 4) || (this->displayMenu("Are you sure you want to Punt/Try for FG now?",
											YesNoDialog, false, false)) == 1)
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
			play = this->displayMenu("Choose your play:", PlayMenu, true, true);
			if (play == 0)
				formation = 0;
			else
			{
				if (play <= (int)runners.size())
				{
					return OffensePlay(formations[formation-1], PLAY_RUN, runners[play-1]);
				}
				while (play <= (int)runners.size() + (int)receivers.size() && play != 0)
				{
					vector<string> PassMenu;
					PassMenu.push_back("Short Pass");
					PassMenu.push_back("Medium Pass");
					PassMenu.push_back("Long Pass");
					
					pass = this->displayMenu("Choose your pass length:", PassMenu, true, true);
					
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
DefensePlay TYFUIUFL::pickDefensePlay(TYFTeam* team)
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
		formation = this->displayMenu("Choose Defensive Formation:", FormationMenu, false, true);
		if (FormationMenu[formation-1] == "Punt Return")
			return DefensePlay(formations[formation-1], DPLAY_PUNTRETURN);
		else if (FormationMenu[formation-1] == "Field Goal Block")
			return DefensePlay(formations[formation-1], DPLAY_FIELDGOALBLOCK);
		
		while (formation != 0)
		{
			play = this->displayMenu("Choose Defensive Play:", PlayMenu, true, true);
			
			if (play == 0)
				formation = 0;
			else
			{
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
ControlFlag TYFUIUFL::setPlayerControl(TYFTeam* team)
{
	vector<string> ControlMenu;
	ControlMenu.push_back("Computer Controlled");
	ControlMenu.push_back("Head Coach");
	ControlMenu.push_back("Offensive Coach");
	ControlMenu.push_back("Defensive Coach");
	stringstream ss;
	ss << "Control Setting for " << team->getName() << ":";
	switch (this->displayMenu(ss.str(), ControlMenu, false, false))
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

/*
 * Ask the Player about the team
 * */
string TYFUIUFL::setTeam()
{
	stringstream ss;
	ss << "Select Team:";
	return this->teamFiles[this->displayMenu(ss.str(), this->teamFiles, false, false) - 1];
}
#endif
