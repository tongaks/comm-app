#include "../headers/window.h"

void Window::AddNewContactDialog(wxCommandEvent& ev) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Enter new contact info", wxDefaultPosition, wxSize(200, 200));

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* name_field = new wxTextCtrl(dialog, wxID_ANY);
    wxTextCtrl* id_field = new wxTextCtrl(dialog, wxID_ANY);
    wxButton* confirm_button = new wxButton(dialog, wxID_OK, "Add");

    name_field->SetHint("Enter name here");
    id_field->SetHint("Enter id here");

    vbox->Add(name_field, 0, wxALL | wxEXPAND, 10);
    vbox->Add(id_field, 0, wxALL | wxEXPAND, 10);
    vbox->Add(confirm_button, 0, wxALL | wxALIGN_CENTER, 10);

    dialog->Bind(wxEVT_CLOSE_WINDOW, [this, dialog](wxCloseEvent &ec) {
        dialog->EndModal(wxID_CANCEL);
    });

    confirm_button->Bind(wxEVT_BUTTON, [this, name_field, id_field](wxCommandEvent &ev) {
        wxString name = name_field->GetValue();
    	wxString id = id_field->GetValue();
        if (name != "" && id != "") {
            AddContactToGrid(name);
            AddInfoToContactsFile(id, name);
        } else Error("Please enter value to the fields");
    });

    dialog->SetSizer(vbox);
	dialog->ShowModal();
}

void Window::SendMessageDialog(wxCommandEvent& ev) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Send message", wxDefaultPosition, wxSize(400, 300));
    
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* msg_field = new wxTextCtrl(dialog, wxID_ANY);
    msg_field->SetHint("Enter message here");
 
    wxButton* send_button = new wxButton(dialog, wxID_OK, "Send");
    wxButton* cancel_button = new wxButton(dialog, wxID_OK, "Cancel");

    dialog->Bind(wxEVT_CLOSE_WINDOW, [this, dialog](wxCloseEvent &ec) {
        dialog->EndModal(wxID_CANCEL);
    });

    cancel_button->Bind(wxEVT_BUTTON, [this, dialog](wxCommandEvent &ev) {
        dialog->EndModal(wxID_CANCEL);
    });

    send_button->Bind(wxEVT_BUTTON, [this, msg_field, dialog](wxCommandEvent &ev) {
        if (!msg_field->GetValue().IsEmpty()) {
            SendMessage(msg_field->GetValue(), current_contact);
        }
        else Error("Please enter a message");
    });

    vbox->Add(msg_field, 0, wxALL | wxEXPAND, 10);
    vbox->Add(send_button, 0, wxALL | wxALIGN_CENTER, 10);
    vbox->Add(cancel_button, 0, wxALL | wxALIGN_CENTER, 10);

    dialog->SetSizer(vbox);
    dialog->ShowModal();
}

void Window::GridSelectHandler(wxGridEvent &event) {
    int selectedRow = event.GetRow();
    std::string current_name;
    std::string name = contacts_grid->GetCellValue(selectedRow, 0).ToStdString();

    if (!name.empty()) {
        current_contact = contact_ids[selectedRow];    
    }

    event.Skip();
}

void Window::AddContactToGrid(wxString name) {
    int rowCount = contacts_grid->GetNumberRows();

    contacts_grid->AppendRows(1);
	contacts_grid->SetCellValue(rowCount, 0, name);
    contacts_grid->SetRowSize(rowCount, 35);
}

void Window::AddInfoToContactsFile(wxString ID, wxString name) {
    std::ofstream contacts_file("contacts.txt", std::ios::app);
    if (!contacts_file) {
        Error("Failed to open contacts.txt");
        return;
    }

    contact_ids.push_back(std::stoi(ID.ToStdString()));
    contacts_file << name << ' ' << ID << '\n';
    contacts_file.close();
}

void Window::ReadContactsFile() {
    std::ifstream contacts_file("contacts.txt");
    if (!contacts_file) {
        Error("Failed to open contacts.txt");
        return;
    }

    std::string line;
    while (getline(contacts_file, line)) {
        std::string name; int id;
        std::istringstream ss(line);
        ss >> name >> id;

        AddContactToGrid(name);
        contact_names.push_back(name);
        contact_ids.push_back(id);
    }
}