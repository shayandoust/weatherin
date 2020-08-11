#include <iostream>
#include <sstream>
#include <cursesw.h>
#include <curl/curl.h>

#include "Misc.hpp"

using namespace std;

static string Url = "https://api.openweathermap.org/data/2.5/weather";

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

static string *Response;

string MiscClass::GetWeatherData(string ApiKey, string CountryCode, string City)
{
	// Concatenate necessary CountryCode and City into Url
	Url = Url + "?q=" + City + "," + CountryCode + "&APPID=" + ApiKey;

	CURL *Curl;
	ostringstream Oss;
	struct curl_slist *Headers = NULL;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Define CURL headers
	Headers = curl_slist_append(Headers, "Accept: application/json");
	Headers = curl_slist_append(Headers, "Content-Type: application/json");

	// Instantiate curl
	if ((Curl = curl_easy_init()))
	{
		curl_easy_setopt(Curl, CURLOPT_URL, Url.c_str());

		// Verify peer especially as we are using https
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYHOST, 1L);

		// Perform operation
		if (curl_easy_perform(Curl) == CURLE_OK)
		{
			char *Ct;
			if (curl_easy_getinfo(Curl, CURLINFO_CONTENT_TYPE, &Ct))
				return *Response;
		}
	}
	// Cleanup
	curl_slist_free_all(Headers);
	curl_easy_cleanup(Curl);
}


// Based on suggestion on StackOverflow
static int Writer(char *Data, size_t Size, size_t Nmemb, string *BufferIn)
{
	// Check if buffer is not empty
	if (BufferIn)
	{
		BufferIn -> append(Data, Size * Nmemb);

		Response = BufferIn;

		return Size * Nmemb;
	}
	return 0;
}
