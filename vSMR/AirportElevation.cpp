#include "stdafx.h"
#include "AirportElevation.hpp"
#include "Logger.h"
#include "rapidjson/document.h"

#include <fstream>
#include <sstream>

std::optional<int> LoadAirportElevation(const std::string& icao, const std::string& dllPath)
{
	std::ifstream file(dllPath + "\\airports.json", std::ios::binary);
	if (!file.is_open())
	{
		Logger::info("Unable to open airports.json; using the ground-speed airborne fallback.");
		return {};
	}

	std::stringstream contents;
	contents << file.rdbuf();
	const std::string json = contents.str();

	rapidjson::Document document;
	if (document.Parse<0>(json.c_str()).HasParseError() ||
		!document.IsObject() ||
		!document.HasMember(icao.c_str()))
	{
		Logger::info("airports.json is invalid or does not contain " + icao + "; using the ground-speed airborne fallback.");
		return {};
	}

	const rapidjson::Value& airport = document[icao.c_str()];
	if (!airport.IsObject() || !airport.HasMember("elevation") || !airport["elevation"].IsInt())
	{
		Logger::info("airports.json has no valid elevation for " + icao + "; using the ground-speed airborne fallback.");
		return {};
	}

	return airport["elevation"].GetInt();
}
