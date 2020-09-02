#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
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
void SignalCallbackHandler(int Sig);
void WriteScreen(WINDOW* Window);
void PollForExit(WINDOW* Window);

MiscClass Misc;

// State declared global variables
string Description;
string ShortDesc;
string Country;
string City;
double CurrTemp;
double FeelsLikeTemp;
double MinTemp; 
double MaxTemp; 
int Pressure; 
int Humidity;
float WindSpeed ; 
int WindDir;

int main(int argc, char* argv[])
{

	WINDOW* MainWindow;
	// Initialise ncurses
	if ((MainWindow = initscr()) == NULL)
	{
		printf("Failed to initialise ncurses");
		exit(EXIT_FAILURE);
	}

	// Register signal handler.
	// Used for when using ctrl+c to exit application
	// TODO: Requires improvement to destroy window
	signal(SIGINT, SignalCallbackHandler);

	string LocationData = Misc.GetLocation(IPLOCATIONAPIKEY);
	json LocationDataJson = Misc.ConvertStringToJson(LocationData);

	string WeatherData = Misc.GetWeatherData(WEATHERAPIKEY,
			LocationDataJson["country"].get<string>().c_str(),
			LocationDataJson["city"].get<string>().c_str());
	json WeatherDataJson = Misc.ConvertStringToJson(WeatherData);

	// Define the global variables
	Description = WeatherDataJson["weather"][0]["description"].get<string>();
	ShortDesc = WeatherDataJson["weather"][0]["main"].get<string>();
	Country = LocationDataJson["country"].get<string>();
	City = LocationDataJson["city"].get<string>();
	CurrTemp = WeatherDataJson["main"]["temp"].get<double>();
	FeelsLikeTemp = WeatherDataJson["main"]["feels_like"].get<double>();
	MinTemp = WeatherDataJson["main"]["temp_min"].get<double>();
	MaxTemp = WeatherDataJson["main"]["temp_max"].get<double>();
	Pressure = WeatherDataJson["main"]["pressure"].get<int>();
	Humidity = WeatherDataJson["main"]["humidity"].get<int>();
	WindSpeed = WeatherDataJson["wind"]["speed"].get<float>();
	WindDir = WeatherDataJson["wind"]["deg"].get<int>();

	WriteScreen(MainWindow);
}

void WriteScreen(WINDOW* Window)
{
	// Clear the current window
	erase();
	// Add various location information to the top left of the screen
	string LocationDescription = "Current weather data in " + City + ", " + Country;
	mvwprintw(Window, 1, 1, LocationDescription.c_str());

	if (ShortDesc == "Sun")
		mvwprintw(Window, 10, 10, Misc.GetCorrespondingAsciiArt(1).c_str());

	//mvwprintw(Window, 20, 20, Misc.GetCorrespondingAsciiArt(WType).c_str());
	
	// Now, add weather description above the ASCII image
	string WeatherDescription = "Weather condition right now: " + Description;
	mvwprintw(Window, 10, 1, WeatherDescription.c_str());
	
	refresh();

	PollForExit(Window);
}

void PollForExit(WINDOW* Window)
{
	// Simply wait, and if the user presses q key, exit
	// q key on keyboard = 113
	if (getch() == 113)
		Cleanup(Window);
}

void SignalCallbackHandler(int Sig)
{
	exit(Sig);
}

void Cleanup(WINDOW* Window)
{
	// Delete and cleanup window
	delwin(Window);
	endwin();
	refresh();

	exit(EXIT_SUCCESS);
}
