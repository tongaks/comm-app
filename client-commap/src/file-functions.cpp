#include "../headers/window.h"

void Window::CheckContactsFile() {
	struct stat sb;
	if (stat("contacts.txt", &sb) != 0) {
		std::ofstream contacts_file("contacts.txt");
		contacts_file.close();
	}
}

void Window::SaveConversation(wxString msg, int sender, int receiver) {
	std::ofstream save(std::to_string(receiver) + ".txt", std::ios::app);
	save << sender << ' ' << msg << '\n';
	save.close();
}

void Window::CheckConversationFile(int id) {
	struct stat sb;
	std::string fname = std::to_string(id) + ".txt";
	if (stat(fname.c_str(), &sb) != 0) {
		std::ofstream file(fname);
		file.close();
	}
}

void Window::ReadContactsFile() {
    std::ifstream check("contacts.txt", std::ios::binary | std::ios::ate);
    if (check.tellg() == 0) return; // check if contacts.txt is empty

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

    current_contact = contact_ids[0];
}

void Window::AddInfoToContactsFile(wxString ID, wxString name) {
    std::ofstream contacts_file("contacts.txt", std::ios::app);
    if (!contacts_file) {
        Error("Failed to open contacts.txt");
        return;
    }

    for (int i = 0; i < name.length(); i++) {
        if (name[i] == ' ' && !isdigit(name[i+1])) name[i] = '_';
    }

    contact_ids.push_back(std::stoi(ID.ToStdString()));
    contacts_file << name << ' ' << ID << '\n';
    contacts_file.close();
}