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

#ifdef GUI
#include <wx/wx.h>
#include <wx/image.h>
#include <string>

#include "TYFGame.h"

using namespace std;

class MainDialog: public wxDialog
{
	public:
		MainDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
		void setScore(TeamScore Team1, TeamScore Team2, int BallPossession);
		void setStatus(string str);
		void clearLog();
		void addLog(string s);
		
	private:
		void set_properties();
		void do_layout();
		void OnClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnChange(wxCommandEvent& event);
		
	protected:
		wxPanel* panel_1;
		wxStaticText* team1Name;
		wxStaticText* team1Score;
		wxPanel* panel_2;
		wxStaticBitmap* bitmap_1;
		wxPanel* panel_1_copy;
		wxStaticText* team2Name;
		wxStaticText* team2Score;
		wxPanel* panel_2_copy;
		wxStaticText* status;
		wxListBox* playLog;
		wxButton* playBtn;

};
#endif
