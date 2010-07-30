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
#define TIXML_USE_STL
#include "tinyxml.h" 

using namespace std;

/*
 * create a team
 * */
TYFTeam::TYFTeam(string name, string shortname)
{
	string filename = "teams/";
	filename.append(name);
	filename.append(".xml");
	this->loadFromFile(filename);
	for (int i = 0; i < 11; i++)
	{
		this->OnField[i] = this->Players[i];
	}
	this->score = 0;
	this->name = name;
	this->shortname = shortname;
}

/*
 * load the team from a xml file
 * */
void TYFTeam::loadFromFile(string filename)
{
	TiXmlDocument XMLdoc(filename.c_str());
	bool loadOkay = XMLdoc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pRoot, *pParm;
		pRoot = XMLdoc.FirstChildElement("root");
		if ( pRoot )
		{
			// Parse parameters
			pParm = pRoot->FirstChildElement("player");
			while ( pParm )
			{
				TYFPlayer* p = new TYFPlayer(pParm);
				this->Players.push_back(p);
				pParm = pParm->NextSiblingElement("player");
			}
		}
		else
		{
			cout << "Cannot find 'root' node" << endl;
		}
	}
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
	return this->shortname;
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
	for (int i = 0; i < 11; i++)
		if ((this->OnField[i]->Position == "KI") || (this->OnField[i]->Position == "PU"))
			return this->OnField[i];
	return NULL;
}

/*
 * return all TYFPlayer instances, whcih are available to run the ball
 * */
vector<TYFPlayer* > TYFTeam::getRunners()
{
	vector<TYFPlayer* > players;
	for (int i = 0; i < 11; i++)
	{
		if ((this->OnField[i]->Position == "HB") || (this->OnField[i]->Position == "FB") || (this->OnField[i]->Position == "QB"))
			players.push_back(this->OnField[i]);
	}
	return players;
}

/*
 * Add Player p to the current field players if he's palying on the 'pos' Position and if
 * the 'formvar' count variable is not yet zero
 * */
void TYFTeam::addPlayer(TYFPlayer *p, string pos, int* formvar)
{
	if ((p->Position == pos) && (*formvar > 0))
	{
		int c = 0;
		while (this->OnField[c] != NULL)
			c++;
		this->OnField[c] = p;
		//cout << c << " " << p->LName << " " << p->Position << *formvar << endl;
		*formvar -= 1;
	}
}

/*
 * align the team in a defense formation
 * */
void TYFTeam::setupDefFormation(DefFormation form)
{
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "CB", &form.CB);
		this->addPlayer(this->Players[i], "LB", &form.LB);
		this->addPlayer(this->Players[i], "SA", &form.SA);
		this->addPlayer(this->Players[i], "DE", &form.DE);
		this->addPlayer(this->Players[i], "DT", &form.DT);
	}
}

/*
 * align the team in an offense formation
 * */
void TYFTeam::setupOffFormation(OffFormation form)
{
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "HB", &form.HB);
		this->addPlayer(this->Players[i], "FB", &form.FB);
		this->addPlayer(this->Players[i], "WR", &form.WR);
		this->addPlayer(this->Players[i], "TE", &form.TE);
		this->addPlayer(this->Players[i], "OG", &form.OG);
		this->addPlayer(this->Players[i], "OT", &form.OT);
		this->addPlayer(this->Players[i], "QB", &form.QB);
		this->addPlayer(this->Players[i], "CE", &form.CE);
	}
}

/*
 * align the team in the punt formation
 * */
void TYFTeam::setupPuntFormation()
{
	int PU = 1;
	int QB = 1;
	int HB = 2;
	int OT = 2;
	int OG = 2;
	int CE = 1;
	int DT = 2;
	
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "PU", &PU);
		this->addPlayer(this->Players[i], "QB", &QB);
		this->addPlayer(this->Players[i], "HB", &HB);
		this->addPlayer(this->Players[i], "OT", &OT);
		this->addPlayer(this->Players[i], "OG", &OG);
		this->addPlayer(this->Players[i], "CE", &CE);
		this->addPlayer(this->Players[i], "DT", &DT);
	}
}

/*
 * align the team in the field goal formation
 * */
void TYFTeam::setupFieldGoalFormation()
{
	this->setupKickoffFormation();
}

/*
 * align the team in the kickoff formation
 * */
void TYFTeam::setupKickoffFormation()
{
	int KI = 1;
	int QB = 1;
	int HB = 2;
	int OT = 2;
	int OG = 2;
	int CE = 1;
	int DT = 2;
	
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "KI", &KI);
		this->addPlayer(this->Players[i], "QB", &QB);
		this->addPlayer(this->Players[i], "HB", &HB);
		this->addPlayer(this->Players[i], "OT", &OT);
		this->addPlayer(this->Players[i], "OG", &OG);
		this->addPlayer(this->Players[i], "CE", &CE);
		this->addPlayer(this->Players[i], "DT", &DT);
	}
}
