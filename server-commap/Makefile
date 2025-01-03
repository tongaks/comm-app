commap: build/App.o build/window.o
	g++ build/App.o build/window.o -o commap `wx-config --cxxflags --libs`

build/App.o: headers/App.h src/App.cpp
	g++ -c src/App.cpp -o build/App.o `wx-config --cxxflags --libs`

build/window.o: headers/window.h src/window.cpp src/window-functions.cpp
	g++ -c src/window.cpp -o build/window.o `wx-config --cxxflags --libs`

clean:
	rm build/*.o