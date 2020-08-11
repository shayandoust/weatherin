#include <iostream>
#include <sstream>
#include <string>
#include <cursesw.h>
#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include "Misc.hpp"

using namespace std;
using namespace nlohmann;

static string WeatherApiUrl = "https://api.openweathermap.org/data/2.5/weather";
static string LocationApiUrl = "https://ipinfo.io/";

string GetJsonResponse(string ApiUrl);
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

json MiscClass::ConvertStringToJson(string JsonString)
{
	return json::parse(JsonString);
}

string MiscClass::GetStringInput()
{
	string Entry;

	// Let the terminal handle all line editing
	nocbreak();
	echo();

	// Takes care of enter and backspacing
	int Ch = getch();
	while (Ch != '\n')
	{
		Entry.push_back(Ch);
		Ch = getch();
	}

	echo();
	cbreak();

	return Entry;
}

string MiscClass::GetWeatherData(string ApiKey, string CountryCode, string City)
{
	// Concatenate necessary CountryCode and City into Url
	WeatherApiUrl = WeatherApiUrl + "?q=" + City + "," + CountryCode + "&APPID=" + ApiKey + "&units=metric";

	return GetJsonResponse(WeatherApiUrl);
}

string MiscClass::GetLocation(string ApiKey)
{
	// Concatenate the Api key into the Url
	LocationApiUrl = LocationApiUrl + "?token=" + ApiKey;

	return GetJsonResponse(LocationApiUrl);
}

string GetJsonResponse(string ApiUrl)
{
	CURL *Curl;
	ostringstream Oss;
	struct curl_slist *Headers = NULL;
	string ResponseBuffer;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Define CURL headers
	Headers = curl_slist_append(Headers, "Accept: application/json");
	Headers = curl_slist_append(Headers, "Content-Type: application/json");

	// Instantiate curl
	if ((Curl = curl_easy_init()))
	{
		curl_easy_setopt(Curl, CURLOPT_URL, ApiUrl.c_str());

		// Verify peer especially as we are using https
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYHOST, 1L);

		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &ResponseBuffer);

		// Perform operation
		if (curl_easy_perform(Curl) != CURLE_OK)
			exit(EXIT_FAILURE);

		return ResponseBuffer;

	}
	// Cleanup
	curl_slist_free_all(Headers);
	curl_easy_cleanup(Curl);
}

// Coutesy of StackOverflow
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	
	return size * nmemb;
}
