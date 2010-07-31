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
	cout << setfill('0');
	cout << "Q" << info.Time.Quarter << " " << (int)info.Time.Time/60 << ":" << setw(2) << (int)info.Time.Time%60 << " - ";
	cout << down[info.Ball.Down-1] << "&";
	if (info.Ball.ToGo == 0)
		cout << "inches";
	else
		cout << info.Ball.ToGo;
	cout << " - ";
	for (int i = 0; i <= 1; i++)
	{
		if (i == info.Ball.Possession)
			cout << "@";
		cout << info.Scores[i].Name << " " << info.Scores[i].Points << " ";
	}
	cout << "- " << this->getBallPosition() << endl;
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
	cout << endl;
}

/**
 * this is called when the UI officially takes over as the main thread
 * Basically it just runs until the game finishes here
 * */
void TYFUIConsole::run()
{
	while (this->Game->nextPlay() == PL_OK) {}
}

void TYFUIConsole::playKickOff(int yards)
{
	cout << "Kickoff of " << yards << " yards" << endl;
}

void TYFUIConsole::playPunt(int yards, bool touchback)
{
	cout << "A Punt of " << yards << " yards" << endl;
	if (touchback)
		cout << "Touchback!" << endl;
}

void TYFUIConsole::playFieldGoal(int distance, bool good)
{
	cout << "A " << distance << " yard fieldgoal attempt" << endl;
	if (good)
		cout << "Kick is good!" << endl;
	else
		cout << "Kick is no good!" << endl;
}

void TYFUIConsole::playPass(TYFPlayer* sender, TYFPlayer* receiver, int distance, PASS_FLAG flag)
{
	if (flag == PASS_INCOMPLETE)
		cout << "Incomplete Pass intended for " << receiver->getFullName() << endl;
	else
		cout << "Pass of " << distance << " yards to " << receiver->getFullName() << endl;
	if (flag == PASS_INTERCEPTED)
		cout << "Ball intercepted by " << receiver->getFullName() << "!" << endl;
}

void TYFUIConsole::playSack(TYFPlayer* quarterback, TYFPlayer* tackler, int loss)
{
	cout << quarterback->getFullName() << " sacked by " << tackler->getFullName() << "; a loss of " << loss << " yards" << endl;
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

void TYFUIConsole::playReturn(int distance, bool faircatch)
{
	GameInfo info = this->Game->getGameInfo();
	if (faircatch)
	{
		cout << "Faircatch at " << this->getBallPosition() << endl;
	}
	else
	{
		cout << "A return of " << distance << " yards." << endl;
	}
}
