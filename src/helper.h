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
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

enum PlayType { PLAY_RUN, PLAY_PASS_LONG, PLAY_PASS, PLAY_PASS_SHORT, PLAY_PUNT, PLAY_KICKOFF, PLAY_FIELDGOAL };
enum DefPlayType { DPLAY_BLITZ, DPLAY_PASSBLOCK, DPLAY_RUNBLOCK, DPLAY_FIELDGOALBLOCK, DPLAY_PUNTRETURN };

/*
 * converts stuff to a string
 * */
template <class T>
inline string to_string (const T& t)
{
	stringstream ss;
	ss << t;
	return ss.str();
}

/*
 * Univeral Random Number Getter.
 * Awesome. Pure Awesome.
 * */
inline int random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
