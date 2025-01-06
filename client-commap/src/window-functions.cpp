#include "../headers/window.h"

void Window::AddNewContactDialog(wxCommandEvent& ev) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Enter new contact info", wxDefaultPosition, wxSize(200, 200));
    dialog->Bind(wxEVT_CLOSE_WINDOW, [this, dialog](wxCloseEvent &ec) {
        dialog->EndModal(wxID_CANCEL);
    });

    wxTextCtrl* name_field = new wxTextCtrl(dialog, wxID_ANY);
    wxTextCtrl* id_field = new wxTextCtrl(dialog, wxID_ANY);
    wxButton* confirm_button = new wxButton(dialog, wxID_OK, "Add");

    name_field->SetHint("Enter name here");
    id_field->SetHint("Enter id here");

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(name_field, 0, wxALL | wxEXPAND, 10);
    vbox->Add(id_field, 0, wxALL | wxEXPAND, 10);
    vbox->Add(confirm_button, 0, wxALL | wxALIGN_CENTER, 10);

    confirm_button->Bind(wxEVT_BUTTON, [this, name_field, id_field, dialog](wxCommandEvent &ev) {
        wxString name = name_field->GetValue();
    	wxString id = id_field->GetValue();
        if (name != "" && id != "") {
            AddContactToGrid(name);
            AddInfoToContactsFile(id, name);
        } else Error("Please enter value to the fields");
        dialog->EndModal(wxID_OK);
    });

    dialog->SetSizer(vbox);
	dialog->ShowModal();
}

void Window::SendMessageDialog(wxCommandEvent& ev) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Send message", wxDefaultPosition, wxSize(400, 300));
    dialog->Bind(wxEVT_CLOSE_WINDOW, [this, dialog](wxCloseEvent &ec) {
        dialog->EndModal(wxID_CANCEL);
    });

    wxTextCtrl* msg_field = new wxTextCtrl(dialog, wxID_ANY, "", wxDP, wxDS, wxTE_MULTILINE);
 
    wxButton* send_button = new wxButton(dialog, wxID_OK, "Send");
    wxButton* cancel_button = new wxButton(dialog, wxID_OK, "Cancel");
    cancel_button->Bind(wxEVT_BUTTON, [this, dialog](wxCommandEvent &ev) {
        dialog->EndModal(wxID_CANCEL);
    });

    std::cout << current_contact << '\n';
    send_button->Bind(wxEVT_BUTTON, [this, msg_field, dialog](wxCommandEvent &ev) {
        if (!msg_field->GetValue().IsEmpty()) {
            SendMessage(msg_field->GetValue(), current_contact);
        }
        else Error("Please enter a message");
    });


    wxBoxSizer* msg_field_sizer = new wxBoxSizer(wxVERTICAL);
    msg_field_sizer->Add(msg_field, 1, wxALL | wxEXPAND, 10);

    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    buttons_sizer->Add(send_button, 0, wxALL | wxALIGN_CENTER, 10);
    buttons_sizer->Add(cancel_button, 0, wxALL | wxALIGN_CENTER, 10);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(msg_field_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0);

    dialog->SetSizer(main_sizer);
    dialog->ShowModal();
}

void Window::GridSelectHandler(wxGridEvent &event) {
    int selectedRow = event.GetRow();
    std::string current_name;
    std::string name = contacts_grid->GetCellValue(selectedRow, 0).ToStdString();

    if (!name.empty()) {
        current_contact = contact_ids[selectedRow];
        // current_contact_name = name; 
    }

    event.Skip();
}

void Window::AddContactToGrid(wxString name) {
    int rowCount = contacts_grid->GetNumberRows();

    contacts_grid->AppendRows(1);
	contacts_grid->SetCellValue(rowCount, 0, name);
    contacts_grid->SetRowSize(rowCount, 35);
}