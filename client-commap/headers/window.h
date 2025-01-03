#ifndef WINDOW_H
#define WINDOW_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <string.h>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

const auto wxDP = wxDefaultPosition;
const auto wxDS = wxDefaultSize;

class Window : public wxFrame {
	wxButton *message_button;
	wxButton *add_button;
	wxButton *delete_button;
	wxButton *setid_button;
	
	wxTextCtrl *id_field;
	wxGrid *contacts_grid;
	
	wxPanel *main_panel;
	wxPanel *contacts_panel;
	wxPanel *buttons_panel;

	wxBoxSizer *main_sizer;
	wxBoxSizer *contacts_sizer;
	wxBoxSizer *buttons_sizer;

	int current_contact = 0;
	int my_id = 89234;

	int client_socket = 0;
	struct sockaddr_in server_info;
	bool is_connected = false;

	std::vector<wxString> contact_names;
	std::vector<int> contact_ids;

public:
	Window(const wxString &title, wxPoint wPoint, wxSize wSize);

	void SetUpSocket();
	void ConnectToServer();
	void HandleConnection();

	void AddNewContactDialog(wxCommandEvent &ev);
	void AddContactToGrid(wxString name);
	void GridSelectHandler(wxGridEvent &event);

	void CreateContactsFile();
	void CheckContactsFile();
	void AddInfoToContactsFile(wxString ID, wxString name);
	void ReadContactsFile();

	void SendMessageDialog(wxCommandEvent &ev);
	bool SendMessage(wxString msg, int id);

	void Notice(std::string msg) {
		wxMessageDialog *dg = new wxMessageDialog(NULL, msg, "Notice", wxOK);
		dg->ShowModal();
	}

	void Error(std::string msg) {
		wxMessageDialog *dg = new wxMessageDialog(NULL, msg, "Error", wxOK | wxICON_ERROR);
		dg->ShowModal();
	}

	void Warning(std::string msg) {
		wxMessageDialog *dg = new wxMessageDialog(NULL, msg, "Warning", wxOK | wxICON_EXCLAMATION);
		dg->ShowModal();
	}
};

#endif