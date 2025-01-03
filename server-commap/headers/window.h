#ifndef WINDOW_H
#define WINDOW_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/filedlg.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>

const auto wxDP = wxDefaultPosition;
const auto wxDS = wxDefaultSize;

class Window : public wxFrame {
public:
	Window(const wxString &title, wxPoint wPoint , wxSize wSize);
};

#endif