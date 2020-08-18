#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

class MiscClass
{
	public:
		std::string GetStringInput();
                std::string GetWeatherData(std::string ApiKey, std::string CountryCode, std::string City);
		std::string GetLocation(std::string ApiKey);
		nlohmann::json ConvertStringToJson(std::string JsonString);
                std::string GetCorrespondingAsciiArt(int WeatherType);

};
