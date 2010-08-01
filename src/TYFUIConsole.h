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
#include "TYFUITemplate.h"
#include "helper.h"

using namespace std;

class TYFUIConsole : public TYFUITemplate
{
	private:
		string getBallPosition();
		string header;
		void cls();
		int displayMenu(string title, vector<string> menuItems, bool back);
	public:
		void beginPlay();
		void endPlay(PLAY_RESULT result);
		void run();
		void playKickOff(TYFPlayer* kicker, int yards);
		void playPunt(TYFPlayer* kicker, int yards, bool touchback);
		void playFieldGoal(TYFPlayer* kicker, int distance, bool good);
		void playPass(TYFPlayer* sender, TYFPlayer* receiver, TYFPlayer* intercepter, int distance, PASS_FLAG flag);
		void playSack(TYFPlayer* quarterback, TYFPlayer* tackler, int loss);
		void playRun(TYFPlayer* runner, int distance);
		void playFumble(TYFPlayer* player, bool recovered);
		void playReturn(TYFPlayer* returner, int distance, bool faircatch);
		void callTwoMinuteWarning();
		void callOutOfBounds();
		OffensePlay pickOffensePlay(TYFTeam* team);
};
