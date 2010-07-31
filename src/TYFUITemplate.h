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

#include "helper.h"
#include "TYFTeam.h"
#include "TYFGame.h"

enum PASS_FLAG { PASS_OK, PASS_INTERCEPTED, PASS_INCOMPLETE };
enum PLAY_RESULT { PLAY_NOTHING, PLAY_FIRST_DOWN, PLAY_TOUCHDOWN, PLAY_SAFETY, PLAY_TURNOVER_ON_DOWNS };

class TYFGame;
class TYFPlayer;
class TYFUITemplate
{
	protected:
		TYFGame *Game;
	public:
		TYFUITemplate(void);
		virtual void run() {};
		virtual void beginPlay() {};
		virtual void endPlay(PLAY_RESULT result) {};
		virtual void playKickOff(TYFPlayer* kicker, int yards) {};
		virtual void playPunt(TYFPlayer* kicker, int yards, bool touchback) {};
		virtual void playFieldGoal(TYFPlayer* kicker, int distance, bool good) {};
		virtual void playPass(TYFPlayer* sender, TYFPlayer* receiver, TYFPlayer* intercepter,
								int distance, PASS_FLAG flag) {};
		virtual void playSack(TYFPlayer* quarterback, TYFPlayer* tackler, int loss) {};
		virtual void playRun(TYFPlayer* runner, int distance) {};
		virtual void playFumble(TYFPlayer* player, bool recovered) {};
		virtual void playReturn(TYFPlayer* returner, int distance, bool faircatch) {};
		virtual void callTwoMinuteWarning() {};
		virtual void callOutOfBounds() {};
		virtual OffensePlay pickOffensePlay(TYFTeam* team) { return OffensePlay(NULL, PLAY_PASS, team->getRandomReceiver()); };
};
