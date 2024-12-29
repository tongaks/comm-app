#include "../headers/window.h"
#include "window-functions.cpp"

Window::Window(const wxString &title, wxPoint wPoint, wxSize wSize)
: wxFrame(NULL, wxID_ANY, title, wPoint, wSize) {
	main_panel = new wxPanel(this, wxID_ANY, wxDP, wxDS);
	contacts_panel = new wxPanel(this, wxID_ANY, wxDP, wxSize(400, 300));
	buttons_panel = new wxPanel(this, wxID_ANY, wxDP, wxDS);

	message_button = new wxButton(buttons_panel, wxID_ANY, "Message", wxDP, wxDS);
	add_button = new wxButton(buttons_panel, wxID_ANY, "Add", wxDP, wxDS);
	delete_button = new wxButton(buttons_panel, wxID_ANY, "Delete", wxDP, wxDS);
	
	contacts_grid = new wxGrid(contacts_panel, wxID_ANY, wxDP, wxDS);
	contacts_grid->CreateGrid(0, 1);
	contacts_grid->SetColLabelValue(0, "Contacts");
	contacts_grid->HideRowLabels();
	
	int grid_width = contacts_grid->GetSize().GetWidth();
    contacts_grid->SetColSize(0, grid_width);
	
    contacts_grid->Bind(wxEVT_SIZE, [this](wxSizeEvent &sv){
    	int new_width = contacts_grid->GetClientSize().GetWidth();
    	contacts_grid->SetColSize(0, new_width);
    	sv.Skip();
    });

	contacts_sizer = new wxBoxSizer(wxVERTICAL);
	contacts_sizer->Add(contacts_grid, 1, wxEXPAND);
	contacts_panel->SetSizer(contacts_sizer);

	buttons_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer->Add(message_button, 0, wxALL, 10);
	buttons_sizer->Add(add_button, 0, wxALL, 10);
	buttons_sizer->Add(delete_button, 0, wxALL, 10);
	buttons_panel->SetSizer(buttons_sizer);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(contacts_panel, 1, wxEXPAND | wxALL, 10);
	main_sizer->Add(buttons_panel, 0, wxALL, 10);
	main_sizer->Add(main_panel, 0, wxEXPAND);

	SetSizerAndFit(main_sizer);

	SetUpSocket();
}