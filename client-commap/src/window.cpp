#include "../headers/window.h"
#include "window-functions.cpp"
#include "socket-functions.cpp"

Window::Window(const wxString &title, wxPoint wPoint, wxSize wSize)
: wxFrame(NULL, wxID_ANY, title, wPoint, wSize) {
	main_panel = new wxPanel(this, wxID_ANY, wxDP, wxDS);
	contacts_panel = new wxPanel(this, wxID_ANY, wxDP, wxSize(400, 300));
	buttons_panel = new wxPanel(this, wxID_ANY, wxDP, wxDS);

	id_field = new wxTextCtrl(buttons_panel, wxID_ANY); // id_field->Enable(false);
	setid_button = new wxButton(buttons_panel, wxID_ANY, "Set ID", wxDP, wxDS); // setid_button->Enable(false);
	message_button = new wxButton(buttons_panel, wxID_ANY, "Message", wxDP, wxDS);
	add_button = new wxButton(buttons_panel, wxID_ANY, "Add", wxDP, wxDS);
	delete_button = new wxButton(buttons_panel, wxID_ANY, "Delete", wxDP, wxDS);
	
	add_button->Bind(wxEVT_BUTTON, &Window::AddNewContactDialog, this);
	message_button->Bind(wxEVT_BUTTON, &Window::SendMessageDialog, this);
	setid_button->Bind(wxEVT_BUTTON, [this](wxCommandEvent &ev) {
		my_id = std::stoi(id_field->GetValue().ToStdString());
		std::string id = std::to_string(my_id);
		send(client_socket, id.c_str(), id.size(), 0);
	});

	contacts_grid = new wxGrid(contacts_panel, wxID_ANY, wxDP, wxDS);
	contacts_grid->CreateGrid(0, 1);
	contacts_grid->SetColLabelValue(0, "Contacts");
	contacts_grid->EnableEditing(false);
	contacts_grid->HideRowLabels();
	contacts_grid->SetSelectionMode(wxGrid::wxGridSelectCells);
    contacts_grid->SelectRow(0);
    // contacts_grid->SetGridCursor(0, 0);
	
	int grid_width = contacts_grid->GetSize().GetWidth();
    contacts_grid->SetColSize(0, grid_width);
	
    contacts_grid->Bind(wxEVT_SIZE, [this](wxSizeEvent &sv){
    	int new_width = contacts_grid->GetClientSize().GetWidth();
    	contacts_grid->SetColSize(0, new_width);
    	sv.Skip();
    });

	contacts_grid->Bind(wxEVT_GRID_SELECT_CELL, &Window::GridSelectHandler, this);

	contacts_sizer = new wxBoxSizer(wxVERTICAL);
	contacts_sizer->Add(contacts_grid, 1, wxEXPAND);
	contacts_panel->SetSizer(contacts_sizer);

	buttons_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer->Add(id_field, 0, wxALL, 10);
	buttons_sizer->Add(setid_button, 0, wxALL, 10);
	buttons_sizer->Add(message_button, 0, wxALL, 10);
	buttons_sizer->Add(add_button, 0, wxALL, 10);
	buttons_sizer->Add(delete_button, 0, wxALL, 10);
	buttons_panel->SetSizer(buttons_sizer);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(contacts_panel, 1, wxEXPAND | wxALL, 10);
	main_sizer->Add(buttons_panel, 0, wxALL, 10);
	main_sizer->Add(main_panel, 0, wxEXPAND);

	SetSizerAndFit(main_sizer);
}