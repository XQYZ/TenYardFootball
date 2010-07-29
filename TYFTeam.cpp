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

#include "TYFTeam.h"
#include <iostream>

using namespace std;

/*
 * create a team
 * */
TYFTeam::TYFTeam(string name)
{
	for (int i = 0; i < 50; i++)
	{
		if (name == "GB")
			this->Players[i] = new TYFPlayer(5);
		else
			this->Players[i] = new TYFPlayer(5);
	}
	for (int i = 0; i < 11; i++)
	{
		this->OnField[i] = this->Players[i];
	}
	this->score = 0;
	this->name = name;
}

/*
 * score n points for this team
 * */
void TYFTeam::scorePoints(int n)
{
	this->score += n;
}

/*
 * get the current score for this team
 * */
int TYFTeam::getPoints()
{
	return this->score;
}

/*
 * returns the name of this team
 * */
string TYFTeam::getName()
{
	return this->name;
}

/*
 * gets the average defense rating of the current field players
 * */
int TYFTeam::getDefenseRating()
{
	int sum = 0;
	for (int i = 0; i < 11; i++)
	{
		sum += this->OnField[i]->getDefenseRating();
	}
	int average = sum / 11;
	return average;
}

/*
 * gets the average offense rating of the current field players
 * */
int TYFTeam::getOffenseRating()
{
	int sum = 0;
	for (int i = 0; i < 11; i++)
	{
		sum += this->OnField[i]->getOffenseRating();
	}
	int average = sum / 11;
	return average;
}

/*
 * return the TYFPlayer instance of the kicker
 * */
TYFPlayer* TYFTeam::getKicker()
{
	return this->OnField[0];
}
