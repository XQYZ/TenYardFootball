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
TYFTeam::TYFTeam(string name)
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
	for (int i = 0; i <= 4; i++)
		this->scoreQuarter[i] = 0;
	this->name = name;
}

/*
 * sets who controlls this team
 * */
void TYFTeam::setController(ControlFlag controlled)
{
	this->controlled = controlled;
}

/*
 * returns true if the player controls this team
 * */
bool TYFTeam::isPlayerControlled(ControlFlag controlled)
{
	if (this->controlled == CONTROL_PLAYER)
		return true;
	else 
		return (this->controlled == controlled);
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
			this->shortname = pRoot->FirstChildElement("shortname")->GetText();
			
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
void TYFTeam::scorePoints(int n, int q)
{
	this->score += n;
	this->scoreQuarter[q-1] += n;
}

/*
 * get the current score for this team
 * */
int TYFTeam::getPoints()
{
	return this->score;
}

int TYFTeam::getPoints(int q)
{
	return this->scoreQuarter[q-1];
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
double TYFTeam::getDefenseRating(PlayType type)
{
	double sum = 0;
	double count = 0;
	for (int i = 0; i < 11; i++)
	{
		int r = this->OnField[i]->getDefenseRating(type);
		if (r != -1)
		{
			sum += r;
			count += 1;
		}
	}
	double average = sum / count;
	return average;
}

/*
 * gets the average offense rating of the current field players
 * */
double TYFTeam::getOffenseRating(PlayType type)
{
	double sum = 0;
	double count = 0;
	for (int i = 0; i < 11; i++)
	{
		int r = this->OnField[i]->getOffenseRating(type);
		if (r != -1)
		{
			sum += r;
			count += 1;
		}
	}
	double average = sum / count;
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
 * return the TYFPlayer instance of the QB
 * */
TYFPlayer* TYFTeam::getQuarterback()
{
	for (int i = 0; i < 11; i++)
		if (this->OnField[i]->Position == "QB")
			return this->OnField[i];
	return NULL;
}

/*
 * return a random player who is on the field
 * */
TYFPlayer* TYFTeam::getRandomPlayer()
{
	return this->OnField[random(0, 10)];
}

/*
 * get a random runner on the field
 * */
TYFPlayer* TYFTeam::getRandomRunner()
{
	// who will run the ball?
	vector<TYFPlayer* > players = this->getRunners();
	
	// random selection
	TYFPlayer* runner = NULL;
	TYFPlayer* temprunner;
	do
	{
		temprunner = players[random(0, players.size() - 1)];
		if (temprunner->getPosition() == "QB")
		{
			// QB runs should be rare
			if (random(0, 30) == 0)
				runner = temprunner;
		}
		else
			runner = temprunner;
	} while (runner == NULL);
	
	return runner;
}

/*
 * get a random person for a blitz
 * */
TYFPlayer* TYFTeam::getRandomTackle()
{
	vector<TYFPlayer* > players = this->getTackles();
	
	return players[random(0, players.size() - 1)];
}

/*
 * get a random receiver on the field
 * */
TYFPlayer* TYFTeam::getRandomReceiver()
{
	vector<TYFPlayer* > players = this->getReceivers();
	
	// random selection
	TYFPlayer* receiver = NULL;
	TYFPlayer* tempreceiver;
	do
	{
		tempreceiver = players[random(0, players.size() - 1)];
		if ((tempreceiver->getPosition() == "HB") || (tempreceiver->getPosition() == "FB"))
		{
			// HB/FB passes should be rarer
			if (random(0, 5) == 0)
				receiver = tempreceiver;
		}
		else
			receiver = tempreceiver;
	} while (receiver == NULL);
	
	return receiver;
}

/*
 * get a random return runner
 * */
TYFPlayer* TYFTeam::getRandomReturner(PlayType type)
{
	// random selection
	TYFPlayer* returner = NULL;
	TYFPlayer* tempreturner;
	do
	{
		tempreturner = this->Players[random(0, this->Players.size() - 1)];
		int r = tempreturner->getReturnRating(type);
		if (r > 0)
			returner = tempreturner;
	} while (returner == NULL);
	
	return returner;
}

/*
 * return all TYFPlayer instances, which are available to tackle in a blitz
 * */
vector<TYFPlayer* > TYFTeam::getTackles()
{
	vector<TYFPlayer* > players;
	for (int i = 0; i < 11; i++)
	{
		if (this->OnField[i]->getBlitzRating() > 0)
			players.push_back(this->OnField[i]);
	}
	return players;
}

/*
 * return all TYFPlayer instances, which are available to run the ball
 * */
vector<TYFPlayer* > TYFTeam::getRunners()
{
	vector<TYFPlayer* > players;
	for (int i = 0; i < 11; i++)
	{
		if ((this->OnField[i]->Position == "HB") || (this->OnField[i]->Position == "FB") ||
		    (this->OnField[i]->Position == "QB"))
			players.push_back(this->OnField[i]);
	}
	return players;
}

/*
 * return all TYFPlayer instances, which are available to receive the ball
 * */
vector<TYFPlayer* > TYFTeam::getReceivers()
{
	vector<TYFPlayer* > players;
	for (int i = 0; i < 11; i++)
	{
		if ((this->OnField[i]->Position == "HB") || (this->OnField[i]->Position == "FB") || 
		    (this->OnField[i]->Position == "WR") || (this->OnField[i]->Position == "TE") ||
		    (this->OnField[i]->Position == "CB") || (this->OnField[i]->Position == "SA") || 
		    (this->OnField[i]->Position == "LB"))
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
		*formvar -= 1;
	}
}

/*
 * align the team in a defense formation
 * */
void TYFTeam::setupDefFormation(DefFormation *form)
{
	DefFormation formation = *form;
	
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (unsigned int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "CB", &formation.CB);
		this->addPlayer(this->Players[i], "LB", &formation.LB);
		this->addPlayer(this->Players[i], "SA", &formation.SA);
		this->addPlayer(this->Players[i], "DE", &formation.DE);
		this->addPlayer(this->Players[i], "DT", &formation.DT);
	}
}

/*
 * align the team in an offense formation
 * */
void TYFTeam::setupOffFormation(OffFormation *form)
{
	OffFormation formation = *form;
	
	for (int i = 0; i < 11; i++)
		this->OnField[i] = NULL;
	
	for (unsigned int i = 0; i <= this->Players.size() - 1; i++)
	{
		this->addPlayer(this->Players[i], "HB", &formation.HB);
		this->addPlayer(this->Players[i], "FB", &formation.FB);
		this->addPlayer(this->Players[i], "WR", &formation.WR);
		this->addPlayer(this->Players[i], "TE", &formation.TE);
		this->addPlayer(this->Players[i], "OG", &formation.OG);
		this->addPlayer(this->Players[i], "OT", &formation.OT);
		this->addPlayer(this->Players[i], "QB", &formation.QB);
		this->addPlayer(this->Players[i], "CE", &formation.CE);
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
	
	for (unsigned int i = 0; i <= this->Players.size() - 1; i++)
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
	
	for (unsigned int i = 0; i <= this->Players.size() - 1; i++)
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
