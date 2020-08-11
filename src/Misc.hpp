#pragma once

#include <iostream>

class MiscClass
{
	public:
		std::string GetStringInput();
                std::string GetWeatherData(std::string ApiKey, std::string CountryCode, std::string City);

};
