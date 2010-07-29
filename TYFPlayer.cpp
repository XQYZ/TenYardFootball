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

#include "TYFPlayer.h"
#include <iostream>
#define TIXML_USE_STL
#include "tinyxml.h" 

using namespace std;

TYFPlayer::TYFPlayer()
{

}

int TYFPlayer::getRunRating()
{
	return this->Run;
}

int TYFPlayer::getDefenseRating()
{
	return 5;
}

int TYFPlayer::getOffenseRating()
{
	return 5;
}

int TYFPlayer::getKickRating()
{
	return this->Kick;
}

int getValue(TiXmlElement *pParm, string name)
{
	return atoi(pParm->FirstChildElement(name)->GetText());
}

void TYFPlayer::loadPlayerFromXMLNode(TiXmlElement *pParm)
{
	this->LName       = pParm->FirstChildElement("name")->GetText();
	this->FName       = pParm->FirstChildElement("firstname")->GetText();
	this->Position    = pParm->FirstChildElement("position")->GetText();
	this->Years       = getValue(pParm, "years");
	this->Durability  = getValue(pParm, "durability");
	this->Power       = getValue(pParm, "power");
	this->Run         = getValue(pParm, "run");
	this->Speed       = getValue(pParm, "speed");
	this->Pass        = getValue(pParm, "pass");
	this->Catch       = getValue(pParm, "catch");
	this->RunBlock    = getValue(pParm, "runBlock");
	this->PassBlock   = getValue(pParm, "passBlock");
	this->RunDefense  = getValue(pParm, "runDef");
	this->PassDefense = getValue(pParm, "passDef");
	this->Punt        = getValue(pParm, "punt");
	this->Blitz       = getValue(pParm, "blitz");
	this->Kick        = getValue(pParm, "kick");
	this->PuntReturn  = getValue(pParm, "puntRet");
	this->KickReturn  = getValue(pParm, "kickRet");	
}
