#include "../headers/App.h"
#include "../headers/window.h"

bool App::OnInit() {
	Window *window = new Window("commapclient", wxDP, wxSize(800, 700)); 
	window->Show(true);
	return true;
}

wxIMPLEMENT_APP(App);