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
#include "UFLMainDialog.h"
#include "UFLUI.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <string>
#include <stdlib.h>
#include "TYFGame.h"
#include "helper.h"

using namespace std;

const int ID_BUTTON = 1;

extern TYFUIUFL *DaUI;

MainDialog::MainDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxTHICK_FRAME)
{
    panel_1 = new wxPanel(this, wxID_ANY);
    team1Name = new wxStaticText(this, wxID_ANY, wxT(""));
    team1Score = new wxStaticText(this, wxID_ANY, wxT(""));
    panel_2 = new wxPanel(this, wxID_ANY);
    bitmap_1 = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("ufl.png"), wxBITMAP_TYPE_ANY));
    panel_1_copy = new wxPanel(this, wxID_ANY);
    team2Name = new wxStaticText(this, wxID_ANY, wxT(""));
    team2Score = new wxStaticText(this, wxID_ANY, wxT(""));
    panel_2_copy = new wxPanel(this, wxID_ANY);
    status = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    const wxString *playLog_choices = NULL;
    playLog = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, playLog_choices, 0);
    playBtn = new wxButton(this, ID_BUTTON, wxT("Play"));
	
	set_properties();
	do_layout();
	SetSize(wxSize(703, 452));
	Centre();
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainDialog::OnClose));
	Connect(ID_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainDialog::OnClick));
	
	DaUI->nextPlay(this);
}

void MainDialog::clearLog()
{
	this->playLog->Clear();
}

void MainDialog::addLog(string s)
{
	this->playLog->Append(wxString(s.c_str(), wxConvUTF8)); 
}

void MainDialog::OnClick(wxCommandEvent& event) 
{
	DaUI->nextPlay(this);
	event.Skip();
}

void MainDialog::setStatus(string str)
{
	this->status->SetLabel(wxString(str.c_str(), wxConvUTF8));
}

void MainDialog::setScore(TeamScore Team1, TeamScore Team2, int BallPossession)
{
	this->team1Name->SetLabel(wxString(Team1.Name.c_str(), wxConvUTF8));
	this->team2Name->SetLabel(wxString(Team2.Name.c_str(), wxConvUTF8));
	this->team1Score->SetLabel(wxString::Format(wxT("%i"), Team1.Points));
	this->team2Score->SetLabel(wxString::Format(wxT("%i"), Team2.Points));
	
	this->team1Name->SetForegroundColour(wxColour(0, 0, 0));
	this->team2Name->SetForegroundColour(wxColour(0, 0, 0));
	if (BallPossession == 0)
		this->team1Name->SetForegroundColour(wxColour(255, 0, 0));
	else
		this->team2Name->SetForegroundColour(wxColour(255, 0, 0));
}

void MainDialog::set_properties()
{
	SetTitle(wxT("UFL - Universal Football League"));
	SetSize(wxSize(1024, 485));
	team1Name->SetForegroundColour(wxColour(255, 0, 0));
	team1Name->SetFont(wxFont(23, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
	team1Score->SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
	team2Name->SetFont(wxFont(23, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
	team2Score->SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
}

void MainDialog::OnClose(wxCloseEvent& event) 
{
	wxMessageDialog *dial = new wxMessageDialog(NULL,
		wxT("Are you sure to quit?"), wxT("Question"),
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	int ret = dial->ShowModal();
	dial->Destroy();

	if (ret == wxID_YES)
	{
		Destroy();
		exit(0);
	}
	else
		event.Veto();
}

void MainDialog::do_layout()
{
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_11 = new wxBoxSizer(wxVERTICAL);
	wxGridSizer* grid_sizer_3 = new wxGridSizer(1, 1, 0, 0);
	wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_12_copy = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_12 = new wxBoxSizer(wxVERTICAL);
	sizer_1->Add(status, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	sizer_12->Add(panel_1, 1, wxEXPAND, 0);
	sizer_12->Add(team1Name, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_12->Add(team1Score, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_12->Add(panel_2, 1, wxEXPAND, 0);
	sizer_8->Add(sizer_12, 1, wxEXPAND, 0);
	sizer_8->Add(bitmap_1, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_12_copy->Add(panel_1_copy, 1, wxEXPAND, 0);
	sizer_12_copy->Add(team2Name, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_12_copy->Add(team2Score, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_12_copy->Add(panel_2_copy, 1, wxEXPAND, 0);
	sizer_8->Add(sizer_12_copy, 1, wxEXPAND, 0);
	sizer_11->Add(sizer_8, 1, wxEXPAND, 0);
	grid_sizer_3->Add(playLog, 0, wxEXPAND, 0);
	sizer_11->Add(grid_sizer_3, 1, wxALL|wxEXPAND, 10);
	sizer_1->Add(sizer_11, 1, wxEXPAND, 0);
	sizer_2->Add(playBtn, 0, 0, 0);
	sizer_1->Add(sizer_2, 0, wxALL|wxALIGN_RIGHT, 5);
	SetSizer(sizer_1);
	Layout();
}
#endif
