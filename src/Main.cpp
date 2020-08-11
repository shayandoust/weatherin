#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cursesw.h>

#include <nlohmann/json.hpp>

#include <unistd.h>

#include "Misc.hpp"

#define IPLOCATIONAPIKEY "3ed74100b8d485"
#define WEATHERAPIKEY "783a8cc9f5dc1a800eb1b61382f8b2c6"

using namespace std;
using namespace nlohmann;

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
	printw("Will attempt to use IP address for location in 5 seconds. Ctrl+C to terminate");
	refresh();
	sleep(5);

	string LocationData = Misc.GetLocation(IPLOCATIONAPIKEY);
	json LocationDataJson = Misc.ConvertStringToJson(LocationData);

	string WeatherData = Misc.GetWeatherData(WEATHERAPIKEY,
			LocationDataJson["country"].get<string>().c_str(),
			LocationDataJson["city"].get<string>().c_str());
	json WeatherDataJson = Misc.ConvertStringToJson(WeatherData);


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
