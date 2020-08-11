#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cursesw.h>

#include <unistd.h>

#include "Misc.hpp"

using namespace std;

void Cleanup(WINDOW* Window);

MiscClass Misc;

int main(int argc, char* argv[])
{
	WINDOW* MainWindow;
	// Initialise ncurses
	if ((MainWindow = initscr()) == NULL)
	{
		printf("Failed to initialise ncurses");
		exit(EXIT_FAILURE);
	}

	// Prompt to use IP address for location
	printw("Attempt to use IP address to get location?: ");
	refresh();

	// Get input
	string Prompt = Misc.GetStringInput();
	::transform(Prompt.begin(), Prompt.end(), Prompt.begin(), ::tolower);

	// Test
	string TestData = Misc.GetWeatherData("783a8cc9f5dc1a800eb1b61382f8b2c6", "uk", "Chester");
	cout << TestData << endl;
	refresh();	

	sleep(10);

	Cleanup(MainWindow);
}

void Cleanup(WINDOW* Window)
{
	// Delete and cleanup window
	delwin(Window);
	endwin();
	refresh();

	exit(EXIT_SUCCESS);
}
